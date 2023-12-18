#pragma once

#include "vktPipeline.hpp"
#include "vktEngineDevice.hpp"
#include "vktGameObject.hpp"
#include "vktCamera.hpp"
#include "vktFrameInfo.hpp"

#include <memory>
#include <vector>

namespace vkt{

    class SimpleRenderSystem
    {
    public:
        SimpleRenderSystem(vktEngineDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

        void renderGameObject(vktFrameInfo& frameinfo, std::vector<VktGameObject> &gameObjects);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        vktEngineDevice& m_device;

        std::unique_ptr<vktPipeline> m_pipeline;
        VkPipelineLayout m_pipelineLayout;
    };

}