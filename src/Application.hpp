#pragma once

#include "vktDescriptors.hpp"
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

        vtwindow m_window{WIDTH, HEIGHT, "VulkanTest"};
        vktEngineDevice m_device{m_window};
        vktRenderer m_renderer{m_window, m_device};

        std::unique_ptr<vktDescriptorPool> globalPool{};
        std::vector<VktGameObject> m_gameObjects;
    };

}