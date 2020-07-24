#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "utils.h"

namespace life::kuma {

	Transform::Transform(const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale) 
		: position(position), orientation(orientation), scale(scale) { UpdateModelMatrix(); }
	Transform::~Transform() { }
	
	glm::mat4 Transform::GetModelMatrix() const {
	    return m_modelmatrix;
	}
	void Transform::Translate(const glm::vec3& direction) {
		position += direction;
        UpdateModelMatrix();
	}
	void Transform::RotateAroundAxis(const glm::vec3& axis, float radians) {
		orientation = glm::rotate(orientation, radians, axis);
        UpdateModelMatrix();
	}
	void Transform::LookAt(const glm::vec3& position, const glm::vec3& up) {
		orientation = glm::toQuat(glm::lookAt(this->position, position, up));
        UpdateModelMatrix();
	}
	void Transform::LookAt(const Transform& object, const glm::vec3& up) {
		LookAt(object.position, up);
	}
	void Transform::LookDirection(const glm::vec3& direction, const glm::vec3& up) {
		LookAt(this->position + direction, up);
	}
	void Transform::Scale(const float scale_uniformly) {
		scale *= scale_uniformly;
        UpdateModelMatrix();
	}
	void Transform::Scale(const glm::vec3& new_size) {
		scale = new_size;
        UpdateModelMatrix();
	}
	void Transform::UpdateModelMatrix() {
        m_modelmatrix = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(orientation) * glm::scale(glm::mat4(1.0f), scale);
	}
}
