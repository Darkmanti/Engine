#pragma once

#include <vector>

#include "GLEW/glew.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

// ���������� ��������� ��������� ��������� �������� ������. ������������ ��� ����������.
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// ����������� �������� ������
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 10.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;


// ����������� ����� ������, ������� ������������ ���� � ��������� ��������������� Eular Angles, Vectors � Matrices ��� ������������� � OpenGL
class Camera
{
public:
	// ��������� ������
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	// ���� ������
	GLfloat Yaw;
	GLfloat Pitch;
	// ����� ������
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	// ����������� ������ ����� �������
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);

	// �������� �������
	glm::mat4 GetViewMatrix();

	// ������������ ����, ���������� �� ����� ������� ����� � ����������. ��������� ������� �������� � ���� ������, ������������ ENUM
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

	// �������� �����, ���������� �� ������� �����. ������� �������� �������� ��� � ����������� x, ��� � � ����������� y.
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

	// ��������� ������� ������, ���������� �� ������� ��������� ����. ��������� ������ ���� �� ������������ ��� ������
	void ProcessMouseScroll(GLfloat yoffset);

private:
	// ��������� front ������ ��� ���������� ����� ������ � ������
	void updateCameraVectors();
};
