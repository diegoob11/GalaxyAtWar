#pragma once
#include "GameObject.h"
#include "mesh.h"
#include "shader.h"

class GameObjectBullet:GameObject {
public:


	GameObjectBullet();
	GameObjectBullet(Vector3 pos, Vector3 vel, float power, float ttl, GameObject* owner);
	~GameObjectBullet();

	void render(Mesh* mesh, Shader* shader);
	void update(float elapsed_time);
};
