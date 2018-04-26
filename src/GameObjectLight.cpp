#include "GameObjectLight.h"

GameObjectLight::GameObjectLight(Vector3 lightpos){
	this->lightpos = lightpos;	
	isCollider = false;
}

Vector3 GameObjectLight::getLight(){
	return this->lightpos;
}
