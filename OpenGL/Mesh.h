#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "Shader.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

struct Triangle {
	glm::vec3 p0;
	glm::vec3 p1;
	glm::vec3 p2;
};

class Mesh {
public:
	/* Mesh Data */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	/* Functions */
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	static Mesh Cube();
	void Draw(Shader shader);
	std::vector<Triangle> Triangles();

private:
	/* Render data */
	unsigned int VAO, VBO, EBO;

	/* Functions */
	void setupMesh();
};