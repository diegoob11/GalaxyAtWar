#include "mesh.h"
#include <cassert>
#include "includes.h"
#include "shader.h"
#include "extra/textparser.h"

using namespace std;
Mesh::Mesh()
{
	vertices_vbo_id = 0;
	normals_vbo_id = 0;
	uvs_vbo_id = 0;
	colors_vbo_id = 0;
    indices_vbo_id = 0;
    vao = 0;
}

Mesh::Mesh( const Mesh& m )
{
	vertices = m.vertices;
	normals = m.normals;
	uvs = m.uvs;
	colors = m.colors;
    indices = m.indices;
    vao = m.vao;
}

Mesh::~Mesh()
{
	if(vertices_vbo_id) glDeleteBuffers(1, &vertices_vbo_id);
	if(normals_vbo_id) glDeleteBuffers(1, &normals_vbo_id);
	if(uvs_vbo_id) glDeleteBuffers(1, &uvs_vbo_id);
	if(colors_vbo_id) glDeleteBuffers(1, &colors_vbo_id);
    if(colors_vbo_id) glDeleteBuffers(1, &indices_vbo_id);
    if(vao)glDeleteVertexArrays(1, &vao);
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
    indices.clear();
}

void Mesh::genBuffers() {
    assert(vertices.size() && "No vertices in this mesh");

    //delete old
    if(vertices_vbo_id) glDeleteBuffers(1, &vertices_vbo_id);
    if(normals_vbo_id) glDeleteBuffers(1, &normals_vbo_id);
    if(uvs_vbo_id) glDeleteBuffers(1, &uvs_vbo_id);
    if(colors_vbo_id) glDeleteBuffers(1, &colors_vbo_id);
    
    //bind vao to store state
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers( 1, &vertices_vbo_id ); //generate one handler (id)
    glBindBuffer( GL_ARRAY_BUFFER, vertices_vbo_id ); //bind the handler
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW ); //upload data
    
    //shader must a_vertex attribute, error if not
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(VERTEX_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    
    if(normals.size())
    {
        glGenBuffers( 1, &normals_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, normals_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(float), &normals[0], GL_STATIC_DRAW ); //upload data
        
        //shader must have a_normal attribute
        glEnableVertexAttribArray(NORMAL_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(NORMAL_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );

    }
    
    if(uvs.size())
    {
        glGenBuffers( 1, &uvs_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, uvs_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, uvs.size() * 2 * sizeof(float), &uvs[0], GL_STATIC_DRAW ); //upload data
        
        //shader must have a_uv attribute
        glEnableVertexAttribArray(UV_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(UV_ATTRIBUTE_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );
        
    }
    
    if(colors.size())
    {
        glGenBuffers( 1, &colors_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, colors_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, uvs.size() * 4 * sizeof(float), &colors[0], GL_STATIC_DRAW ); //upload data

        //shader must have a_color attribute
        glEnableVertexAttribArray(COLOR_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(COLOR_ATTRIBUTE_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );
        
    }
    
    if (indices.size() == 0){
        //if there is no index buffer, make an 'emergency' serial one here
        for (size_t i = 0; i < vertices.size(); i++) {
            indices.push_back((unsigned int)i);
        }
    }
    
    //create index array
    glGenBuffers(1, &indices_vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0); //unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer
    
}

void Mesh::render( int primitive, Shader* sh )
{
	glBindVertexArray(vao);

	glDrawElements(primitive, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

}



void Mesh::createPlane(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
	indices.clear();

	//create four vertices for corner of quad

	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(-size, 0, size));
	vertices.push_back( Vector3(-size,0,-size) );
	vertices.push_back( Vector3(size,0,-size) );
	

	//all of them have the same normal
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );


	//texture coordinates
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(0,1) );
	uvs.push_back( Vector2(0,0) );
	uvs.push_back( Vector2(1,0) );
    
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(1);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);


    
    this->genBuffers();
}

