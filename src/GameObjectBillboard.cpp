#include "GameObjectBillboard.h"
#include "managers.h"

GameObjectBillboard::GameObjectBillboard() {

}

GameObjectBillboard::~GameObjectBillboard() {

}
//
//void GameObjectBillboard::setName(string name) {
//	this->name = name;
//}
//
//void GameObjectBillboard::setMesh(Mesh* mesh) {
//	this->mesh = mesh;
//}
//
//void GameObjectBillboard::setTexture(Texture* texture) {
//	this->texture = texture;
//}
//
//void GameObjectBillboard::setShader(Shader* shader) {
//	this->shader = shader;
//}
//
//void GameObjectBillboard::setColor(Vector3 color) {
//	this->color = color;
//}
//void GameObjectBillboard::render() {
//
//	//Vector3 right;
//	//Vector3 up;
//	//Vector3 front;
//
//	//front = (g_scene.camera->eye - position).normalize();
//	//right = (g_scene.camera->up.cross(front)).normalize();
//	//up = g_scene.camera->up.normalize();
//
//	////cout << "front: " << front.x << ", " << front.y << ", " << front.z << endl;
//	////cout << "right: " << right.x << ", " << right.y << ", " << right.z << endl;
//	////cout << "up: " << front.x << ", " << front.y << ", " << front.z << endl;
//
//	////cout << "position: " << position.x << ", " << position.y << ", " << position.z << endl;
//
//	//this->m.m[0] = right.x;
//	//this->m.m[1] = right.y;
//	//this->m.m[2] = right.z;
//	//this->m.m[4] = g_scene.camera->up.x;
//	//this->m.m[5] = g_scene.camera->up.y;
//	//this->m.m[6] = g_scene.camera->up.z;
//	//this->m.m[8] = front.x;
//	//this->m.m[9] = front.y;
//	//this->m.m[10] = front.z;
//
//	//this->m.m[12] = position.x;
//	//this->m.m[13] = position.y;
//	//this->m.m[14] = position.z;
//
//	//*** Drawing code starts here ***
//
//	//create our mvp
//	Matrix44 mvp = m * g_scene.camera->viewprojection_matrix;
//	mvp.m[0] = 1.0;
//	mvp.m[1] = 0.0;
//	mvp.m[2] = 0.0;
//	mvp.m[4] = 0.0;
//	mvp.m[5] = 1.0;
//	mvp.m[6] = 0.0;
//	mvp.m[8] = 0.0;
//	mvp.m[9] = 0.0;
//	mvp.m[10] = 1.0;
//
//	Vector3 up = Vector3(g_scene.camera->view_matrix.m[4], g_scene.camera->view_matrix.m[5], g_scene.camera->view_matrix.m[6]);
//	Vector3 right = Vector3(g_scene.camera->view_matrix.m[1], g_scene.camera->view_matrix.m[2], g_scene.camera->view_matrix.m[3]);
//
//	//create inverse transpose model matrix for normals
//	Matrix44 inverse_transpose_m = m;
//	inverse_transpose_m.inverse();
//	inverse_transpose_m.transpose();
//
//	//glEnable(GL_BLEND);
//	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	//enable shader and set uniforms
//	this->shader->enable();
//	this->shader->setMatrix44("u_mvp", mvp);
//	this->shader->setUniform3("center", position);
//	this->shader->setUniform3("right", right);
//	this->shader->setUniform3("size", Vector3(25, 25, 25));
//	this->shader->setUniform3("up", up);
//	this->shader->setMatrix44("u_normal_matrix", inverse_transpose_m);
//	this->shader->setUniform3("u_camera_pos", g_scene.camera->eye);
//	this->shader->setTexture("u_texture_diffuse", this->texture);
//
//	//call render function for mesh, passing shader as param
//	//mesh->render() binds the relevant attributes, and drawing code
//	this->mesh->render(GL_TRIANGLES, this->shader);
//
//	//disable the shader
//	this->shader->disable();
//
//	//disable blending to draw text on top
//	glDisable(GL_BLEND);
//
//	for (int i = 0; i < this->children.size(); i++) {
//		this->children[i]->render();
//	}
//}
//
//void GameObjectBillboard::update(float dt) {
//
//}