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
#include "TextureCube.h"

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


		// 加载 shader
		ShaderProgram lightShaderProgram("res/shaders/Light.shader");

		ShaderProgram shaderProgram("res/shaders/Basic.shader");

		shaderProgram.Bind();
		//shaderProgram.SetUniform1i("ourTexture1", 0);
		//shaderProgram.SetUniform1i("ourTexture2", 1);

		Model ourModel("res/models/nanosuit/nanosuit.obj");
		Model lightCube("res/models/cube/cube.obj");


		float transparentVertices[] = {
			// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
			0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
			0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

			0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
			1.0f,  0.5f,  0.0f,  1.0f,  0.0f
		};
		// transparent VAO
		unsigned int transparentVAO, transparentVBO;
		glGenVertexArrays(1, &transparentVAO);
		glGenBuffers(1, &transparentVBO);
		glBindVertexArray(transparentVAO);
		glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);

		std::vector<glm::vec3> vegetation
		{
			glm::vec3(-1.5f, 0.0f, -0.48f),
			glm::vec3(1.5f, 0.0f, 0.51f),
			glm::vec3(0.0f, 0.0f, 0.7f),
			glm::vec3(-0.3f, 0.0f, -2.3f),
			glm::vec3(0.5f, 0.0f, -0.6f)
		};

		ShaderProgram grassShaderProgram("res/shaders/Grass.shader");
		grassShaderProgram.Bind();
		Texture2D grass("res/textures/grass.png");
		grassShaderProgram.SetUniform1i("ourTexture", 0);


#pragma region SkyBox 加载
		ShaderProgram skyboxShaderProgram("res/shaders/SkyBox.shader");
		float skyboxVertices[] = {
			// positions          
			-70.0f,  70.0f, -70.0f,
			-70.0f, -70.0f, -70.0f,
			 70.0f, -70.0f, -70.0f,
			 70.0f, -70.0f, -70.0f,
			 70.0f,  70.0f, -70.0f,
			-70.0f,  70.0f, -70.0f,		//
			-70.0f, -70.0f, 7.00f,
			-70.0f, -70.0f, -70.0f,
			-70.0f,  70.0f, -70.0f,
			-70.0f,  70.0f, -70.0f,
			-70.0f,  70.0f,  70.0f,
			-70.0f, -70.0f,  70.0f,

			 70.0f, -70.0f, -70.0f,
			 70.0f, -70.0f,  70.0f,
			 70.0f,  70.0f,  70.0f,
			 70.0f,  70.0f,  70.0f,
			 70.0f,  70.0f, -70.0f,
			 70.0f, -70.0f, -70.0f,

			-70.0f, -70.0f,  70.0f,
			-70.0f,  70.0f,  70.0f,
			 70.0f,  70.0f,  70.0f,
			 70.0f,  70.0f,  70.0f,
			 70.0f, -70.0f,  70.0f,
			-70.0f, -70.0f,  70.0f,

			-70.0f,  70.0f, -70.0f,
			 70.0f,  70.0f, -70.0f,
			 70.0f,  70.0f,  70.0f,
			 70.0f,  70.0f,  70.0f,
			-70.0f,  70.0f,  70.0f,
			-70.0f,  70.0f, -70.0f,

			-70.0f, -70.0f, -70.0f,
			-70.0f, -70.0f,  70.0f,
			 70.0f, -70.0f, -70.0f,
			 70.0f, -70.0f, -70.0f,
			-70.0f, -70.0f,  70.0f,
			 70.0f, -70.0f,  70.0f
		};
		// skybox VAO

		unsigned int skyboxVAO, skyboxVBO;
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindVertexArray(0);
		std::vector<std::string> faces
		{
			"res/textures/skybox/skybox/right.jpg",
			"res/textures/skybox/skybox/left.jpg",
			"res/textures/skybox/skybox/top.jpg",
			"res/textures/skybox/skybox/bottom.jpg",
			"res/textures/skybox/skybox/front.jpg",
			"res/textures/skybox/skybox/back.jpg"
		};
		TextureCube textureCube(faces);
		skyboxShaderProgram.Bind();
		skyboxShaderProgram.SetUniform1i("skybox", 0);
#pragma endregion
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
			// render
			// ------
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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

			// 绘制天空盒
			glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
			skyboxShaderProgram.Bind();
			//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
			skyboxShaderProgram.SetUniformMat4f("view", view);
			skyboxShaderProgram.SetUniformMat4f("projection", projection);
			// skybox cube
			glBindVertexArray(skyboxVAO);
			textureCube.Bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS); // set depth function back to default

			// 绘制一个光源
			lightShaderProgram.Bind();
			lightShaderProgram.SetUniformMat4f("projection", projection);
			lightShaderProgram.SetUniformMat4f("view", view);
			glm::mat4 lightModel = glm::mat4(1.0f);

			lightModel = glm::translate(lightModel, lightPos); // translate it down so it's at the center of the scene
			lightModel = glm::scale(lightModel, glm::vec3(0.1f));	// it's a bit too big for our scene, so scale it down
			lightModel = glm::rotate(lightModel, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			lightShaderProgram.SetUniformMat4f("model", lightModel);

			lightCube.Draw(&shaderProgram);

			// 绘制3D模型
			shaderProgram.Bind();
			// view/projection transformations			
			shaderProgram.SetUniformMat4f("projection", projection);
			shaderProgram.SetUniformMat4f("view", view);
			// render the loaded model
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -3.0f, -8.0f)); // translate it down so it's at the center of the scene
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

			// grass 
			grassShaderProgram.Bind();
			grassShaderProgram.SetUniformMat4f("projection", projection);
			grassShaderProgram.SetUniformMat4f("view", view);
			glBindVertexArray(transparentVAO);
			grass.Bind();
			for (unsigned int i = 0; i < vegetation.size(); i++)
			{
				glm::mat4 grassModel(1.0);
				grassModel = glm::translate(grassModel, vegetation[i]);
				grassShaderProgram.SetUniformMat4f("model", grassModel);

				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			glBindVertexArray(0);


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

		glDeleteVertexArrays(1, &skyboxVAO);
		glDeleteBuffers(1, &skyboxVBO);
		glDeleteVertexArrays(1, &transparentVAO);
		glDeleteBuffers(1, &transparentVBO);
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
