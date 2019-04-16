#include "Mesh.h"

#include "GLEW/glew.h"

Mesh::Mesh()
{

}

void Mesh::DrawMesh()
{
	// ��������
	// ��������� � �� � ��

	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, faces_Count);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}