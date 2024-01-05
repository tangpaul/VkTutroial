#include "Application.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "keyboardMovementController.hpp"
#include "simpleRenderSystem.hpp"
#include "vktCamera.hpp"
#include "vktBuffer.hpp"

#include <stdexcept>
#include <array>
#include <chrono>
#include <iostream>
#include <iomanip>
#define _USE_MATH_DEFINES 
#include <math.h>
namespace vkt {

    struct GLobalUbo
    {
        glm::mat4 projectionView{1.f};
        glm::vec4 ambientColor{1.f, 1.f, 1.f, .02f}; // 0.02 is ambient strength
        glm::vec3 lightPosition{-1.f};
        alignas(16) glm::vec4 lightColor{1.f}; // w is light strength
    };

    Application::Application(){
        globalPool = vktDescriptorPool::Builder(m_device)
            .setMaxSets(vktEngineSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, vktEngineSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();
        loadGameObjects();
    }

    Application::~Application()
    {
    }

    void PrintMatrix(const glm::mat4& matrix) {
        for (int i = 0; i < 4; ++i) { // 行
            for (int j = 0; j < 4; ++j) { // 列
                // 打印矩阵的单个元素，matrix[j] 是第 j 列，[i] 是列中的第 i 个元素
                std::cout << std::setw(10) << matrix[j][i] << " ";
            }
            std::cout << std::endl; // 每行结束后换行
        }
    }

    void Application::run(){
        std::vector<std::unique_ptr<vktBuffer>> uniformBuffers(vktEngineSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (size_t i = 0; i < uniformBuffers.size(); i++)
        {
            uniformBuffers[i] = std::make_unique<vktBuffer>(
                m_device,
                sizeof(GLobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            );
            uniformBuffers[i]->map();
        }

        auto globalSetLayout = vktDescriptorSetLayout::Builder(m_device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(vktEngineSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (size_t i = 0; i < globalDescriptorSets.size(); i++)
        {
            auto bufferInfo = uniformBuffers[i]->descriptorInfo();
            vktDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }
                             
        SimpleRenderSystem simpleRenderSystem{
            m_device, 
            m_renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()
        };

        //set camera
        vktCamera camera{};
        camera.setViewTarget(glm::vec3(-1.f, -2.f, -1.f), glm::vec3(0.f, 0.f, 2.5f));

        //set keyboard controller
        auto viewerObject = VktGameObject::createGameObject();
        viewerObject.transform.translation.z = -2.5f;
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        float theta = 0.f;      
        while(!m_window.shouldClose()){
            glfwPollEvents();

            //time caculate
            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(m_window.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            theta = theta + (frameTime * M_PI);
            theta = fmod(theta, 2 * M_PI);
            glm::vec3 dynamicLightPosition = glm::normalize(glm::vec3{-cos(theta), -1, -sin(theta)});

            float aspectRatio = m_renderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspectRatio, 0.1f, 100.f);

            if(auto commandBuffer = m_renderer.beginFrame()){
                int frameIndex = m_renderer.getFrameIndex();
                vktFrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex]};
                
                //update
                GLobalUbo ubo{};
                ubo.projectionView = camera.getProjectionMatrix() * camera.getView();
                ubo.lightPosition = dynamicLightPosition;
                uniformBuffers[frameIndex]->writeToBuffer(&ubo);
                uniformBuffers[frameIndex]->flush();
 
                //render
                m_renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObject(frameInfo, m_gameObjects);
                m_renderer.endSwapChainRenderPass(commandBuffer);
                m_renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(m_device.device());
    }

    void Application::loadGameObjects()
    {
        std::shared_ptr<vktModel> lveModel = vktModel::createModelFromFile(m_device, "F:/06_YouTube/03_VulkanTutroials/VkTutroial/models/flat_vase.obj");
        auto flatVase = VktGameObject::createGameObject();
        flatVase.model = lveModel;
        flatVase.transform.translation = {-0.5f, 0.4f, 0.f};
        flatVase.transform.scale = glm::vec3(3.f, 1.5f, 3.f);
        m_gameObjects.push_back(std::move(flatVase));

        lveModel = vktModel::createModelFromFile(m_device, "F:/06_YouTube/03_VulkanTutroials/VkTutroial/models/smooth_vase.obj");
        auto smoothVase = VktGameObject::createGameObject();
        smoothVase.model = lveModel;
        smoothVase.transform.translation = {0.5f, 0.4f, 0.f};
        smoothVase.transform.scale = glm::vec3(3.f, 1.5f, 3.f);
        m_gameObjects.push_back(std::move(smoothVase));

        lveModel = vktModel::createModelFromFile(m_device, "F:/06_YouTube/03_VulkanTutroials/VkTutroial/models/quad.obj");
        auto floor = VktGameObject::createGameObject();
        floor.model = lveModel;
        floor.transform.translation = {0.f, .5f, 0.f};
        floor.transform.scale = {3.f, 1.f, 3.f};
        m_gameObjects.push_back(std::move(floor));
    }

}