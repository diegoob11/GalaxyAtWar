#include "GameObject.h"
#include <algorithm>
using namespace std;

GameObject::GameObject() {
	parent = NULL;
	isCollider = false;
	
}

void GameObject::addChild(GameObject* obj) {
	//make this object the parent of the child
	obj->parent = this;
	
	//add to list
	this->children.push_back(obj);
}

void GameObject::removeChild(GameObject* obj) {
	//moves obj to end of vector
	vector<GameObject*>::iterator i = remove(this->children.begin(),
		this->children.end(), obj);
	
	//actually removes it
	this->children.erase(i, this->children.end());

	//delete(obj);
}

void GameObject::clear() {
	
}

Matrix44 GameObject::getGlobalMatrix() {
	if (this->parent)
		return this->parent->getGlobalMatrix() * this->m;
	else
		return this->m;
}

void GameObject::render() {
	for (int i = 0; i < children.size(); i++) {
		this->children[i]->render();
	}
}

void GameObject::update(float dt) {
	for (int i = 0; i < children.size(); i++) {
		this->children[i]->update(dt);
	}
}

void GameObject::hit(float dmg) {

}

void GameObject::colliding() {

}
