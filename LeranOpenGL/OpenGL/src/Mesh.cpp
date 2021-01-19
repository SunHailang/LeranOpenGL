#include "Mesh.h"
#include <iostream>
#include "ShaderProgram.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"


Mesh::Mesh(std::vector<MeshVertex> _vertices, std::vector<unsigned int> _indices, std::vector<MeshTexture> _textures)
	:vertices(_vertices), indices(_indices), textures(_textures)
{
	SetupMesh();
}

void Mesh::Draw(ShaderProgram shaderProgram)
{



	va.Bind();
}

void Mesh::SetupMesh()
{
	VertexBuffer vb(&vertices[0], sizeof(vertices));
	IndexBuffer ib(&indices[0], indices.size());
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);
	ib.Bind();
	va.Unbind();
}

