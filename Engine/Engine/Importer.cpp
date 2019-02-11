#include "Importer.h"

#include "GameObject.h"

namespace Importer
{
	uint32_t ImportObj(GameObject &go)
	{
		//Вертексы
		std::ifstream file;

		file.open("iron.obj");

		if (!file.is_open())
		{
			return 1;
		}

		std::string str;

		while (!file.eof())
		{
			file >> str;

			if (str.length() == 1)
			{
				if (str[0] == 'v')
				{
					++go.GetCountV();
					++go.GetCountV();
					++go.GetCountV();
				}
				else if (str[0] == 'f')
				{
					getline(file, str);

					if (str.find('/') != std::string::npos)
					{
						std::replace(str.begin(), str.end(), '/', ' ');

						// Есть вариант оптимизации
						int index(0);
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

					go.GetCountF() += countSpace;

					continue;
				}
			}
			else
			{
				if (str.substr(0, 2) == "vt")
				{
					++go.GetCountVT();
					++go.GetCountVT();
				}
			}

			getline(file, str);
		}

		file.close();

		file.open("iron.obj");

		if (!file.is_open())
		{
			return 1;
		}

		*go.GetV() = new float[go.GetCountV()];
		*vt = new float[go.GetCountVT()];
		*fv = new int[go.GetCountF() / 2];
		*ft = new int[go.GetCountF() / 2];

		uint64_t iV(0);								// Итератор количества вертексов
		uint64_t iVT(0);							// Итератор количества вертексов текстурных
		uint64_t iFV(0);							// Итератор количества полигонов
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
					double x, y, z;

					file >> str;
					x = std::stof(str);

					file >> str;
					y = std::stof(str);

					file >> str;
					z = std::stof(str);

					v[iV] = x; ++iV;
					v[iV] = y; ++iV;
					v[iV] = z; ++iV;
				}
				else if (str[0] == 'f')
				{
					getline(file, str);

					//str = str.substr(1, str.length() - 1);

					if (str.find('/') != std::string::npos)
					{
						std::replace(str.begin(), str.end(), '/', ' ');

						// Есть вариант оптимизации
						int index(0);
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

							fv[iFV] = f1; ++iFV;
							ft[iFT] = f2; ++iFT;
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

							fv[iFV] = f1; ++iFV;
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

					vt[iVT] = u; ++iVT;
					vt[iVT] = v; ++iVT;
				}
			}

			getline(file, str);
		}

		file.close();
	}
};
