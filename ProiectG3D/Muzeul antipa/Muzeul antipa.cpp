#include <stdlib.h> // necesare pentru citirea shaderStencilTesting-elor
#include <stdio.h>
#include <math.h> 
#include<vector>
#include <GL/glew.h>

#define GLM_FORCE_CTOR_INIT 
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "OBJ_Loader.h"
#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")
#include "Camera.h"
#include "Shader.h"

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

double deltaTime = 0.0f;   
double lastFrame = 0.0f;

//variabe used in code
bool rot = false;
objl::Loader Loader;
Camera* pCamera = nullptr;
unsigned int planeVAO = 0;
float vertices[82000];
unsigned int indices[72000];
GLuint floorVAO, floorVBO, floorEBO;
GLuint floorVAO1, floorVBO1, floorEBO1;
GLuint floorVAO2, floorVBO2, floorEBO2;
GLuint floorVAO3, floorVBO3, floorEBO3;
GLuint floorVAO4, floorVBO4, floorEBO4;
GLuint floorVAO5, floorVBO5, floorEBO5;

//function used
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int CreateTexture(const std::string& strTexturePath);

//creating the room
void renderWall(GLuint& floorVAO, GLuint& floorVBO, GLuint& floorEBO);
void renderFloor(const Shader& shader);
void renderFloor();
void renderWallRoom(const Shader& shader);

int main(int argc, char** argv)
{
	std::string strFullExeFileName = argv[0];
	std::string strExePath;
	const size_t last_slash_idx = strFullExeFileName.rfind('\\');
	if (std::string::npos != last_slash_idx) {
		strExePath = strFullExeFileName.substr(0, last_slash_idx);
	}

	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Explorarea muzeului Antipa", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewInit();

	// Create camera
	pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0, 1.0, 3.0));

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	Shader shadowMappingShader("..\\Shaders\\ShadowMapping.vs", "..\\Shaders\\ShadowMapping.fs");
	Shader shadowMappingDepthShader("..\\Shaders\\ShadowMappingDepth.vs", "..\\Shaders\\ShadowMappingDepth.fs");

	// configure depth map FBO
	const unsigned int SHADOW_WIDTH = 4098, SHADOW_HEIGHT = 4098;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	// create depth texture
	unsigned int depthMap;
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	pCamera->Reshape(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	pCamera->MouseControl((float)xpos, (float)ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
	pCamera->ProcessMouseScroll((float)yOffset);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(UP, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(DOWN, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		pCamera->Reset(width, height);

	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		rot = true;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		rot = false;
	}

}

unsigned int CreateTexture(const std::string& strTexturePath)
{
	unsigned int textureId = -1;

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(strTexturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "Failed to load texture: " << strTexturePath << std::endl;
	}
	stbi_image_free(data);

	return textureId;
}

void renderWall(GLuint& floorVAO, GLuint& floorVBO, GLuint& floorEBO)
{
	// initialize (if necessary)
	if (floorVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;


		Loader.LoadFile("..\\OBJ\\room.obj");
		for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
		{
			objl::Mesh curMesh = Loader.LoadedMeshes[1];
			int size = curMesh.Vertices.size();

			for (int j = 0; j < curMesh.Vertices.size(); j++)
			{

				verticess.push_back((float)curMesh.Vertices[j].Position.X);
				verticess.push_back((float)curMesh.Vertices[j].Position.Y);
				verticess.push_back((float)curMesh.Vertices[j].Position.Z);
				verticess.push_back((float)curMesh.Vertices[j].Normal.X);
				verticess.push_back((float)curMesh.Vertices[j].Normal.Y);
				verticess.push_back((float)curMesh.Vertices[j].Normal.Z);
				verticess.push_back((float)curMesh.Vertices[j].TextureCoordinate.X);
				verticess.push_back((float)curMesh.Vertices[j].TextureCoordinate.Y);
			}
			for (int j = 0; j < verticess.size(); j++)
			{
				vertices[j] = verticess.at(j);
			}

			for (int j = 0; j < curMesh.Indices.size(); j++)
			{

				indicess.push_back((float)curMesh.Indices[j]);

			}
			for (int j = 0; j < curMesh.Indices.size(); j++)
			{
				indices[j] = indicess.at(j);
			}
		}
		glGenVertexArrays(1, &floorVAO);
		glGenBuffers(1, &floorVBO);
		glGenBuffers(1, &floorEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(floorVAO);
		glEnableVertexAttribArray(0);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(floorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void renderFloor(const Shader& shader)
{
	// floor
	glm::mat4 model;
	shader.SetMat4("model", model);
	renderFloor();

	model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
	shader.SetMat4("model", model);
	renderFloor();

}

void renderFloor()
{
	unsigned int planeVBO;

	if (planeVAO == 0) {
		// set up vertex data (and buffer(s)) and configure vertex attributes
		float planeVertices[] = {
			// positions            // normals         // texcoords
			25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
			-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

			25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  0.0f, 25.0f,
			25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
		};
		// plane VAO
		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);
	}

	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void renderWallRoom(const Shader& shader)
{
	//first room

	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO1, floorVBO1, floorEBO1);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO2, floorVBO2, floorEBO2);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO3, floorVBO3, floorEBO3);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO4, floorVBO4, floorEBO4);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO5, floorVBO5, floorEBO5);

	//second room

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-97.0f, -1.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO1, floorVBO1, floorEBO1);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-50.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO2, floorVBO2, floorEBO2);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-50.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO3, floorVBO3, floorEBO3);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-50.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO4, floorVBO4, floorEBO4);

}