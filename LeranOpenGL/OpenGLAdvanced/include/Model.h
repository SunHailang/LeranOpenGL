#pragma once

#include <iostream>
#include <vector>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Model
{
public:
	/*函数*/
	Model(const std::string &path, bool gamma = false);
	~Model();

	void Draw(ShaderProgram *shaderProgram) const;

private:
	/*模型数据*/
	std::vector<Mesh> meshs;
	std::string directory;

	std::vector<Texture> textures_loaded;

	bool gammaCorrection;

	/*函数*/
	void loadModel(const std::string &path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);

	unsigned int TextureFromFile(const char *path, const std::string diectory, bool gamma = false);

};