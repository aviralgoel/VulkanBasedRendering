#pragma once

#include "lve_model.hpp"
#include <unordered_map>

// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>

namespace lve {

    struct TransformComponent {
        glm::vec3 translation{};
        glm::vec3 scale{ 1.f, 1.f, 1.f };
        glm::vec3 rotation{};

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 mat4() {
            const float c3 = glm::cos(rotation.z);
            const float s3 = glm::sin(rotation.z);
            const float c2 = glm::cos(rotation.x);
            const float s2 = glm::sin(rotation.x);
            const float c1 = glm::cos(rotation.y);
            const float s1 = glm::sin(rotation.y);
            return glm::mat4{
                {
                    scale.x * (c1 * c3 + s1 * s2 * s3),
                    scale.x * (c2 * s3),
                    scale.x * (c1 * s2 * s3 - c3 * s1),
                    0.0f,
                },
                {
                    scale.y * (c3 * s1 * s2 - c1 * s3),
                    scale.y * (c2 * c3),
                    scale.y * (c1 * c3 * s2 + s1 * s3),
                    0.0f,
                },
                {
                    scale.z * (c2 * s1),
                    scale.z * (-s2),
                    scale.z * (c1 * c2),
                    0.0f,
                },
                {translation.x, translation.y, translation.z, 1.0f} };
        }
        glm::mat3 normalMatrix();

    };
    struct PointLightObject {
        float lightIntensity = 1.0f;
    };

    class LveGameObject {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, LveGameObject>;

        static LveGameObject createGameObject() {
            static id_t currentId = 0;
            return LveGameObject{ currentId++ };
        }
        static LveGameObject makePointLight(float lightIntensity = 5.0f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)) {
            lve::LveGameObject gameObject = createGameObject();
            gameObject.color = color;
            gameObject.transform.scale.x = radius;
            gameObject.pointLight = std::make_unique<PointLightObject>();
            gameObject.pointLight->lightIntensity = lightIntensity;

            return gameObject;
        }
        

        LveGameObject(const LveGameObject&) = delete;
        LveGameObject& operator=(const LveGameObject&) = delete;
        LveGameObject(LveGameObject&&) = default;
        LveGameObject& operator=(LveGameObject&&) = default;

        id_t getId() { return id; }

        glm::vec3 color{};
        TransformComponent transform{};

        // optional component

        std::shared_ptr<LveModel> model{};
        std::unique_ptr<PointLightObject> pointLight = nullptr;

    private:
        LveGameObject(id_t objId) : id{ objId } {}

        id_t id;
    };
}  // namespace lve
