// Инклуды для БМП
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

// Инклуды для obj
#include <cstring>
#include "Debug.h"

// Инклуд GLEW
#include "GLEW/glew.h"

// Класс мешей
#include "Mesh.h"

// CIEXYZTRIPLE_ stuff
typedef int FXPT2DOT30_;

typedef struct {
	FXPT2DOT30_ ciexyzX;
	FXPT2DOT30_ ciexyzY;
	FXPT2DOT30_ ciexyzZ;
} CIEXYZ_;

typedef struct {
	CIEXYZ_  ciexyzRed;
	CIEXYZ_  ciexyzGreen;
	CIEXYZ_  ciexyzBlue;
} CIEXYZTRIPLE_;

// bitmap file header
typedef struct {
	unsigned short bfType;
	unsigned int   bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int   bfOffBits;
} BITMAPFILEHEADER_;

// bitmap info header
typedef struct {
	unsigned int   biSize;
	unsigned int   biWidth;
	unsigned int   biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int   biCompression;
	unsigned int   biSizeImage;
	unsigned int   biXPelsPerMeter;
	unsigned int   biYPelsPerMeter;
	unsigned int   biClrUsed;
	unsigned int   biClrImportant;
	unsigned int   biRedMask;
	unsigned int   biGreenMask;
	unsigned int   biBlueMask;
	unsigned int   biAlphaMask;
	unsigned int   biCSType;
	CIEXYZTRIPLE_   biEndpoints;
	unsigned int   biGammaRed;
	unsigned int   biGammaGreen;
	unsigned int   biGammaBlue;
	unsigned int   biIntent;
	unsigned int   biProfileData;
	unsigned int   biProfileSize;
	unsigned int   biReserved;
} BITMAPINFOHEADER_;

// rgb quad
typedef struct {
	unsigned char  rgbBlue;
	unsigned char  rgbGreen;
	unsigned char  rgbRed;
	unsigned char  rgbReserved;
} RGBQUAD_;

unsigned char* bmp_reader(const char* fileName, int &h, int &w);

