#include "vktwindow.h"

#include <stdexcept>

namespace vkt{

    vtwindow::vtwindow(int w, int h, std::string name): m_width(w), m_height(h), m_windowName(name) {
        this->initWindow();
    }

    vtwindow::~vtwindow(){
        glfwDestroyWindow(this->m_window);
        glfwTerminate();
    }

    void vtwindow::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        this->m_window = glfwCreateWindow(this->m_width, this->m_height, this->m_windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(this->m_window, this);
        glfwSetFramebufferSizeCallback(this->m_window, framebufferResizeCallback);
    }

    void vtwindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface){
        if(glfwCreateWindowSurface(instance, this->m_window, nullptr, surface) != VK_SUCCESS){
            throw std::runtime_error("failed to create window surface!");
        }
    }
    
    void vtwindow::framebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        auto vtkwindow = reinterpret_cast<vtwindow *>(glfwGetWindowUserPointer(window));
        vtkwindow->m_framebufferResized = true;
        vtkwindow->m_width = width;
        vtkwindow->m_height = height;
    }
}