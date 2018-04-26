#pragma once
#include "GameObjectMesh.h"
#include "camera.h"
#include "bass.h"
using namespace std;

class GameObjectPlayer : public GameObjectMesh {
public:
	float speed;
	float pitch_speed;
	float roll_speed;
	bool camera;

	/****************************/
	GameObjectPlayer();
	~GameObjectPlayer();
	GameObjectPlayer(string name, Mesh* mesh, Texture* texture, Shader* shader, Vector3 color);

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