#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_front;
	glm::vec3 m_worldFront;
	glm::vec3 m_worldUp;
	glm::vec3 m_worldRight;
	float m_yaw;
	float m_pitch;
	float m_velocity;
	float m_rotateVelocity;
public:
	Camera();
	~Camera();
	void MoveXYPlane(float xoffset, float yoffset, float deltaTime);
	void MoveYZPlane(float xoffset, float yoffset, float deltaTime);
	void MoveXZPlane(float xoffset, float yoffset, float deltaTime);
	void Rotate(float xoffset, float yoffset, float deltaTime);
	void UpdateVector();
	glm::mat4 GetViewMatrix();
	glm::vec3 GetWorldPosition();
	void MoveTo(float x, float y, float z);
};

