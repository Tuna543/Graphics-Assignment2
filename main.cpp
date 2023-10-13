//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "table_sofa.h"
#include "fan.h"
#include "tool.h"
#include "cylinders.h"
#include "glass.h"
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0;
float rotateAngle_Y = 0;
float rotateAngle_Z = 0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
bool fan_turn = false;
bool rotate_around = false;
// camera
Camera camera(glm::vec3(0.0f, 2.5f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 0.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

glm::mat4 transforamtion(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz) {
	glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
	translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
	rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
	rotateYMatrix = glm::rotate(identityMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
	rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
	scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
	model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
	return model;
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("vertexShader.vs", "fragmentShader.fs");
	//0.5686f, 0.3529f, 0.2039f,
	//VAO
	// deep brown color 
	float table_top[] = {
		0.0f, 0.0f, 0.0f, 0.5686f, 0.3529f, 0.2039f,
		0.5f, 0.0f, 0.0f, 0.5686f, 0.3529f, 0.2039f,
		0.5f, 0.5f, 0.0f, 0.5686f, 0.3529f, 0.2039f,
		0.0f, 0.5f, 0.0f, 0.5686f, 0.3529f, 0.2039f,

		0.5f, 0.0f, 0.0f, 0.5686f, 0.3529f, 0.2039f,
		0.5f, 0.5f, 0.0f, 0.5686f, 0.3529f, 0.2039f,
		0.5f, 0.0f, 0.5f, 0.5686f, 0.3529f, 0.2039f,
		0.5f, 0.5f, 0.5f, 0.5686f, 0.3529f, 0.2039f,

		0.0f, 0.0f, 0.5f, 0.5686f, 0.3529f, 0.2039f,
		0.5f, 0.0f, 0.5f, 0.5686f, 0.3529f, 0.2039f,
		0.5f, 0.5f, 0.5f, 0.5686f, 0.3529f, 0.2039f,
		0.0f, 0.5f, 0.5f, 0.5686f, 0.3529f, 0.2039f,

		0.0f, 0.0f, 0.5f, 0.5686f, 0.3529f, 0.2039f,
		0.0f, 0.5f, 0.5f, 0.5686f, 0.3529f, 0.2039f,
		0.0f, 0.5f, 0.0f, 0.5686f, 0.3529f, 0.2039f,
		0.0f, 0.0f, 0.0f, 0.5686f, 0.3529f, 0.2039f,

		0.5f, 0.5f, 0.5f, 0.5686f, 0.3529f, 0.2039f,
		0.5f, 0.5f, 0.0f, 0.5686f, 0.3529f, 0.2039f,
		0.0f, 0.5f, 0.0f, 0.5686f, 0.3529f, 0.2039f,
		0.0f, 0.5f, 0.5f, 0.5686f, 0.3529f, 0.2039f,

		0.0f, 0.0f, 0.0f, 0.5686f, 0.3529f, 0.2039f,
		0.5f, 0.0f, 0.0f, 0.5686f, 0.3529f, 0.2039f,
		0.5f, 0.0f, 0.5f, 0.5686f, 0.3529f, 0.2039f,
		0.0f, 0.0f, 0.5f, 0.5686f, 0.3529f, 0.2039f
	};
	//.58f,0.573f,0.576f,
	//VAO2
	//deep ash color
	float table_leg[] = {
		0.0f, 0.0f, 0.0f, .58f,0.573f,0.576f,
		0.5f, 0.0f, 0.0f, .58f,0.573f,0.576f,
		0.5f, 0.5f, 0.0f, .58f,0.573f,0.576f,
		0.0f, 0.5f, 0.0f, .58f,0.573f,0.576f,

		0.5f, 0.0f, 0.0f, .58f,0.573f,0.576f,
		0.5f, 0.5f, 0.0f, .58f,0.573f,0.576f,
		0.5f, 0.0f, 0.5f, .58f,0.573f,0.576f,
		0.5f, 0.5f, 0.5f, .58f,0.573f,0.576f,

		0.0f, 0.0f, 0.5f, .58f,0.573f,0.576f,
		0.5f, 0.0f, 0.5f, .58f,0.573f,0.576f,
		0.5f, 0.5f, 0.5f, .58f,0.573f,0.576f,
		0.0f, 0.5f, 0.5f, .58f,0.573f,0.576f,

		0.0f, 0.0f, 0.5f, .58f,0.573f,0.576f,
		0.0f, 0.5f, 0.5f, .58f,0.573f,0.576f,
		0.0f, 0.5f, 0.0f, .58f,0.573f,0.576f,
		0.0f, 0.0f, 0.0f, .58f,0.573f,0.576f,

		0.5f, 0.5f, 0.5f, .58f,0.573f,0.576f,
		0.5f, 0.5f, 0.0f, .58f,0.573f,0.576f,
		0.0f, 0.5f, 0.0f, .58f,0.573f,0.576f,
		0.0f, 0.5f, 0.5f, .58f,0.573f,0.576f,

		0.0f, 0.0f, 0.0f, .58f,0.573f,0.576f,
		0.5f, 0.0f, 0.0f, .58f,0.573f,0.576f,
		0.5f, 0.0f, 0.5f, .58f,0.573f,0.576f,
		0.0f, 0.0f, 0.5f, .58f,0.573f,0.576f
	};
	//.322f,0.322f,0.322f,
	//vao3 tools light ash color
	float chair_leg[] = {
		0.0f, 0.0f, 0.0f, .322f,0.322f,0.322f,
		0.5f, 0.0f, 0.0f, .322f,0.322f,0.322f,
		0.5f, 0.5f, 0.0f, .322f,0.322f,0.322f,
		0.0f, 0.5f, 0.0f, .322f,0.322f,0.322f,

		0.5f, 0.0f, 0.0f, .322f,0.322f,0.322f,
		0.5f, 0.5f, 0.0f, .322f,0.322f,0.322f,
		0.5f, 0.0f, 0.5f, .322f,0.322f,0.322f,
		0.5f, 0.5f, 0.5f, .322f,0.322f,0.322f,

		0.0f, 0.0f, 0.5f, .322f,0.322f,0.322f,
		0.5f, 0.0f, 0.5f, .322f,0.322f,0.322f,
		0.5f, 0.5f, 0.5f, .322f,0.322f,0.322f,
		0.0f, 0.5f, 0.5f, .322f,0.322f,0.322f,

		0.0f, 0.0f, 0.5f, .322f,0.322f,0.322f,
		0.0f, 0.5f, 0.5f, .322f,0.322f,0.322f,
		0.0f, 0.5f, 0.0f, .322f,0.322f,0.322f,
		0.0f, 0.0f, 0.0f, .322f,0.322f,0.322f,

		0.5f, 0.5f, 0.5f, .322f,0.322f,0.322f,
		0.5f, 0.5f, 0.0f, .322f,0.322f,0.322f,
		0.0f, 0.5f, 0.0f, .322f,0.322f,0.322f,
		0.0f, 0.5f, 0.5f, .322f,0.322f,0.322f,

		0.0f, 0.0f, 0.0f, .322f,0.322f,0.322f,
		0.5f, 0.0f, 0.0f, .322f,0.322f,0.322f,
		0.5f, 0.0f, 0.5f, .322f,0.322f,0.322f,
		0.0f, 0.0f, 0.5f, .322f,0.322f,0.322f
	};
	//0.6390f, 0.34f, 0.2745f,
	float chair_sides[] = {
		0.0f, 0.0f, 0.0f, 0.6390f, 0.34f, 0.2745f,
		0.5f, 0.0f, 0.0f, 0.6390f, 0.34f, 0.2745f,
		0.5f, 0.5f, 0.0f, 0.6390f, 0.34f, 0.2745f,
		0.0f, 0.5f, 0.0f, 0.6390f, 0.34f, 0.2745f,

		0.5f, 0.0f, 0.0f, 0.6390f, 0.34f, 0.2745f,
		0.5f, 0.5f, 0.0f, 0.6390f, 0.34f, 0.2745f,
		0.5f, 0.0f, 0.5f, 0.6390f, 0.34f, 0.2745f,
		0.5f, 0.5f, 0.5f, 0.6390f, 0.34f, 0.2745f,

		0.0f, 0.0f, 0.5f, 0.6390f, 0.34f, 0.2745f,
		0.5f, 0.0f, 0.5f, 0.6390f, 0.34f, 0.2745f,
		0.5f, 0.5f, 0.5f, 0.6390f, 0.34f, 0.2745f,
		0.0f, 0.5f, 0.5f, 0.6390f, 0.34f, 0.2745f,

		0.0f, 0.0f, 0.5f, 0.6390f, 0.34f, 0.2745f,
		0.0f, 0.5f, 0.5f, 0.6390f, 0.34f, 0.2745f,
		0.0f, 0.5f, 0.0f, 0.6390f, 0.34f, 0.2745f,
		0.0f, 0.0f, 0.0f, 0.6390f, 0.34f, 0.2745f,

		0.5f, 0.5f, 0.5f, 0.6390f, 0.34f, 0.2745f,
		0.5f, 0.5f, 0.0f, 0.6390f, 0.34f, 0.2745f,
		0.0f, 0.5f, 0.0f, 0.6390f, 0.34f, 0.2745f,
		0.0f, 0.5f, 0.5f, 0.6390f, 0.34f, 0.2745f,

		0.0f, 0.0f, 0.0f, 0.6390f, 0.34f, 0.2745f,
		0.5f, 0.0f, 0.0f, 0.6390f, 0.34f, 0.2745f,
		0.5f, 0.0f, 0.5f, 0.6390f, 0.34f, 0.2745f,
		0.0f, 0.0f, 0.5f, 0.6390f, 0.34f, 0.2745f
	};
	//0.643f, 0.2039f, 0.243f,
	float chair_back[] = {
		0.0f, 0.0f, 0.0f, 0.643f, 0.2039f, 0.243f,
		0.5f, 0.0f, 0.0f, 0.643f, 0.2039f, 0.243f,
		0.5f, 0.5f, 0.0f, 0.643f, 0.2039f, 0.243f,
		0.0f, 0.5f, 0.0f, 0.643f, 0.2039f, 0.243f,

		0.5f, 0.0f, 0.0f, 0.643f, 0.2039f, 0.243f,
		0.5f, 0.5f, 0.0f, 0.643f, 0.2039f, 0.243f,
		0.5f, 0.0f, 0.5f, 0.643f, 0.2039f, 0.243f,
		0.5f, 0.5f, 0.5f, 0.643f, 0.2039f, 0.243f,

		0.0f, 0.0f, 0.5f, 0.643f, 0.2039f, 0.243f,
		0.5f, 0.0f, 0.5f, 0.643f, 0.2039f, 0.243f,
		0.5f, 0.5f, 0.5f, 0.643f, 0.2039f, 0.243f,
		0.0f, 0.5f, 0.5f, 0.643f, 0.2039f, 0.243f,

		0.0f, 0.0f, 0.5f, 0.643f, 0.2039f, 0.243f,
		0.0f, 0.5f, 0.5f, 0.643f, 0.2039f, 0.243f,
		0.0f, 0.5f, 0.0f, 0.643f, 0.2039f, 0.243f,
		0.0f, 0.0f, 0.0f, 0.643f, 0.2039f, 0.243f,

		0.5f, 0.5f, 0.5f, 0.643f, 0.2039f, 0.243f,
		0.5f, 0.5f, 0.0f, 0.643f, 0.2039f, 0.243f,
		0.0f, 0.5f, 0.0f, 0.643f, 0.2039f, 0.243f,
		0.0f, 0.5f, 0.5f, 0.643f, 0.2039f, 0.243f,

		0.0f, 0.0f, 0.0f, 0.643f, 0.2039f, 0.243f,
		0.5f, 0.0f, 0.0f, 0.643f, 0.2039f, 0.243f,
		0.5f, 0.0f, 0.5f, 0.643f, 0.2039f, 0.243f,
		0.0f, 0.0f, 0.5f, 0.643f, 0.2039f, 0.243f
	};
	//0.69f, 0.69f, 0.69f,
	float floor[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,

		0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f
	};
	//pink color
	//1.0f, 0.72f, 0.8f,
	float side_walls[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 0.72f, 0.8f,
		0.5f, 0.0f, 0.0f, 1.0f, 0.72f, 0.8f,
		0.5f, 0.5f, 0.0f, 1.0f, 0.72f, 0.8f,
		0.0f, 0.5f, 0.0f, 1.0f, 0.72f, 0.8f,

		0.5f, 0.0f, 0.0f, 1.0f, 0.72f, 0.8f,
		0.5f, 0.5f, 0.0f, 1.0f, 0.72f, 0.8f,
		0.5f, 0.0f, 0.5f, 1.0f, 0.72f, 0.8f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.72f, 0.8f,

		0.0f, 0.0f, 0.5f, 1.0f, 0.72f, 0.8f,
		0.5f, 0.0f, 0.5f, 1.0f, 0.72f, 0.8f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.72f, 0.8f,
		0.0f, 0.5f, 0.5f, 1.0f, 0.72f, 0.8f,

		0.0f, 0.0f, 0.5f, 1.0f, 0.72f, 0.8f,
		0.0f, 0.5f, 0.5f, 1.0f, 0.72f, 0.8f,
		0.0f, 0.5f, 0.0f, 1.0f, 0.72f, 0.8f,
		0.0f, 0.0f, 0.0f, 1.0f, 0.72f, 0.8f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.72f, 0.8f,
		0.5f, 0.5f, 0.0f, 1.0f, 0.72f, 0.8f,
		0.0f, 0.5f, 0.0f, 1.0f, 0.72f, 0.8f,
		0.0f, 0.5f, 0.5f, 1.0f, 0.72f, 0.8f,

		0.0f, 0.0f, 0.0f, 1.0f, 0.72f, 0.8f,
		0.5f, 0.0f, 0.0f, 1.0f, 0.72f, 0.8f,
		0.5f, 0.0f, 0.5f, 1.0f, 0.72f, 0.8f,
		0.0f, 0.0f, 0.5f, 1.0f, 0.72f, 0.8f
	};

	//0.99f, 0.84f, 0.70f,
	float front_back_walls[] = {
		0.0f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,

		0.5f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,

		0.0f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,

		0.0f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,

		0.5f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,

		0.0f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f
	};



	//0f, 0f, 0f,
	float bar_table[] = {
		0.0f, 0.0f, 0.0f, 0.137f, 0.1176f, 0.1098f,
		0.5f, 0.0f, 0.0f, 0.137f, 0.1176f, 0.1098f,
		0.5f, 0.5f, 0.0f, 0.137f, 0.1176f, 0.1098f,
		0.0f, 0.5f, 0.0f, 0.137f, 0.1176f, 0.1098f,

		0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 0.137f, 0.1176f, 0.1098f,
		0.5f, 0.5f, 0.0f, 0.137f, 0.1176f, 0.1098f,
		0.0f, 0.5f, 0.0f, 0.137f, 0.1176f, 0.1098f,
		0.0f, 0.5f, 0.5f, 0.137f, 0.1176f, 0.1098f,

		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f
	};
	//purple color
	float hangerwall[] = {
		0.0f, 0.0f, 0.0f, 0.635f, 0.396f, 0.588f,
		0.5f, 0.0f, 0.0f, 0.635f, 0.396f, 0.588f,
		0.5f, 0.5f, 0.0f, 0.635f, 0.396f, 0.588f,
		0.0f, 0.5f, 0.0f, 0.635f, 0.396f, 0.588,

		0.5f, 0.0f, 0.0f, 0.635f, 0.396f, 0.588f,
		0.5f, 0.5f, 0.0f, 0.635f, 0.396f, 0.588f,
		0.5f, 0.0f, 0.5f, 0.635f, 0.396f, 0.588f,
		0.5f, 0.5f, 0.5f, 0.635f, 0.396f, 0.588f,

		0.0f, 0.0f, 0.5f, 0.635f, 0.396f, 0.588f,
		0.5f, 0.0f, 0.5f, 0.635f, 0.396f, 0.588f,
		0.5f, 0.5f, 0.5f, 0.635f, 0.396f, 0.588f,
		0.0f, 0.5f, 0.5f, 0.635f, 0.396f, 0.588f,

		0.0f, 0.0f, 0.5f, 0.635f, 0.396f, 0.588f,
		0.0f, 0.5f, 0.5f, 0.635f, 0.396f, 0.588f,
		0.0f, 0.5f, 0.0f, 0.635f, 0.396f, 0.588f,
		0.0f, 0.0f, 0.0f, 0.635f, 0.396f, 0.588f,

		0.5f, 0.5f, 0.5f, 0.6588f, 0.6588f, 0.6588f,
		0.5f, 0.5f, 0.0f, 0.6588f, 0.6588f, 0.6588f,
		0.0f, 0.5f, 0.0f, 0.6588f, 0.6588f, 0.6588f,
		0.0f, 0.5f, 0.5f, 0.6588f, 0.6588f, 0.6588f,

		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f
	};

	//0.29f, 0.0f, 0.29f,
	float cabinate[] = {
		0.0f, 0.0f, 0.0f, 00.42f, 0.32f, 0.26f,
		0.5f, 0.0f, 0.0f, 00.42f, 0.32f, 0.26f,
		0.5f, 0.5f, 0.0f, 00.42f, 0.32f, 0.26f,
		0.0f, 0.5f, 0.0f, 00.42f, 0.32f, 0.26f,

		0.5f, 0.0f, 0.0f, 00.42f, 0.32f, 0.26f,
		0.5f, 0.5f, 0.0f, 00.42f, 0.32f, 0.26f,
		0.5f, 0.0f, 0.5f, 00.42f, 0.32f, 0.26f,
		0.5f, 0.5f, 0.5f, 00.42f, 0.32f, 0.26f,

		0.0f, 0.0f, 0.5f, 00.42f, 0.32f, 0.26f,
		0.5f, 0.0f, 0.5f, 00.42f, 0.32f, 0.26f,
		0.5f, 0.5f, 0.5f, 00.42f, 0.32f, 0.26f,
		0.0f, 0.5f, 0.5f, 00.42f, 0.32f, 0.26f,

		0.0f, 0.0f, 0.5f, 00.42f, 0.32f, 0.26f,
		0.0f, 0.5f, 0.5f, 00.42f, 0.32f, 0.26f,
		0.0f, 0.5f, 0.0f, 00.42f, 0.32f, 0.26f,
		0.0f, 0.0f, 0.0f, 00.42f, 0.32f, 0.26f,

		0.5f, 0.5f, 0.5f, 0.78f, 0.66f, 0.443f,
		0.5f, 0.5f, 0.0f, 0.78f, 0.66f, 0.443f,
		0.0f, 0.5f, 0.0f, 0.78f, 0.66f, 0.443f,
		0.0f, 0.5f, 0.5f, 0.78f, 0.66f, 0.443f,

		0.0f, 0.0f, 0.0f, 0.78f, 0.66f, 0.443f,
		0.5f, 0.0f, 0.0f, 0.78f, 0.66f, 0.443f,
		0.5f, 0.0f, 0.5f, 0.78f, 0.66f, 0.443f,
		0.0f, 0.0f, 0.5f, 0.78f, 0.66f, 0.443f
	};
	//0.892f, 0.945f, 0.9686f,
	float ceiling[] = {
		0.0f, 0.0f, 0.0f, 0.892f, 0.945f, 0.9686f,
		0.5f, 0.0f, 0.0f, 0.892f, 0.945f, 0.9686f,
		0.5f, 0.5f, 0.0f, 0.892f, 0.945f, 0.9686f,
		0.0f, 0.5f, 0.0f, 0.892f, 0.945f, 0.9686f,

		0.5f, 0.0f, 0.0f, 0.892f, 0.945f, 0.9686f,
		0.5f, 0.5f, 0.0f, 0.892f, 0.945f, 0.9686f,
		0.5f, 0.0f, 0.5f, 0.892f, 0.945f, 0.9686f,
		0.5f, 0.5f, 0.5f, 0.892f, 0.945f, 0.9686f,

		0.0f, 0.0f, 0.5f, 0.892f, 0.945f, 0.9686f,
		0.5f, 0.0f, 0.5f, 0.892f, 0.945f, 0.9686f,
		0.5f, 0.5f, 0.5f, 0.892f, 0.945f, 0.9686f,
		0.0f, 0.5f, 0.5f, 0.892f, 0.945f, 0.9686f,

		0.0f, 0.0f, 0.5f, 0.892f, 0.945f, 0.9686f,
		0.0f, 0.5f, 0.5f, 0.892f, 0.945f, 0.9686f,
		0.0f, 0.5f, 0.0f, 0.892f, 0.945f, 0.9686f,
		0.0f, 0.0f, 0.0f, 0.892f, 0.945f, 0.9686f,

		0.5f, 0.5f, 0.5f, 0.892f, 0.945f, 0.9686f,
		0.5f, 0.5f, 0.0f, 0.892f, 0.945f, 0.9686f,
		0.0f, 0.5f, 0.0f, 0.892f, 0.945f, 0.9686f,
		0.0f, 0.5f, 0.5f, 0.892f, 0.945f, 0.9686f,

		0.0f, 0.0f, 0.0f, 0.892f, 0.945f, 0.9686f,
		0.5f, 0.0f, 0.0f, 0.892f, 0.945f, 0.9686f,
		0.5f, 0.0f, 0.5f, 0.892f, 0.945f, 0.9686f,
		0.0f, 0.0f, 0.5f, 0.892f, 0.945f, 0.9686f
	};

	//1.0f, 1.0f, 1.0f,
	float fan_cup[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,

		0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f
	};
	//0.42f, 0.32f, 0.26f,
	float fan_hanging_rod[] = {
		0.0f, 0.0f, 0.0f, 0.42f, 0.32f, 0.26f,
		0.5f, 0.0f, 0.0f, 0.42f, 0.32f, 0.26f,
		0.5f, 0.5f, 0.0f, 0.42f, 0.32f, 0.26f,
		0.0f, 0.5f, 0.0f, 0.42f, 0.32f, 0.26f,

		0.5f, 0.0f, 0.0f, 0.42f, 0.32f, 0.26f,
		0.5f, 0.5f, 0.0f, 0.42f, 0.32f, 0.26f,
		0.5f, 0.0f, 0.5f, 0.42f, 0.32f, 0.26f,
		0.5f, 0.5f, 0.5f, 0.42f, 0.32f, 0.26f,

		0.0f, 0.0f, 0.5f, 0.42f, 0.32f, 0.26f,
		0.5f, 0.0f, 0.5f, 0.42f, 0.32f, 0.26f,
		0.5f, 0.5f, 0.5f, 0.42f, 0.32f, 0.26f,
		0.0f, 0.5f, 0.5f, 0.42f, 0.32f, 0.26f,

		0.0f, 0.0f, 0.5f, 0.42f, 0.32f, 0.26f,
		0.0f, 0.5f, 0.5f, 0.42f, 0.32f, 0.26f,
		0.0f, 0.5f, 0.0f, 0.42f, 0.32f, 0.26f,
		0.0f, 0.0f, 0.0f, 0.42f, 0.32f, 0.26f,

		0.5f, 0.5f, 0.5f, 0.42f, 0.32f, 0.26f,
		0.5f, 0.5f, 0.0f, 0.42f, 0.32f, 0.26f,
		0.0f, 0.5f, 0.0f, 0.42f, 0.32f, 0.26f,
		0.0f, 0.5f, 0.5f, 0.42f, 0.32f, 0.26f,

		0.0f, 0.0f, 0.0f, 0.42f, 0.32f, 0.26f,
		0.5f, 0.0f, 0.0f, 0.42f, 0.32f, 0.26f,
		0.5f, 0.0f, 0.5f, 0.42f, 0.32f, 0.26f,
		0.0f, 0.0f, 0.5f, 0.42f, 0.32f, 0.26f
	};
	//0.722f, 0.6f, 0.443f,
	float fan_blade[] = {
		0.0f, 0.0f, 0.0f, 0.722f, 0.6f, 0.443f,
		0.5f, 0.0f, 0.0f, 0.722f, 0.6f, 0.443f,
		0.5f, 0.5f, 0.0f, 0.722f, 0.6f, 0.443f,
		0.0f, 0.5f, 0.0f, 0.722f, 0.6f, 0.443f,

		0.5f, 0.0f, 0.0f, 0.722f, 0.6f, 0.443f,
		0.5f, 0.5f, 0.0f, 0.722f, 0.6f, 0.443f,
		0.5f, 0.0f, 0.5f, 0.722f, 0.6f, 0.443f,
		0.5f, 0.5f, 0.5f, 0.722f, 0.6f, 0.443f,

		0.0f, 0.0f, 0.5f, 0.722f, 0.6f, 0.443f,
		0.5f, 0.0f, 0.5f, 0.722f, 0.6f, 0.443f,
		0.5f, 0.5f, 0.5f, 0.722f, 0.6f, 0.443f,
		0.0f, 0.5f, 0.5f, 0.722f, 0.6f, 0.443f,

		0.0f, 0.0f, 0.5f, 0.722f, 0.6f, 0.443f,
		0.0f, 0.5f, 0.5f, 0.722f, 0.6f, 0.443f,
		0.0f, 0.5f, 0.0f, 0.722f, 0.6f, 0.443f,
		0.0f, 0.0f, 0.0f, 0.722f, 0.6f, 0.443f,

		0.5f, 0.5f, 0.5f, 0.722f, 0.6f, 0.443f,
		0.5f, 0.5f, 0.0f, 0.722f, 0.6f, 0.443f,
		0.0f, 0.5f, 0.0f, 0.722f, 0.6f, 0.443f,
		0.0f, 0.5f, 0.5f, 0.722f, 0.6f, 0.443f,

		0.0f, 0.0f, 0.0f, 0.722f, 0.6f, 0.443f,
		0.5f, 0.0f, 0.0f, 0.722f, 0.6f, 0.443f,
		0.5f, 0.0f, 0.5f, 0.722f, 0.6f, 0.443f,
		0.0f, 0.0f, 0.5f, 0.722f, 0.6f, 0.443f
	};

	//12 triangles for a cube
	unsigned int cube_indices[] = {
		0, 3, 2,
		2, 1, 0,

		4, 5, 7,
		7, 6, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};



	float black_color[] = {
		0.0f, 0.0f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.0f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.5f, 0.0f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.0f, .0f, .0f, .0f,

		0.5f, 0.0f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.5f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.0f, 0.5f, .0f, .0f, .0f,
		0.5f, 0.5f, 0.5f, .0f, .0f, .0f,

		0.0f, 0.0f, 0.5f, .0f, .0f, .0f,
		0.5f, 0.0f, 0.5f, .0f, .0f, .0f,
		0.5f, 0.5f, 0.5f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.5f, .0f, .0f, .0f,

		0.0f, 0.0f, 0.5f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.5f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.0f, .0f, .0f, .0f,
		0.0f, 0.0f, 0.0f, .0f, .0f, .0f,

		0.5f, 0.5f, 0.5f, .0f, .0f, .0f,
		0.5f, 0.5f, 0.0f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.0f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.5f, .0f, .0f, .0f,

		0.0f, 0.0f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.0f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.0f, 0.5f, .0f, .0f, .0f,
		0.0f, 0.0f, 0.5f, .0f, .0f, .0f
	};

	float ver_arr[] = {

		1.0f, 1.0f, 0.0f , .145f,.9755f,.96f,
		0.809017f, 1.0f, 0.587785f ,.145f,.9755f,.96f,
		0.309017f, 1.0f, 0.951057f ,.145f,.9755f,.96f,
		-0.309017f, 1.0f, 0.951057f ,.145f,.9755f,.96f,
		-0.809017f, 1.0f, 0.587785f ,.145f,.9755f,.96f,
		-1.0f, 1.0f, 1.22465e-16f ,.145f,.9755f,.96f,
		-0.809017f, 1.0f, -0.587785f ,.145f,.9755f,.96f,
		-0.309017f, 1.0f, -0.951057f ,.145f,.9755f,.96f,
		0.309017f, 1.0f, -0.951057f ,.145f,.9755f,.96f,
		0.809017f, 1.0f, -0.587785f ,.145f,.9755f,.96f,

		1.0f, -1.0f, 0.0f ,.145f,.9755f,.96f,
		0.809017f, -1.0f, 0.587785f ,.145f,.9755f,.96f,
		0.309017f, -1.0f, 0.951057f ,.145f,.9755f,.96f,
		-0.309017f, -1.0f, 0.951057f ,.145f,.9755f,.96f,
		-0.809017f, -1.0f, 0.587785f ,.145f,.9755f,.96f,
		-1.0f, -1.0f, 1.22465e-16f ,.145f,.9755f,.96f,
		-0.809017f, -1.0f, -0.587785f ,.145f,.9755f,.96f,
		-0.309017f, -1.0f, -0.951057f ,.145f,.9755f,.96f,
		0.309017f, -1.0f, -0.951057f ,.145f,.9755f,.96f,
		0.809017f, -1.0f, -0.587785f ,.145f,.9755f,.96f,


		1.0f, -1.0f, 0.0f ,.58f,0.573f,0.576f,
		0.809017f, -1.0f, 0.587785f ,.58f,0.573f,0.576f,
		0.309017f, -1.0f, 0.951057f ,.58f,0.573f,0.576f,
		-0.309017f, -1.0f, 0.951057f ,.58f,0.573f,0.576f,
		-0.809017f, -1.0f, 0.587785f ,.58f,0.573f,0.576f,
		-1.0f, -1.0f, 1.22465e-16f ,.58f,0.573f,0.576f,
		-0.809017f, -1.0f, -0.587785f ,.58f,0.573f,0.576f,
		-0.309017f, -1.0f, -0.951057f ,.58f,0.573f,0.576f,
		0.309017f, -1.0f, -0.951057f ,.58f,0.573f,0.576f,
		0.809017f, -1.0f, -0.587785f ,.58f,0.573f,0.576f,

		0.0,-1.0,0.0, .58f,0.573f,0.576f
	};

	// blue color
	// .145f,.9755f,.96f,

	unsigned int ind_arr[] = {
		0, 11, 1,
		11, 0, 10,
		1, 12, 2,
		12, 1, 11,
		2, 13, 3,
		13, 2, 12,
		3, 14, 4,
		14, 3, 13,
		4, 15, 5,
		15, 4, 14,
		5, 16, 6,
		16, 5, 15,
		6, 17, 7,
		17, 6, 16,
		7, 18, 8,
		18, 7, 17,
		8, 19, 9,
		19, 8, 18,
		9, 10, 0,
		10, 9, 19,

		30,20,21,
		30,21,22,
		30,22,23,
		30,23,24,
		30,24,25,
		30,25,26,
		30,26,27,
		30,27,28,
		30,28,29,
		30,29,20

		/*
		21,10,11,
		21,11,12,
		21,12,13,
		21,13,14,
		21,14,15,
		21,15,16,
		21,16,17,
		21,17,18,
		21,18,19,
		21,19,10*/

	};

	unsigned int VBOL, VAOL, EBOL;
	glGenVertexArrays(1, &VAOL);
	glGenBuffers(1, &VBOL);
	glGenBuffers(1, &EBOL);
	glBindVertexArray(VAOL);
	glBindBuffer(GL_ARRAY_BUFFER, VBOL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(black_color), black_color, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOL);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(table_top), table_top, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOH, VAOH, EBOH;
	glGenVertexArrays(1, &VAOH);
	glGenBuffers(1, &VBOH);
	glGenBuffers(1, &EBOH);
	glBindVertexArray(VAOH);
	glBindBuffer(GL_ARRAY_BUFFER, VBOH);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hangerwall), hangerwall, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOH);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	unsigned int VBO2, VAO2, EBO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(table_leg), table_leg, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBO3, VAO3, EBO3;
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);
	glBindVertexArray(VAO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(chair_leg), chair_leg, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBO4, VAO4, EBO4;
	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);
	glBindVertexArray(VAO4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(chair_sides), chair_sides, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBO5, VAO5, EBO5;
	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glGenBuffers(1, &EBO5);
	glBindVertexArray(VAO5);
	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(chair_back), chair_back, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOG, VAOG, EBOG;
	glGenVertexArrays(1, &VAOG);
	glGenBuffers(1, &VBOG);
	glGenBuffers(1, &EBOG);
	glBindVertexArray(VAOG);
	glBindBuffer(GL_ARRAY_BUFFER, VBOG);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOG);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOW1, VAOW1, EBOW1;
	glGenVertexArrays(1, &VAOW1);
	glGenBuffers(1, &VBOW1);
	glGenBuffers(1, &EBOW1);
	glBindVertexArray(VAOW1);
	glBindBuffer(GL_ARRAY_BUFFER, VBOW1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(front_back_walls), front_back_walls, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOW1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOW2, VAOW2, EBOW2;
	glGenVertexArrays(1, &VAOW2);
	glGenBuffers(1, &VBOW2);
	glGenBuffers(1, &EBOW2);
	glBindVertexArray(VAOW2);
	glBindBuffer(GL_ARRAY_BUFFER, VBOW2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(side_walls), side_walls, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOW2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOB, VAOB, EBOB;
	glGenVertexArrays(1, &VAOB);
	glGenBuffers(1, &VBOB);
	glGenBuffers(1, &EBOB);
	glBindVertexArray(VAOB);
	glBindBuffer(GL_ARRAY_BUFFER, VBOB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bar_table), bar_table, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOC, VAOC, EBOC;
	glGenVertexArrays(1, &VAOC);
	glGenBuffers(1, &VBOC);
	glGenBuffers(1, &EBOC);
	glBindVertexArray(VAOC);
	glBindBuffer(GL_ARRAY_BUFFER, VBOC);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cabinate), cabinate, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOC);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOT, VAOT, EBOT;
	glGenVertexArrays(1, &VAOT);
	glGenBuffers(1, &VBOT);
	glGenBuffers(1, &EBOT);
	glBindVertexArray(VAOT);
	glBindBuffer(GL_ARRAY_BUFFER, VBOT);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ceiling), ceiling, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOT);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	//Fan
	unsigned int VBOF1, VAOF1, EBOF1;
	glGenVertexArrays(1, &VAOF1);
	glGenBuffers(1, &VBOF1);
	glGenBuffers(1, &EBOF1);
	glBindVertexArray(VAOF1);
	glBindBuffer(GL_ARRAY_BUFFER, VBOF1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fan_cup), fan_cup, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOF1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOF2, VAOF2, EBOF2;
	glGenVertexArrays(1, &VAOF2);
	glGenBuffers(1, &VBOF2);
	glGenBuffers(1, &EBOF2);
	glBindVertexArray(VAOF2);
	glBindBuffer(GL_ARRAY_BUFFER, VBOF2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fan_hanging_rod), fan_hanging_rod, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOF2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOF3, VAOF3, EBOF3;
	glGenVertexArrays(1, &VAOF3);
	glGenBuffers(1, &VBOF3);
	glGenBuffers(1, &EBOF3);
	glBindVertexArray(VAOF3);
	glBindBuffer(GL_ARRAY_BUFFER, VBOF3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fan_blade), fan_blade, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOF3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	int i = 0;
	Table_Sofa table_chair[4];
	Tool tools[5];
	Glass glass[5];
	Cylinders cylinder[5];
	//cylinder.generateVertices();
	//float* ver_arr = cylinder.arr;
	//int* ind_arr = cylinder.indices;

	unsigned int VBO_CIRC, VAO_CIRC, EBO_CIRC;
	glGenVertexArrays(1, &VAO_CIRC);
	glGenBuffers(1, &VBO_CIRC);
	glGenBuffers(1, &EBO_CIRC);
	glBindVertexArray(VAO_CIRC);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_CIRC);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ver_arr), ver_arr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_CIRC);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind_arr), ind_arr, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// activate shader
		ourShader.use();
		glm::mat4 model;
		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		float degree = 0;
		glm::mat4 view = camera.GetViewMatrix();
		float r = glm::length(camera.Position - glm::vec3(view[3]));
		ourShader.setMat4("view", view);

		//Table chair
		float shiftx = 4, shiftz = 0;
		for (int i = 0; i < 4; i++) {
			table_chair[i].tox = shiftx;
			table_chair[i].toz = shiftz;
			ourShader = table_chair[i].ret_shader(ourShader, VAO, VAO2, VAOC, VAO4, VAO5);
			shiftz -= 2;
		}
		
		//Tools
		float shiftx_tool = -0.1, shiftz_tool = 0;
		for (int i = 0; i < 5; i++) {
			tools[i].tox = shiftx_tool;
			tools[i].toz = shiftz_tool;
			ourShader = tools[i].ret_shader(ourShader, VAO_CIRC, VAO2, VAO3);
			shiftz_tool -= 2;
		}
		


		//Floor
		model = transforamtion(-2.5, -.8, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 20, 0.1, 24);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOG);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//front_back_walls
		model = transforamtion(-2.5, -.75, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 20, 7, 0.2);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOW1);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		
		model = transforamtion(-2.5, -.75, 3, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 20, 7, 0.2);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOW1);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		
		
		//side_walls
		model = transforamtion(-2.5, -.75, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .2, 7, 24);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOW2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = transforamtion(7.5, -.75, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .2, 7, 24);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOW2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		
		//Rack
		//backside of rack
		model = transforamtion(-2.35, -0.75, -7, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .2, 5, 17);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOC);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//both side of rack
		//inner side
		model = transforamtion(-2.35, -0.75, -7, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2, 5, 0.2);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOC);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//outer side
		model = transforamtion(-2.35, -0.75, 1.4, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2, 5, 0.2);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOC);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//3 racks holding utensils
		//first rack
		model = transforamtion(-2.35, -.625, -7, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2, .2, 17);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOC);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//second rack
		model = transforamtion(-2.35, 0.375, -7, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2, .2, 17);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOC);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//third rack
		model = transforamtion(-2.35, 1.375, -7, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2, .2, 17);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOC);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//placing glasses on rack
		float shiftx_glass = -2, shiftz_glass = 0.0;
		for (int i = 0; i < 5; i++) {
			glass[i].tox = shiftx_glass;
			glass[i].toz = shiftz_glass;
			ourShader = glass[i].ret_shader(ourShader, VAO_CIRC, VAO2, VAO3);
			shiftz_glass -= 1.5;
		}



		//Big Bar table
		model = transforamtion(-0.75, -0.75, -7, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1.5, 2, 17);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOB);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//window
		//pordar hanger
		model = transforamtion(.65, 1.4, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 8.4, 1, .75);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOH);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//black portion
		model = transforamtion(1, -.6, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 7, 4, .5);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOB);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//middle portion
		model = transforamtion(1.15, -.35, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.05, 3.5, .51);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOG);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//middle portion
		model = transforamtion(2.825, -.35, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.05, 3.5, .51);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOG);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//Ceiling
		model = transforamtion(-2.5, 2.75, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 20, 0.1, 24);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOT);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//Fan
		model = transforamtion(2, 2.75, -6, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1, -.25, 1);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOF1);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = transforamtion(2.125, 2.35, -5.875, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .5, .5, .5);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOF2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		for (int i = 0; i < 4; i++) {
			model = transforamtion(-.4 + 2 * i, -.75, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .01, .01, 24);
			ourShader.setMat4("model", model);
			glBindVertexArray(VAOL);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		for (int i = 0; i < 5; i++) {
			model = transforamtion(-2.4, -.75, -7 + 2 * i, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 24, .01, .01);
			ourShader.setMat4("model", model);
			glBindVertexArray(VAOL);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}


		//Fan circle
		//lower portion
		model = transforamtion(2.25, 2.35, -5.75, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .35, 0.1, .35);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO_CIRC);
		glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, 0);
		//upper portion
		model = transforamtion(2.25, 2.45, -5.75, rotateAngle_X, rotateAngle_Y, 180.0f, .35, 0.01, .35);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO_CIRC);
		glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, 0);


		Fan fan;
		ourShader = fan.local_rotation(ourShader, VAOF3, i);

		if (fan_turn)
			i += 5;
		if (rotate_around)
			camera.ProcessKeyboard(Y_LEFT, deltaTime);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
	glDeleteVertexArrays(1, &VAO3);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &EBO3);
	glDeleteVertexArrays(1, &VAO4);
	glDeleteBuffers(1, &VBO4);
	glDeleteBuffers(1, &EBO4);
	glDeleteVertexArrays(1, &VAO5);
	glDeleteBuffers(1, &VBO5);
	glDeleteBuffers(1, &EBO5);
	glDeleteVertexArrays(1, &VAOG);
	glDeleteBuffers(1, &VBOG);
	glDeleteBuffers(1, &EBOG);
	glDeleteVertexArrays(1, &VAOW1);
	glDeleteBuffers(1, &VBOW1);
	glDeleteBuffers(1, &EBOW1);
	glDeleteVertexArrays(1, &VAOW2);
	glDeleteBuffers(1, &VBOW2);
	glDeleteBuffers(1, &EBOW2);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		camera.ProcessKeyboard(DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		camera.ProcessKeyboard(P_UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		camera.ProcessKeyboard(P_DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		camera.ProcessKeyboard(Y_LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		camera.ProcessKeyboard(Y_RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		camera.ProcessKeyboard(R_LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.ProcessKeyboard(R_RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		if (!fan_turn) {
			fan_turn = true;
		}
		else {
			fan_turn = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		if (!rotate_around) {
			rotate_around = true;
		}
		else {
			rotate_around = false;
		}
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
