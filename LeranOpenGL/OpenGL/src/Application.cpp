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

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

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

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew init failed." << std::endl;
		glfwTerminate();
		return -1;
	}
	std::cout << "[OpenGL Version]: " << glfwGetVersionString() << std::endl;
	{
		struct Vec3
		{
			float x;
			float y;
			float z;
		};
		float m_width = 192.0f;
		float m_height = 192.0f;

		Vec3 v1 = { -0.5f, 0.5f,0.0f }; // 左上角
		Vec3 v2 = { -0.5f + m_width / SCREEN_WIDTH, 0.0f, 0.0f }; // 右上角
		Vec3 v3 = { 0.0f, 0.5f + m_height / SCREEN_HEIGHT, 0.0f }; // 左下角
		Vec3 v4 = { -0.5f + m_width / SCREEN_WIDTH,0.5f + m_height / SCREEN_HEIGHT, 0.0f }; // 右下角


		// 绘制三角形
		float positions[] = {
			v3.x, v3.y, v3.z, 0.0f, 0.0f,	// 0 左下角
			v4.x, v4.y, v4.z, 1.0f, 0.0f,	// 1 右下角
			v2.x, v2.y, v2.z, 1.0f, 1.0f,	// 2 右上角
			v1.x, v1.y, v1.z, 0.0f, 1.0f	// 3 左上角
		};

		unsigned int indexs[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(positions, sizeof(positions));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indexs, 6);

		ShaderProgram program("res/shaders/Basic.shader");
		program.Bind();
		program.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		Texture texture("res/textures/log.png");
		texture.Bind();
		program.SetUniform1i("u_Texture", 0);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		program.Unbind();

		Renderer renderer;

		while (!glfwWindowShouldClose(window))
		{
			/* renderer here */
			renderer.Clear();

			program.Bind();

			renderer.Draw(va, ib, program);


			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}