#pragma once
#include <string>
#include "framework.h"
#include "camera.h"
#include "mesh.h"
#include "shader.h"
using namespace std;


class GameObject {
public: 
	GameObject();
	string name;
	GameObject* parent;
	vector<GameObject*> children;
	Matrix44 m;

	int type; //The type of the object, 1 is player, 2 is objective, 3 enemies

	float life;
	bool isCollider;
	OBB obb; // Stores the obb
	Shader* obb_shader;
	Vector3 obb_color;
	bool shouldDrawOBB; //Boolean to control if the AABB should or not be drawn
	bool in_range; 

	void addChild(GameObject* obj);
	void removeChild(GameObject* obj);
	void clear();
	Matrix44 getGlobalMatrix();
	virtual void hit(float dmg);
	virtual void colliding();

	virtual void render();
	virtual void update(float dt);
};