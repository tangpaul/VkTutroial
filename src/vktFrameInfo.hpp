#pragma once

#include "vktCamera.hpp"

#include <vulkan/vulkan.h>

namespace vkt
{
    struct vktFrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer& commandBuffer;
        vktCamera& camera;
        VkDescriptorSet& gloabalDescriptorSet;
    };
    
} // namespace vkt
