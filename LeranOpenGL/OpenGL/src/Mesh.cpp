#include "Mesh.h"
#include <iostream>
#include "ShaderProgram.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


Mesh::Mesh(std::vector<MeshVertex> _vertices, std::vector<unsigned int> _indices, std::vector<MeshTexture> _textures)
	:vertices(_vertices), indices(_indices), textures(_textures)
{
	SetupMesh();
}

void Mesh::Draw(ShaderProgram shaderProgram)
{
	VertexArray va;
	VertexBuffer vb;
	IndexBuffer ib;
}

void Mesh::SetupMesh()
{

}

