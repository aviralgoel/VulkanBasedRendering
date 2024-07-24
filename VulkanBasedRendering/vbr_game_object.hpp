#pragma once

#include "vbr_model.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace vbr
{
	struct TransformComponent {
		glm::vec3 translation{};
		glm::vec3 scale { 1.0f, 1.0f, 1.0f };
		glm::vec3 rotation{};

		glm::mat4 mat4()
		{	
			// translate, rotate.y, rotate.x, rotate.z, scale
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation);
			transform = glm::rotate(transform, (rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			transform = glm::rotate(transform, (rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			transform = glm::rotate(transform, (rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			transform = glm::scale(transform, scale);
			return transform;
		}
	};
	class VbrGameObject {
	
	public:
		using id_t = unsigned int;
		static VbrGameObject createGameObject()
		{
			static id_t currentId = 0;
			return VbrGameObject(currentId++);
		}

		VbrGameObject(const VbrGameObject&) = delete;
		VbrGameObject& operator=(const VbrGameObject&) = delete;
		VbrGameObject(VbrGameObject&&) = default;
		VbrGameObject &operator=(VbrGameObject &&) = default;


		id_t getId() const { return id; }

		std::shared_ptr<VbrModel> model;
		glm::vec3 color{};
		TransformComponent transform{};
	private:
		VbrGameObject(id_t objId) : id(objId) {}
		id_t id;
	};
}