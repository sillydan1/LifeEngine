#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP
#include <kuma/openglmath>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <string>

namespace life::kuma {

#define LIFE_RADIANS(degrees) glm::radians(degrees)
#define LIFE_DEGREES(rads) glm::degrees(rads)
	class Transform {
	public:
		glm::vec3 position;
		glm::quat orientation;
		glm::vec3 scale;
	
		// Constructor(s) & Destructor(s)
		Transform(const glm::vec3& position = {0.0f, 0.0f, 0.0f}, 
				  const glm::quat& orientation = {0.0f, 0.0f, 0.0f, 1.0f}, 
				  const glm::vec3& scale = {1.0f, 1.0f, 1.0});
		~Transform();
				
		// Mutators
		void Translate(const glm::vec3& direction);
		void RotateAroundAxis(const glm::vec3& axis, float radians);
		void LookAt(const glm::vec3& position, const glm::vec3& up = {0.0f, 1.0f, 0.0f});
		void LookAt(const Transform& object, const glm::vec3& up = {0.0f, 1.0f, 0.0f});
		void LookDirection(const glm::vec3& direction, const glm::vec3& up = {0,1,0});
		void Scale(const float scale_uniformly);
		void Scale(const glm::vec3& new_size);
		
		// Getters
		glm::mat4 GetModelMatrix() const;
		glm::vec3 GetRotationEulerAngles() const { 
			return glm::vec3(std::atan2(2 * orientation.x * orientation.w - 2 * orientation.y * orientation.z, 1 - 2 * orientation.x * orientation.x - 2 * orientation.z * orientation.z),
							 std::atan2(2 * orientation.y * orientation.w - 2 * orientation.x * orientation.z, 1 - 2 * orientation.y * orientation.y - 2 * orientation.z * orientation.z),
							 std::asin( 2 * orientation.x * orientation.y + 2 * orientation.z * orientation.w)); }
		glm::vec3 GetRight() 	const { return glm::vec3( 1, 0, 0) * glm::conjugate(orientation); }
		glm::vec3 GetUp() 		const { return glm::vec3( 0, 1, 0) * glm::conjugate(orientation); }
		glm::vec3 GetForward() 	const { return glm::vec3( 0, 0, 1) * glm::conjugate(orientation); }
		
		void UpdateModelMatrix();
	private:
        glm::mat4 m_modelmatrix;
	};

}
#endif //TRANSFORM_HPP
