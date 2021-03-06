#pragma once

class Script;
class Vector;
class Shader;
class Mesh;

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

#include "GLEW/glew.h"

class GameObject
{
private:
	Vector *p_location;
	Vector *p_rotation;
	Vector *p_scale;

public:
	GLuint		VAO, VBO, EBO;

	Shader		*shader, *selectShader;

	glm::mat4	model;

	Mesh		*Meshes;

	int			objCount;

	bool		is_Select;

	char		path[256];

	char		**scriptNames;

	float        angle_x;
	float        angle_y;
	float        angle_z;

	GameObject(Shader* _shader, const char* dirPath);

	void setModel(glm::vec3 trans_, glm::vec3 model_, GLfloat degree_, glm::vec3 axis_);

	void rotateModel(float angle_, glm::vec3 axis);

	void Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos);

	__declspec(property(get = get_location, put = set_location)) Vector location;
	__declspec(property(get = get_rotation, put = set_rotation)) Vector rotation;
	__declspec(property(get = get_scale, put = set_scale)) Vector scale;

	Vector &get_location();
	Vector &get_rotation();
	Vector &get_scale();

	void set_location();
	void set_rotation();
	void set_scale();

	~GameObject();
};
