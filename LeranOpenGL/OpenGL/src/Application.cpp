#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "ShaderProgram.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Camera.h"
#include "Time.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 540


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	// Enable vsync
	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew init failed." << std::endl;
		glfwTerminate();
		return -1;
	}
	std::cout << "[OpenGL Version]: " << glfwGetVersionString() << std::endl;
	{
		// 绘制三角形
		//float positions[] = {
		//	0.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// 0 左下角
		//	192.0f, 0.0f, 0.0f, 1.0f, 0.0f,	// 1 右下角
		//	192.0f, 192.0f, 0.0f, 1.0f, 1.0f,	// 2 右上角
		//	0.0f, 192.0f, 0.0f, 0.0f, 1.0f	// 3 左上角
		//};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		//float positions[] = {
		//	// positions          // texture coords
		//	0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		//	0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		//	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
		//};
		//unsigned int indices[] = {
		//	0, 1, 3, // first triangle
		//	1, 2, 3  // second triangle
		//};
		float positions[] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
		};

		//float positions[] = {
		//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		//	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		//	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		//	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		//	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		//	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		//	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		//};
		// world space positions of our cubes
		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};


		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		glEnable(GL_DEPTH_TEST);

		VertexArray va;
		VertexBuffer vb(positions, sizeof(positions));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		//IndexBuffer ib(indices, 0);

		VertexArray lightVAO;
		VertexBufferLayout lightLayout;
		lightLayout.Push<float>(3);
		lightLayout.Push<float>(3);
		lightLayout.Push<float>(2);
		lightVAO.AddBuffer(vb, lightLayout);
		lightVAO.Unbind();

		
		

		//glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		//glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)960 / (float)450, 0.1f, 100.0f);
		//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));


		ShaderProgram program("res/shaders/Basic.shader");
		program.Bind();
		program.SetUniform4f("u_Color", 0.2f, 0.3f, 0.3f, 1.0f);

		//Texture texture("res/textures/log.png");
		//texture.Bind();		
		//program.SetUniform1i("u_Texture", 0);



		ShaderProgram lithtProgram("res/shaders/Light.shader");
		Texture tex("res/textures/container2.png");
		tex.Bind();
		lithtProgram.Bind();
		Texture tex1("res/textures/container2_specular.png");
		tex1.Bind();
		lithtProgram.SetUniform1i("material.diffuse", 0);
		lithtProgram.SetUniform1i("material.specular", 1);
		


		va.Unbind();
		vb.Unbind();
		//ib.Unbind();
		program.Unbind();
		lithtProgram.Unbind();

		Renderer renderer;

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


		glm::vec3 translation(0.0f, -0.5f, -4.0f);

		while (!glfwWindowShouldClose(window))
		{
			/* renderer here */
			renderer.Clear();

			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;


			processInput(window);


			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			program.Bind();

			//renderer.Draw(va, ib, program);

			// create transformations
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
			glm::vec3 lightPos(translation.x, translation.y, translation.z);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.3f));
			model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
			//view = glm::translate(view, translation);
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
			//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

			float radius = 20.0f;
			float camX = sin(glfwGetTime()) * radius;
			float camZ = cos(glfwGetTime()) * radius;
			view = camera.GetViewMatrix();
			//view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

			//view = glm::translate(glm::mat4(1.0f), translation);
			glm::mat4 mvp = projection * view * model;
			program.SetUniformMat4f("u_MVP", mvp);

			va.Bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//for (unsigned int i = 0; i < 10; i++)
			//{
			//	// calculate the model matrix for each object and pass it to shader before drawing
			//	glm::mat4 model = glm::mat4(1.0f);
			//	model = glm::translate(model, cubePositions[i]);
			//	float angle = 20.0f * i;
			//	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			//	//ourShader.setMat4("model", model);

			//	glm::mat4 mvp = projection * view * model;
			//	program.SetUniformMat4f("u_MVP", mvp);

			//	glDrawArrays(GL_TRIANGLES, 0, 36);
			//}
			



			lithtProgram.Bind();
			lithtProgram.SetUniform3f("objectColor", 1.0f, 0.5f, 0.31f);
			lithtProgram.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
			glm::vec3 cubePos(0.2f, -2.5f, -3.0f);
			glm::mat4 lightmodel = glm::mat4(1.0f);
			//glm::mat4 lightprojection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
			lightmodel = glm::translate(lightmodel, cubePos);
			lightmodel = glm::scale(lightmodel, glm::vec3(3.0f));
			//lightmodel = glm::rotate(lightmodel, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			//glm::mat4 lightMVP = projection * view * lightmodel;


			lithtProgram.SetUniformMat4f("projection", projection);
			lithtProgram.SetUniformMat4f("view", view);
			lithtProgram.SetUniformMat4f("model", lightmodel);

			glm::mat4 lightMVP = projection * view * lightmodel;

			//lithtProgram.SetUniformMat4f("u_MVP", lightMVP);

			lithtProgram.SetUniform3f("lightPos", lightPos);
			lithtProgram.SetUniform3f("viewPos", camera.Position);

			//lithtProgram.SetUniform3f("material.ambient", 1.0f, 0.5f, 0.31f);
			
			lithtProgram.SetUniform1f("material.shininess", 32.0f);

			glm::vec3 lightColor(1.0f);
			//lightColor.x = sin(glfwGetTime() * 2.0f);
			//lightColor.y = sin(glfwGetTime() * 0.7f);
			//lightColor.z = sin(glfwGetTime() * 1.3f);

			glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // 降低影响
			glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响

			lithtProgram.SetUniform3f("light.ambient", ambientColor);
			lithtProgram.SetUniform3f("light.diffuse", diffuseColor); // 将光照调暗了一些以搭配场景
			lithtProgram.SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);

			lightVAO.Bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// 2. Show a simple window that we create ourselves. 
			// We use a Begin/End pair to created a named window.
			{
				ImGui::Begin("Application");

				ImGui::SliderFloat3("TranslationX", &translation.x, -5.0f, 5.0f);

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

	glfwDestroyWindow(window);

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