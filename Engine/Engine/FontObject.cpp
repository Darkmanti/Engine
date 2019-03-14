#include "FontObject.h"

FontObject::FontObject(Shader* shader_, int heightFont, int glyphs_, char const* filename, int firstChar_, int x_, int y_)
{
	unsigned char* ttf_buffer = new unsigned char[1 << 20];
	unsigned char* temp_bitmap = new unsigned char[x_ * y_];

	stbtt_bakedchar* cdata_ = new stbtt_bakedchar[glyphs_];
	cdata = cdata_;

	fread(ttf_buffer, 1, 1 << 20, fopen(filename, "rb"));
	stbtt_BakeFontBitmap(ttf_buffer, 0, heightFont, temp_bitmap, x_, y_, firstChar_, glyphs_, cdata);
	// can free ttf_buffer at this point
	glGenTextures(1, &font);
	glBindTexture(GL_TEXTURE_2D, font);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, x_, y_, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
	// can free temp_bitmap at this point
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	shader = shader_;
	firstChar = firstChar_;
	glyphs = glyphs_;
	width = x_; height = y_;
}

void FontObject::Print(float x, float y, char* text, glm::mat4 ortho)
{
	while (*text)
	{
		if (*text >= firstChar && *text < glyphs)
		{
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(cdata, width, height, *text - firstChar, &x, &y, &q, 1);

			GLfloat vertices[] = {
				q.x0, q.y0, 0.0f, q.s0, q.t0,
				q.x1, q.y0, 0.0f, q.s1, q.t0,
				q.x1, q.y1, 0.0f, q.s1, q.t1,
				q.x1, q.y1, 0.0f, q.s1, q.t1,
				q.x0, q.y1, 0.0f, q.s0, q.t1,
				q.x0, q.y0, 0.0f, q.s0, q.t0
			};

			GLuint VAO, VBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vertices), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);

			shader->use();
			glBindTexture(GL_TEXTURE_2D, font);
			shader->setUniform("ourTexture", 0);
			shader->setUniform("ortho", ortho);

			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
		++text;
	}
}