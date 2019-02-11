#include "Importer.h"

namespace Importer
{
	uint32_t ImportObj(const char *fileName, GameObject &go)
	{
		//Вертексы
		std::ifstream file;

		file.open(fileName);

		if (!file.is_open())
		{
			return 1;
		}

		std::string str;

		int32_t &countV(go.GetCountV());
		int32_t &countVT(go.GetCountVT());
		int32_t &countF(go.GetCountF());

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

					if (str.find('/') != std::string::npos)
					{
						std::replace(str.begin(), str.end(), '/', ' ');

						// Есть вариант оптимизации
						size_t index(0);
						while (true)
						{
							index = str.find("  ", index);

							if (index == std::string::npos)
							{
								break;
							}

							str.replace(index, 2, " ");

							index += 1;
						}
					}

					int countSpace(0);
					for (int i = 0; i < str.length(); ++i)
					{
						if (str[i] == ' ')
						{
							++countSpace;
						}
					}

					countF += countSpace;

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

		file.close();

		file.open(fileName);

		if (!file.is_open())
		{
			return 1;
		}

		float *V = go.GetV();
		float *VT = go.GetVT();
		int32_t *F = go.GetF();
		int32_t *FT = go.GetFT();

		if (!V) delete[] V;
		if (!VT) delete[] VT;
		if (!F) delete[] F;
		if (!FT) delete[] FT;

		V = new float[countV];
		VT = new float[countVT];
		F = new int[countF / 2];
		FT = new int[countF / 2];

		uint64_t iV(0);								// Итератор количества вертексов
		uint64_t iVT(0);							// Итератор количества вертексов текстурных
		uint64_t iF(0);								// Итератор количества полигонов
		uint64_t iFT(0);							// Итератор количества полигонов текстурных

		if (!file.is_open())
		{
			return 1;
		}

		while (!file.eof())
		{
			file >> str;

			if (str.length() == 1)
			{
				if (str[0] == 'v')
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

						// Есть вариант оптимизации
						size_t index(0);
						while (true)
						{
							index = str.find("  ", index);

							if (index == std::string::npos)
							{
								break;
							}

							str.replace(index, 2, " ");

							index += 1;
						}

						int countSpace(0);
						for (int i = 0; i < str.length(); ++i)
						{
							if (str[i] == ' ')
							{
								++countSpace;
							}
						}

						std::stringstream tmp(str);

						for (int i = 0; i < countSpace / 2; ++i)
						{
							int32_t f1, f2;

							tmp >> f1;
							tmp >> f2;

							F[iF] = f1; ++iF;
							FT[iFT] = f2; ++iFT;
						}
					}
					else
					{
						int countSpace(0);
						for (int i = 0; i < str.length(); ++i)
						{
							if (str[i] == ' ')
							{
								++countSpace;
							}
						}

						std::stringstream tmp(str);

						for (int i = 0; i < countSpace / 2; ++i)
						{
							int32_t f1;

							tmp >> f1;

							F[iF] = f1; ++iF;
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

					VT[iVT] = u; ++iVT;
					VT[iVT] = v; ++iVT;
				}
			}

			getline(file, str);
		}

		go.SetV(V);
		go.SetVT(VT);
		go.SetF(F);
		go.SetFT(FT);

		file.close();

		glGenVertexArrays(1, &go.GetVAO());
		glBindVertexArray(go.GetVAO());

		glGenBuffers(1, &go.GetVBO());
		glBindBuffer(GL_ARRAY_BUFFER, go.GetVBO());

		glGenBuffers(1, &go.GetEBO());

		glBufferData(GL_ARRAY_BUFFER, countV * sizeof(float), V, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go.GetEBO());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, countF / 2 * sizeof(float), F, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		return 0;
	}
};
