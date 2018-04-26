#pragma once
#include "GameObject.h"
#include "GameObjectEnvironment.h"
#include "camera.h"
#include "BulletManager.h"
using namespace std;

class Scene
{
public:
	Scene();
	~Scene();
	//scene graph starts from here
	GameObject* root;
	//the camera used to render the scene
	Camera* camera;
	GameObjectEnvironment* environment_go;
	vector<GameObject*> colliders;

	//a list of all lights - could be within graph or not
	vector<Vector3> lights;

	//recursive function, finds all collider objects in scene
	void getColliders(GameObject* current);
	bool testCollision(GameObject* i, GameObject* j);
	bool testFurtherCollision(GameObject* i, GameObject* j);

	//standard render and update functions
	void render();
	void update(float);
};