#include "simpleRenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

namespace vkt {

    struct SimplePushConstantData
    {
        glm::mat4 modelMatrix{1.f};
        glm::mat4 normalMaxtrix{1.f};
    };

    SimpleRenderSystem::SimpleRenderSystem(vktEngineDevice& device, VkRenderPass renderPass,VkDescriptorSetLayout globalSetLayout)
    : m_device(device)
    {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }

    SimpleRenderSystem::~SimpleRenderSystem()
    {
        vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);

    }

    void SimpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        std::vector<VkDescriptorSetLayout> setLayouts{globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
        pipelineLayoutInfo.pSetLayouts = setLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1; 
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if(vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
    {
        assert(m_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        vktPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = m_pipelineLayout;
        m_pipeline = std::make_unique<vktPipeline>(
            m_device,
            "../../shaders/firstshader.vert.spv", 
            "../../shaders/firstshader.frag.spv",
            pipelineConfig);
    }

    void SimpleRenderSystem::renderGameObject(vktFrameInfo& frameinfo,std::vector<VktGameObject> &gameObjects)
    {
        m_pipeline->bind(frameinfo.commandBuffer);

        vkCmdBindDescriptorSets(
            frameinfo.commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_pipelineLayout,
            0,
            1,
            &frameinfo.gloabalDescriptorSet,
            0,
            nullptr);

        // auto projectionView = frameinfo.camera.getProjectionMatrix() * frameinfo.camera.getView();

        for (auto& obj : gameObjects)
        {
            SimplePushConstantData push{};
            push.modelMatrix = obj.transform.mat4();
            push.normalMaxtrix = obj.transform.normalMaxtrix();

            vkCmdPushConstants(
                frameinfo.commandBuffer,
                m_pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push);

            obj.model->bind(frameinfo.commandBuffer);
            obj.model->draw(frameinfo.commandBuffer);
        }
    }
}