#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	srand(time(NULL));

	GLFWwindow* window = glfwCreateWindow(800,800, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 800);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	unsigned int vertexShader, fragmentShader;
	std::string vertexShaderData, fragmentShaderData;

	std::ifstream vertexShaderFile("default_vertex_shader.txt");
	std::ifstream fragmentShaderFile("default_fragment_shader.txt");

	if (vertexShaderFile.is_open())
	{
		vertexShaderFile.seekg(0, std::ios::end);
		int size = vertexShaderFile.tellg();
		vertexShaderData.resize(size);
		vertexShaderFile.seekg(0, std::ios::beg);
		vertexShaderFile.read(&vertexShaderData[0], size);
	}

	if (fragmentShaderFile.is_open())
	{
		fragmentShaderFile.seekg(0, std::ios::end);
		int size = fragmentShaderFile.tellg();
		fragmentShaderData.resize(size);
		fragmentShaderFile.seekg(0, std::ios::beg);
		fragmentShaderFile.read(&fragmentShaderData[0], size);
	}

	const char * vertexShaderSource = vertexShaderData.c_str();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	const char * fragmentShaderSource = fragmentShaderData.c_str();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // 우측 상단
	 0.5f, -0.5f, 0.0f,  // 우측 하단
	-0.5f, -0.5f, 0.0f,  // 좌측 하단
	-0.5f,  0.5f, 0.0f   // 좌측 상단
	};

	unsigned int indices[] = {  // 0부터 시작한다는 것을 명심하세요!
	0, 1, 3,   // 첫 번째 삼각형
	1, 2, 3    // 두 번째 삼각형
	};

	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		float fColor_r = 0;
		float fColor_g = 0;
		float fColor_b = 0;

		glClearColor(fColor_r, fColor_g, fColor_b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		/*glDrawArrays(GL_TRIANGLES, 0, 3);*/
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}