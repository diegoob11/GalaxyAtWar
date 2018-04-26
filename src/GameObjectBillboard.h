#pragma once
#include "GameObjectMesh.h"
using namespace std;

class GameObjectBillboard : public GameObjectMesh {
public:
	GameObjectBillboard();
	~GameObjectBillboard();
	void render();
	void update(float dt);
};