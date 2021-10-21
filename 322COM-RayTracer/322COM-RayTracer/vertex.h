#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

struct Vertex
{
   glm::vec4 coords;
   glm::vec2 texCoords;
};

struct VertexWtihNormal
{
	glm::vec4 coords;
	glm::vec3 normals;
};

struct triVertex
{
	glm::vec3 vertex;
	glm::vec3 normal;
};

struct VertexWithAll
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 textcoord;
	glm::vec3 normal;
};

#endif
