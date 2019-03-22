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
		int32_t countVN(0);

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
					++countF;
					++countF;

					continue;
				}
			}
			else if (str.length() == 2)
			{
				if (str.substr(0, 2) == "vt")
				{
					++countVT;
					++countVT;
				}

				if (str.substr(0, 2) == "vn")
				{
					++countVN;
					++countVN;
					++countVN;
				}
			}

			getline(file, str);
		}

		file.close();

		// Engine::dirAppData + "\\" + 
		file.open(fileName);

		if (!file.is_open())
		{
			return 1;
		}

		GLfloat *V = new GLfloat[countF * 2 + countF * 3 + countF * 3];
		GLfloat *V_t = new GLfloat[countF * 3];
		GLfloat *VT = new GLfloat[countF * 2];
		GLfloat *VN = new GLfloat[countF * 3];
		GLuint	*F = new GLuint[countF];
		GLuint *FT = new GLuint[countF];
		GLuint *FN = new GLuint[countF];

		uint64_t iV(0);								// Итератор количества вертексов
		uint64_t iVT(0);							// Итератор количества вертексов текстурных
		uint64_t iF(0);								// Итератор количества полигонов
		uint64_t iFT(0);							// Итератор количества полигонов текстурных
		uint64_t iVN(0);							// Итератор количества нормалей
		uint64_t iFN(0);							// Итератор количества полигонов нормалей

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

					V_t[iV] = x; ++iV;
					V_t[iV] = y; ++iV;
					V_t[iV] = z; ++iV;
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
							FN[iFN] = --f3; ++iFN;
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
			else if (str.length() == 2)
			{
				if (str.substr(0, 2) == "vt")
				{
					float u, v;

					file >> str;
					u = std::stof(str);

					file >> str;
					v = std::stof(str);

					VT[iVT] = u; ++iVT;
					VT[iVT] = v; ++iVT;
				}
				else if (str.substr(0, 2) == "vn")
				{
					float x, y, z;

					file >> str;
					x = std::stof(str);

					file >> str;
					y = std::stof(str);

					file >> str;
					z = std::stof(str);

					VN[iVN] = x; ++iVN;
					VN[iVN] = y; ++iVN;
					VN[iVN] = z; ++iVN;
				}
			}

			getline(file, str);
		}

		file.close();

		// Далее идёт переназначение массивов вершин относительно индексов

		uint64_t j(0);

		for (int i = 0; i < countF; i++)
		{
			uint64_t t = F[i] + 1;
			V[j] = V_t[t * 3 - 3]; j++;
			V[j] = V_t[t * 3 - 2]; j++;
			V[j] = V_t[t * 3 - 1]; j++;
		}

		j = 0;

		for (int i = 0; i < countF; i++)
		{
			uint64_t t = FT[i] + 1;
			V[(countF * 3) + j] = VT[t * 2 - 2]; j++;
			V[(countF * 3) + j] = VT[t * 2 - 1]; j++;
		}

		j = 0;

		for (int i = 0; i < countF; i++)
		{
			uint64_t t = FN[i] + 1;
			V[(countF * 3 + countF * 2) + j] = VN[t * 2 - 3]; j++;
			V[(countF * 3 + countF * 2) + j] = VN[t * 2 - 2]; j++;
			V[(countF * 3 + countF * 2) + j] = VN[t * 2 - 1]; j++;
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		//glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, ((countF * 2) + (countF * 3) + (countF * 3)) * sizeof(GLfloat), V, GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, (countF) * sizeof(GLuint), F, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((countF * 3) * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((countF * 3 + countF * 2) * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

		return 0;
	}
};