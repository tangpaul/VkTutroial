#include "Application.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "simpleRenderSystem.hpp"

#include <stdexcept>
#include <array>

namespace vkt {

    Application::Application(){
        loadGameObjects();
    }

    Application::~Application()
    {
    }

    void Application::run(){
        SimpleRenderSystem simpleRenderSystem{m_device, m_renderer.getSwapChainRenderPass()};

        while(!m_window.shouldClose()){
            glfwPollEvents();

            if(auto commandBuffer = m_renderer.beginFrame()){
                m_renderer.beginSwapChainRenderPass(commandBuffer);

                simpleRenderSystem.renderGameObject(commandBuffer, m_gameObjects);

                m_renderer.endSwapChainRenderPass(commandBuffer);
                m_renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(m_device.device());
    }

    void Application::loadGameObjects()
    {
        // std::vector<vktModel::Vertex> vertices{
        //     {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        //     {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        //     {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        // };

        // sierpinski
        std::vector<vktModel::Vertex> vertices{};
        sierpinski(vertices, 3, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});

        auto vktmodel = std::make_shared<vktModel>(m_device, vertices);

        auto triangle = VktGameObject::createGameObject();
        triangle.model = vktmodel;
        triangle.color = {.1f, .8f, .1f};
        triangle.transform2d.translation = {0.2f, 0.0f};
        triangle.transform2d.scale = {2.f, 0.5f};
        triangle.transform2d.rotation = .25f * glm::two_pi<float>();

        m_gameObjects.push_back(std::move(triangle));
    }

    void Application::sierpinski(std::vector<vktModel::Vertex> &vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top)
    {
        static glm::vec3 red = {1.0f, 0.0f, 0.0f};
        static glm::vec3 green = {0.0f, 1.0f, 0.0f};
        static glm::vec3 blue = {0.0f, 0.0f, 1.0f};
        if (depth <= 0) {
            vertices.push_back({top, red});
            vertices.push_back({right, green});
            vertices.push_back({left, blue});
        } else {
            auto leftTop = 0.5f * (left + top);
            auto rightTop = 0.5f * (right + top);
            auto leftRight = 0.5f * (left + right);
            sierpinski(vertices, depth - 1, left, leftRight, leftTop);
            sierpinski(vertices, depth - 1, leftRight, right, rightTop);
            sierpinski(vertices, depth - 1, leftTop, rightTop, top);
        }
    }
}