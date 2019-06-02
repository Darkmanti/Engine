#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include "GLEW/glew.h"

#include "Mesh.h"

constexpr int StrLen = 512;

// CIEXYZTRIPLE stuff
typedef int FXPT2DOT30;

typedef struct {
	FXPT2DOT30 ciexyzX;
	FXPT2DOT30 ciexyzY;
	FXPT2DOT30 ciexyzZ;
} CIEXYZ;

typedef struct {
	CIEXYZ  ciexyzRed;
	CIEXYZ  ciexyzGreen;
	CIEXYZ  ciexyzBlue;
} CIEXYZTRIPLE;

// bitmap file header
typedef struct {
	unsigned short bfType;
	unsigned int   bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int   bfOffBits;
} BITMAPFILEHEADER;

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
	CIEXYZTRIPLE   biEndpoints;
	unsigned int   biGammaRed;
	unsigned int   biGammaGreen;
	unsigned int   biGammaBlue;
	unsigned int   biIntent;
	unsigned int   biProfileData;
	unsigned int   biProfileSize;
	unsigned int   biReserved;
} BITMAPINFOHEADER;

// rgb quad
typedef struct {
	unsigned char  rgbBlue;
	unsigned char  rgbGreen;
	unsigned char  rgbRed;
	unsigned char  rgbReserved;
} RGBQUAD;

unsigned char bitextract(const unsigned int byte, const unsigned int mask);
unsigned char* bmp_reader(const char* fileName, int &h, int &w);

uint32_t ImportObj(const char* objPath, const char* dirPath, Mesh* Meshs)
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
											//unsigned int texture;
											//int h, w;
											//file_mtl >> str;
											//char texture_Path[StrLen];
											//strcpy(texture_Path, dirPath);
											//strcat(texture_Path, "/");
											//strcat(texture_Path, str);
											//unsigned char* image = bmp_reader(texture_Path, h, w);

											//glGenTextures(1, &texture);
											//glBindTexture(GL_TEXTURE_2D, texture);

											//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
											//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

											//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
											//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

											//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
											//glGenerateMipmap(GL_TEXTURE_2D);
											//glBindTexture(GL_TEXTURE_2D, 0);
											//free(image);

											//Meshs[io_o].diffuse_texture = texture;
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

// read bytes
template <typename Type>
void read(std::ifstream &fp, Type &result, std::size_t size) {
	fp.read(reinterpret_cast<char*>(&result), size);
}

unsigned char* bmp_reader(const char* fileName, int &h, int &w)
{

	// открываем файл
	std::ifstream fileStream(fileName, std::ifstream::binary);
	if (!fileStream) {
		std::cout << "Error opening file '" << fileName << "'." << std::endl;
		return 0;
	}

	// заголовк изображения
	BITMAPFILEHEADER fileHeader;
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
	BITMAPINFOHEADER fileInfoHeader;
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
	RGBQUAD **rgbInfo = new RGBQUAD*[fileInfoHeader.biHeight];

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		rgbInfo[i] = new RGBQUAD[fileInfoHeader.biWidth];
	}

	// определение размера отступа в конце каждой строки
	int linePadding = ((fileInfoHeader.biWidth * (fileInfoHeader.biBitCount / 8)) % 4) & 3;

	// чтение
	unsigned int bufer;

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
			read(fileStream, bufer, fileInfoHeader.biBitCount / 8);

			rgbInfo[i][j].rgbRed = bitextract(bufer, fileInfoHeader.biRedMask);
			rgbInfo[i][j].rgbGreen = bitextract(bufer, fileInfoHeader.biGreenMask);
			rgbInfo[i][j].rgbBlue = bitextract(bufer, fileInfoHeader.biBlueMask);
			rgbInfo[i][j].rgbReserved = bitextract(bufer, fileInfoHeader.biAlphaMask);
		}
		fileStream.seekg(linePadding, std::ios_base::cur);
	}

	w = fileInfoHeader.biWidth;
	h = fileInfoHeader.biHeight;


	unsigned char* out = new unsigned char[w*h * 4];
	int n = 0;

	// вывод
	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
			out[n] = rgbInfo[i][j].rgbRed;
			out[n+1] = rgbInfo[i][j].rgbGreen;
			out[n+2] = rgbInfo[i][j].rgbBlue;
			out[n+3] = rgbInfo[i][j].rgbReserved;

			++n;
			++n;
			++n;
			++n;

			/*std::cout << std::hex
				<< +rgbInfo[i][j].rgbRed << " "
				<< +rgbInfo[i][j].rgbGreen << " "
				<< +rgbInfo[i][j].rgbBlue << " "
				<< +rgbInfo[i][j].rgbReserved
				<< std::endl;*/


		}
	}
	return out;
}

unsigned char bitextract(const unsigned int byte, const unsigned int mask) {
	if (mask == 0) {
		return 0;
	}

	// определение количества нулевых бит справа от маски
	int
		maskBufer = mask,
		maskPadding = 0;

	while (!(maskBufer & 1)) {
		maskBufer >>= 1;
		maskPadding++;
	}

	// применение маски и смещение
	return (byte & mask) >> maskPadding;
}
