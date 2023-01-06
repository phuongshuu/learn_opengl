#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <Shader.h>

void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "First Triangles", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Faild to create opengl Window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize glad!" << std::endl;
		return -1;
	}
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//Draw triagle
	float vertices[] = {
		-0.5f, -0.5f, 0.f,
		0.5f, -0.5f, 0.f,
		.5f, .5f, .0f,

		 -0.5f, 0.5f, 0.0f,
		 .0f, .8f, .0f,
		 .0f, .5f, .0f,

		 .0, .5f, .0f,
		 -.5f, -.5f, .0f, 
		 .5f, -.5f, .0f
	};

	//VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	//VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	////EBO
	//unsigned int indices[] = {  // note that we start from 0!
	//0, 1, 2,   // first triangle
	//2, 3, 0,
	//3, 2, 4 // second triangle
	//};
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	//enable attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//shader
	Shader red("Shaders/triangle.vsh", "Shaders/triangleRed.fsh");
	Shader yellow("Shaders/triangle.vsh", "Shaders/triangleYellow.fsh");
	Shader green("Shaders/triangle.vsh", "Shaders/triGreen.fsh");



	//render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glBindVertexArray(VAO);
		red.use();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		yellow.use();
		glDrawArrays(GL_TRIANGLES, 3, 3);
		green.use();
		glDrawArrays(GL_TRIANGLES, 6, 3);
		//glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}