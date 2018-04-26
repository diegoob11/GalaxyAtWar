#include "GameObject.h"
#include "GameObjectMesh.h"
#include "Scene.h"
#include <math.h>

Scene::Scene() {
	this->root = new GameObject();
}

Scene::~Scene() {
	this->root->clear();
	delete this->root;
}

void Scene::render() {
	environment_go->render();
	this->root->render();
}

void Scene::update(float dt) {
	this->root->update(dt);
}

void Scene::getColliders(GameObject* current) {
	if (current->isCollider) {
		colliders.push_back(current);
	}
	int i = 0;
	for (vector<GameObject*>::iterator it = current->children.begin(); it != current->children.end(); ++it) {
		getColliders(*it);
		i++;
	}
}

bool Scene::testCollision(GameObject* o1, GameObject* o2) {
	bool col;

	if (o1->obb.min.x > o2->obb.max.x || o1->obb.max.x < o2->obb.min.x || o1->obb.min.y > o2->obb.max.y || o1->obb.max.y < o2->obb.min.y || o1->obb.max.z < o2->obb.min.z || o1->obb.min.z > o2->obb.max.z) {
		col = false;
	}
	else {
		col = testFurtherCollision(o1, o2);
	}

	return col;
}

bool Scene::testFurtherCollision(GameObject* o1, GameObject* o2) {
	GameObjectMesh* A = dynamic_cast<GameObjectMesh*>(o1);
	GameObjectMesh* B = dynamic_cast<GameObjectMesh*>(o2);

	//update collision model matrix for each mesh 
	A->mesh->collision_model->setTransform(A->m.m); 
	B->mesh->collision_model->setTransform(B->m.m);
	//test 
	bool col = A->mesh->collision_model->collision(B->mesh->collision_model);
	if (col) {
		//do something!
		A->colliding();
		B->colliding();
		A->life -= 50;
		B->life -= 50;
	}

	return col;
}