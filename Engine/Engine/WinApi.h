#pragma once

#define VK_0						0x30
#define VK_1						0x31
#define VK_2						0x32
#define VK_3						0x33
#define VK_4						0x34
#define VK_5						0x35
#define VK_6						0x36
#define VK_7						0x37
#define VK_8						0x38
#define VK_9						0x39

#define VK_A						0x41
#define VK_B						0x42
#define VK_C						0x43
#define VK_D						0x44
#define VK_E						0x45
#define VK_F						0x46
#define VK_G						0x47
#define VK_H						0x48
#define VK_I						0x49
#define VK_J						0x4A
#define VK_K						0x4B
#define VK_L						0x4C
#define VK_M						0x4D
#define VK_N						0x4E
#define VK_O						0x4F
#define VK_P						0x50
#define VK_Q						0x51
#define VK_R						0x52
#define VK_S						0x53
#define VK_T						0x54
#define VK_U						0x55
#define VK_V						0x56
#define VK_W						0x57
#define VK_X						0x58
#define VK_Y						0x59
#define VK_Z						0x5A

extern WNDCLASSEX			pWndEngineClassEx;				// Структура класса окна

// Различные дескрипторы
extern HDC					hDC;							// Дескриптор устройства
extern HGLRC				hRC;							// Дескпритор

extern HWND				hWndEngine;						// Главное окно редактора

extern HANDLE				debugConsole;

extern OPENFILENAME		OFN{ 0 };
extern LPSTR				filePath;

extern char				szDirect[MAX_PATH],
extern szFileName[MAX_PATH];

extern bool				isLoaded;

extern int64_t				mouseOffsetX,
extern mouseOffsetY,
extern lastMousePosX,
extern lastMousePosY;

extern POINT				mousePos{};

extern RECT				windowRenderRect{},
extern windowEngineRect{};

extern bool				isCameraAction;

extern HINSTANCE			hInstance;

extern const uint16_t NumberOfKeys = 256;

extern bool					previousKeyboardState[NumberOfKeys];

extern std::string				lastProjectFileName = "MyProject",
						lastLocationFileName = "MyLocation",
						projectPath;

extern Shader					ourShader("Shader//shader.vs", "Shader//shader.fs"),
						fontShader("Shader//FontShader.vs", "Shader//FontShader.fs"),
						selectShader("Shader//SelectShader.vs", "Shader//SelectShader.fs");

//Инициализация текста
extern FontObject				font1(&fontShader, 32, 256, "Resource/OpenSans-Regular.ttf", 32, 512, 512);

extern GLuint					texture1,
						texture2,
						texture3,
						texture4,
						texture5,
						denisjpg;

extern GLfloat					deltaTime = 0.016f;
extern Camera					camera(glm::vec3(0.0f, 0.0f, 15.0f));

void loadImage(GLuint &texture, char const* fileName, int Par);

LRESULT WndEngineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

ATOM RegisterWindowEngine();

uint8_t ShowInterface(const int16_t nCmdShow);
uint8_t CreateWindowEngine();
uint8_t InitInterface();
uint8_t InitImgui();

void InitInput();

bool isKeyDown(int key);
bool isKeyFirstPressed(int key);
bool isKeyFirstReleased(int key);

void CameraControllAction();

void Debug(unsigned int str);
void Debug(const char *sms);
void Debug(float str);
void Debug(int str);

void Clear();

void Loop();
