#pragma once

#include "vktEngineDevice.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace vkt 
{

class vktModel
{
public:
    struct Vertex
    {
        glm::vec2 position;
        glm::vec3 color;

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

public:
    vktModel(vktEngineDevice& device, const std::vector<Vertex>& vertice);
    ~vktModel();

    vktModel(const vktModel&) = delete;
    void operator=(const vktModel&) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

private:
    void createVertexBuffer(const std::vector<Vertex>& vertices);

private:
    vktEngineDevice& m_device;
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    uint32_t m_vertexCount;
};

} // namespace vkt