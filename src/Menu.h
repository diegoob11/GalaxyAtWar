#pragma once
#include "mesh.h"
#include "texture.h"
#include "camera.h"
#include "shader.h"

class Menu {
public:
	Menu();
	Menu(int sw, int sh);
	~Menu();

	std::vector<Mesh*> meshes;
	Camera* cam;
	std::vector<Texture*> entries;
	int current_entry;
	Shader* shader;

	void setMeshes(std::vector<Mesh*> meshes);
	void setShader(Shader* shader);
	void setTextures(std::vector<Texture*> textures);
	void render();
	void update(float dt);
	void splashScreen();
	void renderLoading();
	void renderUI(int text, int mesh);
	void renderLife(bool player, float life, int text);
};