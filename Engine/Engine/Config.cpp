//// Загрузка интерфейса
//void LoadConfigInterface()
//{
//	GLuint	&windowWidth(Output::windowWidth),
//			&windowHeight(Output::windowHeight),
//
//			&windowRenderWidth(Output::windowRenderWidth),
//			&windowRenderHeight(Output::windowRenderHeight);
//
//	std::fstream file;
//
//	file.open(dirAppData + std::string("\\settings.cfg"), std::ios::in);
//
//	if (!file.is_open())
//	{
//		file.open(dirAppData + std::string("\\settings.cfg"), std::ios::out);
//
//		file << 1366;			file << '\n';
//		file << 768;			file << '\n';
//		file << 1366;			file << '\n';
//		file << 768 - 16;		file << '\n';
//
//		file.close();
//
//		windowWidth = 1366;
//		windowHeight = 768;
//		windowRenderWidth = 1366;
//		windowRenderHeight = 768 - 16;
//
//		return;
//	}
//
//	std::string::size_type size;
//	std::string str;
//
//	file >> str;
//	windowWidth = std::stoi(str, &size);
//
//	file >> str;
//	windowHeight = std::stoi(str, &size);
//
//	file >> str;
//	windowRenderWidth = std::stoi(str, &size);
//
//	file >> str;
//	windowRenderHeight = std::stoi(str, &size);
//
//	file.close();
//}
//
//// Сохранение интерфейса
//void SaveConfigInterface()
//{
//	std::ofstream file;
//
//	file.open(dirAppData + std::string("\\settings.cfg"), std::ios::out);
//
//	if (!file.is_open())
//	{
//		return;
//	}
//
//	GLuint	&windowWidth(Output::windowWidth),
//		&windowHeight(Output::windowHeight),
//
//		&windowRenderWidth(Output::windowRenderWidth),
//		&windowRenderHeight(Output::windowRenderHeight);
//
//	file << windowWidth;			file << '\n';
//	file << windowHeight;			file << '\n';
//	file << windowRenderWidth;		file << '\n';
//	file << windowRenderHeight;		file << '\n';
//
//	file.close();
//}