#pragma once
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include <unordered_map>
#include "bass.h"

using namespace std;

class ResourceManager
{
private:

	//ResourceManager();
	//~ResourceManager();

	//maps for storing handles to pointers to asset objects
	unordered_map<string, Mesh*>m_meshes;
	unordered_map<string, Texture*>m_textures;
	unordered_map<string, Shader*>m_shaders;
	unordered_map<string, HCHANNEL>m_channels;

public:

	void sartUp();
	void shutDown();

	void loadMesh(const string, const string);
	void loadTexture(const string, const string);
	bool loadShader(const string, const string, const string);
	void loadSample(const string, const string, bool loop);

	void unloadMesh(const string);
	void unloadTexture(const string);
	void unloadShader(const string);

	Mesh* getMesh(const string);
	Texture* getTexture(const string);
	Shader* getShader(const string);
	HCHANNEL getChannel(const string);

};

