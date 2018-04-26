#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "managers.h"
#include "GameObjectMesh.h"
#include "GameObjectPlayer.h"
#include "GameObjectLight.h"
#include "LevelParser.h"
#include <cmath>
using namespace std;

//some globals
float angle = 0;
RenderToTexture* rt = NULL;

Game* Game::instance = NULL;

Game::Game(SDL_Window* window)
{
	this->window = window;
	instance = this;

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	SDL_GetWindowSize( window, &window_width, &window_height );
	cout << " * Window size: " << window_width << " x " << window_height << endl;

	keystate = NULL;
	mouse_locked = true;
}

//Here we have already GL working, so we can create meshes and textures
void Game::init(void)
{
	// Initialize ingame variables 
	state = 0;
	current_level = 0;
	oof = false;
	weapon_selected = 1;
	level_ok.push_back(true);
	level_ok.push_back(false);
	level_ok.push_back(false);
	level_ok.push_back(false);
	player_life = 1000;
	enemy_life = 3000;
	menu = new Menu(window_width, window_height);
    
    //SDL_SetWindowSize(window, 50,50);
	//create our camera
	g_scene.camera = new Camera();
	g_scene.camera->lookAt(Vector3(0, 20, 40), Vector3(0, 0, 0), Vector3(0, 1, 0)); //position the camera and point to 0,0,0
	g_scene.camera->setPerspective(70.0f, (float)window_width / (float)window_height, 0.1f, 16000.0f); //set the projection, we want to be perspective

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	initMenu();

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse

	//music	
	BASS_Init(1, 44100, 0, 0, NULL);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	menu->splashScreen();
	SDL_GL_SwapWindow(this->window);

	LevelParser::loadBasics();
	playMainTheme();
}

//what to do when the image has to be draw
void Game::render(void)
{
	switch (state) {
	case 0: // Menu
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		menu->render();
		SDL_GL_SwapWindow(this->window);
		break;
	case 1: // Game
		//set the clear color (the background color)
		glClearColor(0.0, 0.0, 0.0, 1.0);

		// Clear the window and the depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//update view and projection matrices, and push gl matrices for fixed pipeline rendering
		g_scene.camera->set();
		// Render everything
		g_scene.render();
		bullet_manager.render();
		magnet_bullet_manager.render();
		enemy_bullet_manager.render();
		menu->renderUI(weapon_selected, 1);
		menu->renderUI(7, 3);
		menu->renderLife(true, player_life, 8);
		menu->renderUI(9, 5);
		menu->renderLife(false, enemy_life, 10);
		// Display control messages
		if (oof) {
			menu->renderUI(5, 2);
		}
		SDL_GL_SwapWindow(this->window);
		break;
	case 2: // Enemy dead
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		menu->renderUI(11, 0);
		SDL_GL_SwapWindow(this->window);
		break;
	case 3: // Player dead
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		menu->renderUI(2, 0);
		SDL_GL_SwapWindow(this->window);
		break;
	}
}

void Game::update(double seconds_elapsed)
{
	switch (state) {
		case 0: 
			menu->update(seconds_elapsed);
			break;
		case 1: 
			g_scene.update(seconds_elapsed);
			bullet_manager.update(seconds_elapsed);
			magnet_bullet_manager.update(seconds_elapsed);
			enemy_bullet_manager.update(seconds_elapsed);
			g_scene.colliders.clear();
			g_scene.getColliders(g_scene.root);
			bullet_manager.testCollision(g_scene.colliders);
			magnet_bullet_manager.testCollision(g_scene.colliders);
			enemy_bullet_manager.testCollision(g_scene.colliders);
			for (int i = 0; i < g_scene.colliders.size(); i++) {
				for (int j = i + 1; j < g_scene.colliders.size(); j++) {
					g_scene.testCollision(g_scene.colliders[i], g_scene.colliders[j]);
				}
			}
			// Control if the player is still in range of the world
			if (player_pos.x > 7000 || player_pos.x < -7000 || player_pos.y > 7000 || player_pos.y < -7000 || player_pos.z > 7000 || player_pos.z < -7000) {
				oof = true;
			}
			if (player_pos.x > 8000 || player_pos.x < -8000 || player_pos.y > 8000 || player_pos.y < -8000 || player_pos.z > 8000 || player_pos.z < -8000) {
				state = 0;
				exitLevel();
				oof = false;
			}
			if (player_pos.x < 7000 && player_pos.x > -7000 && player_pos.y < 7000 && player_pos.y > -7000 && player_pos.z < 7000 && player_pos.z > -7000) {
				oof = false;
			}
			break;
		case 2: // Enemy dead
			if (keystate[SDL_SCANCODE_ESCAPE]) {
				Sleep(400);
				exit(0);
			}
			break;
		case 3: // Player dead
			if (keystate[SDL_SCANCODE_ESCAPE]) {
				Sleep(400);
				exit(0);
			}
			break;
	}
}

//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		//case SDLK_ESCAPE: exit(0); //ESC key, kill the app
	}
}


