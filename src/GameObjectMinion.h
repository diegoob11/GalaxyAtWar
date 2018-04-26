#pragma once
#include "scene.h"
#include "GameObjectMesh.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
using namespace std;

class GameObjectMinion : public GameObjectMesh {
public:
	float speed;
	float pitch_speed;
	float roll_speed;
	float range;

	//Controlling the movement - AI vectors 
	Vector3 to_target; //vector to target

	/****************************/
	GameObjectMinion();
	~GameObjectMinion();

	void setName(string name);
	void setMesh(Mesh* mesh);
	void setTexture(Texture* texture);
	void setShader(Shader* shader);
	void setColor(Vector3 color);
	void setSpeeds(float pitch, float roll);
	void hit(float dmg);
	void colliding();

	void render();
	void update(float dt);
};