#pragma once
#include "GameObject.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
using namespace std;

class GameObjectEnvironment : public GameObject {
public:
	string name;
	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	Vector3 color;

	/****************************/
	GameObjectEnvironment();
	~GameObjectEnvironment();
	GameObjectEnvironment(string name, Mesh* mesh, Texture* texture, Shader* shader, Vector3 color);

	void setName(string name);
	void setMesh(Mesh* mesh);
	void setTexture(Texture* texture);
	void setShader(Shader* shader);
	void setColor(Vector3 color);

	void render();
	void update(float dt);
};