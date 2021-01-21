#include <iostream>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
	const char* glsl_version = "#version 130";
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
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_DEPTH_TEST));

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		std::vector<Vertex> vertexVec;
		vertexVec.push_back({ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) });
		vertexVec.push_back({ glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) });
		vertexVec.push_back({ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) });
		vertexVec.push_back({ glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) });

		std::vector<Texture> texVec;
		texVec.push_back({ 0, "texture_diffuse1" });
		std::vector<unsigned int> indiecsVec;
		indiecsVec.push_back(0);
		indiecsVec.push_back(1);
		indiecsVec.push_back(2);
		indiecsVec.push_back(1);
		indiecsVec.push_back(2);
		indiecsVec.push_back(3);

		Mesh ourMesh(vertexVec, indiecsVec, texVec);


		// 加载 shader
		ShaderProgram lightShaderProgram("res/shaders/Light.shader");

		ShaderProgram shaderProgram("res/shaders/Basic.shader");
		// 加载 Texture2D
		Texture2D tex2D("res/models/nanosuit/arm_dif.png");
		Texture2D tex2D1("res/models/nanosuit/body_dif.png");
		shaderProgram.Bind();
		//shaderProgram.SetUniform1i("ourTexture1", 0);
		//shaderProgram.SetUniform1i("ourTexture2", 1);

		Model ourModel("res/models/nanosuit/nanosuit.obj");
		//Model ourModel("res/models/cube/cube.obj");


		//tex2D.Unbind();
		shaderProgram.Unbind();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		float rotation = 0;
		glm::vec3 lightPos = glm::vec3(-0.5f, 1.0f, 0.0f);

		while (!glfwWindowShouldClose(window))
		{
			glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// per-frame time logic
			// --------------------
			float currentFrame = (float)glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			// input
			// -----
			processInput(window);

			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();

			// 绘制一个光源
			lightShaderProgram.Bind();
			lightShaderProgram.SetUniformMat4f("projection", projection);
			lightShaderProgram.SetUniformMat4f("view", view);
			glm::mat4 lightModel = glm::mat4(1.0f);

			lightModel = glm::translate(lightModel, lightPos); // translate it down so it's at the center of the scene
			lightModel = glm::scale(lightModel, glm::vec3(0.3f));	// it's a bit too big for our scene, so scale it down
			lightShaderProgram.SetUniformMat4f("model", lightModel);

			

			ourMesh.Draw(&lightShaderProgram);

			shaderProgram.Bind();

			// view/projection transformations			
			shaderProgram.SetUniformMat4f("projection", projection);
			shaderProgram.SetUniformMat4f("view", view);
			// render the loaded model
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -3.0f, -5.0f)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(0.3f));	// it's a bit too big for our scene, so scale it down
			model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			shaderProgram.SetUniformMat4f("model", model);

			float x = sin(currentFrame);

			shaderProgram.SetUniform3f("lightColor", glm::vec3(1.0f));
			shaderProgram.SetUniform3f("lightPos", lightPos);
			shaderProgram.SetUniform3f("lightAmbient", glm::vec3(0.05f, 0.05f, 0.05f));
			shaderProgram.SetUniform3f("lightDiffuse", glm::vec3(1.0f, 0.5f, 0.31f));
			shaderProgram.SetUniform3f("lightSpecular", glm::vec3(0.5f, 0.5f, 0.5f));
			shaderProgram.SetUniform3f("viewPos", camera.Position);

			ourModel.Draw(&shaderProgram);


			// 2. Show a simple window that we create ourselves. 
			// We use a Begin/End pair to created a named window.
			{
				ImGui::Begin("Application");

				ImGui::SliderFloat3("lightPos", &lightPos.x, -2.0f, 2.0f);
				ImGui::SliderFloat("rotation", &rotation, -180.0f, 180.0f);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
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

	//camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
