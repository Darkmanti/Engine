#pragma once

class Mesh
{
public:

	char					name[256];

	unsigned int			VAO, VBO;

	unsigned long long		faces_Count;

	unsigned int			diffuse_texture, specular_texture, bump_texture;

	Mesh();

	void DrawMesh();
};