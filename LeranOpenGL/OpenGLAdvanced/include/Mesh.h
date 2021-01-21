#pragma once
#include <iostream>
#include <vector>

#include "glm/glm.hpp"
#include "assimp/types.h"

class ShaderProgram;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	unsigned int id;
	std::string type;
	aiString path;  // 我们储存纹理的路径用于与其它纹理进行比较
};

class Mesh
{
public:

	/*函数*/
	Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures);
	~Mesh();

	void Draw(ShaderProgram *shaderProgram) const;

	inline unsigned int GetVAO() const { return VAO; }
	inline unsigned int GetVBO() const { return VBO; }
	inline unsigned int GetEBO() const { return EBO; }

private:
	/*网格数据*/
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	/*渲染数据*/
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	/*函数*/
	void SetupMesh();

};

