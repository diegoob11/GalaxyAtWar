/*  by Alun Evans 2016 LaSalle (alunthomasevasevans@gmail.com)
	based on framework by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	The Mesh contains the info about how to render a mesh and also how to parse it from a file.
*/

#ifndef MESH_H
#define MESH_H

#include <vector>
#include "framework.h"
#include "coldet\src\coldet.h"
class Shader;

struct OBB {
	Vector3 center; // center of the obb
	std::vector < Vector3 > axis; // the three axis of the obb
	Vector3 min;
	Vector3 max;
};

class Mesh
{

public:
	std::vector< Vector3 > vertices; //here we store the vertices
	std::vector< Vector3 > normals;	 //here we store the normals
	std::vector< Vector2 > uvs;	 //here we store the texture coordinates
	std::vector< Vector4 > colors;	 //here we store colors by vertex
	std::vector< unsigned int > indices; //here we store the indices of the mesh
	OBB obb;  // Stores the obb
	std::vector< unsigned int > obb_indices; //stores the indices of the aabb
	std::vector< Vector3 > obb_vertices;

	CollisionModel3D* collision_model; //collision model for bullet collision

	unsigned int obb_vao;
	unsigned int obb_vertices_vbo_id;
	unsigned int obb_indices_vbo_id;

	unsigned int vao;
	unsigned int vertices_vbo_id;
	unsigned int normals_vbo_id;
	unsigned int uvs_vbo_id;
	unsigned int colors_vbo_id;
	unsigned int indices_vbo_id;

	Mesh();
	Mesh(const Mesh& m);
	~Mesh();

	void clear();
	void render(int primitive, Shader* sh);

	void genBuffers(); //create the VAO and VBOs for this mesh

	void createPlane(float size);

	bool loadASE(const char* filename);
	OBB findOBB();
	void createOBBGeometry();
	void renderOBB(Shader* sh);
	void createMenuMesh();
	void createQuad(float v1, float v2, float v3, float v4);
	void setCollisionModel();
};
#endif