void LoadObj(Mesh* meshs, const char* dirPath, int& objCount)
{
	const char* dirName = strrchr(dirPath, 47) + 1;
	char objPath[256];
	strcpy(objPath, dirPath);
	strcat(objPath, "/");
	strcat(objPath, dirName);
	strcat(objPath, ".obj");

	Debug("start loading object :: "); Debug(dirName); Debug("\n");

	LARGE_INTEGER fileSize;
	GetFileSize(objPath, &fileSize);
	fileSize.QuadPart = fileSize.QuadPart + 1;

	LPVOID voidBuffer = malloc(fileSize.QuadPart);
	ReadFileToBuffer(objPath, voidBuffer, fileSize);
	char* buffer = (char*)voidBuffer;
	buffer[fileSize.QuadPart - 1] = '\0';

	unsigned int i(0);

	// Путь к либе материалов
	char mtlPath[256] = "";

	// Количество полигонов
	unsigned int faces_count(0);

	// Количество вершин
	unsigned int vertices_v_count(0);
	// Количество текстурных вершин
	unsigned int vertices_vt_count(0);
	// Количество нормалей вершин
	unsigned int vertices_vn_count(0);

	while (i < fileSize.QuadPart)
	{
		if (buffer[i] == 'v' && buffer[i + 1] == ' ')
		{
			// Вершины
			vertices_v_count++;
			while (buffer[i] != '\n')
				i++;
		}
		else if (buffer[i] == 'v' && buffer[i + 1] == 't')
		{
			// Вершины текстурные
			vertices_vt_count++;
			while (buffer[i] != '\n')
				i++;
		}
		else if (buffer[i] == 'v' && buffer[i + 1] == 'n')
		{
			// Вершины нормаль
			vertices_vn_count++;
			while (buffer[i] != '\n')
				i++;
		}
		else if (buffer[i] == 'f')
		{
			// faces
			faces_count++;
			while (buffer[i] != '\n' && buffer[i] != '\0')
				i++;
		}
		else if (buffer[i] == 'o')
		{
			// Объект
			objCount++;
			while (buffer[i] != '\n')
				i++;
		}
		else if (buffer[i] == 'm')
		{
			// mtllib
			char mtllib[256] = "";
			i += 7;
			int j(0);
			while (buffer[i] != '\n' && buffer[i] != '\r')
			{
				mtllib[j] = buffer[i];
				i++;
				j++;
			}

			strcpy(mtlPath, dirPath);
			strcat(mtlPath, "/");
			strcat(mtlPath, mtllib);
		}
		else
		{
			while (buffer[i] != '\n' && buffer[i] != '\0')
				i++;
		}

		i++;
	}

	faces_count *= 3;

	vertices_v_count *= 3;
	vertices_vt_count *= 2;
	vertices_vn_count *= 3;

	realloc(meshs, sizeof(Mesh) * objCount);

	float* vertices_v_list = new float[vertices_v_count];
	float* vertices_vt_list = new float[vertices_vt_count];
	float* vertices_vn_list = new float[vertices_vn_count];

	unsigned int* faces_v_list = new unsigned int[faces_count];
	unsigned int* faces_vt_list = new unsigned int[faces_count];
	unsigned int* faces_vn_list = new unsigned int[faces_count];

	unsigned int iv_v(0);	// Итератор вершин
	unsigned int iv_vt(0);	// Итератор вершин текстурных
	unsigned int iv_vn(0);	// Итератор вершин нормалей
	unsigned int iv_f(0);	// Итератор индексов
	long int io_o(-1);		// Итератор объектов

	i = 0;

	while (i < fileSize.QuadPart)
	{
		if (buffer[i] == '#')
		{
			while (buffer[i] != '\n' && buffer[i] != '\0')
				i++;
		}
		else if (buffer[i] == 'm')
		{
			while (buffer[i] != '\n' && buffer[i] != '\0')
				i++;
		}
		else if (buffer[i] == 'v' && buffer[i + 1] == ' ')
		{
			// vertices
			char temp[16] = "";
			int j(0);
			i++;
			i++;
			while (buffer[i] != ' ')
			{
				temp[j] = buffer[i];
				j++;
				i++;
			}
			j = 0;
			vertices_v_list[iv_v] = atof(temp);
			iv_v++;
			i++;
			while (buffer[i] != ' ')
			{
				temp[j] = buffer[i];
				j++;
				i++;
			}
			j = 0;
			vertices_v_list[iv_v] = atof(temp);
			iv_v++;
			i++;
			while (buffer[i] != '\n')
			{
				temp[j] = buffer[i];
				j++;
				i++;
			}
			vertices_v_list[iv_v] = atof(temp);
			iv_v++;
		}
		else if (buffer[i] == 'v' && buffer[i + 1] == 't')
		{
			// UV coord
			char temp[16] = "";
			int j(0);
			i++;
			i++;
			i++;
			while (buffer[i] != ' ')
			{
				temp[j] = buffer[i];
				j++;
				i++;
			}
			j = 0;
			vertices_vt_list[iv_vt] = atof(temp);
			iv_vt++;
			i++;
			while (buffer[i] != '\n')
			{
				temp[j] = buffer[i];
				j++;
				i++;
			}
			vertices_vt_list[iv_vt] = atof(temp);
			iv_vt++;
		}
		else if (buffer[i] == 'v' && buffer[i + 1] == 'n')
		{
			// normals
			char temp[16] = "";
			int j(0);
			i++;
			i++;
			i++;
			while (buffer[i] != ' ')
			{
				temp[j] = buffer[i];
				j++;
				i++;
			}
			j = 0;
			vertices_vn_list[iv_vn] = atof(temp);
			iv_vn++;
			i++;
			while (buffer[i] != ' ')
			{
				temp[j] = buffer[i];
				j++;
				i++;
			}
			j = 0;
			vertices_vn_list[iv_vn] = atof(temp);
			iv_vn++;
			i++;
			while (buffer[i] != '\n')
			{
				temp[j] = buffer[i];
				j++;
				i++;
			}
			vertices_vn_list[iv_vn] = atof(temp);
			iv_vn++;
		}
		else if (buffer[i] == 'f')
		{
			// faces
			while (buffer[i] != ' ')
				i++;
			for (int q = 0; q < 3; q++)
			{
				char temp[16] = "";
				int j(0);
				i++;

				while (buffer[i] != '/')
				{
					temp[j] = buffer[i];
					j++;
					i++;
				}
				faces_v_list[iv_f] = atoi(temp) - 1;
				i++;
				j = 0;
				memset(temp, 0, 16);

				while (buffer[i] != '/')
				{
					temp[j] = buffer[i];
					j++;
					i++;
				}
				faces_vt_list[iv_f] = atoi(temp) - 1;
				i++;
				j = 0;
				memset(temp, 0, 16);

				while (buffer[i] != ' ' && buffer[i] != '\n')
				{
					temp[j] = buffer[i];
					j++;
					i++;
				}
				faces_vn_list[iv_f] = atoi(temp) - 1;
				iv_f++;
			}
		}
		else if (buffer[i] == 'u')
		{
			// Usemtl
			i += 7;
			char usemtl[256] = "";
			int j(0);
			while (buffer[i] != '\n' && buffer[i] != '\r')
			{
				usemtl[j] = buffer[i];
				i++;
				j++;
			}

			LARGE_INTEGER fileSize_mtl;
			GetFileSize(mtlPath, &fileSize_mtl);
			fileSize_mtl.QuadPart = fileSize_mtl.QuadPart + 1;

			LPVOID mtl_void_Buffer = malloc(fileSize_mtl.QuadPart);
			ReadFileToBuffer(mtlPath, mtl_void_Buffer, fileSize_mtl);
			char* mtl_buffer = (char*)mtl_void_Buffer;
			mtl_buffer[fileSize_mtl.QuadPart - 1] = '\0';

			unsigned int mtl_it(0);

			while (mtl_it < fileSize_mtl.QuadPart)
			{
				if (mtl_buffer[mtl_it] == '#')
				{
					// Комментарий (пропуск)
					while (mtl_buffer[mtl_it] != '\n' && mtl_buffer[mtl_it] != '\0')
						mtl_it++;
				}
				else if (mtl_buffer[mtl_it] == 'n')
				{
					mtl_it += 7;
					j = 0;
					char usemtl_cmp[256] = "";
					while (mtl_buffer[mtl_it] != '\n' && mtl_buffer[mtl_it] != '\r')
					{
						usemtl_cmp[j] = mtl_buffer[mtl_it];
						j++;
						mtl_it++;
					}

					if (strcmp(usemtl, usemtl_cmp) == 0)
					{
						while (mtl_buffer[mtl_it] != 'n' && mtl_buffer[mtl_it] != '\0')
						{
							if (mtl_buffer[mtl_it] == 'N' && mtl_buffer[mtl_it + 1] == 's')
							{
								// Коэффициент specular
							}
							else if (mtl_buffer[mtl_it] == 'K' && mtl_buffer[mtl_it + 1] == 'a')
							{
								// ambient
							}
							else if (mtl_buffer[mtl_it] == 'K' && mtl_buffer[mtl_it + 1] == 'd')
							{
								// diffuse
							}
							else if (mtl_buffer[mtl_it] == 'K' && mtl_buffer[mtl_it + 1] == 's')
							{
								// Цвет specular
							}
							else if (mtl_buffer[mtl_it] == 'K' && mtl_buffer[mtl_it + 1] == 'e')
							{
								// emissive
							}
							else if (mtl_buffer[mtl_it] == 'N' && mtl_buffer[mtl_it + 1] == 'i')
							{
								// если честно не знаю что это
							}
							else if (mtl_buffer[mtl_it] == 'd')
							{
								// Прозрачность
							}
							else if (mtl_buffer[mtl_it] == 'm' && mtl_buffer[mtl_it + 1] == 'a' && mtl_buffer[mtl_it + 4] == 'K' && mtl_buffer[mtl_it + 5] == 'd')
							{
								// Дифузная текстура
								unsigned int texture;
								int h, w;

								mtl_it += 7;
								j = 0;
								char textureName[256] = "";
								while (mtl_buffer[mtl_it] != '\n' && mtl_buffer[mtl_it] != '\r' && mtl_buffer[mtl_it] != '\0')
								{
									textureName[j] = mtl_buffer[mtl_it];
									mtl_it++;
									j++;
								}

								char full_texture_path[256] = "";
								strcpy(full_texture_path, dirPath);
								strcat(full_texture_path, "/");
								strcat(full_texture_path, textureName);

								unsigned char* image = bmp_reader(full_texture_path, h, w);

								glGenTextures(1, &texture);
								glBindTexture(GL_TEXTURE_2D, texture);

								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
								glGenerateMipmap(GL_TEXTURE_2D);
								glBindTexture(GL_TEXTURE_2D, 0);
								free(image);

								meshs[io_o].diffuse_texture = texture;
							}
							mtl_it++;
						}
					}
				}
				else
				{
					// Пропуск
					while (mtl_buffer[mtl_it] != '\n' && mtl_buffer[mtl_it] != '\0')
						mtl_it++;
				}

				mtl_it++;
			}

			delete[] mtl_void_Buffer;
		}
		else if (buffer[i] == 'o' && buffer[i + 1] == ' ')
		{
			// Объект
			if (io_o != -1)
			{
				// Идёт переназначение массивов вершин относительно индексов
				unsigned int j(0);

				// Итоговый массив вершин
				float *vertices = new float[iv_f * 2 + iv_f * 3 + iv_f * 3];

				for (int q = 0; q < iv_f; q++)
				{
					uint64_t t = faces_v_list[q] + 1;
					vertices[j] = vertices_v_list[t * 3 - 3]; j++;
					vertices[j] = vertices_v_list[t * 3 - 2]; j++;
					vertices[j] = vertices_v_list[t * 3 - 1]; j++;
				}

				j = 0;

				for (int q = 0; q < iv_f; q++)
				{
					uint64_t t = faces_vt_list[q] + 1;
					vertices[(iv_f * 3) + j] = vertices_vt_list[t * 2 - 2]; j++;
					vertices[(iv_f * 3) + j] = vertices_vt_list[t * 2 - 1]; j++;
				}

				j = 0;

				for (int q = 0; q < iv_f; q++)
				{
					uint64_t t = faces_vn_list[q] + 1;
					vertices[(iv_f * 3 + iv_f * 2) + j] = vertices_vn_list[t * 3 - 3]; j++;
					vertices[(iv_f * 3 + iv_f * 2) + j] = vertices_vn_list[t * 3 - 2]; j++;
					vertices[(iv_f * 3 + iv_f * 2) + j] = vertices_vn_list[t * 3 - 1]; j++;
				}

				// Генерация VAO, VBO

				glGenVertexArrays(1, &meshs[io_o].VAO);
				glGenBuffers(1, &meshs[io_o].VBO);

				glBindVertexArray(meshs[io_o].VAO);

				glBindBuffer(GL_ARRAY_BUFFER, meshs[io_o].VBO);
				glBufferData(GL_ARRAY_BUFFER, ((iv_f * 2) + (iv_f * 3) + (iv_f * 3)) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
				glEnableVertexAttribArray(0);

				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((iv_f * 3) * sizeof(GLfloat)));
				glEnableVertexAttribArray(1);

				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((iv_f * 3 + iv_f * 2) * sizeof(GLfloat)));
				glEnableVertexAttribArray(2);

				glBindVertexArray(0);

				meshs[io_o].faces_Count = iv_f;

				iv_f = 0;

				io_o++;

				delete[] vertices;
			}
			else
			{
				io_o = 0;
				while (buffer[i] != '\0' && buffer[i] != '\n')
					i++;
			}
		}
		else if (buffer[i] == '\0' || i > fileSize.QuadPart)
		{
			// Идёт переназначение массивов вершин относительно индексов
			unsigned int j(0);

			// Итоговый массив вершин
			float *vertices = new float[iv_f * 2 + iv_f * 3 + iv_f * 3];

			for (int q = 0; q < iv_f; q++)
			{
				uint64_t t = faces_v_list[q] + 1;
				vertices[j] = vertices_v_list[t * 3 - 3]; j++;
				vertices[j] = vertices_v_list[t * 3 - 2]; j++;
				vertices[j] = vertices_v_list[t * 3 - 1]; j++;
			}

			j = 0;

			for (int q = 0; q < iv_f; q++)
			{
				uint64_t t = faces_vt_list[q] + 1;
				vertices[(iv_f * 3) + j] = vertices_vt_list[t * 2 - 2]; j++;
				vertices[(iv_f * 3) + j] = vertices_vt_list[t * 2 - 1]; j++;
			}

			j = 0;

			for (int q = 0; q < iv_f; q++)
			{
				uint64_t t = faces_vn_list[q] + 1;
				vertices[(iv_f * 3 + iv_f * 2) + j] = vertices_vn_list[t * 3 - 3]; j++;
				vertices[(iv_f * 3 + iv_f * 2) + j] = vertices_vn_list[t * 3 - 2]; j++;
				vertices[(iv_f * 3 + iv_f * 2) + j] = vertices_vn_list[t * 3 - 1]; j++;
			}

			// Генерация VAO, VBO

			glGenVertexArrays(1, &meshs[io_o].VAO);
			glGenBuffers(1, &meshs[io_o].VBO);

			glBindVertexArray(meshs[io_o].VAO);

			glBindBuffer(GL_ARRAY_BUFFER, meshs[io_o].VBO);
			glBufferData(GL_ARRAY_BUFFER, ((iv_f * 2) + (iv_f * 3) + (iv_f * 3)) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((iv_f * 3) * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((iv_f * 3 + iv_f * 2) * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);

			glBindVertexArray(0);

			meshs[io_o].faces_Count = iv_f;

			iv_f = 0;

			io_o++;

			delete[] vertices;
		}
		i++;
	}

	delete[] vertices_v_list;
	delete[] vertices_vt_list;
	delete[] vertices_vn_list;

	delete[] faces_v_list;
	delete[] faces_vt_list;
	delete[] faces_vn_list;

	delete[] voidBuffer;
}

// read bytes
template <typename Type>
void read(std::ifstream &fp, Type &result, std::size_t size) {
	fp.read(reinterpret_cast<char*>(&result), size);
}

unsigned char* bmp_reader(const char* fileName, int &h, int &w)
{

	LARGE_INTEGER fileSize;
	GetFileSize(fileName, &fileSize);
	fileSize.QuadPart = fileSize.QuadPart + 1;

	LPVOID voidBuffer = malloc(fileSize.QuadPart);
	ReadFileToBuffer(fileName, voidBuffer, fileSize);
	char* buffer = (char*)voidBuffer;
	buffer[fileSize.QuadPart - 1] = '\0';

	// открываем файл
	std::ifstream fileStream(fileName, std::ifstream::binary);
	if (!fileStream) {
		std::cout << "Error opening file '" << fileName << "'." << std::endl;
		return 0;
	}

	// заголовк изображения
	BITMAPFILEHEADER_ fileHeader;
	read(fileStream, fileHeader.bfType, sizeof(fileHeader.bfType));
	read(fileStream, fileHeader.bfSize, sizeof(fileHeader.bfSize));
	read(fileStream, fileHeader.bfReserved1, sizeof(fileHeader.bfReserved1));
	read(fileStream, fileHeader.bfReserved2, sizeof(fileHeader.bfReserved2));
	read(fileStream, fileHeader.bfOffBits, sizeof(fileHeader.bfOffBits));

	if (fileHeader.bfType != 0x4D42) {
		std::cout << "Error: '" << fileName << "' is not BMP file." << std::endl;
		return 0;
	}

	// информация изображения
	BITMAPINFOHEADER_ fileInfoHeader;
	read(fileStream, fileInfoHeader.biSize, sizeof(fileInfoHeader.biSize));

	// bmp core
	if (fileInfoHeader.biSize >= 12) {
		read(fileStream, fileInfoHeader.biWidth, sizeof(fileInfoHeader.biWidth));
		read(fileStream, fileInfoHeader.biHeight, sizeof(fileInfoHeader.biHeight));
		read(fileStream, fileInfoHeader.biPlanes, sizeof(fileInfoHeader.biPlanes));
		read(fileStream, fileInfoHeader.biBitCount, sizeof(fileInfoHeader.biBitCount));
	}

	// получаем информацию о битности
	int colorsCount = fileInfoHeader.biBitCount >> 3;
	if (colorsCount < 3) {
		colorsCount = 3;
	}

	int bitsOnColor = fileInfoHeader.biBitCount / colorsCount;
	int maskValue = (1 << bitsOnColor) - 1;

	// bmp v1
	if (fileInfoHeader.biSize >= 40) {
		read(fileStream, fileInfoHeader.biCompression, sizeof(fileInfoHeader.biCompression));
		read(fileStream, fileInfoHeader.biSizeImage, sizeof(fileInfoHeader.biSizeImage));
		read(fileStream, fileInfoHeader.biXPelsPerMeter, sizeof(fileInfoHeader.biXPelsPerMeter));
		read(fileStream, fileInfoHeader.biYPelsPerMeter, sizeof(fileInfoHeader.biYPelsPerMeter));
		read(fileStream, fileInfoHeader.biClrUsed, sizeof(fileInfoHeader.biClrUsed));
		read(fileStream, fileInfoHeader.biClrImportant, sizeof(fileInfoHeader.biClrImportant));
	}

	// bmp v2
	fileInfoHeader.biRedMask = 0;
	fileInfoHeader.biGreenMask = 0;
	fileInfoHeader.biBlueMask = 0;

	if (fileInfoHeader.biSize >= 52) {
		read(fileStream, fileInfoHeader.biRedMask, sizeof(fileInfoHeader.biRedMask));
		read(fileStream, fileInfoHeader.biGreenMask, sizeof(fileInfoHeader.biGreenMask));
		read(fileStream, fileInfoHeader.biBlueMask, sizeof(fileInfoHeader.biBlueMask));
	}

	// если маска не задана, то ставим маску по умолчанию
	if (fileInfoHeader.biRedMask == 0 || fileInfoHeader.biGreenMask == 0 || fileInfoHeader.biBlueMask == 0) {
		fileInfoHeader.biRedMask = maskValue << (bitsOnColor * 2);
		fileInfoHeader.biGreenMask = maskValue << bitsOnColor;
		fileInfoHeader.biBlueMask = maskValue;
	}

	// bmp v3
	if (fileInfoHeader.biSize >= 56) {
		read(fileStream, fileInfoHeader.biAlphaMask, sizeof(fileInfoHeader.biAlphaMask));
	}
	else {
		fileInfoHeader.biAlphaMask = maskValue << (bitsOnColor * 3);
	}

	// bmp v4
	if (fileInfoHeader.biSize >= 108) {
		read(fileStream, fileInfoHeader.biCSType, sizeof(fileInfoHeader.biCSType));
		read(fileStream, fileInfoHeader.biEndpoints, sizeof(fileInfoHeader.biEndpoints));
		read(fileStream, fileInfoHeader.biGammaRed, sizeof(fileInfoHeader.biGammaRed));
		read(fileStream, fileInfoHeader.biGammaGreen, sizeof(fileInfoHeader.biGammaGreen));
		read(fileStream, fileInfoHeader.biGammaBlue, sizeof(fileInfoHeader.biGammaBlue));
	}

	// bmp v5
	if (fileInfoHeader.biSize >= 124) {
		read(fileStream, fileInfoHeader.biIntent, sizeof(fileInfoHeader.biIntent));
		read(fileStream, fileInfoHeader.biProfileData, sizeof(fileInfoHeader.biProfileData));
		read(fileStream, fileInfoHeader.biProfileSize, sizeof(fileInfoHeader.biProfileSize));
		read(fileStream, fileInfoHeader.biReserved, sizeof(fileInfoHeader.biReserved));
	}

	// проверка на поддерку этой версии формата
	if (fileInfoHeader.biSize != 12 && fileInfoHeader.biSize != 40 && fileInfoHeader.biSize != 52 &&
		fileInfoHeader.biSize != 56 && fileInfoHeader.biSize != 108 && fileInfoHeader.biSize != 124) {
		std::cout << "Error: Unsupported BMP format." << std::endl;
		return 0;
	}

	if (fileInfoHeader.biBitCount != 16 && fileInfoHeader.biBitCount != 24 && fileInfoHeader.biBitCount != 32) {
		std::cout << "Error: Unsupported BMP bit count." << std::endl;
		return 0;
	}

	if (fileInfoHeader.biCompression != 0 && fileInfoHeader.biCompression != 3) {
		std::cout << "Error: Unsupported BMP compression." << std::endl;
		return 0;
	}

	// rgb info
	/*RGBQUAD_ **rgbInfo = new RGBQUAD_*[fileInfoHeader.biHeight];

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		rgbInfo[i] = new RGBQUAD_[fileInfoHeader.biWidth];
	}*/

	// определение размера отступа в конце каждой строки
	int linePadding = ((fileInfoHeader.biWidth * (fileInfoHeader.biBitCount / 8)) % 4) & 3;

	// чтение
	unsigned int bufer;

	w = fileInfoHeader.biWidth;
	h = fileInfoHeader.biHeight;

	unsigned char* out = new unsigned char[w * h * 4];

	int n = 0;

	int maskPaddingRed = 0;
	int maskPaddingGreen = 0;
	int maskPaddingBlue = 0;
	int maskPaddingAlpha = 0;

	unsigned int maskRed = fileInfoHeader.biRedMask;
	unsigned int maskGreen = fileInfoHeader.biGreenMask;
	unsigned int maskBlue = fileInfoHeader.biBlueMask;
	unsigned int maskAlpha = fileInfoHeader.biAlphaMask;

	if (maskRed != 0) {
		while (!(maskRed & 1)) {
			maskRed >>= 1;
			maskPaddingRed++;
		}
	}

	if (maskGreen != 0) {
		while (!(maskGreen & 1)) {
			maskGreen >>= 1;
			maskPaddingGreen++;
		}
	}

	if (maskBlue != 0) {
		while (!(maskBlue & 1)) {
			maskBlue >>= 1;
			maskPaddingBlue++;
		}
	}

	if (maskAlpha != 0) {
		while (!(maskAlpha & 1)) {
			maskAlpha >>= 1;
			maskPaddingAlpha++;
		}
	}

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
			read(fileStream, bufer, fileInfoHeader.biBitCount / 8);

			if(fileInfoHeader.biRedMask == 0)
			{
				out[n] = 0;
			}
			else 
			{
				out[n] = (bufer & fileInfoHeader.biRedMask) >> maskPaddingRed;//bitextract(bufer, fileInfoHeader.biRedMask);
			}

			if (fileInfoHeader.biGreenMask == 0)
			{
				out[n+1] = 0;
			}
			else
			{
				out[n+1] = (bufer & fileInfoHeader.biGreenMask) >> maskPaddingGreen;//bitextract(bufer, fileInfoHeader.biGreenMask);
			}

			if (fileInfoHeader.biBlueMask == 0)
			{
				out[n+2] = 0;
			}
			else
			{
				out[n+2] = (bufer & fileInfoHeader.biBlueMask) >> maskPaddingBlue;//bitextract(bufer, fileInfoHeader.biBlueMask);
			}

			if (fileInfoHeader.biAlphaMask == 0)
			{
				out[n+3] = 0;
			}
			else
			{
				out[n+3] = (bufer & fileInfoHeader.biAlphaMask) >> maskPaddingAlpha;//bitextract(bufer, fileInfoHeader.biAlphaMask);
			}

			n = n + 4;
		}
		fileStream.seekg(linePadding, std::ios_base::cur);
	}

	/*w = fileInfoHeader.biWidth;
	h = fileInfoHeader.biHeight;


	unsigned char* out = new unsigned char[w * h * 4];
	int n = 0;*/



	// вывод
	//for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
	//	for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
	//		out[n] = rgbInfo[i][j].rgbRed;
	//		out[n+1] = rgbInfo[i][j].rgbGreen;
	//		out[n+2] = rgbInfo[i][j].rgbBlue;
	//		out[n+3] = rgbInfo[i][j].rgbReserved;

	//		++n;
	//		++n;
	//		++n;
	//		++n;

	//		/*std::cout << std::hex
	//			<< +rgbInfo[i][j].rgbRed << " "
	//			<< +rgbInfo[i][j].rgbGreen << " "
	//			<< +rgbInfo[i][j].rgbBlue << " "
	//			<< +rgbInfo[i][j].rgbReserved
	//			<< std::endl;*/


	//	}
	//}
	return out;
}

//unsigned char bitextract(const unsigned int byte, const unsigned int mask) {
//	if (mask == 0) {
//		return 0;
//	}
//
//	// определение количества нулевых бит справа от маски
//	int
//		maskBufer = mask,
//		maskPadding = 0;
//
//	while (!(maskBufer & 1)) {
//		maskBufer >>= 1;
//		maskPadding++;
//	}
//
//	// применение маски и смещение
//	return (byte & mask) >> maskPadding;
//}
