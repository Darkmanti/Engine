#pragma once

#include "GLEW/glew.h"

#include "Shader.h"
#include "Texture.h"
#include "Engine.h"

class GameObject
{
private:
	bool	enabled,
			isMesh;

	GLuint VAO, VBO, EBO;

	int32_t countV,		//Количество элементов в массиве
			countVT,	//Количество элементов текстурных координат
			countF;		//Количество элементов в массиве полигонов

	float *V;
	float *VT;
	GLuint *F;
	GLuint *FT;

	Shader	*shader;
	Texture *texture;
public:
	GameObject();
	~GameObject();
	
	void Start();
	void Update();

	bool IsMesh();

	GLuint *GetVAO();
	GLuint *GetVBO();
	GLuint *GetEBO();

	int32_t *GetCountV();
	int32_t *GetCountVT();
	int32_t *GetCountF();

	float *GetV();
	float *GetVT();
	GLuint *GetF();
	GLuint *GetFT();

	void SetV(float *v);
	void SetVT(float *vt);
	void SetF(GLuint *f);
	void SetFT(GLuint *ft);

	void Render(GLuint width, GLuint height);
	void Enable();
	void Disable();
};
