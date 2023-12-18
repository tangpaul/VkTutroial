#pragma once

#include "vktModel.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace vkt
{
    struct TransformComponent
    {
        glm::vec3 translation{};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::vec3 rotation{};

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 mat4();
        glm::mat3 normalMaxtrix();
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
        TransformComponent transform{};

    private:
        VktGameObject(id_t objid) : m_id(objid) {}
        id_t m_id;
    };
}