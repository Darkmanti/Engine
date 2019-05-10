#pragma once

#include <string>

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

	uint64_t	Vcount, Fcount;

	Shader		*shader, *selectShader;

	GLuint		texture;

	glm::mat4	model;

	Mesh		*Meshs;

	int			obj_count;

	bool		is_Select;

	std::string	path;

	char		**scriptNames;

	GameObject(Shader* _shader, const char* dirPath);

	void setModel(glm::vec3 trans_, glm::vec3 model_, GLfloat degree_, glm::vec3 axis_);

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
