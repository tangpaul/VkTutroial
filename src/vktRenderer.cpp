#include "vktRenderer.hpp"

#include <stdexcept>
#include <array>
#include <iostream>

namespace vkt {

    vktRenderer::vktRenderer(vtwindow& window, vktEngineDevice& device)
    : m_window(window), m_device(device)
    {
        recreateSwapChain();
        createCommandBuffers(); 
    }

    vktRenderer::~vktRenderer()
    {
        freeCommandBuffers();
    }

    void vktRenderer::createCommandBuffers()
    {
        m_commandBuffers.resize(vktEngineSwapChain::MAX_FRAMES_IN_FLIGHT);
        std::cout << "Command Buffer size: " << m_commandBuffers.size() << std::endl;

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_device.getCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

        if(vkAllocateCommandBuffers(m_device.device(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS){
            throw std::runtime_error("failed to allocate command buffers!");
        }
        
    }

    void vktRenderer::freeCommandBuffers()
    {
        vkFreeCommandBuffers(
            m_device.device(),
            m_device.getCommandPool(),
            static_cast<uint32_t>(m_commandBuffers.size()),
            m_commandBuffers.data());

        m_commandBuffers.clear();
    }
   

    void vktRenderer::recreateSwapChain()
    {
        auto extent = m_window.getExtent();
        while(extent.width == 0 || extent.height == 0){
            extent = m_window.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(m_device.device());

        if (m_swapChain == nullptr){
            m_swapChain = nullptr;
            m_swapChain = std::make_unique<vktEngineSwapChain>(m_device, extent);
        } else {
            std::shared_ptr<vktEngineSwapChain> oldSwapChain = std::move(m_swapChain);
            m_swapChain = std::make_unique<vktEngineSwapChain>(m_device, extent, oldSwapChain);

            if (!oldSwapChain->compareSwapFormats(*m_swapChain.get()))
            {
                throw std::runtime_error("Swap chain image(or depth) format has changed!");
            }  
        }
    }

    VkCommandBuffer vktRenderer::beginFrame()
    {
        assert(!isFrameStarted && "Can't call beginFrame while already in progress");

        auto result = m_swapChain->acquireNextImage(&currentImageIndex);

        if(result == VK_ERROR_OUT_OF_DATE_KHR){
            recreateSwapChain();
            return nullptr;
        }

        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS){
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        return commandBuffer;
    }

    void vktRenderer::endFrame()
    {
        assert(isFrameStarted && "Can't call endFrame while frame is not in progress");

        auto commandBuffer = getCurrentCommandBuffer();
        if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS){
            throw std::runtime_error("failed to record command buffer!");
        }

        auto result = m_swapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window.wasWindowResized()){
            m_window.resetWindowResizedFlag();
            recreateSwapChain();
        } else if(result != VK_SUCCESS){
            throw std::runtime_error("failed to present swap chain image!");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % vktEngineSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void vktRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_swapChain->getRenderPass();
        renderPassInfo.framebuffer = m_swapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = m_swapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE); 

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_swapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(m_swapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissors{{0,0}, m_swapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissors);
    }

    void vktRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");

        vkCmdEndRenderPass(commandBuffer);
    }
}
