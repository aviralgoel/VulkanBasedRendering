#pragma once

#include "vbr_model.hpp"
#include <memory>

namespace vbr
{
	struct Transform2dComponent {
		glm::vec2 translation{};
		glm::vec2 scale { 1.0f, 4.0f };
		float rotation;
		glm::mat2 mat2() { 
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotationMat{
				{c, s}, {-s, c}
			};
			glm::mat2 scaleMat{
				{scale.x, 0.0f},
				{0.0f, scale.y}
			};
			return rotationMat*scaleMat; }
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
		Transform2dComponent transform2d{};
	private:
		VbrGameObject(id_t objId) : id(objId) {}
		id_t id;
	};
}