#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"

#include <vector>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);

class Model
{
public:
	/* Model data */
	std::vector<Texture> textures_loaded;		// stores all the textures loaded so fat,
								// optimization to make sure textures aren't loaded more than once.

	/* Functions */
	Model(std::string const& path);
	void Draw(Shader& shader);

private:
	/* Model Data */
	std::vector<Mesh> meshes;
	std::string directory;
	/* Functions */
	void loadModel(std::string const &path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void loadMaterialTextures(std::vector<Texture>& textures, aiMaterial* mat, aiTextureType type, std::string typeName);
public:
	
	void printDebugInfo();
};

#endif