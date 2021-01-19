#pragma once
#include <iostream>
#include <vector>

#include "glm/glm.hpp"

#include "ShaderProgram.h"

#include "VertexArray.h"



struct MeshVertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct MeshTexture
{
	unsigned int id;
	std::string type;
};

class Mesh
{
public:
	/*网格数据*/
	std::vector<MeshVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<MeshTexture> textures;

	/*函数*/
	Mesh(std::vector<MeshVertex> _vertices, std::vector<unsigned int> _indices, std::vector<MeshTexture> _textures);
	void Draw(ShaderProgram shaderProgram);

private:
	/*渲染数据*/
	VertexArray va;
	//VertexBuffer vb;
	//IndexBuffer ib;

	/*函数*/
	void SetupMesh();

};

