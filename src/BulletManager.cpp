#include "BulletManager.h"
#include "managers.h"
#include "GameObjectEnemy.h"
using namespace std;

BulletManager::BulletManager() {
	can_fire = true;
}

BulletManager::~BulletManager() {

}

BulletManager::BulletManager(Mesh* theMesh, Shader* theShader, int max_bullets, Vector3 color) {
	this->bullet_mesh = theMesh;
	this->shader = theShader;
	this->bullets_vector.resize(max_bullets);
	this->color = color;
	for (int i = 0; i < max_bullets; i++){
		bullets_vector[i].gravity = Vector3(0, 10, -5);
	}
}

void BulletManager::render() {
	//*** Drawing code starts here ***
	for (int i = 0; i < bullets_vector.size(); i++) {
		if (bullets_vector[i].valid) {
			//create our mvp
			Matrix44 m;
			m.traslate(bullets_vector[i].position.x, bullets_vector[i].position.y, bullets_vector[i].position.z);

			Matrix44 mvp = m * g_scene.camera->viewprojection_matrix;

			//enable shader and set uniforms
			shader->enable();
			shader->setMatrix44("u_mvp", mvp);
			shader->setUniform3("u_camera_pos", g_scene.camera->eye);
			shader->setUniform3("color", color);

			//call render function for mesh, passing shader as param
			//mesh->render() binds the relevant attributes, and drawing code
			bullet_mesh->render(GL_TRIANGLES, shader);

			//disable the shader
			shader->disable();

			//disable blending to draw text on top
			glDisable(GL_BLEND);
		}
	}
}

void BulletManager::update(float dt) {
	for (int i = 0; i < bullets_vector.size(); i++) {
		if (bullets_vector[i].valid) {
			bullets_vector[i].old_position = bullets_vector[i].position;
			bullets_vector[i].position = bullets_vector[i].position + (bullets_vector[i].velocity + bullets_vector[i].gravity) * dt;
			
			// Test bullet collision with objects in the scene
			for (int i = 0; i < g_scene.colliders.size(); i++) {

			}
			
			if (bullets_vector[i].ttl <= 0) {
				bullets_vector[i].valid = false;
			}
			else {
				bullets_vector[i].ttl--;
			}
		}
	}
}

void BulletManager::createBullet(Vector3 pos, Vector3 vel, float power, float ttl, GameObject* owner) {
	Bullet b;
	b.position = pos;
	b.velocity = vel;
	b.power = power;
	b.ttl = ttl;
	b.owner = owner;
	b.valid = true;

	for (int i = 0; i < bullets_vector.size(); i++) {
		if (!bullets_vector[i].valid) {
			bullets_vector[i] = b;
			break;
		}
	}
}

void BulletManager::testCollision(std::vector<GameObject*>&colliders) {
	for (size_t i = 0; i < bullets_vector.size(); i++) {
		//skip current bullet if it`s not valid
		if (!bullets_vector[i].valid) continue;

		//loop all colliders
		for (size_t j = 0; j < colliders.size(); j++) {
			if (bullets_vector[i].owner == colliders[j]) continue;

			GameObjectMesh* go = dynamic_cast<GameObjectMesh*>(colliders[j]);

			go->mesh->collision_model->setTransform(go->m.m);

			if (go->mesh->collision_model->rayCollision(bullets_vector[i].old_position.v, bullets_vector[i].position.v, false, 0, 1)) {
				bullets_vector[i].valid = false;

				Vector3 collision_point;
				go->mesh->collision_model->getCollisionPoint(collision_point.v, true);

				go->hit(bullets_vector[i].power);

				continue;
			}
		}
	}
}