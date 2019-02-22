#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// функции описание которых будет в конце этого документа
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int key, int action, int mode);
void Do_Movement();

// глобальные переменные
Camera camera(glm::vec3(0.0f, 7.0f, 15.0f));
bool keys[1024];
GLfloat lastX = 640, lastY = 360;
bool firstMouse = true;

// Новая ветка тут

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;

GLfloat opacity = 1.0f;
bool spawn = false;

struct bullet_object
{
	glm::mat4 bullet_world;
	double spawn_time;
};

struct cube_object
{
	glm::mat4 cube_world;
	bool destroyed = false;
};

void chek_colision(bullet_object& bullet, cube_object& cube);
void loadImage(GLuint& texture, const char* fileName);

int main()
{

	// описание контекста
	GLint width, height;
	std::ifstream config("config.ini");
	config >> width;
	config >> height;
	config.close();

	srand(time(NULL));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Manti Engine", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// подключене шейдера
	Shader ourShader("Shader//shader.vs", "Shader//shader.fs");
	Shader lightShader("Shader//LampShader.vs", "Shader//LampShader.fs");
	Shader lightObjectShader("Shader//lightShader.vs", "Shader//lightShader.fs");
	Shader modelShader("Shader//ModelShader.vs", "Shader//ModelShader.fs");

	Model ourModel("Resource//nanosuit//nanosuit.obj");

	// функция чтобы объекты который перекрывает другой объект, не прорисовывался
	glEnable(GL_DEPTH_TEST);

	// геометрия вершин куба
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,	 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	 0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,	 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,	 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	 0.0f,  1.0f,  0.0f
	};

	// места расположение кубиков
	glm::vec3 cubePositions[] = {
		glm::vec3(-7.0f, 0.0f, -1.0f),
		glm::vec3(-6.5f, 0.0f, -4.0f),
		glm::vec3(-6.0f, 0.0f, -7.0f),
		glm::vec3(-5.5f, 0.0f, -10.0f),
		glm::vec3(-5.0f, 0.0f, -13.0f),
		glm::vec3(7.0f, 0.0f, -1.0f),
		glm::vec3(6.5f, 0.0f, -4.0f),
		glm::vec3(6.0f, 0.0f, -7.0f),
		glm::vec3(5.5f, 0.0f, -10.0f),
		glm::vec3(5.0f, 0.0f, -13.0f),
		glm::vec3(0.0f, 2.0f, -20.0f),
		glm::vec3(0.0f, -1.0f, 0.0f)
	};

	GLint w, h, comp;

	// генерация буферов и указателей для правильного прохода по вершинному массиву
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Загрузка текстур
	GLuint texture1, texture2, texture3, texture4, texture5, texture6, texture7;
	loadImage(texture1, "Resource/container.jpg");
	loadImage(texture2, "Resource/228.jpg");
	loadImage(texture3, "Resource/grass.jpg");
	loadImage(texture4, "Resource/bullet.jpg");
	loadImage(texture5, "Resource/fire.jpg");
	loadImage(texture6, "Resource/container2.png");
	loadImage(texture7, "Resource/container2_specular.png");

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::scale(trans, glm::vec3(1.0, 1.0, 1.0));

	// для того чтобы консоль не пустовала выводится вот это)))))
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);

	std::vector<bullet_object> bullets;
	std::vector<cube_object>cube_random;

	glm::vec3 sourceLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 sourceLightPos = glm::vec3(-1.0f, 10.0f, -8.0f);

	// Установка стандартного значения для теста глубины (z буффер)
	// Эта функция всегда работает без объявление по умолчанию
	// Можно переопределить на другое значение
	glDepthFunc(GL_LESS);

	while (!glfwWindowShouldClose(window))
	{
		// расчёт времени затраченного на кадр
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// ивенты нажатий на клаву, мышь и кнопки окна
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwPollEvents();
		Do_Movement();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// обновление матриц проекции и вида
		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(camera.Zoom, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

		// Модель
		modelShader.use();

		// Направленный свет (типо Солнце)
		modelShader.setUniform("dirLight.direction", -0.2f, -1.0f, -0.3f);
		modelShader.setUniform("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		modelShader.setUniform("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		modelShader.setUniform("dirLight.specular", 0.5f, 0.5f, 0.5f);

		// Источник света №1
		modelShader.setUniform("pointLights[0].position", sourceLightPos);
		modelShader.setUniform("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		modelShader.setUniform("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f);
		modelShader.setUniform("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		modelShader.setUniform("pointLights[0].constant", 1.0f);
		modelShader.setUniform("pointLights[0].linear", 0.0014f);
		modelShader.setUniform("pointLights[0].quadratic", 0.0007f);

		// Фонарик
		modelShader.setUniform("spotLight.position", camera.Position);
		modelShader.setUniform("spotLight.direction", camera.Front);
		modelShader.setUniform("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		modelShader.setUniform("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		modelShader.setUniform("spotLight.specular", 1.0f, 1.0f, 1.0f);
		modelShader.setUniform("spotLight.constant", 1.0f);
		modelShader.setUniform("spotLight.linear", 0.009f);
		modelShader.setUniform("spotLight.quadratic", 0.012f);
		modelShader.setUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		modelShader.setUniform("spotLight.outerCutOff", glm::cos(glm::radians(18.0f)));

		// Местоположение камеры и блеск
		modelShader.setUniform("viewPos", camera.Position);
		modelShader.setUniform("material.shininess", 8.0f);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f));
		model = glm::rotate(model, 9.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelShader.setUniform("view", view);
		modelShader.setUniform("projection", projection);
		modelShader.setUniform("model", model);
		ourModel.Draw(ourShader);

		ourShader.use();
		ourShader.setUniform("ourTexture1", 0);

		glActiveTexture(GL_TEXTURE0);

		// отправка в uniform матриц проекции и обзора
		ourShader.setUniform("view", view);
		ourShader.setUniform("projection", projection);

		// отправка в uniform матрицы прозрачности текстуры
		ourShader.setUniform("opacity", opacity);

		trans = glm::mat4(1.0f);
		glBindVertexArray(VAO);

		// поприсовка основных 12 кубов
		for (GLuint i = 0; i < 12; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			if (i == 10)
			{
				trans = glm::scale(trans, glm::vec3(4.0, 4.0, 4.0));
				glBindTexture(GL_TEXTURE_2D, texture2);
			}
			else if (i == 11)
			{
				trans = glm::scale(trans, glm::vec3(20.0, 0.0, 20.0));
				glBindTexture(GL_TEXTURE_2D, texture3);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, texture1);
				model = glm::rotate(model, (GLfloat)sin(glfwGetTime()) * 1.8f, glm::vec3(0.0f, 1.0f, 0.0f));
			}
			ourShader.setUniform("model", model);
			ourShader.setUniform("transform", trans);

			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		// Спавн пуль если нажата ЛКМ
		if (spawn == true)
		{
			bullet_object temp;
			glm::mat4 bullet_world = glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.2f, 5.5f));
			glm::mat4 inv_view = glm::inverse(camera.GetViewMatrix());
			bullet_world = inv_view * bullet_world;

			temp.bullet_world = bullet_world;
			temp.spawn_time = glfwGetTime();

			bullets.push_back(temp);
		}
		// Спавн кубов если их меньше 10 и текущее время с начала работы программы кратно 20
		if (cube_random.size() < 10 && (int)glfwGetTime() % 20 == 1)
		{
			cube_object temp;
			glm::mat4 cube_world = glm::translate(glm::mat4(1.0f), glm::vec3((rand() % 20) - 30.0f, rand() % 10, 30+(rand() % 20)));
			temp.cube_world = cube_world;
			cube_random.push_back(temp);
		}
		// отрисовка рандомных кубиков
		for (int i = 0; i < cube_random.size(); i++)
		{
			if (cube_random[i].destroyed == false)
			{
				cube_random[i].cube_world = cube_random[i].cube_world * glm::translate(glm::mat4(1.0f), glm::vec3(sin(glfwGetTime()) * 0.5f, 0, 0));
				trans = glm::scale(glm::mat4(1.0f), glm::vec3(2.0, 2.0, 2.0));
				glBindTexture(GL_TEXTURE_2D, texture5);
				ourShader.setUniform("model", cube_random[i].cube_world);
				ourShader.setUniform("transform", trans);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else
			{
				cube_random.erase(cube_random.begin() + i);
			}
		}
		// отрисовка пуль
		for (int i = 0; i < bullets.size(); i++)
		{
			double speed = 10;
			bullets[i].bullet_world = bullets[i].bullet_world * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -((glfwGetTime() - bullets[i].spawn_time) * speed)));

			trans = glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 1.0));
			glBindTexture(GL_TEXTURE_2D, texture4);
			ourShader.setUniform("model", bullets[i].bullet_world);
			ourShader.setUniform("transform", trans);

			glDrawArrays(GL_TRIANGLES, 0, 36);
			for (int j = 0; j < cube_random.size(); j++)
			{
				chek_colision(bullets[i], cube_random[j]);
			}
			if (glfwGetTime() - bullets[i].spawn_time > 2.0f)
			{
				bullets.erase(bullets.begin() + i);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		//sourceLightPos = glm::vec3(0.0f + (cos(glfwGetTime()) * 20), 10.0f, -5.0f);

		// Шейдер источника света
		lightShader.use();

		lightShader.setUniform("view", view);
		lightShader.setUniform("projection", projection);

		lightShader.setUniform("lightColor", sourceLightColor);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(sourceLightPos));
		trans = glm::mat4(1.0f);
		trans = glm::scale(trans, glm::vec3(2.0, 2.0, 2.0));
		lightShader.setUniform("model", model);
		lightShader.setUniform("transform", trans);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Шейдер объектов под освещением
		lightObjectShader.use();

		lightObjectShader.setUniform("view", view);
		lightObjectShader.setUniform("projection", projection);

		// Направленный свет (типо Солнце)
		lightObjectShader.setUniform("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightObjectShader.setUniform("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightObjectShader.setUniform("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightObjectShader.setUniform("dirLight.specular", 0.5f, 0.5f, 0.5f);

		// Источник света №1
		lightObjectShader.setUniform("pointLights[0].position", sourceLightPos);
		lightObjectShader.setUniform("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		lightObjectShader.setUniform("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f);
		lightObjectShader.setUniform("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightObjectShader.setUniform("pointLights[0].constant", 1.0f);
		lightObjectShader.setUniform("pointLights[0].linear", 0.0014f);
		lightObjectShader.setUniform("pointLights[0].quadratic", 0.0007f);

		// Фонарик
		lightObjectShader.setUniform("spotLight.position", camera.Position);
		lightObjectShader.setUniform("spotLight.direction", camera.Front);
		lightObjectShader.setUniform("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightObjectShader.setUniform("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightObjectShader.setUniform("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightObjectShader.setUniform("spotLight.constant", 1.0f);
		lightObjectShader.setUniform("spotLight.linear", 0.009f);
		lightObjectShader.setUniform("spotLight.quadratic", 0.012f);
		lightObjectShader.setUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		lightObjectShader.setUniform("spotLight.outerCutOff", glm::cos(glm::radians(18.0f)));

		// Местоположение камеры
		lightObjectShader.setUniform("viewPos", camera.Position);

		// Материал объекта
		lightObjectShader.setUniform("material.diffuse", 0);
		lightObjectShader.setUniform("material.specular", 1);
		lightObjectShader.setUniform("material.shininess", 8.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture6);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture7);

		for (int i = 0; i < 100; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-40.0f + i*8, 2.0f, 0.0f));
			trans = glm::mat4(1.0f);
			trans = glm::scale(trans, glm::vec3(4.0, 4.0, 4.0));
			lightObjectShader.setUniform("model", model);
			lightObjectShader.setUniform("transform", trans);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	// удаление и выключение всего не нужного после завершения основного цилка (хотя по факту всё это делает return 0 и в моём случае эти функции не нужны)
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}
// описание функций
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void Do_Movement()
{
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_UP] && opacity < 1.0)
		opacity += 0.01;
	if (keys[GLFW_KEY_DOWN] && opacity > 0.0)
		opacity -= 0.01;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void mouse_button_callback(GLFWwindow* window, int key, int action, int mode)
{
	if (key == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
		camera.Zoom = 45.0f;
	if (key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		camera.Zoom = 44.5f;
	if (key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		camera.Zoom = 45.0f;
	if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		spawn = true;
	if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		spawn = false;
}

void chek_colision(bullet_object& bullet, cube_object& cube)
{
	glm::vec4 one = bullet.bullet_world * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 two = cube.cube_world * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	if ((two.x + 2.0f >= one.x && one.x + 0.2f >= two.x) && (two.y + 2.0f >= one.y && one.y + 0.2f >= two.y) && (two.z + 2.0f >= one.z && one.z + 1.0f >= two.z))
	{
		cube.destroyed = true;
	}
}

void loadImage(GLuint &texture, char const* fileName)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	stbi_set_flip_vertically_on_load(true);
	GLint w, h, comp;
	unsigned char* image = stbi_load(fileName, &w, &h, &comp, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image);
}