#include <iostream>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLDebug.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Model.h"
#include "Camera.h"

void glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main()
{
	GLFWwindow *window;
	if (glfwInit() != GL_TRUE)
	{
		std::cout << "Failed to glfwInit." << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LeranOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to Create Window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to glew Init." << std::endl;
		glfwTerminate();
		return -1;
	}
	std::cout << "OpenGL Version: " << glfwGetVersionString() << std::endl;
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	{
		std::vector<Vertex> vertexVec;
		vertexVec.push_back({ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) });
		vertexVec.push_back({ glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) });
		vertexVec.push_back({ glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) });
		vertexVec.push_back({ glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) });

		std::vector<Texture> texVec;
		texVec.push_back({ 0, "texture_diffuse1" });
		std::vector<unsigned int> indiecsVec;
		indiecsVec.push_back(0);
		indiecsVec.push_back(1);
		indiecsVec.push_back(2);
		indiecsVec.push_back(1);
		indiecsVec.push_back(2);
		indiecsVec.push_back(3);

		Mesh mesh(vertexVec, indiecsVec, texVec);

		// 加载 shader
		ShaderProgram shaderProgram("res/shaders/Basic.shader");
		// 加载 Texture2D
		Texture2D tex2D("res/models/nanosuit/arm_dif.png");
		Texture2D tex2D1("res/models/nanosuit/body_dif.png");
		shaderProgram.Bind();
		shaderProgram.SetUniform1i("ourTexture1", 0);
		shaderProgram.SetUniform1i("ourTexture2", 1);

		Model ourModel("res/models/nanosuit/nanosuit.obj");



		//tex2D.Unbind();
		shaderProgram.Unbind();

		while (!glfwWindowShouldClose(window))
		{
			glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// per-frame time logic
			// --------------------
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			// input
			// -----
			processInput(window);

			shaderProgram.Bind();

			// view/projection transformations
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			shaderProgram.SetUniformMat4f("projection", projection);
			shaderProgram.SetUniformMat4f("view", view);

			// render the loaded model
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -3.0f, -5.0f)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(0.3f));	// it's a bit too big for our scene, so scale it down
			shaderProgram.SetUniformMat4f("model", model);

			ourModel.Draw(&shaderProgram);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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
	camera.ProcessMouseScroll(yoffset);
}
