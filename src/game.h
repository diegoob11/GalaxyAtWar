/*  by Alun Evans 2016 LaSalle (alunthomasevasevans@gmail.com)
	based on framework by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "BulletManager.h"
#include "camera.h"
#include "Menu.h"
#include "bass.h"

class Game
{
public:
	int state; //decides whether to render/update the menu or the game itself
	Menu* menu;
	static Game* instance;
	HSAMPLE hSample;
	HCHANNEL musicChannel;

	//game logic
	int current_level;
	int weapon_selected;
	std::vector<bool> level_ok;
	Vector3 player_pos;
	bool oof;
	float player_life;
	float enemy_life;

	//bullets
	BulletManager bullet_manager;
	BulletManager magnet_bullet_manager;
	BulletManager enemy_bullet_manager;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;
    
    float time;

	//keyboard state
	const Uint8* keystate;

	//mouse state
	int mouse_state; //tells which buttons are pressed
	Vector2 mouse_position; //last mouse position
	Vector2 mouse_delta; //mouse movement in the last frame
	bool mouse_locked; //tells if the mouse is locked (not seen)
	
	Camera* camera; //our global camera

	Game(SDL_Window* window);
	void init( void );
	void render( void );
	void update( double dt );

	void onKeyPressed( SDL_KeyboardEvent event );
	void onMouseButton( SDL_MouseButtonEvent event );
	void playMainTheme();
	void playWonTheme();
	void playLostTheme();
	void stopMainTheme();
    void onResize( SDL_Event e );
	void changeLevel(int level);
	void exitLevel();
	void initMenu();
    
	void setWindowSize(int width, int height);
};


#endif 