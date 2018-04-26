#include "GameObjectBullet.h"
#include "managers.h"

using namespace std;

GameObjectBullet::GameObjectBullet() {};

GameObjectBullet::~GameObjectBullet() {};

GameObjectBullet::GameObjectBullet(Vector3 pos, Vector3 vel, float power, float ttl, GameObject* owner) {
	this->position = pos;
	this->velocity = vel;
	this->power = power;
	this->ttl = ttl;
	this->owner = owner;
}

void GameObjectBullet::render(Mesh* mesh, Shader* shader) {
	
}

void GameObjectBullet::update(float dt) {
	if (valid) {
		old_position = position;
		position = position + (velocity + gravity) * dt;
		if (ttl <= 0) {
			valid = false;
		}
		else {
			ttl--;
		}
	}
}
