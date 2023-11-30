#pragma once

#include "vktwindow.h"
#include "vktPipeline.hpp"
#include "vktEngineDevice.hpp"
#include "vktGameObject.hpp"
#include "vktRenderer.hpp"

#include <memory>
#include <vector>

namespace vkt{

    class Application
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        Application();
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void run();

    private:
        void loadGameObjects();

        void sierpinski(std::vector<vktModel::Vertex> &vertices,
                        int depth,
                        glm::vec2 left,
                        glm::vec2 right,
                        glm::vec2 top);

        vtwindow m_window{WIDTH, HEIGHT, "VulkanTest"};
        vktEngineDevice m_device{m_window};
        vktRenderer m_renderer{m_window, m_device};
        
        std::vector<VktGameObject> m_gameObjects;
    };

}