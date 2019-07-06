#include "Camera.h"
#include <iostream>

Camera::Camera()
{
	m_position = glm::vec3(0.f, 0.f, 0.f);
	m_direction = glm::vec3(0.f, 0.f, -1.0f);
	// self coordinates
	m_front = glm::vec3(0.f, 0.f, 1.0f);
	m_right = glm::vec3(1.f, 0.f, 0.f);
	m_up = glm::vec3(0.f, 1.0f, 0.f);
	// world coordinates
	m_worldFront = m_front;
	m_worldRight = m_right;
	m_worldUp = m_up;
	// rotation
	m_yaw = 90.0f;
	m_pitch = 0.0f;
	// input configuration
	m_velocity = 10.0f;
	m_rotateVelocity = 10.0f;
}


Camera::~Camera()
{

}

void Camera::MoveXYPlane(float xoffset,float yoffset, float deltaTime) 
{
	glm::vec3 movement = m_right * xoffset * m_velocity * deltaTime +
		m_up * yoffset * m_velocity * deltaTime;
	m_position = m_position + movement;
}

void Camera::MoveXZPlane(float xoffset, float yoffset, float deltaTime)
{
	glm::vec3 movement = m_right * xoffset * m_velocity * deltaTime +
		m_front * yoffset * m_velocity * deltaTime;
	m_position = m_position + movement;
}

// checked
void Camera::MoveYZPlane(float xoffset, float yoffset, float deltaTime)
{
	glm::vec3 movement = m_up * xoffset * m_velocity * deltaTime + 
		m_front * yoffset * m_velocity * deltaTime;
	m_position = m_position + movement;
}


void Camera::Rotate(float xoffset, float yoffset, float deltaTime)
{
	m_yaw += xoffset * m_rotateVelocity * deltaTime;
	m_pitch += yoffset * m_rotateVelocity * deltaTime;
	if (m_pitch > 89.f)
		m_pitch = 89.f;
	if (m_pitch < -89.f)
		m_pitch = -89.f;
	UpdateVector();
}


glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(m_position, m_position - m_front, m_worldUp);
}

glm::vec3 Camera::GetWorldPosition()
{
	return m_position;
}


void Camera::UpdateVector()
{
	glm::vec3 front;
	front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
	front.y = glm::sin(glm::radians(m_pitch));
	front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
	front = glm::normalize(front);
	
	// update world coordinate
	m_direction = -front;
	m_worldRight = glm::normalize(glm::cross(front, m_worldUp));
	m_worldFront = glm::normalize(glm::cross(m_worldUp, m_worldRight));

	// update self coordinate
	m_front = front;
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}


void Camera::MoveTo(float x, float y, float z) {
	m_position = glm::vec3(x, y, z);
}

