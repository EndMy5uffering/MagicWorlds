#include "Transform.h"

void Transform::SetPosition(const glm::vec3& pos)
{
	m_position = glm::vec4{ pos, 1.0 };
}

glm::vec4 Transform::GetPosition()
{
	return m_position;
}

void Transform::Move(const glm::vec3& dist)
{
	m_position += glm::vec4(dist, 0);
}

void Transform::SetScale(float width, float height, float deapth)
{
	m_scale = glm::vec3{ width, height, deapth };
}

void Transform::SetScale(const glm::vec3& scale)
{
	m_scale = scale;
}

glm::vec3 Transform::GetScale()
{
	return m_scale;
}

void Transform::SetRotation(const glm::quat& rot)
{
	m_rotation = rot;
}

void Transform::LookAt(const glm::vec3& dir, const glm::vec3& up)
{
	m_rotation = glm::quatLookAt(dir, up);
}

glm::quat Transform::GetRotation()
{
	return m_rotation;
}

glm::mat4 Transform::GetMatrix()
{
	return glm::mat4{ m_scale.x, 0, 0, m_position.x, 0, m_scale.y, 0, m_position.y, 0, 0, 1, m_position.z, 0, 0, 0, 1 } * glm::toMat4(m_rotation);
}
