#pragma once
#include "mesh.h"
#include "GameObject.h"

using namespace std;

struct Bullet {
	bool valid; //Is it alive?
	Vector3 position;
	Vector3 old_position;
	Vector3 velocity; //Movement vector
	float ttl; //TimeToLive
	float power; //Damage
	GameObject* owner; //Who fired
	Vector3 gravity; //Gravity towards the sun
};

class BulletManager {
public:
	Mesh* bullet_mesh; //Mesh to render the bullet
	vector<Bullet> bullets_vector; //Array of all bullets
	Shader* shader; //Shader used to draw the bullet
	bool can_fire; //Controls fire rate
	Vector3 color; //Bullet color
	

	BulletManager();
	~BulletManager();
	BulletManager(Mesh* theMesh, Shader* theShader, int max_bullets, Vector3 Color);

	void render();
	void update(float elapsed_time);
	void createBullet(Vector3 pos, Vector3 vel, float power, float ttl, GameObject* owner);
	void testCollision(std::vector<GameObject*>&colliders);
};
