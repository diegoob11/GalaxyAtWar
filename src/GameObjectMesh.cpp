#include "GameObjectMesh.h"
#include "camera.h"
#include "managers.h"
using namespace std;

GameObjectMesh::GameObjectMesh() {
	this->isCollider = false;
	this->color = Vector3(0.5, 0.5, 0.5);
	this->obb_shader = g_resourceManager.getShader("simple");
}

GameObjectMesh::~GameObjectMesh() {
	
}

GameObjectMesh::GameObjectMesh(string name, Mesh* mesh, Texture* texture, Shader* shader, Vector3 color) {
	this->name = name;
	this->mesh = mesh;
	this->texture = texture;
	this->shader = shader;
	this->color = color;
	this->isCollider = true;
}

void GameObjectMesh::setName(string name) {
	this->name = name;
}

void GameObjectMesh::setMesh(Mesh* mesh) {
	this->mesh = mesh;
}

void GameObjectMesh::setTexture(Texture* texture) {
	this->texture = texture;
}

void GameObjectMesh::setShader(Shader* shader) {
	this->shader = shader;
}

void GameObjectMesh::setColor(Vector3 color) {
	this->color = color;
}

void GameObjectMesh::render() {
	//*** Drawing code starts here ***

	//m.setTranslation(g_scene.camera->center.x, g_scene.camera->center.y, g_scene.camera->center.z);

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
	this->mesh->render(GL_TRIANGLES, this->shader);

	//disable the shader
	this->shader->disable();

	//disable blending to draw text on top
	glDisable(GL_BLEND);

	//render obb
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


void GameObjectMesh::update(float dt) {

	this->obb.center = Vector3(this->m.m[12], this->m.m[13], this->m.m[14]);

	this->obb.min = Vector3(obb.center.x - obb.axis[0].x, obb.center.y - obb.axis[1].y, obb.center.z - obb.axis[2].z);
	this->obb.max = Vector3(obb.center.x + obb.axis[0].x, obb.center.y + obb.axis[1].y, obb.center.z + obb.axis[2].z);

	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->update(dt);
	}
}