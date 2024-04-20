#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

class Transform
{

private:
	glm::vec4 m_position;
	glm::vec3 m_scale;
	glm::quat m_rotation;
	glm::mat4 m_transform_cash;

public:

	void SetPosition(const glm::vec3& pos);
	glm::vec4 GetPosition();
	void Move(const glm::vec3& dist);
	
	void SetScale(float width, float height, float deapth);
	void SetScale(const glm::vec3& scale);
	glm::vec3 GetScale();

	void SetRotation(const glm::quat& rot);
	void LookAt(const glm::vec3& dir, const glm::vec3& up);
	glm::quat GetRotation();

	glm::mat4 GetMatrix();
};

