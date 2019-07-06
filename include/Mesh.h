#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include "shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

struct Vertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture{
	unsigned int id;
	// type: texture_diffuse, texture_specular
	std::string type;
	std::string path;
};

class Mesh{
	public:
		/* Mesh data */
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		/* Functions */
		Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices,
			std::vector<Texture> &textures);
		void Draw(Shader &shader);
	private:
		/* Render data */
		unsigned int VAO, VBO, EBO;
		/* Functions */
		void setupMesh();
};

#endif