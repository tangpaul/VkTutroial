#pragma once

#include "vktEngineDevice.hpp"
#include "vktBuffer.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace vkt 
{

class vktModel
{
public:
    struct Vertex
    {
        glm::vec3 position{};
        glm::vec3 color{};
        glm::vec3 normal{};
        glm::vec2 uv{};

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

        bool operator==(const Vertex& other) const {
            return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
        }
    };

    struct Builder
    {
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};

        void loadModel(const std::string& filepath);
    };
    

public:
    vktModel(vktEngineDevice& device, const vktModel::Builder& builder);
    ~vktModel();

    vktModel(const vktModel&) = delete;
    void operator=(const vktModel&) = delete;

    static std::unique_ptr<vktModel> createModelFromFile(vktEngineDevice& device, const std::string& filepath);

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

private:
    void createVertexBuffer(const std::vector<Vertex>& vertices);
    void createIndexBuffer(const std::vector<uint32_t>& indices);

private:
    vktEngineDevice& m_device;

    std::unique_ptr<vktBuffer> m_vertexBuffer;
    uint32_t m_vertexCount;

    bool hasIndexBuffer = false;
    std::unique_ptr<vktBuffer> m_indexBuffer;
    uint32_t m_indexCount;
};

} // namespace vkt