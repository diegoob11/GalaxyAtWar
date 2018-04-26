#include "GameObjectPlayer.h"
#include "camera.h"
#include "managers.h"
#include "game.h"
#include "GameObjectEnemy.h"
using namespace std;

GameObjectPlayer::GameObjectPlayer() {
	this->isCollider = true;
	this->camera = true;
	this->obb_shader = g_resourceManager.getShader("simple");
	this->type = 1;
	this->life = 1000;
}

GameObjectPlayer::~GameObjectPlayer() {

}

GameObjectPlayer::GameObjectPlayer(string name, Mesh* mesh, Texture* texture, Shader* shader, Vector3 color) {
	this->name = name;
	this->mesh = mesh;
	this->texture = texture;
	this->shader = shader;
	this->color = color;
	this->isCollider = true;
}

void GameObjectPlayer::setName(string name) {
	this->name = name;
}

void GameObjectPlayer::setMesh(Mesh* mesh) {
	this->mesh = mesh;
}

void GameObjectPlayer::setTexture(Texture* texture) {
	this->texture = texture;
}

void GameObjectPlayer::setShader(Shader* shader) {
	this->shader = shader;
}

void GameObjectPlayer::setColor(Vector3 color) {
	this->color = color;
}

void GameObjectPlayer::setSpeeds(float pitch, float roll) {
	this->pitch_speed = pitch;
	this->roll_speed = roll;
}

void GameObjectPlayer::hit(float dmg) {
	if (life > 0) {
		this->life -= dmg;
	}
	cout << "player life: " << life << endl;
}

void GameObjectPlayer::colliding() {
	life -= 50;
	BASS_ChannelPlay(g_resourceManager.getChannel("chewbacca"), true);
}

void GameObjectPlayer::render() {
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
	this->mesh->render(GL_TRIANGLES, this->shader);

	//disable the shader
	this->shader->disable();

	//disable blending to draw text on top
	glDisable(GL_BLEND);
	if (this->shouldDrawOBB) {
		glEnable(GL_BLEND);
		obb_shader->enable();
		this->obb_shader->setMatrix44("u_mvp", mvp);
		this->mesh->renderOBB(obb_shader);
		obb_shader->disable();
		glDisable(GL_BLEND);
	}

	for(int i = 0; i < this->children.size(); i++){
		this->children[i]->render();
	}
}

void GameObjectPlayer::update(float dt) {
	speed = -dt * 100; //the speed is defined by the seconds_elapsed so it goes constant
	Game* game = Game::instance;

	Vector3 f = this->m.frontVector().normalize();

	Vector3 player_pos = Vector3(this->m.m[12], this->m.m[13], this->m.m[14]);
	Vector3 ideal_pos;
	Vector3 up = Vector3(this->m.m[4], this->m.m[5], this->m.m[6]).normalize();
	Vector3 right = Vector3(this->m.m[0], this->m.m[1], this->m.m[2]).normalize();

	//async input to move the mesh around and do stuff

	if (game->keystate[SDL_SCANCODE_1]) {
		game->weapon_selected = 1;
		BASS_ChannelPlay(g_resourceManager.getChannel("changeWeapon"), true);
	}

	if (game->keystate[SDL_SCANCODE_2]) {
		game->weapon_selected = 2;
		BASS_ChannelPlay(g_resourceManager.getChannel("changeWeapon"), true);
	}

	if (game->keystate[SDL_SCANCODE_3]) {
		game->weapon_selected = 3;
		BASS_ChannelPlay(g_resourceManager.getChannel("changeWeapon"), true);
	}

	if (game->keystate[SDL_SCANCODE_4]) {
		game->weapon_selected = 4;
		BASS_ChannelPlay(g_resourceManager.getChannel("changeWeapon"), true);
	}

	//move faster with left shift
	if (game->keystate[SDL_SCANCODE_LSHIFT]) speed *= 6;

	//Stay still
	if (game->keystate[SDL_SCANCODE_LCTRL]) speed = 0;

	if (game->keystate[SDL_SCANCODE_UP]) {
		this->m.rotateLocal(dt * pitch_speed, Vector3(1, 0, 0));
	}

	if (game->keystate[SDL_SCANCODE_DOWN]) {
		this->m.rotateLocal(-dt * pitch_speed, Vector3(1, 0, 0));
	}
	
	if (game->keystate[SDL_SCANCODE_RIGHT]) {
		this->m.rotateLocal(dt * roll_speed, Vector3(0,0,1));
	}
	
	if (game->keystate[SDL_SCANCODE_LEFT]) {
		this->m.rotateLocal(-dt * roll_speed, Vector3(0, 0, 1));
	}

	if (game->keystate[SDL_SCANCODE_P]) {
		if (camera) {
			camera = false;
		}
		else {
			camera = true;
		}
	}

	if (game->keystate[SDL_SCANCODE_SPACE]) {
		if (game->weapon_selected == 1) {
			game->bullet_manager.createBullet(player_pos + f + up * 50, f * -6000, 20, 700, this);
			BASS_ChannelPlay(g_resourceManager.getChannel("laserBeam"), true);
		}
		else if (game->weapon_selected == 2) {
			game->magnet_bullet_manager.createBullet(player_pos + f + up * -1, f * -2000, 60, 500, this);
			BASS_ChannelPlay(g_resourceManager.getChannel("magnetGun"), true);
		}
		else if (game->weapon_selected == 3) {
			BASS_ChannelPlay(g_resourceManager.getChannel("error"), true);
		}
		else if (game->weapon_selected == 4) {
			BASS_ChannelPlay(g_resourceManager.getChannel("error"), true);
		}
	}

	if (game->keystate[SDL_SCANCODE_ESCAPE]) {
		Sleep(400);
		game->exitLevel();
		game->state = 0;
	}

	if (camera) {
		ideal_pos = Vector3(player_pos + f * 200 + up * 60);
		g_scene.camera->lookAt(g_scene.camera->eye.lerp(ideal_pos, 0.1), player_pos + f*-20, up);
	}
	else if (!camera) {
		ideal_pos = Vector3(player_pos + f * -30 + up * 60 + right * 60);
		g_scene.camera->lookAt(g_scene.camera->eye.lerp(ideal_pos, 0.1), player_pos + f*-200 + right * -80, up);
	}

	this->m.traslate(f.x*speed, f.y*speed, f.z*speed);

	this->obb.center = player_pos;
	this->obb.min = Vector3(obb.center.x - obb.axis[0].x, obb.center.y - obb.axis[1].y, obb.center.z - obb.axis[2].z);
	this->obb.max = Vector3(obb.center.x + obb.axis[0].x, obb.center.y + obb.axis[1].y, obb.center.z + obb.axis[2].z);

	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->update(dt);
	}

	game->player_pos = player_pos;
	game->player_life = life;

	if (life <= 0) {
		BASS_ChannelPlay(g_resourceManager.getChannel("explosion"), false);
		game->state = 3;
		game->playLostTheme();
	}

}