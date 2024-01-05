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

//vertices and indices for the objects
float vertices[82000];
unsigned int indices[72000];

// VAO, VBO and EBO for the objects 
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

//creating and rendering the room with textures and objects
void renderWall(GLuint& floorVAO, GLuint& floorVBO, GLuint& floorEBO,int p);
void renderFloor(const Shader& shader);
void renderFloor();
void renderWallRoom(const Shader& shader);

//creating textures
void renderZebra(const Shader& shader);
void renderSkeletal(const Shader& shader);
void renderSkeletal2(const Shader& shader);
void renderTerodactil(const Shader& shader);
void renderGlassWindows(const Shader& shader);
void renderDino3(const Shader& shader);
void renderTree(const Shader& shader);
void renderWoodpecker(const Shader& shader);
void renderDuck(const Shader& shader);
void renderPelican(const Shader& shader);
void renderStork(const Shader& shader);
void renderParrot(const Shader& shader);
void renderpedastal(const Shader& shader);
void renderGrass(const Shader& shader);
void renderGrassBush(const Shader& shader);
void renderGround(const Shader& shader);
void renderDino2(const Shader& shader);
void renderDino3(const Shader& shader);
void renderDinoSkull(const Shader& shader);

//rendering objects
void renderZebra();
void renderSkeletal();
void renderSkeletal2();
void renderTerodactil();
void renderDino3();
void renderGlassWindows();
void renderTree();
void renderWoodpecker();
void renderDuck();
void renderPelican();
void renderStork();
void renderParrot();
void renderpedastal();
void renderGrass();
void renderGrassBush();
void renderGround();
void renderDino2();
void renderTrexTop();
void renderTrexBottom();
void renderDinoSkull();

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

	// lighting info
	glm::vec3 lightPos(-25.0f, 6.0f, -1.0f);
	glEnable(GL_CULL_FACE);

	// build and compile shaders
	Shader shadowMappingShader("..\\Shaders\\ShadowMapping.vs", "..\\Shaders\\ShadowMapping.fs");
	Shader shadowMappingDepthShader("..\\Shaders\\ShadowMappingDepth.vs", "..\\Shaders\\ShadowMappingDepth.fs");

	// shader configuration
	shadowMappingShader.Use();
	shadowMappingShader.SetInt("diffuseTexture", 0);
	shadowMappingShader.SetInt("shadowMap", 1);

	// load textures
	unsigned int wallTexture = CreateTexture(strExePath + "\\wall.jpg");
	unsigned int floorTexture = CreateTexture(strExePath + "\\floor.jpg");
	unsigned int zebraTexture = CreateTexture(strExePath + "\\zebra.jpg");
	unsigned int SkeletalTexture = CreateTexture(strExePath + "\\NHMW-Geo-Plateosaurus_low res_1.jpg");
	unsigned int Skeletal2Texture = CreateTexture(strExePath + "\\NHMW-Geo-2023-0332-0001-Dinornis-robustus-lowres_1.jpg");
	unsigned int terodactilTexture = CreateTexture(strExePath + "\\bones1.jpg");
	unsigned int platformTexture = CreateTexture(strExePath + "\\black.jpg");
	unsigned int glassTexture = CreateTexture(strExePath + "\\glass.jpg");
	unsigned int treeTexture = CreateTexture(strExePath + "\\tree.jpg");
	unsigned int woodpeckerTexture = CreateTexture(strExePath + "\\woodpecker.jpg");
	unsigned int duckTexture = CreateTexture(strExePath + "\\duck.jpg");
	unsigned int pelicanTexture = CreateTexture(strExePath + "\\pelican.jpg");
	unsigned int storkTexture = CreateTexture(strExePath + "\\stork.jpg");
	unsigned int parrotTexture = CreateTexture(strExePath + "\\parrot.jpg");
	unsigned int pedastalTexture = CreateTexture(strExePath + "\\pedastal.jpg");
	unsigned int GrassTexture = CreateTexture(strExePath + "\\Grass.jpg");
	unsigned int GrassBushTexture = CreateTexture(strExePath + "\\GrassBush.jpg");
	unsigned int GroundTexture = CreateTexture(strExePath + "\\Ground.jpg");
	unsigned int dino3 = CreateTexture(strExePath + "\\dino3.jpg");
	unsigned int dino2 = CreateTexture(strExePath + "\\dino2.jpg");
	unsigned int bonesTexture = CreateTexture(strExePath + "\\bones1.jpg");

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

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (rot)
		{
			lightPos.x = 3.0 * sin(currentFrame);
			lightPos.z = 3.0 * cos(currentFrame);

		}

		// input
		processInput(window);

		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1. render depth of scene to texture (from light's perspective)
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(10.0f, -10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		// render scene from light's point of view
		shadowMappingDepthShader.Use();
		shadowMappingDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderFloor(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wallTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderWallRoom(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, zebraTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderZebra(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, SkeletalTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderSkeletal(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,Skeletal2Texture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderSkeletal2(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terodactilTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderTerodactil(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, glassTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderGlassWindows(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);*/


		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, treeTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderTree(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodpeckerTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderWoodpecker(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, duckTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderDuck(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pelicanTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderPelican(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, storkTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderStork(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, parrotTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderParrot(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pedastalTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderpedastal(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GrassTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderGrass(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GrassBushTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderGrassBush(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GroundTexture);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderGround(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, dino2);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        renderDino2(shadowMappingDepthShader);
        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, dino3);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderDino3(shadowMappingDepthShader);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bonesTexture);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        renderDinoSkull(shadowMappingDepthShader);
        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// reset viewport
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 2. render scene as normal using the generated depth/shadow map 
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shadowMappingShader.Use();
		glm::mat4 projection = pCamera->GetProjectionMatrix();
		glm::mat4 view = pCamera->GetViewMatrix();
		shadowMappingShader.SetMat4("projection", projection);
		shadowMappingShader.SetMat4("view", view);
		// set light uniforms
		shadowMappingShader.SetVec3("viewPos", pCamera->GetPosition());
		shadowMappingShader.SetVec3("lightPos", lightPos);
		shadowMappingShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderFloor(shadowMappingShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wallTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderWallRoom(shadowMappingShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bonesTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderDinoSkull(shadowMappingShader);

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, zebraTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderZebra(shadowMappingShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, SkeletalTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderSkeletal(shadowMappingShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Skeletal2Texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderSkeletal2(shadowMappingShader);*/

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terodactilTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderTerodactil(shadowMappingShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, treeTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderTree(shadowMappingShader);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodpeckerTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderWoodpecker(shadowMappingShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, duckTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderDuck(shadowMappingShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pelicanTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderPelican(shadowMappingShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, storkTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderStork(shadowMappingShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, parrotTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderParrot(shadowMappingShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pedastalTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderpedastal(shadowMappingShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GrassTexture);
		glActiveTexture(GL_TEXTURE1);
	    glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderGrass(shadowMappingShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GrassBushTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
	    renderGrassBush(shadowMappingShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GroundTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderGround(shadowMappingShader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, dino3);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glDisable(GL_CULL_FACE);
        renderDino3(shadowMappingShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, dino2);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glDisable(GL_CULL_FACE);
		renderDino2(shadowMappingShader);

		//transparent object
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_ONE, GL_ONE);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, glassTexture);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, depthMap);
		//glDisable(GL_CULL_FACE);
		//renderGlassWindows(shadowMappingShader);
		//glDisable(GL_BLEND);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	delete pCamera;

	glfwTerminate();
	return 0;
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

void renderWall(GLuint& floorVAO, GLuint& floorVBO, GLuint& floorEBO,int p)
{
	// initialize (if necessary)
	if (floorVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;


		Loader.LoadFile("..\\OBJ\\room.obj");
		for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
		{
			objl::Mesh curMesh = Loader.LoadedMeshes[p];
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
	renderWall(floorVAO1, floorVBO1, floorEBO1,1);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO2, floorVBO2, floorEBO2,2);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO3, floorVBO3, floorEBO3,3);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO4, floorVBO4, floorEBO4,4);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO5, floorVBO5, floorEBO5,5);

	//second room

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-97.0f, -1.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO1, floorVBO1, floorEBO1,1);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-50.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO2, floorVBO2, floorEBO2,2);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-50.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO3, floorVBO3, floorEBO3,3);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-50.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.f));
	shader.SetMat4("model", model);
	renderWall(floorVAO4, floorVBO4, floorEBO4,4);

}

void renderZebra(const Shader& shader)
{
	//zebra

	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-40.5f, 0.0f, -13.4f));
	model = glm::scale(model, glm::vec3(0.5f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	shader.SetMat4("model", model);
	renderZebra();
}
unsigned int indicesZ[7200000];
objl::Vertex verZ[2000000];

GLuint zebraVAO, zebraVBO, zebraEBO;

void renderZebra()
{
	// initialize (if necessary)
	if (zebraVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\ZEBRA.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verZ[j] = v;
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
			indicesZ[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &zebraVAO);
		glGenBuffers(1, &zebraVBO);
		glGenBuffers(1, &zebraEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, zebraVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verZ), verZ, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, zebraEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesZ), &indicesZ, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(zebraVAO);
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
	glBindVertexArray(zebraVAO);
	glBindBuffer(GL_ARRAY_BUFFER, zebraVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, zebraEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

unsigned int indicesE[720000];
objl::Vertex verE[820000];

GLuint elephantVAO, elephantVBO, elephantEBO;

void renderSkeletal()
{
	// initialize (if necessary)
	if (elephantVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\NHMW-Geo-Plateosaurus_low res.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verE[j] = v;
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
			indicesE[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &elephantVAO);
		glGenBuffers(1, &elephantVBO);
		glGenBuffers(1, &elephantEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, elephantVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verE), verE, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elephantEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesE), &indicesE, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(elephantVAO);
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
	glBindVertexArray(elephantVAO);
	glBindBuffer(GL_ARRAY_BUFFER, elephantVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elephantEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void renderSkeletal(const Shader& shader)
{
	

	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-62.5f, 4.0f, -13.0f));
	model = glm::scale(model, glm::vec3(0.0035f));
	model = glm::rotate(model, glm::radians(100.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	shader.SetMat4("model", model);
	renderSkeletal();
}

void renderSkeletal2(const Shader& shader)
{
	

	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-62.5f, 4.0f, -13.0f));
	model = glm::scale(model, glm::vec3(0.0035f));
	model = glm::rotate(model, glm::radians(100.0f), glm::vec3(0.0f, 1.0f, 5.0f));
	shader.SetMat4("model", model);
	renderSkeletal2();
}

void renderTerodactil(const Shader& shader)
{
	

	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-2.0f, 16.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.25f));
	model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.SetMat4("model", model);
	renderTerodactil();
}

unsigned int indicesP[720000];
objl::Vertex verP[820000];

GLuint pantherVAO, pantherVBO, pantherEBO;

void renderSkeletal2()
{
	// initialize (if necessary)
	if (pantherVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\NHMW-Geo-2023-0332-0001-Dinornis-robustus-lowres.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verP[j] = v;
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
			indicesP[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &pantherVAO);
		glGenBuffers(1, &pantherVBO);
		glGenBuffers(1, &pantherEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, pantherVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verP), verP, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pantherEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesP), &indicesP, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(pantherVAO);
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
	glBindVertexArray(pantherVAO);
	glBindBuffer(GL_ARRAY_BUFFER, pantherVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pantherEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

unsigned int indicesG[720000];
objl::Vertex verG[820000];

GLuint giraffeVAO, giraffeVBO, giraffeEBO;

void renderTerodactil()
{
	// initialize (if necessary)
	if (giraffeVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\model_Dinosaur Pure_20210908_162913359.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verG[j] = v;
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
			indicesG[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &giraffeVAO);
		glGenBuffers(1, &giraffeVBO);
		glGenBuffers(1, &giraffeEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, giraffeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verG), verG, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, giraffeEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesG), &indicesG, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(giraffeVAO);
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
	glBindVertexArray(giraffeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, giraffeVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, giraffeEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void renderTree(const Shader& shader)
{
	//tree 1

	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-39.0f, -1.0f, 13.4f));
	model = glm::scale(model, glm::vec3(0.1f));
	shader.SetMat4("model", model);
	renderTree();

	// tree 2

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-58.8f, -1.0f, 13.4f));
	model = glm::scale(model, glm::vec3(0.1f));
	model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.SetMat4("model", model);
	renderTree();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-30.8f, -1.0f, 18.4f));
	model = glm::scale(model, glm::vec3(0.1f));
	model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.SetMat4("model", model);
	renderTree();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-68.8f, -1.0f, 18.4f));
	model = glm::scale(model, glm::vec3(0.1f));
	model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.SetMat4("model", model);
	renderTree();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-49.8f, -1.0f, 18.4f));
	model = glm::scale(model, glm::vec3(0.1f));
	model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.SetMat4("model", model);
	renderTree();

}

void renderWoodpecker(const Shader& shader)
{

	//woodpecker

	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-56.0f, 10.0f, 13.9f));
	model = glm::scale(model, glm::vec3(0.025f));
	model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(50.f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.f), glm::vec3(0.0f, 0.0f, -1.0f));

	shader.SetMat4("model", model);
	renderWoodpecker();
}

void renderDuck(const Shader& shader)
{

	//duck

	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-43.0f, 0.5f, 11.9f));
	model = glm::scale(model, glm::vec3(0.027f));
	model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(50.f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.SetMat4("model", model);
	renderDuck();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-48.0f, 0.5f, 13.9f));
	model = glm::scale(model, glm::vec3(0.027f));
	model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(50.f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.SetMat4("model", model);
	renderDuck();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-43.0f, 0.5f, 15.9f));
	model = glm::scale(model, glm::vec3(0.027f));
	model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(50.f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.SetMat4("model", model);
	renderDuck();
}

void renderPelican(const Shader& shader)
{

	//pelican

	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-53.7f, -0.5f, 13.9f));
	model = glm::scale(model, glm::vec3(0.06f));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(-1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(30.f), glm::vec3(0.0f, 0.0f, -1.0f));
	shader.SetMat4("model", model);
	renderPelican();

}

void renderStork(const Shader& shader)
{

	//stork

	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-34.0f, -0.5f, 12.9f));
	model = glm::scale(model, glm::vec3(0.18f));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(-1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, glm::radians(50.f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.SetMat4("model", model);
	renderStork();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-30.0f, -0.5f, 10.0f));
	model = glm::scale(model, glm::vec3(0.18f));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(-1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.0f, 0.0f, -3.0f));
	model = glm::rotate(model, glm::radians(50.f), glm::vec3(0.0f, 0.0f, 2.0f));
	shader.SetMat4("model", model);
	renderStork();
}

