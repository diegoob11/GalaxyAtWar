#include "Menu.h"
#include "game.h"
#include "managers.h"
using namespace std;

Menu::Menu() {

}

Menu::Menu(int sw, int sh) {
	cam = new Camera();
	cam->setOrthographic(0,sw, 0, sh, 0.1, 2);
	cam->updateViewMatrix();
}

Menu::~Menu() {
}

void Menu::setMeshes(std::vector<Mesh*> meshes) {
	this->meshes = meshes;
}

void Menu::setShader(Shader* shader) {
	this->shader = shader;
}

void Menu::setTextures(vector<Texture*> textures) {
	this->entries = textures;
	this->current_entry = 1;
}

void Menu::render() {
	//enable shader and set uniforms
	this->shader->enable();
	this->shader->setTexture("u_texture_diffuse", this->entries[this->current_entry]);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	this->meshes[0]->render(GL_TRIANGLES, this->shader);

	//disable the shader
	this->shader->disable();

	//disable blending to draw text on top
	glDisable(GL_BLEND);
}

void Menu::update(float dt) {
	Game* game = Game::instance;

	if (game->keystate[SDL_SCANCODE_LEFT]) {
		this->current_entry--;
		if (this->current_entry < 1) {
			this->current_entry = 4;
		}
		Sleep(200);
	}

	if (game->keystate[SDL_SCANCODE_RIGHT]) {
		this->current_entry++;
		if (this->current_entry > 4) {
			this->current_entry = 1;
		}
		Sleep(200);
	}

	if (game->keystate[SDL_SCANCODE_RETURN]) {
		if (game->level_ok[current_entry - 1]) {
			BASS_ChannelPlay(g_resourceManager.getChannel("select"), true);
		}
		else {
			BASS_ChannelPlay(g_resourceManager.getChannel("error"), true);
		}
		if (this->current_entry == 1 && game->level_ok[0]) {
			game->changeLevel(1);
		}
		else if (this->current_entry == 2 && game->level_ok[1]) {
			game->changeLevel(2);
		}
		else if (this->current_entry == 3 && game->level_ok[2]) {
			game->changeLevel(3);
		}
		else if (this->current_entry == 4 && game->level_ok[3]) {
			game->changeLevel(4);
		}
	}

	if (game->keystate[SDL_SCANCODE_ESCAPE]) {
		exit(0);
	}
}

void Menu::splashScreen() {
	//enable shader and set uniforms
	this->shader->enable();
	this->shader->setTexture("u_texture_diffuse", this->entries[0]);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	this->meshes[0]->render(GL_TRIANGLES, this->shader);

	//disable the shader
	this->shader->disable();

	//disable blending to draw text on top
	glDisable(GL_BLEND);
}

void Menu::renderLoading() {
	//enable shader and set uniforms
	this->shader->enable();
	this->shader->setTexture("u_texture_diffuse", this->entries[5]);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	this->meshes[0]->render(GL_TRIANGLES, this->shader);

	//disable the shader
	this->shader->disable();

	//disable blending to draw text on top
	glDisable(GL_BLEND);
}

void Menu::renderUI(int text, int mesh) {
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	this->shader->enable();
	this->shader->setTexture("u_texture_diffuse", this->entries[5 + text]);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	this->meshes[mesh]->render(GL_TRIANGLES, this->shader);

	//disable the shader
	this->shader->disable();

	//disable blending to draw text on top
	glDisable(GL_BLEND);
}

void Menu::renderLife(bool player, float life, int text) {
	Game* game = Game::instance;

	if (life < 0) life = 0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->shader->enable();
	this->shader->setTexture("u_texture_diffuse", this->entries[5 + text]);

	if (player) {
		this->meshes[4]->createQuad(-0.85, 0.90, -0.8 + game->player_life / 1000, 0.95);
	}
	else {
		this->meshes[4]->createQuad(-0.85, 0.71, -0.8 + game->enemy_life / 3000, 0.75);
	}
	

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	this->meshes[4]->render(GL_TRIANGLES, this->shader);

	//disable the shader
	this->shader->disable();

	//disable blending to draw text on top
	glDisable(GL_BLEND);

}