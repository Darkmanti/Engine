#include "Importer.h"

#include "Mesh.h"

#include <fstream>
#include <cstring>

namespace Importer
{
	constexpr int StrLen = 512;

	uint32_t ImportObj(const char *fileName, GLuint& VAO, GLuint& VBO, GLuint& EBO, uint64_t& countV, uint64_t& countF)
	{
		std::ifstream file;
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
			V[(countF * 3 + countF * 2) + j] = VN[t * 3 - 3]; j++;
			V[(countF * 3 + countF * 2) + j] = VN[t * 3 - 2]; j++;
			V[(countF * 3 + countF * 2) + j] = VN[t * 3 - 1]; j++;
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, ((countF * 2) + (countF * 3) + (countF * 3)) * sizeof(GLfloat), V, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((countF * 3) * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((countF * 3 + countF * 2) * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

		return 0;
	}

	uint32_t Import(const char* objPath, const char* dirPath, Mesh* Meshs)
	{
		std::ifstream file_obj;
		file_obj.open(objPath, std::ios_base::in);

		// Строка для временных операций
		char str[StrLen];

		// Имя файла библиотеки материалов
		char mtllib[StrLen];

		// Количество полигонов
		unsigned int faces_count(0);

		// Количество вершин
		unsigned int vertices_v_count(0);
		// Количество текстурных вершин
		unsigned int vertices_vt_count(0);
		// Количество нормалей вершин
		unsigned int vertices_vn_count(0);

		// Инициализация названий других файлов и количества вершин
		while (!file_obj.eof())
		{
			file_obj >> str;

			if (strlen(str) == 6)
			{
				if (strcmp(str, "mtllib") == 0)
				{
					file_obj >> str;
					strcpy(mtllib, str);
				}
			}
			else if (strlen(str) == 1) 
			{
				if (strcmp(str, "f") == 0)
				{
					++faces_count;
				}
				else if (strcmp(str, "v") == 0)
				{
					++vertices_v_count;
				}
			}
			else if (strlen(str) == 2)
			{
				if (strcmp(str, "vt") == 0)
				{
					++vertices_vt_count;
				}
				else if (strcmp(str, "vn") == 0)
				{
					++vertices_vn_count;
				}
			}
			file_obj.getline(str, StrLen);
		}

		file_obj.close();

		faces_count *= 3;

		vertices_v_count *= 3;
		vertices_vt_count *= 2;
		vertices_vn_count *= 3;

		// Временный массив вершин
		float *vertices_v_temp = new float[vertices_v_count];
		// Временный массив вершин текстурных
		float *vertices_vt_temp = new float[vertices_vt_count];
		// Временный массив вершин нормалей
		float *vertices_vn_temp = new float[vertices_vn_count];

		// Временный массив индексов
		unsigned int *faces_v_temp = new unsigned int[faces_count];
		// Временный массив индексов текстурных
		unsigned int *faces_vt_temp = new unsigned int[faces_count];
		// Временный массив индексов нормалей
		unsigned int *faces_vn_temp = new unsigned int[faces_count];

		// Создание пути к mtllib
		char mtllibPath[StrLen];
		strcpy(mtllibPath, dirPath);
		strcat(mtllibPath, "/");
		strcat(mtllibPath, mtllib);

		file_obj.open(objPath, std::ios_base::in);
		unsigned int iv_v(0);	// Итератор вершин
		unsigned int iv_vt(0);	// Итератор вершин текстурных
		unsigned int iv_vn(0);	// Итератор вершин нормалей
		unsigned int iv_f(0);	// Итератор индексов
		long int io_o(-1);	// Итератор объектов

		while (!file_obj.eof())
		{
			file_obj >> str;

			if (strlen(str) == 1)
			{
				if (strcmp(str, "v") == 0)
				{
					file_obj >> str;
					vertices_v_temp[iv_v] = std::stof(str); ++iv_v;
					file_obj >> str;
					vertices_v_temp[iv_v] = std::stof(str); ++iv_v;
					file_obj >> str;
					vertices_v_temp[iv_v] = std::stof(str); ++iv_v;
				}
				else if (strcmp(str, "f") == 0)
				{
					file_obj.getline(str, StrLen);
					if (strchr(str, 47) != NULL)
					{
						char *str_temp = strtok(str, "/ ");
						faces_v_temp[iv_f] = std::stoi(str_temp) - 1;
						str_temp = strtok(NULL, "/ ");
						faces_vt_temp[iv_f] = std::stoi(str_temp) - 1;
						str_temp = strtok(NULL, "/ ");
						faces_vn_temp[iv_f] = std::stoi(str_temp) - 1; ++iv_f;
						str_temp = strtok(NULL, "/ ");
						faces_v_temp[iv_f] = std::stoi(str_temp) - 1;
						str_temp = strtok(NULL, "/ ");
						faces_vt_temp[iv_f] = std::stoi(str_temp) - 1;
						str_temp = strtok(NULL, "/ ");
						faces_vn_temp[iv_f] = std::stoi(str_temp) - 1; ++iv_f;
						str_temp = strtok(NULL, "/ ");
						faces_v_temp[iv_f] = std::stoi(str_temp) - 1;
						str_temp = strtok(NULL, "/ ");
						faces_vt_temp[iv_f] = std::stoi(str_temp) - 1;
						str_temp = strtok(NULL, "/ ");
						faces_vn_temp[iv_f] = std::stoi(str_temp) - 1; ++iv_f;
						
						continue; // Необходимо для того чтобы не бралась ещё одна строка в конце общего цикла
					}
					else
					{
						for (int i = 0; i < 3; i++)
						{
							// На случай если f не разделенеы "/"
						}
					}

				}
				else if (strcmp(str, "o") == 0)
				{
					if (io_o != -1)
					{
						// Идёт переназначение массивов вершин относительно индексов
						unsigned int j(0);

						// Итоговый массив вершин
						float *vertices = new float[iv_f * 2 + iv_f * 3 + iv_f * 3];

						for (int i = 0; i < iv_f; i++)
						{
							uint64_t t = faces_v_temp[i] + 1;
							vertices[j] = vertices_v_temp[t * 3 - 3]; j++;
							vertices[j] = vertices_v_temp[t * 3 - 2]; j++;
							vertices[j] = vertices_v_temp[t * 3 - 1]; j++;
						}

						j = 0;

						for (int i = 0; i < iv_f; i++)
						{
							uint64_t t = faces_vt_temp[i] + 1;
							vertices[(iv_f * 3) + j] = vertices_vt_temp[t * 2 - 2]; j++;
							vertices[(iv_f * 3) + j] = vertices_vt_temp[t * 2 - 1]; j++;
						}

						j = 0;

						for (int i = 0; i < iv_f; i++)
						{
							uint64_t t = faces_vn_temp[i] + 1;
							vertices[(iv_f * 3 + iv_f * 2) + j] = vertices_vn_temp[t * 3 - 3]; j++;
							vertices[(iv_f * 3 + iv_f * 2) + j] = vertices_vn_temp[t * 3 - 2]; j++;
							vertices[(iv_f * 3 + iv_f * 2) + j] = vertices_vn_temp[t * 3 - 1]; j++;
						}

						// Генерация VAO, VBO

						glGenVertexArrays(1, &Meshs[io_o].VAO);
						glGenBuffers(1, &Meshs[io_o].VBO);

						glBindVertexArray(Meshs[io_o].VAO);

						glBindBuffer(GL_ARRAY_BUFFER, Meshs[io_o].VBO);
						glBufferData(GL_ARRAY_BUFFER, ((iv_f * 2) + (iv_f * 3) + (iv_f * 3)) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

						glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
						glEnableVertexAttribArray(0);

						glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((iv_f * 3) * sizeof(GLfloat)));
						glEnableVertexAttribArray(1);

						glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((iv_f * 3 + iv_f * 2) * sizeof(GLfloat)));
						glEnableVertexAttribArray(2);

						glBindVertexArray(0);

						Meshs[io_o].faces_Count = iv_f;

						iv_f = 0;

						++io_o;
					}
					else 
					{
						io_o = 0;
					}
				}
			}
			else if (strlen(str) == 2)
			{
				if (strcmp(str, "vt") == 0)
				{
					file_obj >> str;
					vertices_vt_temp[iv_vt] = std::stof(str); ++iv_vt;
					file_obj >> str;
					vertices_vt_temp[iv_vt] = std::stof(str); ++iv_vt;
				}
				else if (strcmp(str, "vn") == 0)
				{
					file_obj >> str;
					vertices_vn_temp[iv_vn] = std::stof(str); ++iv_vn;
					file_obj >> str;
					vertices_vn_temp[iv_vn] = std::stof(str); ++iv_vn;
					file_obj >> str;
					vertices_vn_temp[iv_vn] = std::stof(str); ++iv_vn;
				}
			}
			else if (strlen(str) == 6)
			{
				if (strcmp(str, "usemtl") == 0)
				{
					char newmtl[StrLen];
					bool flag_new_material(true);
					file_obj >> newmtl;
					std::ifstream file_mtl;
					file_mtl.open(mtllibPath, std::ios_base::in);

					while (flag_new_material && !file_mtl.eof())
					{
						file_mtl >> str;
						if (strlen(str) == 6)
						{
							if (strcmp(str, "newmtl") == 0)
							{
								file_mtl >> str;
								if (strcmp(str, newmtl) == 0)
								{
									while (flag_new_material && !file_mtl.eof())
									{
										file_mtl >> str;
										if (strlen(str) == 2)
										{
											if (strcmp(str, "Ns") == 0)
											{
												// Коэффициент specular
											}
											else if (strcmp(str, "Ka") == 0)
											{
												// ambient
											}
											else if (strcmp(str, "Kd") == 0)
											{
												// diffuse
											}
											else if (strcmp(str, "Ks") == 0)
											{
												// Цвет specular
											}
											else if (strcmp(str, "Ke") == 0)
											{
												// emissive
											}
											else if (strcmp(str, "Ni") == 0)
											{
												// если честно не знаю что это
											}
										}
										else if (strlen(str) == 1)
										{
											if (strcmp(str, "d") == 0)
											{
												// Прозрачность
											}
										}
										else if (strlen(str) == 6)
										{
											if (strcmp(str, "map_Kd") == 0)
											{
												// Текстура диффузная
											}
											else if (strcmp(str, "newmtl") == 0)
											{
												flag_new_material = false;
											}
										}
										file_mtl.getline(str, StrLen);
									}
								}
							}
						}
						file_mtl.getline(str, StrLen);
					}
				}
			}
			file_obj.getline(str, StrLen);
			if (file_obj.eof())
			{
				// Идёт переназначение массивов вершин относительно индексов
				unsigned int j(0);

				// Итоговый массив вершин
				float *vertices = new float[iv_f * 2 + iv_f * 3 + iv_f * 3];

				for (int i = 0; i < iv_f; i++)
				{
					uint64_t t = faces_v_temp[i] + 1;
					vertices[j] = vertices_v_temp[t * 3 - 3]; j++;
					vertices[j] = vertices_v_temp[t * 3 - 2]; j++;
					vertices[j] = vertices_v_temp[t * 3 - 1]; j++;
				}

				j = 0;

				for (int i = 0; i < iv_f; i++)
				{
					uint64_t t = faces_vt_temp[i] + 1;
					vertices[(iv_f * 3) + j] = vertices_vt_temp[t * 2 - 2]; j++;
					vertices[(iv_f * 3) + j] = vertices_vt_temp[t * 2 - 1]; j++;
				}

				j = 0;

				for (int i = 0; i < iv_f; i++)
				{
					uint64_t t = faces_vn_temp[i] + 1;
					vertices[(iv_f * 3 + iv_f * 2) + j] = vertices_vn_temp[t * 3 - 3]; j++;
					vertices[(iv_f * 3 + iv_f * 2) + j] = vertices_vn_temp[t * 3 - 2]; j++;
					vertices[(iv_f * 3 + iv_f * 2) + j] = vertices_vn_temp[t * 3 - 1]; j++;
				}

				// Генерация VAO, VBO

				glGenVertexArrays(1, &Meshs[io_o].VAO);
				glGenBuffers(1, &Meshs[io_o].VBO);

				glBindVertexArray(Meshs[io_o].VAO);

				glBindBuffer(GL_ARRAY_BUFFER, Meshs[io_o].VBO);
				glBufferData(GL_ARRAY_BUFFER, ((iv_f * 2) + (iv_f * 3) + (iv_f * 3)) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
				glEnableVertexAttribArray(0);

				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((iv_f * 3) * sizeof(GLfloat)));
				glEnableVertexAttribArray(1);

				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((iv_f * 3 + iv_f * 2) * sizeof(GLfloat)));
				glEnableVertexAttribArray(2);

				glBindVertexArray(0);

				Meshs[io_o].faces_Count = iv_f;

				iv_f = 0;

				++io_o;
			}
		}
		file_obj.close();

		return 1;
	}
};