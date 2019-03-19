#include "Importer.h"

namespace Importer
{
	uint32_t ImportObj(const char *fileName, GLuint& VAO, GLuint& VBO, GLuint& EBO, uint64_t& countV, uint64_t& countF)
	{
		// Вертексы
		std::ifstream file;
		// Engine::dirAppData + "\\" + 
		file.open(fileName);

		if (!file.is_open())
		{
			return 1;
		}

		std::string str;

		int32_t countVT(0);

		while (!file.eof())
		{
			file >> str;

			if (str.length() == 1)
			{
				if (str[0] == 'v')
				{
					++countV;
					++countV;
					++countV;
				}
				else if (str[0] == 'f')
				{
					getline(file, str);

					++countF;

					continue;
				}
			}
			else
			{
				if (str.substr(0, 2) == "vt")
				{
					++countVT;
					++countVT;
				}
			}

			getline(file, str);
		}

		countF *= 3;

		file.close();

		// Engine::dirAppData + "\\" + 
		file.open(fileName);

		if (!file.is_open())
		{
			return 1;
		}

		GLfloat *V = new GLfloat[countV];
		GLfloat *VT = new GLfloat[countVT];
		GLuint	*F = new GLuint[countF];
		GLfloat *FT = new GLfloat[countF];

		uint64_t iV(0);								// Итератор количества вертексов
		uint64_t iVT(0);							// Итератор количества вертексов текстурных
		uint64_t iF(0);								// Итератор количества полигонов
		uint64_t iFT(0);							// Итератор количества полигонов текстурных

		while (!file.eof())
		{
			file >> str;

			if (str.length() == 1)
			{
				if (str[0] == '#')
				{

				}
				else if (str[0] == 'v')
				{
					float x, y, z;

					file >> str;
					x = std::stof(str);

					file >> str;
					y = std::stof(str);

					file >> str;
					z = std::stof(str);

					V[iV] = x; ++iV;
					V[iV] = y; ++iV;
					V[iV] = z; ++iV;
				}
				else if (str[0] == 'f')
				{
					getline(file, str);

					//str = str.substr(1, str.length() - 1);

					if (str.find('/') != std::string::npos)
					{
						std::replace(str.begin(), str.end(), '/', ' ');

						std::stringstream tmp(str);

						for (int i = 0; i < 3; ++i)
						{
							uint32_t f1, f2, f3;

							tmp >> f1;
							tmp >> f2;
							tmp >> f3;

							F[iF] = --f1; ++iF;
							FT[iFT] = --f2; ++iFT;
						}
					}
					else
					{
						std::stringstream tmp(str);

						for (int i = 0; i < 3; ++i)
						{
							int32_t f1, f2, f3;

							tmp >> f1;
							tmp >> f2;
							tmp >> f3;

							F[iF] = --f1; ++iF;
							FT[iFT] = --f2; ++iFT;
						}
					}

					continue;
				}
			}
			else
			{
				if (str.substr(0, 2) == "vt")
				{
					float u, v;

					file >> str;
					u = std::stof(str);

					file >> str;
					v = std::stof(str);

					V[iVT + countV] = u; ++iVT;
					V[iVT + countV] = v; ++iVT;
				}
			}

			getline(file, str);
		}

		file.close();

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, countV * sizeof(V), V, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, countF * sizeof(float), F, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		//glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(countV * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

		return 0;
	}
};