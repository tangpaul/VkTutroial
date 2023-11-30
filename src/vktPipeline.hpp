#pragma once

#include "vktEngineDevice.hpp"

#include <string>
#include <vector>

namespace vkt
{

struct PipelineConfigInfo
{
    PipelineConfigInfo(const PipelineConfigInfo &) = delete;
    PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;

    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class vktPipeline
{
public:
    vktPipeline(
        vktEngineDevice &device,
        const std::string &vertShaderPath,
        const std::string &fragShaderPath,
        const PipelineConfigInfo &configInfo);
    ~vktPipeline();

    // Not copyable or movable
    vktPipeline(const vktPipeline &) = delete;
    vktPipeline& operator=(const vktPipeline &) = delete;

    void bind(VkCommandBuffer commandBuffer);

    static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

private:
    static std::vector<char> readFile(const std::string &filePath);

    void createGraphicsPipeline(
        const std::string &vertShaderPath,
        const std::string &fragShaderPath,
        const PipelineConfigInfo &configInfo);

    void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

    vktEngineDevice& m_device;
    VkPipeline m_graphicsPipeline;
    VkShaderModule m_vertShaderModule;
    VkShaderModule m_fragShaderModule;
};
}