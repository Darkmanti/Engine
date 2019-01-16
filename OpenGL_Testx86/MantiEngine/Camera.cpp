#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position, GLfloat yaw, GLfloat pitch)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	this->Position = position;
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	//Возвращаем направления взгляда камеры
	return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	//обработка движения относительно полученной enum переменной. Через enum чтобы абстрагироваться
	GLfloat velocity = this->MovementSpeed * deltaTime;
	if (direction == FORWARD)
		this->Position += this->Front * velocity;
	if (direction == BACKWARD)
		this->Position -= this->Front * velocity;
	if (direction == LEFT)
		this->Position -= this->Right * velocity;
	if (direction == RIGHT)
		this->Position += this->Right * velocity;
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	//изменяем тангаж и рысканье относительно движения мыши
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw += xoffset;
	this->Pitch += yoffset;

	// Условие для того чтобы экран не перевернулся
	if (constrainPitch)
	{
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
	}

	// Обновить Front, Right и Up векторы с использованием обновленных углов Эйлера
	this->updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
	//Зум который на самом деле угол обзора
	if (this->Zoom >= 44.0f && this->Zoom <= 47.0f)
		this->Zoom -= yoffset / 50.0;
	if (this->Zoom <= 44.0f)
		this->Zoom = 44.0f;
	if (this->Zoom >= 47.0f)
		this->Zoom = 47.0f;
}

void Camera::updateCameraVectors()
{
	// Рассчёт нового Front вектора
	glm::vec3 front;
	front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	front.y = sin(glm::radians(this->Pitch));
	front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	this->Front = glm::normalize(front);
	// Пересчёт Right и Up векторов
	this->Right = glm::normalize(glm::cross(this->Front, glm::vec3(0.0f, 1.0f, 0.0f)));  // Нормализовать векторы, поскольку их длина приближается к 0, чем больше вы смотрите вверх или вниз, что приводит к более медленному движению.
	this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}