void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::setWindowSize(int width, int height)
{
    //cout << "window resized: " << width << "," << height << endl;
	glViewport( 0,0, width, height );
	g_scene.camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

void Game::changeLevel(int level) {
	switch (level) {
		case 1:
			if (current_level == 0) {
				//Load level
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				menu->renderLoading();
				SDL_GL_SwapWindow(this->window);

				LevelParser::loadLevel("data/levels/level1.xml");
				bullet_manager = BulletManager(g_resourceManager.getMesh("bullet"), g_resourceManager.getShader("simple"), 100, Vector3(0.0, 0.8, 0.8));
				magnet_bullet_manager = BulletManager(g_resourceManager.getMesh("bullet2"), g_resourceManager.getShader("simple"), 2, Vector3(0.0, 1.0, 0.0));
				enemy_bullet_manager = BulletManager(g_resourceManager.getMesh("bullet"), g_resourceManager.getShader("simple"), 20, Vector3(0.0, 1.0, 0.0));

				stopMainTheme();
				BASS_ChannelPlay(g_resourceManager.getChannel("level1"), true);
			}
			current_level = 1;
			state = 1;
			break;
	}
	
}

void Game::exitLevel() {
	//g_scene.~Scene();
	if (current_level == 1) {
		BASS_ChannelPause(g_resourceManager.getChannel("level1"));
	}
	playMainTheme();
}

void Game::playMainTheme() {
	BASS_ChannelPlay(g_resourceManager.getChannel("main"), true);
}

void Game::playWonTheme() {
	if (current_level == 1) {
		BASS_ChannelPause(g_resourceManager.getChannel("level1"));
	}
	BASS_ChannelPlay(g_resourceManager.getChannel("win"), true);
}

void Game::playLostTheme() {
	if (current_level == 1) {
		BASS_ChannelPause(g_resourceManager.getChannel("level1"));
	}
	BASS_ChannelPlay(g_resourceManager.getChannel("gameOver"), true);
}
void Game::stopMainTheme() {
	BASS_ChannelPause(g_resourceManager.getChannel("main"));
}

void Game::initMenu() {
	//create the screen space quad
	vector<Mesh*> meshes;
	Mesh* mesh = new Mesh();
	Mesh* mesh2 = new Mesh();
	Mesh* mesh3 = new Mesh();
	Mesh* mesh4 = new Mesh();
	Mesh* mesh5 = new Mesh();
	Mesh* mesh6 = new Mesh();

	mesh->createMenuMesh();
	mesh2->createQuad(-0.5, -0.95, 0.5, -0.65);
	mesh3->createQuad(-0.5, -0.4, 0.5, 0.4);
	mesh4->createQuad(-0.95, 0.85, -0.85, 0.95);
	mesh6->createQuad(-0.95, 0.65, -0.85, 0.75);

	meshes.push_back(mesh);
	meshes.push_back(mesh2);
	meshes.push_back(mesh3);
	meshes.push_back(mesh4);
	meshes.push_back(mesh5);
	meshes.push_back(mesh6);

	//Create the menu
	vector<Texture*> menu_textures;
	menu_textures.resize(17);
	g_resourceManager.loadTexture("splash", "data/assets/splash.tga");
	g_resourceManager.loadTexture("loading", "data/assets/loading.tga");
	g_resourceManager.loadTexture("level1", "data/assets/menu1.tga");
	g_resourceManager.loadTexture("level2", "data/assets/menu2.tga");
	g_resourceManager.loadTexture("level3", "data/assets/menu3.tga");
	g_resourceManager.loadTexture("level4", "data/assets/menu4.tga");
	g_resourceManager.loadTexture("weapons1", "data/assets/weapons1.tga");
	g_resourceManager.loadTexture("weapons2", "data/assets/weapons2.tga");
	g_resourceManager.loadTexture("weapons3", "data/assets/weapons3.tga");
	g_resourceManager.loadTexture("weapons4", "data/assets/weapons4.tga");
	g_resourceManager.loadTexture("oof", "data/assets/oof.tga");
	g_resourceManager.loadTexture("crashed", "data/assets/lost.tga");
	g_resourceManager.loadTexture("falcon_logo", "data/assets/falcon_logo.tga");
	g_resourceManager.loadTexture("falcon_life", "data/assets/falcon_life.tga");
	g_resourceManager.loadTexture("enemy_logo", "data/assets/enemy_logo.tga");
	g_resourceManager.loadTexture("enemy_life", "data/assets/enemy_life.tga");
	g_resourceManager.loadTexture("won", "data/assets/win.tga");

	menu_textures[0] = g_resourceManager.getTexture("splash");
	menu_textures[1] = g_resourceManager.getTexture("level1");
	menu_textures[2] = g_resourceManager.getTexture("level2");
	menu_textures[3] = g_resourceManager.getTexture("level3");
	menu_textures[4] = g_resourceManager.getTexture("level4");
	menu_textures[5] = g_resourceManager.getTexture("loading");
	menu_textures[6] = g_resourceManager.getTexture("weapons1");
	menu_textures[7] = g_resourceManager.getTexture("weapons2");
	menu_textures[8] = g_resourceManager.getTexture("weapons3");
	menu_textures[9] = g_resourceManager.getTexture("weapons4");
	menu_textures[10] = g_resourceManager.getTexture("oof");
	menu_textures[11] = g_resourceManager.getTexture("crashed");
	menu_textures[12] = g_resourceManager.getTexture("falcon_logo");
	menu_textures[13] = g_resourceManager.getTexture("falcon_life");
	menu_textures[14] = g_resourceManager.getTexture("enemy_logo");
	menu_textures[15] = g_resourceManager.getTexture("enemy_life");
	menu_textures[16] = g_resourceManager.getTexture("won");

	g_resourceManager.loadShader("menu", "data/shaders/screenspace.vert", "data/shaders/textscreenspace.frag");

	menu->setMeshes(meshes);
	menu->setTextures(menu_textures);
	menu->setShader(g_resourceManager.getShader("menu"));
}