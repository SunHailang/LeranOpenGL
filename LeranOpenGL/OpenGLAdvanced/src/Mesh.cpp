#include "Mesh.h"
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "ShaderProgram.h"
#include "GLDebug.h"

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures)
	:vertices(_vertices), indices(_indices), textures(_textures)
{
	SetupMesh();
}

Mesh::~Mesh()
{
	//GLCall(glDeleteVertexArrays(1, &VAO));
	//GLCall(glDeleteBuffers(1, &VBO));
	//GLCall(glDeleteBuffers(1, &EBO));
}


void Mesh::Draw(ShaderProgram *shaderProgram) const
{
	unsigned int textureIndex = 0;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + textureIndex); // 在绑定之前激活相应的纹理单元
		std::string name = textures.at(i).type.c_str();
		unsigned int id = textures.at(i).id;
		shaderProgram->SetUniform1i(name, textureIndex);
		textureIndex++;
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}


	// 绘制网格
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::SetupMesh()
{
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glGenBuffers(1, &EBO));

	GLCall(glBindVertexArray(VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));

	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

	// 顶点位置
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
	// 顶点法线
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));
	// 顶点纹理坐标
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));

	GLCall(glBindVertexArray(0));
}

