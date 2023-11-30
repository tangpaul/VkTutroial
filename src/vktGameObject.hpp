#pragma once

#include "vktModel.hpp"

#include <memory>

namespace vkt
{
    struct Transform2dComponent
    {
        glm::vec2 translation{};
        glm::vec2 scale{1.0f, 1.0f};
        float rotation;

        glm::mat2 mat2(){
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotationMat{{c, s}, {-s, c}};

            glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};
            // return scaleMat * rotationMat;
            return rotationMat * scaleMat;
        }
    };

    class VktGameObject
    {
    public:
        using id_t = unsigned int;

        static VktGameObject createGameObject(){
            static id_t id = 0;
            return VktGameObject(id++);
        }

        //delete copy constructor
        VktGameObject(const VktGameObject&) = delete;
        //delete copy assignment
        VktGameObject& operator=(const VktGameObject&) = delete;
        //default move constructor
        VktGameObject(VktGameObject&&) = default;
        //default move assignment
        VktGameObject& operator=(VktGameObject&&) = default;

        id_t id() const { return m_id; }
        
        std::shared_ptr<vktModel> model;
        glm::vec3 color{};
        Transform2dComponent transform2d{};

    private:
        VktGameObject(id_t objid) : m_id(objid) {}
        id_t m_id;
    };
}