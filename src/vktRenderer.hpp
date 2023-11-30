#pragma once

#include "vktwindow.h"
#include "vktPipeline.hpp"
#include "vktEngineDevice.hpp"
#include "vktEngineSwapChain.hpp"
#include "vktGameObject.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace vkt{

    class vktRenderer
    {
    public:
        vktRenderer(vtwindow& window, vktEngineDevice& device);
        ~vktRenderer();

        vktRenderer(const vktRenderer&) = delete;
        vktRenderer& operator=(const vktRenderer&) = delete;

        VkRenderPass getSwapChainRenderPass() const { return m_swapChain->getRenderPass(); }

        bool isFrameInProgress() const {return isFrameStarted;}

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return m_commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        vtwindow& m_window;
        vktEngineDevice& m_device;
        std::unique_ptr<vktEngineSwapChain> m_swapChain;
        std::vector<VkCommandBuffer> m_commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex{0};
        bool isFrameStarted{0};
    };

}