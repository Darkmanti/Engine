#pragma once
#include "GLEW/glew.h"
#include "GLM/glm.hpp"

#include "Shader.h"
#include "Texture.h"

class GameObject
{
private:
	bool enabled;

	GLuint VAO, VBO, EBO;

	int32_t countV;		//Количество элементов в массиве
	int32_t countVT;	//Количество элементов текстурных координат
	int32_t countF;		//Количество элементов в массиве полигонов

	float *V;
	float *VT;
	int32_t *F;
	int32_t *FT;

	Shader *shader;
	Texture *texture;
public:
	GameObject();
	~GameObject();
	
	int32_t &GetCountV();
	int32_t &GetCountVT();
	int32_t &GetCountF();

	float *GetV();
	float *GetVT();
	int32_t *GetF();
	int32_t *GetFT();

	void Render();
	void Enable();
	void Disable();
};
