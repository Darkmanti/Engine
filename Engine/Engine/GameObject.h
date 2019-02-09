#pragma once
#include "GLEW/glew.h"

class GameObject
{
private:
	GLuint VAO, VBO;
public:
	GameObject();
	~GameObject();
	void func(GLfloat vertices[]);
	void func2();
};
