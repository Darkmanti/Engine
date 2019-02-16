#pragma once

#include <vector>

#include "GLEW/glew.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

// Определяет несколько возможных вариантов движения камеры. Используется как абстракция.
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Стандартные значения камеры
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 10.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;


// Абстрактный класс камеры, который обрабатывает ввод и вычисляет соответствующие Eular Angles, Vectors и Matrices для использования в OpenGL
class Camera
{
public:
	// Аттрибуты камеры
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	// Углы Эйлера
	GLfloat Yaw;
	GLfloat Pitch;
	// Опции камеры
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	// Конструктор камеры через вектора
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);

	// Просмотр матрицы
	glm::mat4 GetViewMatrix();

	// обрабатывает ввод, полученный от любой системы ввода с клавиатуры. Принимает входной параметр в виде камеры, определенной ENUM
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

	// Процессы ввода, полученные от системы ввода. Ожидает значение смещения как в направлении x, так и в направлении y.
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

	// Обработка входных данных, полученных от события прокрутки мыши. Требуется только вход по вертикальной оси колеса
	void ProcessMouseScroll(GLfloat yoffset);

private:
	// Вычисляет front вектор для обновления углов Эйлера у камеры
	void updateCameraVectors();
};
