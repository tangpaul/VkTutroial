#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <string>

namespace vkt{

    class vtwindow{
    public:
        vtwindow(int w, int h, std::string name);
        ~vtwindow();

        vtwindow(const vtwindow&) = delete;
        vtwindow& operator=(const vtwindow&) = delete;

        bool shouldClose() { return glfwWindowShouldClose(this->m_window); }
        VkExtent2D getExtent() { return { static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height) }; }
        bool wasWindowResized() { return m_framebufferResized; }
        void resetWindowResizedFlag() { m_framebufferResized = false; }
        GLFWwindow* getGLFWwindow() { return m_window; }
        
        void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

    private:
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        void initWindow();

    private:
        int m_width;
        int m_height;
        bool m_framebufferResized = false;
        std::string m_windowName;

        GLFWwindow* m_window = nullptr;
    };
    
}