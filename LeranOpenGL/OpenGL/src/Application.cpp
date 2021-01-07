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

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
		// 绘制三角形
		float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f,	// 0 左下角
			 0.5f, -0.5f, 1.0f, 0.0f,	// 1 右下角
			 0.5f,  0.5f, 1.0f, 1.0f,	// 2 右上角
			-0.5f,  0.5f, 0.0f, 1.0f	// 3 左上角
		};

		unsigned int indexs[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
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
		float r = 0.8f;
		float increnment = 0.05f;

		while (!glfwWindowShouldClose(window))
		{
			/* renderer here */
			renderer.Clear();

			program.Bind();
			//program.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			renderer.Draw(va, ib, program);

			/*if (r > 1.0f)
				increnment = -0.05f;
			else if (r < 0.0f)
				increnment = 0.05f;
			r += increnment;*/


			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}