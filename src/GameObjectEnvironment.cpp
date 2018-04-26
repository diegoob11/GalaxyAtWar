#include "GameObjectEnvironment.h"
#include "camera.h"
#include "managers.h"
using namespace std;

GameObjectEnvironment::GameObjectEnvironment() {
	isCollider = false;
}

GameObjectEnvironment::~GameObjectEnvironment() {

}

GameObjectEnvironment::GameObjectEnvironment(string name, Mesh* mesh, Texture* texture, Shader* shader, Vector3 color) {
	this->name = name;
	this->mesh = mesh;
	this->texture = texture;
	this->shader = shader;
	this->color = color;
}


void GameObjectEnvironment::setName(string name) {
	this->name = name;
}

void GameObjectEnvironment::setMesh(Mesh* mesh) {
	this->mesh = mesh;
}

void GameObjectEnvironment::setTexture(Texture* texture) {
	this->texture = texture;
}

void GameObjectEnvironment::setShader(Shader* shader) {
	this->shader = shader;
}

void GameObjectEnvironment::setColor(Vector3 color) {
	this->color = color;
}

void GameObjectEnvironment::render() {
	//*** Drawing code starts here ***

	glCullFace(GL_FRONT);
	glDisable(GL_DEPTH_TEST);

	//create model matrix for our mesh
	m.setTranslation(g_scene.camera->eye.x, g_scene.camera->eye.y, g_scene.camera->eye.z);

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
	this->shader->setTexture("u_texture_diffuse", this->texture);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	this->mesh->render(GL_TRIANGLES, g_resourceManager.getShader("text"));

	//disable the shader
	g_resourceManager.getShader("text")->disable();

	//disable blending to draw text on top
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
}

void GameObjectEnvironment::update(float dt) {
}