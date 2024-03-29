#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <Shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


unsigned char* loadImage(int& width, int&height, int &nrChannels, const char* path);
void framebuffer_size_callback(GLFWwindow*, int, int);


void testGlm()
{
	glm::vec4 vec(1.f, .0f, .0f, 1.f);
	glm::mat4 trans = glm::mat4(1.f);
	trans = glm::translate(trans, glm::vec3(1.f, 1.f, .0f));
	vec = trans * vec;
	std::cout << "[" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "]" << std::endl;
}

glm::mat4 scaleTrans(const glm::vec3& scale)
{
	glm::mat4 res(1.f);
	res = glm::scale(res, scale);
	return res;
}

glm::mat4 scaleTrans(const float& scale)
{
	glm::mat4 res(1.f);
	res = glm::scale(res, glm::vec3(scale));
	return res;
}

glm::mat4 translateTrans(const glm::vec2 offset)
{
	glm::mat4 res(1.0f);
	res = glm::translate(res, glm::vec3(offset.x, offset.y, .0f));
	return res;
}

glm::mat4 translateTrans(const glm::vec3 offset)
{
	glm::mat4 res(1.0f);
	res = glm::translate(res, glm::vec3(offset.x, offset.y, offset.z));
	return res;
}

glm::mat4 rotateTrans(const glm::vec3& rot)
{
	glm::mat4 res(1.0f);
	res = glm::rotate(res, glm::radians(rot.x), glm::vec3(1.f, .0f, .0f));
	res = glm::rotate(res, glm::radians(rot.y), glm::vec3(.0f, 1.0f, .0f));
	res = glm::rotate(res, glm::radians(rot.z), glm::vec3(.0f, 0.0f, .1f));
	return res;
}

double getGlobalTime()
{
	return glfwGetTime();
}

static double randomValue = 0;
static float offsetX = 0;
static float offsetY = 0;
static float rotateZ = 0;
static float rotateX = 0;
static float rotateY = 0;

static float allScale = 1;
static double speed = 0.1;

static float aspect = 800.0f / 600.0f;
static float fov = 45.0f;


struct TextureData
{
	unsigned int ID;
	int width, height, nrChannels;
	const char* path;
	unsigned char* data;
	TextureData(const char* texPath) : path{ texPath }
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		float borderColor[] = { 1.0f, 1.0f, .0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		stbi_set_flip_vertically_on_load(true);
		data = loadImage(width, height, nrChannels, texPath);
		if (data)
		{
			if (nrChannels == 3) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			if (nrChannels == 4) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			std::cout << "Load " << texPath << " succeeded!" << std::endl;
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
};
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
		offsetY += speed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
		offsetY -= speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		aspect -= speed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		aspect += speed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		fov += speed;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		fov -= speed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		rotateZ += speed;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotateX += speed;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotateY += speed;
}



int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "CoordinateSystems", nullptr, nullptr);
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

	//code
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	std::cout << "Test Glm" << std::endl;
	testGlm();
	//Vertices
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	int indicies[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);


	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//shaders
	Shader test("Shaders/Test/test.vsh", "Shaders/Test/test.fsh");

	//textures
	TextureData tex("Textures/container.jpg");
	TextureData face("Textures/awesomeface.png");

	glEnable(GL_DEPTH_TEST);
	//render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//render
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		test.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex.ID);
		test.setInt("img", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, face.ID);
		test.setInt("face", 1);

		//model matrix
		glm::mat4 model = rotateTrans(glm::vec3(sin((float) getGlobalTime()) * 360.0f, sin((float)getGlobalTime()) * 360.0f, .0f));
		test.setMat("model", 1, GL_FALSE, model);
		//view matrix
		glm::mat4 view = translateTrans(glm::vec3(.0f, .0f, offsetY - 3.f));
		test.setMat("view", 1, GL_FALSE, view);
		//projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
		test.setMat("prj", 1, GL_FALSE, projection);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = translateTrans(glm::vec3(1.5f, .5f, -3.0f)) * rotateTrans(glm::vec3(-sin((float)getGlobalTime()) * 360.0f, -sin((float)getGlobalTime()) * 360.0f, .0f));;
		test.setMat("model", 1, GL_FALSE, model);
		test.setMat("view", 1, GL_FALSE, view);
		test.setMat("prj", 1, GL_FALSE, projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	//aspect = float(width) / float(height);
}

unsigned char* loadImage(int& width, int&height, int &nrChannels, const char* path)
{
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	return data;
}