//load our own ASE file
bool Mesh::loadASE(const char* filename) {

	TextParser t;

	if (t.create(filename) == false) {
		std::cout << "Couldn't open file" << std::endl;
		return false;
	}

	t.seek("*MESH_NUMVERTEX");
	int num_vertices = t.getint();

	t.seek("*MESH_NUMFACES");
	int num_faces = t.getint();
		
	//auxiliary vectors
	std::vector<Vector3> unique_verts;
	std::vector<Vector2> unique_uvs;

	//resize our vectors to the correct number
	vertices.resize(num_faces * 3);
	normals.resize(num_faces * 3);
	uvs.resize(num_faces * 3);

	unique_verts.resize(num_vertices);	

	//store vertices in auxiliary vector
	for (int i = 0; i < num_vertices; i++) {	
		t.seek("*MESH_VERTEX");
		int new_index = t.getint();
		float new_x = t.getfloat();
		float new_y = t.getfloat();
		float new_z = t.getfloat();

		Vector3 new_vec3(-new_x, new_z, new_y);

		unique_verts[new_index] = new_vec3;
	}
	
	//store vertices in the correct order
	for (int i = 0; i < num_faces; i++) {
		t.seek("*MESH_FACE");
		t.seek("A:");
		int vert1 = t.getint();
		t.seek("B:");
		int vert2 = t.getint();
		t.seek("C:");
		int vert3 = t.getint();

		vertices[i * 3 + 0] = unique_verts[vert1];
		vertices[i * 3 + 1] = unique_verts[vert2];
		vertices[i * 3 + 2] = unique_verts[vert3];

	}
	
	//store uv vertices in auxiliary vector
	t.seek("*MESH_NUMTVERTEX");
	int num_vertuvs = t.getint();
	unique_uvs.resize(num_vertuvs);
	for (int i = 0; i < num_vertuvs; i++) {
		t.seek("*MESH_TVERT");
		int new_index = t.getint();
		float new_u = t.getfloat();
		float new_v = t.getfloat();

		unique_uvs[new_index].set(new_u, new_v);
	}

	//store uv vertices in the correct order
	t.seek("*MESH_NUMTVFACES");
	int num_uvs = t.getint();
	for (int i = 0; i < num_faces; i++) {
			t.seek("*MESH_TFACE");
			int text_index = t.getint();
			float x = t.getfloat();
			float y = t.getfloat();
			float z = t.getfloat();


			uvs[i * 3 + 0] = unique_uvs[x];
			uvs[i * 3 + 1] = unique_uvs[y];
			uvs[i * 3 + 2] = unique_uvs[z];
	}

	//Store normals in the correct order in its vector
	for (int i = 0; i < num_faces; i++) {
		for (int j = 0; j < 3; j++){
			t.seek("*MESH_VERTEXNORMAL");
			int normal_index = t.getint();
			float v_nx = t.getfloat();
			float v_ny = t.getfloat();
			float v_nz = t.getfloat();

			Vector3 new_norm(-v_nx, v_nz, v_ny);
			normals[i * 3 + j] = new_norm;
		}
	}

	unique_verts.clear();
	unique_uvs.clear();

	this->genBuffers();

	return true;
}

OBB Mesh::findOBB() {
	//To create the vertices we have to find the minimum and maximum point in every edge.
	float minx = 0;
	float maxx = 0;
	float miny = 0;
	float maxy = 0;
	float minz = 0;
	float maxz = 0;
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i].x <= minx) {
			minx = vertices[i].x;
		}
		if (vertices[i].x >= maxx) {
			maxx = vertices[i].x;
		}
		if (vertices[i].y <= miny) {
			miny = vertices[i].y;
		}
		if (vertices[i].y >= maxy) {
			maxy = vertices[i].y;
		}
		if (vertices[i].z <= minz) {
			minz = vertices[i].z;
		}
		if (vertices[i].z >= maxz) {
			maxz = vertices[i].z;
		}
	}

	Vector3 dist = (Vector3(maxx, maxy, maxz) - Vector3(minx, miny, minz));
	obb.center = Vector3(minx, miny, minz) + Vector3(dist.x / 2, dist.y / 2, dist.z / 2);

	obb.axis.clear();
	obb.axis.resize(3);
	obb.axis[0] = (Vector3((maxx - minx) / 2, 0, 0));
	obb.axis[1] = (Vector3(0, (maxy - miny) / 2, 0));
	obb.axis[2] = (Vector3(0, 0, (maxz - minz) / 2));

	//Store the vertices in the aabb array;
	obb_vertices.clear();
	obb_vertices.resize(8);
	obb_vertices[0] = (Vector3((obb.center - obb.axis[0]).x, (obb.center - obb.axis[1]).y, (obb.center - obb.axis[2]).z)); //0
	obb_vertices[1] = (Vector3((obb.center + obb.axis[0]).x, (obb.center - obb.axis[1]).y, (obb.center - obb.axis[2]).z)); //1
	obb_vertices[2] = (Vector3((obb.center + obb.axis[0]).x, (obb.center + obb.axis[1]).y, (obb.center - obb.axis[2]).z)); //2
	obb_vertices[3] = (Vector3((obb.center - obb.axis[0]).x, (obb.center + obb.axis[1]).y, (obb.center - obb.axis[2]).z)); //3
	obb_vertices[4] = (Vector3((obb.center - obb.axis[0]).x, (obb.center - obb.axis[1]).y, (obb.center + obb.axis[2]).z)); //4
	obb_vertices[5] = (Vector3((obb.center + obb.axis[0]).x, (obb.center - obb.axis[1]).y, (obb.center + obb.axis[2]).z)); //5
	obb_vertices[6] = (Vector3((obb.center + obb.axis[0]).x, (obb.center + obb.axis[1]).y, (obb.center + obb.axis[2]).z)); //6 
	obb_vertices[7] = (Vector3((obb.center - obb.axis[0]).x, (obb.center + obb.axis[1]).y, (obb.center + obb.axis[2]).z)); //7

	obb.min = obb_vertices[0];
	obb.max = obb_vertices[6];

	return obb;
}