void renderParrot(const Shader& shader)
{

	//parrot

	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-60.7f, 5.f, 13.f));
	model = glm::scale(model, glm::vec3(0.12f));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(-1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(40.f), glm::vec3(0.0f, 0.0f, -1.0f));
	shader.SetMat4("model", model);
	renderParrot();
}
void renderpedastal(const Shader& shader)
{
	//pedastal 1
	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(12.5f, 7.2f, 14.4f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f));
	shader.SetMat4("model", model);
	renderpedastal();

	//pedastal 2
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(12.5f, 7.2f, -12.4f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f));
	shader.SetMat4("model", model);
	renderpedastal();
}

void renderGlassWindows(const Shader& shader)
{
	//window 
	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-0.5f, -1.0f, -25.4f));
	model = glm::scale(model, glm::vec3(1.25f));
	shader.SetMat4("model", model);
	renderGlassWindows();


	//window 
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-0.5f, -1.0f, 0.4f));
	model = glm::scale(model, glm::vec3(1.25f));
	shader.SetMat4("model", model);
	renderGlassWindows();

}

float vertices1[82000];
unsigned int indices1[72000];
objl::Vertex ver1[82000];

GLuint glassVAO, glassVBO, glassEBO;
void renderGlassWindows()
{
	if (glassVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\glassWindow.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();

		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;
			ver1[j] = v;
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
			indices1[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &glassVAO);
		glGenBuffers(1, &glassVBO);
		glGenBuffers(1, &glassEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, glassVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ver1), ver1, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glassEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), &indices1, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(glassVAO);
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
	glBindVertexArray(glassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, glassVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glassEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}


unsigned int indicesT[72000];
objl::Vertex verT[82000];

GLuint treeVAO, treeVBO, treeEBO;

void renderTree()
{
	// initialize (if necessary)
	if (treeVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\acacia.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verT[j] = v;
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
			indicesT[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &treeVAO);
		glGenBuffers(1, &treeVBO);
		glGenBuffers(1, &treeEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, treeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verT), verT, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, treeEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesT), &indicesT, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(treeVAO);
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
	glBindVertexArray(treeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, treeVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, treeEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}



unsigned int indicesW[72000];
objl::Vertex verW[82000];
GLuint woodpeckerVAO, woodpeckerVBO, woodpeckerEBO;

void renderWoodpecker()
{
	// initialize (if necessary)
	if (woodpeckerVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\woodpecker.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verW[j] = v;
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
			indicesW[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &woodpeckerVAO);
		glGenBuffers(1, &woodpeckerVBO);
		glGenBuffers(1, &woodpeckerEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, woodpeckerVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verW), verW, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, woodpeckerEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesW), &indicesW, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(woodpeckerVAO);
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
	glBindVertexArray(woodpeckerVAO);
	glBindBuffer(GL_ARRAY_BUFFER, woodpeckerVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, woodpeckerEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

unsigned int indicesDuck[72000];
objl::Vertex verDuck[82000];
GLuint duckVAO, duckVBO, duckEBO;

void renderDuck()
{
	// initialize (if necessary)
	if (duckVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\duck.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verDuck[j] = v;
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
			indicesDuck[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &duckVAO);
		glGenBuffers(1, &duckVBO);
		glGenBuffers(1, &duckEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, duckVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verDuck), verDuck, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, duckEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesDuck), &indicesDuck, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(duckVAO);
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
	glBindVertexArray(duckVAO);
	glBindBuffer(GL_ARRAY_BUFFER, duckVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, duckEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}


unsigned int indicesPelican[720000];
objl::Vertex verPelican[820000];
GLuint pelicanVAO, pelicanVBO, pelicanEBO;

void renderPelican()
{
	// initialize (if necessary)
	if (pelicanVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\pelican.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verPelican[j] = v;
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
			indicesPelican[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &pelicanVAO);
		glGenBuffers(1, &pelicanVBO);
		glGenBuffers(1, &pelicanEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, pelicanVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verPelican), verPelican, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pelicanEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPelican), &indicesPelican, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(pelicanVAO);
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
	glBindVertexArray(pelicanVAO);
	glBindBuffer(GL_ARRAY_BUFFER, pelicanVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pelicanEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

unsigned int indicesStork[720000];
objl::Vertex verStork[820000];
GLuint storkVAO, storkVBO, storkEBO;

void renderStork()
{
	// initialize (if necessary)
	if (storkVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\stork.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verStork[j] = v;
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
			indicesStork[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &storkVAO);
		glGenBuffers(1, &storkVBO);
		glGenBuffers(1, &storkEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, storkVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verStork), verStork, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, storkEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesStork), &indicesStork, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(storkVAO);
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
	glBindVertexArray(storkVAO);
	glBindBuffer(GL_ARRAY_BUFFER, storkVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, storkEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}


unsigned int indicesParrot[720000];
objl::Vertex verParrot[820000];
GLuint  parrotVAO, parrotVBO, parrotEBO;

void renderParrot()
{
	// initialize (if necessary)
	if (parrotVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\parrot.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verParrot[j] = v;
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
			indicesParrot[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &parrotVAO);
		glGenBuffers(1, &parrotVBO);
		glGenBuffers(1, &parrotEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, parrotVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verParrot), verParrot, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, parrotEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesParrot), &indicesParrot, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(parrotVAO);
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
	glBindVertexArray(parrotVAO);
	glBindBuffer(GL_ARRAY_BUFFER, parrotVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, parrotEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

unsigned int indicesPed[72000];
objl::Vertex verPed[82000];

GLuint pedastalVAO, pedastalVBO, pedastalEBO;

void renderpedastal()
{
	// initialize (if necessary)
	if (pedastalVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\pedastal.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verPed[j] = v;
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
			indicesPed[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &pedastalVAO);
		glGenBuffers(1, &pedastalVBO);
		glGenBuffers(1, &pedastalEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, pedastalVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verPed), verPed, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pedastalEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPed), &indicesPed, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(pedastalVAO);
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
	glBindVertexArray(pedastalVAO);
	glBindBuffer(GL_ARRAY_BUFFER, pedastalVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pedastalEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}


unsigned int indicesDT[720000];
objl::Vertex verDT[900000];

GLuint dinoTeroVAO, dinoTeroVBO, dinoTeroEBO;
void renderDinoTero()
{
	// initialize (if necessary)
	if (dinoTeroVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\terodactil.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verDT[j] = v;
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
			indicesDT[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &dinoTeroVAO);
		glGenBuffers(1, &dinoTeroVBO);
		glGenBuffers(1, &dinoTeroEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, dinoTeroVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verDT), verDT, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dinoTeroEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesDT), &indicesDT, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(dinoTeroVAO);
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
	glBindVertexArray(dinoTeroVAO);
	glBindBuffer(GL_ARRAY_BUFFER, dinoTeroVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dinoTeroEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderPylon()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -20.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			1.0f,  20.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			1.0f, -20.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			1.0f,  20.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -20.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  20.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -20.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			1.0f, -20.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			1.0f,  20.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			1.0f,  20.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  20.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -20.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  20.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  20.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -20.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -20.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -20.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  20.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			1.0f,  20.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			1.0f, -20.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			1.0f,  20.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			1.0f, -20.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			1.0f,  20.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			1.0f, -20.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -20.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			1.0f, -20.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			1.0f, -20.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			1.0f, -20.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -20.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -20.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  20.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			1.0f,  20.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			1.0f,  20.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			1.0f,  20.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  20.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  20.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
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
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void renderGrass(const Shader& shader)
{

	//Grass

	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-37.42f, 0.45f, 18.6f));
	model = glm::scale(model, glm::vec3(2.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	shader.SetMat4("model", model);
	renderGrass();


	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-61.25f, 0.4f, 19.95f));
	model = glm::scale(model, glm::vec3(2.6f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	shader.SetMat4("model", model);
	renderGrass();


}

void renderGrassBush(const Shader& shader)
{

	//GrassBush
	for (int i = 0; i < 5; i++)
	{
		if (i % 2 == 0)
		{
			glm::mat4 model;
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(-60.0f - i * 1.3, -0.2f, -10.5f));
			model = glm::scale(model, glm::vec3(0.003f));
			model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			shader.SetMat4("model", model);
			renderGrassBush();
		}
		else
			if (i % 3 == 0)
			{
				continue;
			}
			else
			{
				glm::mat4 model;
				model = glm::mat4();
				model = glm::translate(model, glm::vec3(-27.0f - i * 1.3, -0.2f, -10.5f));
				model = glm::scale(model, glm::vec3(0.005f));
				model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				shader.SetMat4("model", model);
				renderGrassBush();
			}
	}

	int j = 0;
	for (int i = 0; i < 8; i++)
	{
		if (i % 2 == 0)
		{
			glm::mat4 model;
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(-27.0f - i * 1.3, -0.2f, -17.0f - j));
			model = glm::scale(model, glm::vec3(0.003f));
			model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			shader.SetMat4("model", model);
			renderGrassBush();
		}
		else
			if (i % 3 == 0)
			{
				glm::mat4 model;
				model = glm::mat4();
				model = glm::translate(model, glm::vec3(-27.0f - i * 1.3, -0.2f, -17.0f - j));
				model = glm::scale(model, glm::vec3(0.002f));
				model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				shader.SetMat4("model", model);
				renderGrassBush();
				i = i + 3;
				j = j + 3;
			}
			else
			{
				glm::mat4 model;
				model = glm::mat4();
				model = glm::translate(model, glm::vec3(-27.0f - i * 1.3, -0.2f, -17.0f - j));
				model = glm::scale(model, glm::vec3(0.005f));
				model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				shader.SetMat4("model", model);
				renderGrassBush();
			}
	}
	int k = 6;
	for (int i = 8; i < 30; i++)
	{
		if (i % 2 == 0)
		{
			glm::mat4 model;
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(-27.0f - i * 1.3, -0.2f, -17.0f + k));
			model = glm::scale(model, glm::vec3(0.003f));
			model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			shader.SetMat4("model", model);
			renderGrassBush();
		}
		else
			if (i % 3 == 0)
			{
				glm::mat4 model;
				model = glm::mat4();
				model = glm::translate(model, glm::vec3(-27.0f - i * 1.3, -0.2f, -17.0f + k));
				model = glm::scale(model, glm::vec3(0.002f));
				model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				shader.SetMat4("model", model);
				renderGrassBush();
				i = i + 3;
				k = k - 3;
			}
			else
			{
				glm::mat4 model;
				model = glm::mat4();
				model = glm::translate(model, glm::vec3(-27.0f - i * 1.3, -0.2f, -17.0f + k));
				model = glm::scale(model, glm::vec3(0.005f));
				model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				shader.SetMat4("model", model);
				renderGrassBush();
			}
	}

	for (int a = 0; a < 2; a++)
	{
		int l = 0;
		for (int i = 0; i < 5; i++)
		{
			if (i % 2 == 0)
			{
				glm::mat4 model;
				model = glm::mat4();
				model = glm::translate(model, glm::vec3(-27.0f - i * 1.3, -0.2f, 17.0f - l - a));
				model = glm::scale(model, glm::vec3(0.003f));
				model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				shader.SetMat4("model", model);
				renderGrassBush();
			}
			else
				if (i % 3 == 0)
				{
					glm::mat4 model;
					model = glm::mat4();
					model = glm::translate(model, glm::vec3(-27.0f - i * 1.3, -0.2f, 17.0f - l - a));
					model = glm::scale(model, glm::vec3(0.002f));
					model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0f));

					shader.SetMat4("model", model);
					renderGrassBush();
					i = i + 3;
					l = l + 3;
				}
				else
				{
					glm::mat4 model;
					model = glm::mat4();
					model = glm::translate(model, glm::vec3(-27.0f - i * 1.3, -0.2f, 17.0f - l - a));
					model = glm::scale(model, glm::vec3(0.005f));
					model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0f));

					shader.SetMat4("model", model);
					renderGrassBush();
				}
		}

		int m = 6;
		for (int i = 8; i < 30; i++)
		{
			if (i % 2 == 0)
			{
				glm::mat4 model;
				model = glm::mat4();
				model = glm::translate(model, glm::vec3(-27.0f - i * 1.3, -0.2f, 17.0f + m - a));
				model = glm::scale(model, glm::vec3(0.003f));
				model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				shader.SetMat4("model", model);
				renderGrassBush();
			}
			else
				if (i % 3 == 0)
				{
					glm::mat4 model;
					model = glm::mat4();
					model = glm::translate(model, glm::vec3(-27.0f - i * 1.3, -0.2f, 17.0f + m - a));
					model = glm::scale(model, glm::vec3(0.002f));
					model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0f));

					shader.SetMat4("model", model);
					renderGrassBush();
					i = i + 3;
					m = m - 3;
				}
				else
				{
					glm::mat4 model;
					model = glm::mat4();
					model = glm::translate(model, glm::vec3(-27.0f - i * 1.3, -0.2f, 17.0f + m - a));
					model = glm::scale(model, glm::vec3(0.005f));
					model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0f));

					shader.SetMat4("model", model);
					renderGrassBush();
				}
		}
	}
}


void renderGround(const Shader& shader)
{

	//Ground

	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-37.42f, 0.45f, -18.6f));
	model = glm::scale(model, glm::vec3(2.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	shader.SetMat4("model", model);
	renderGround();


	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-61.25f, 0.4f, -19.95f));
	model = glm::scale(model, glm::vec3(2.6f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	shader.SetMat4("model", model);
	renderGround();


}

unsigned int indicesGr[72000];
objl::Vertex verGr[82000];
GLuint GrassVAO, GrassVBO, GrassEBO;

void renderGrass()
{
	// initialize (if necessary)
	if (GrassVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\Grass.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verGr[j] = v;
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
			indicesGr[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &GrassVAO);
		glGenBuffers(1, &GrassVBO);
		glGenBuffers(1, &GrassEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, GrassVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verGr), verGr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GrassEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesGr), &indicesGr, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(GrassVAO);
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
	glBindVertexArray(GrassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, GrassVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GrassEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

unsigned int indicesGb[720000];
objl::Vertex verGb[1000000];
GLuint GrassBushVAO, GrassBushVBO, GrassBushEBO;

void renderGrassBush()
{
	// initialize (if necessary)
	if (GrassBushVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\GrassBush.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verGb[j] = v;
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
			indicesGb[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &GrassBushVAO);
		glGenBuffers(1, &GrassBushVBO);
		glGenBuffers(1, &GrassBushEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, GrassBushVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verGb), verGb, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GrassBushEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesGb), &indicesGb, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(GrassBushVAO);
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
	glBindVertexArray(GrassBushVAO);
	glBindBuffer(GL_ARRAY_BUFFER, GrassBushVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GrassBushEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

unsigned int indicesGo[720000];
objl::Vertex verGo[1000000];
GLuint GroundVAO, GroundVBO, GroundEBO;

void renderGround()
{
	// initialize (if necessary)
	if (GroundVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\Ground.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verGo[j] = v;
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
			indicesGo[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &GroundVAO);
		glGenBuffers(1, &GroundVBO);
		glGenBuffers(1, &GroundEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, GroundVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verGo), verGo, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GroundEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesGo), &indicesGo, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(GroundVAO);
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
	glBindVertexArray(GroundVAO);
	glBindBuffer(GL_ARRAY_BUFFER, GroundVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GroundEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void renderDino3(const Shader& shader)
{
	//dino
	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-58.5f, 0.0f, -13.4f));
	model = glm::scale(model, glm::vec3(0.2f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.SetMat4("model", model);
	renderDino3();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-54.5f, 0.0f, -13.4f));
	model = glm::scale(model, glm::vec3(0.13f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.SetMat4("model", model);
	renderDino3();


}

unsigned int indicesDT3[720000];
objl::Vertex verDT3[900000];

GLuint dino3VAO, dino3VBO, dino3EBO;
void renderDino3()
{
	// initialize (if necessary)
	if (dino3VAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\dino3.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verDT3[j] = v;
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
			indicesDT3[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &dino3VAO);
		glGenBuffers(1, &dino3VBO);
		glGenBuffers(1, &dino3EBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, dino3VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verDT3), verDT3, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dino3EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesDT3), &indicesDT3, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(dino3VAO);
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
	glBindVertexArray(dino3VAO);
	glBindBuffer(GL_ARRAY_BUFFER, dino3VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dino3EBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void renderDino2(const Shader& shader)
{
	//dino
	glm::mat4 model;



	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-40.5f, 0.0f, -13.4f));
	model = glm::scale(model, glm::vec3(1.30f));
	model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.SetMat4("model", model);
	renderDino2();

}

unsigned int indicesDT2[720000];
objl::Vertex verDT2[900000];

GLuint dino2VAO, dino2VBO, dino2EBO;
void renderDino2()
{
	// initialize (if necessary)
	if (dino2VAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\dino2.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verDT2[j] = v;
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
			indicesDT2[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &dino2VAO);
		glGenBuffers(1, &dino2VBO);
		glGenBuffers(1, &dino2EBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, dino2VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verDT2), verDT2, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dino2EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesDT2), &indicesDT2, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(dino2VAO);
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
	glBindVertexArray(dino2VAO);
	glBindBuffer(GL_ARRAY_BUFFER, dino2VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dino2EBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void renderDinoSkull(const Shader& shader)
{
	glm::mat4 model;
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(12.0f, -0.5f, -24.5f));
	model = glm::scale(model, glm::vec3(4.0f));
	shader.SetMat4("model", model);
	renderTrexBottom();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(12.0f, -0.5f, -24.5f));
	model = glm::scale(model, glm::vec3(4.0f));
	shader.SetMat4("model", model);
	renderTrexTop();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(12.0f, -0.5f, -50.5f));
	model = glm::scale(model, glm::vec3(4.0f));
	shader.SetMat4("model", model);
	renderTrexBottom();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(12.0f, -0.5f, -50.5f));
	model = glm::scale(model, glm::vec3(4.0f));
	shader.SetMat4("model", model);
	renderTrexTop();


}

float vertices5[82000];
unsigned int indices5[72000];
unsigned int indicesD1[72000];
objl::Vertex verD1[82000];
GLuint trexTopVAO, trexTopVBO, trexTopEBO;
void renderTrexTop()
{
	if (trexTopVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\Trex.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[1];
		int size = curMesh.Vertices.size();
		objl::Vertex v;
		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			v.Position.X = (float)curMesh.Vertices[j].Position.X;
			v.Position.Y = (float)curMesh.Vertices[j].Position.Y;
			v.Position.Z = (float)curMesh.Vertices[j].Position.Z;
			v.Normal.X = (float)curMesh.Vertices[j].Normal.X;
			v.Normal.Y = (float)curMesh.Vertices[j].Normal.Y;
			v.Normal.Z = (float)curMesh.Vertices[j].Normal.Z;
			v.TextureCoordinate.X = (float)curMesh.Vertices[j].TextureCoordinate.X;
			v.TextureCoordinate.Y = (float)curMesh.Vertices[j].TextureCoordinate.Y;


			verD1[j] = v;
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
			indicesD1[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &trexTopVAO);
		glGenBuffers(1, &trexTopVBO);
		glGenBuffers(1, &trexTopEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, trexTopVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verD1), verD1, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trexTopEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesD1), &indicesD1, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(trexTopVAO);
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
	glBindVertexArray(trexTopVAO);
	glBindBuffer(GL_ARRAY_BUFFER, trexTopVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trexTopEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

float vertices6[82000];
unsigned int indices6[72000];
GLuint trexBottomVAO, trexBottomVBO, trexBottomEBO;
void renderTrexBottom()
{
	if (trexBottomVAO == 0)
	{

		std::vector<float> verticess;
		std::vector<float> indicess;



		Loader.LoadFile("..\\OBJ\\Trex.obj");
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
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
			vertices6[j] = verticess.at(j);
		}

		for (int j = 0; j < curMesh.Indices.size(); j++)
		{

			indicess.push_back((float)curMesh.Indices[j]);

		}
		for (int j = 0; j < curMesh.Indices.size(); j++)
		{
			indices6[j] = indicess.at(j);
		}

		glGenVertexArrays(1, &trexBottomVAO);
		glGenBuffers(1, &trexBottomVBO);
		glGenBuffers(1, &trexBottomEBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, trexBottomVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices6), vertices6, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trexBottomEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices6), &indices6, GL_DYNAMIC_DRAW);
		// link vertex attributes
		glBindVertexArray(trexBottomVAO);
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
	glBindVertexArray(trexBottomVAO);
	glBindBuffer(GL_ARRAY_BUFFER, trexBottomVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trexBottomEBO);
	int indexArraySize;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
	glDrawElements(GL_TRIANGLES, indexArraySize / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}