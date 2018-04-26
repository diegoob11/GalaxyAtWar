#include "ResourceManager.h"

using namespace std;


//start up our Resource Manager
void startUp() {

}

//shut down our Resource Manager
void shutDown() {

}

//loads an ASE Mesh
void ResourceManager::loadMesh(string name, string path) {
	Mesh *mesh = new Mesh();
	mesh->loadASE(path.c_str());
	m_meshes[name] = mesh;
}

//loads a texture
void ResourceManager::loadTexture(string name, string path) {
	Texture *text = new Texture();
	text->load(path.c_str());
	m_textures[name] = text;
}

//loads a shader
bool ResourceManager::loadShader(string name, string path1, string path2) {
	try {
		Shader *shader = new Shader();
		shader->load(path1.c_str(), path2.c_str());
		m_shaders[name] = shader;
	}
	catch (exception) {
		return false;
	}
	return true;
}

void ResourceManager::loadSample(string name, string path, bool loop) {
	HSAMPLE hSample;
	if (loop) {
		hSample = BASS_SampleLoad(false, path.c_str(), 0, 0, 3, BASS_SAMPLE_LOOP);
	}
	else {
		hSample = BASS_SampleLoad(false, path.c_str(), 0, 0, 3, 0);
	}
	HCHANNEL hChannel = BASS_SampleGetChannel(hSample, false);
	m_channels[name] = hChannel;
}

//unloads a mesh
void ResourceManager::unloadMesh(const string name) {

}

//unloads a texture
void ResourceManager::unloadTexture(const string name) {

}

//unloads a shader
void ResourceManager::unloadShader(const string name) {

}

//returns an specific mesh, MUST have been previously loaded
Mesh* ResourceManager::getMesh(const string name) {
	try {
		m_meshes.at(name);
	}
	catch (out_of_range) {
		cout << "Mesh not found." << endl;
	}

	return m_meshes.at(name);
}

//returns an specific texture, MUST have been previously loaded
Texture* ResourceManager::getTexture(const string name) {
	try {
		m_textures.at(name);
	}
	catch (out_of_range) {
		cout << "Texture not found." << endl;
	}

	return m_textures.at(name);
}

//returns an specific shader, MUST have been previously loaded
Shader* ResourceManager::getShader(const string name) {
	try {
		m_shaders.at(name);
	}
	catch (out_of_range) {
		cout << "Shader not found." << endl;
	}

	return m_shaders.at(name);
}

HCHANNEL ResourceManager::getChannel(const string name) {
	try {
		m_channels.at(name);
	}
	catch (out_of_range) {
		cout << "Channel not found." << endl;
	}

	return m_channels.at(name);
}