void Mesh::createOBBGeometry() {
	//delete old
	if (obb_vertices_vbo_id) glDeleteBuffers(1, &vertices_vbo_id);

	//bind vao to store state
	glGenVertexArrays(1, &obb_vao);
	glBindVertexArray(obb_vao);

	glGenBuffers(1, &obb_vertices_vbo_id); //generate one handler (id)
	glBindBuffer(GL_ARRAY_BUFFER, obb_vertices_vbo_id); //bind the handler
	glBufferData(GL_ARRAY_BUFFER, obb_vertices.size() * 3 * sizeof(float), &obb_vertices[0], GL_STATIC_DRAW); //upload data

	//shader must a_vertex attribute, error if not
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION);
	glVertexAttribPointer(VERTEX_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//Store the indices to draw a cube
	obb_indices.push_back(0);
	obb_indices.push_back(1);
	obb_indices.push_back(1);
	obb_indices.push_back(2);
	obb_indices.push_back(2);
	obb_indices.push_back(3);
	obb_indices.push_back(3);
	obb_indices.push_back(0);
	obb_indices.push_back(0);
	obb_indices.push_back(4);
	obb_indices.push_back(1);
	obb_indices.push_back(5);
	obb_indices.push_back(2);
	obb_indices.push_back(6);
	obb_indices.push_back(3);
	obb_indices.push_back(7);
	obb_indices.push_back(7);
	obb_indices.push_back(6);
	obb_indices.push_back(6);
	obb_indices.push_back(5);
	obb_indices.push_back(5);
	obb_indices.push_back(4);
	obb_indices.push_back(4);
	obb_indices.push_back(7);

	//create index array
	glGenBuffers(1, &obb_indices_vbo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obb_indices_vbo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obb_indices.size() * sizeof(unsigned int), &obb_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0); //unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer
}

void Mesh::renderOBB(Shader* sh) {

	glBindVertexArray(obb_vao);

	glDrawElements(GL_LINES, obb_indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Mesh::createMenuMesh() {
	vertices.resize(4);
	uvs.resize(4);

	vertices[0] = Vector3(-1, -1, 0); uvs[0] = Vector2(0, 0);
	vertices[1] = Vector3(1, -1, 0); uvs[1] = Vector2(1, 0);
	vertices[2] = Vector3(1, 1, 0); uvs[2] = Vector2(1, 1);
	vertices[3] = Vector3(-1, 1, 0); uvs[3] = Vector2(0, 1);

	indices.resize(6);
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	genBuffers();
}

void Mesh::createQuad(float v1, float v2, float v3, float v4) {
	vertices.resize(4);
	uvs.resize(4);

	vertices[0] = Vector3(v1, v2, 0); uvs[0] = Vector2(0, 0);
	vertices[1] = Vector3(v3, v2, 0); uvs[1] = Vector2(1, 0);
	vertices[2] = Vector3(v3, v4, 0); uvs[2] = Vector2(1, 1);
	vertices[3] = Vector3(v1, v4, 0); uvs[3] = Vector2(0, 1);

	indices.resize(6);
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	genBuffers();
}

void Mesh::setCollisionModel() {
	collision_model = newCollisionModel3D();

	collision_model->setTriangleNumber(indices.size() / 3);

	for (size_t i = 0; i < indices.size() / 3; i++) {
		Vector3 v1 = vertices[indices[i * 3]];
		Vector3 v2 = vertices[indices[i * 3 + 1]];
		Vector3 v3 = vertices[indices[i * 3 + 2]];

		collision_model->addTriangle(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
	}
	collision_model->finalize();
}