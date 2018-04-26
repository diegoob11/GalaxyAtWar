#pragma once

#include "scene.h"
#include "GameObjectMesh.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
using namespace std;

class GameObjectEnemy : public GameObjectMesh {
public:
	float speed;
	float pitch_speed;
	float roll_speed;
	
	//Controlling the movement - AI vectors 
	Vector3 orig_position;
	float radius; // Alert radius to run away from the player
	float range; // Alert radius to fire
	Vector3 to_player; //vector to player
	Vector3 to_target; //vector to target

	/****************************/
	GameObjectEnemy();
	~GameObjectEnemy();
	GameObjectEnemy(string name, Mesh* mesh, Texture* texture, Shader* shader, Vector3 color);

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