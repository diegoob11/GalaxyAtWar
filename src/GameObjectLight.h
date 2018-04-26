#pragma once

#include "scene.h"
#include "GameObject.h"

class GameObjectLight : public GameObject {
public:
	Vector3 lightpos;

	GameObjectLight();
	GameObjectLight(Vector3 lightpos);
	~GameObjectLight();

	Vector3 getLight();
};