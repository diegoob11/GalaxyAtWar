#include "GameObjectEnemy.h"
#include "bass.h"
#include "managers.h"
#include "game.h"
#include <random>
using namespace std;

GameObjectEnemy::GameObjectEnemy() {
	this->isCollider = true;
	this->obb_shader = g_resourceManager.getShader("simple");
	this->type = 3;
	this->radius = 2000;
	this->orig_position = Vector3(200, 700, -500);
	this->obb_color = Vector3(0.8, 0.8, 0.0);
	this->life = 3000;
	this->range = 2500;
	this->in_range = false;
}

GameObjectEnemy::~GameObjectEnemy() {
	this->parent->removeChild(this);
}

GameObjectEnemy::GameObjectEnemy(string name, Mesh* mesh, Texture* texture, Shader* shader, Vector3 color) {
	this->name = name;
	this->mesh = mesh;
	this->texture = texture;
	this->shader = shader;
	this->color = color;
	this->isCollider = true;
}

void GameObjectEnemy::setName(string name) {
	this->name = name;
}

void GameObjectEnemy::setMesh(Mesh* mesh) {
	this->mesh = mesh;
}

void GameObjectEnemy::setTexture(Texture* texture) {
	this->texture = texture;
}

void GameObjectEnemy::setShader(Shader* shader) {
	this->shader = shader;
}

void GameObjectEnemy::setColor(Vector3 color) {
	this->color = color;
}

void GameObjectEnemy::setSpeeds(float pitch, float roll) {
	this->pitch_speed = pitch;
	this->roll_speed = roll;
}

void GameObjectEnemy::hit(float dmg) {
	if (life > 0) {
		this->life -= dmg;
	}
	cout << "enemy life: " << life << endl;
}

void GameObjectEnemy::colliding() {
	life -= 50;
}

void GameObjectEnemy::render() {
	//*** Drawing code starts here ***

	//create our mvp
	Matrix44 mvp = m * g_scene.camera->viewprojection_matrix;

	//create inverse transpose model matrix for normals
	Matrix44 inverse_transpose_m = m;
	inverse_transpose_m.inverse();
	inverse_transpose_m.transpose();

	//enable shader and set uniforms
	this->shader->enable();
	this->shader->setMatrix44("u_normal_matrix", inverse_transpose_m);
	this->shader->setMatrix44("u_mvp", mvp);
	this->shader->setUniform3("u_camera_pos", g_scene.camera->eye);
	this->shader->setUniform3("u_light", g_scene.lights[0]);
	this->shader->setTexture("u_texture_diffuse", this->texture);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	this->mesh->render(GL_TRIANGLES, shader);

	//disable the shader
	this->shader->disable();

	//disable blending to draw text on top
	glDisable(GL_BLEND);	
	if (this->shouldDrawOBB) {
		glEnable(GL_BLEND);
		obb_shader->enable();
		this->obb_shader->setMatrix44("u_mvp", mvp);
		this->obb_shader->setUniform3("color", this->obb_color);
		this->mesh->renderOBB(obb_shader);
		obb_shader->disable();
		glDisable(GL_BLEND);
	}

	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->render();
	}
}


void GameObjectEnemy::update(float dt) {
	speed = -dt * 600;
	Game* game = Game::instance;

	Vector3 object_pos = Vector3(this->m.m[12], this->m.m[13], this->m.m[14]);
	to_player = game->player_pos - object_pos;

	if (to_player.x < range && to_player.y < range && to_player.z < range) in_range = true;

	if (to_player.x < radius && to_player.y < radius && to_player.z < radius) {
		to_target = to_player * -1;
	}
	if (to_player.x > radius || to_player.y > radius || to_player.z > radius) {
		to_target = to_player;
	}

	Vector3 front = this->m.frontVector().normalize();

	float angle = acos(to_target.dot(front));
	Vector3 axis = to_target.cross(front);

	Matrix44 inv = this->m; 
	inv.inverse(); 
	Vector3 rot_axis = inv.rotateVector(axis); 
	this->m.rotateLocal(-0.005, rot_axis);

	this->m.traslate(front.x*speed, front.y*speed, front.z*speed);

	this->obb.center = object_pos;

	this->obb.min = Vector3(obb.center.x - obb.axis[0].x, obb.center.y - obb.axis[1].y, obb.center.z - obb.axis[2].z);
	this->obb.max = Vector3(obb.center.x + obb.axis[0].x, obb.center.y + obb.axis[1].y, obb.center.z + obb.axis[2].z);

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1, 2);
	if (distribution(generator) == 1 && in_range) {
		game->enemy_bullet_manager.createBullet(object_pos + front, front * -6000, 5, 700, this);
	}

	game->enemy_life = life;

	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->update(dt);
	}

	if (life <= 0) {
		BASS_ChannelPlay(g_resourceManager.getChannel("explosion"), false);
		//this->~GameObjectEnemy();
		game->state = 2;
		game->playWonTheme();
	}
}