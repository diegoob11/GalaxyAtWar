#include "LevelParser.h"
#include "GameObject.h"
#include "GameObjectEnvironment.h"
#include "GameObjectLight.h"
#include "GameObjectMesh.h"
#include "GameObjectPlayer.h"
#include "GameObjectEnemy.h"
#include "GameObjectMinion.h"
#include "managers.h"

namespace LevelParser {

	//Recursive function to load a player into the scene
	void loadPlayer(xml_node<> *mesh_node, GameObject* parent) {
		GameObjectPlayer* new_mesh = new GameObjectPlayer();

		new_mesh->setName(mesh_node->first_node("name")->value());
		new_mesh->setMesh(g_resourceManager.getMesh(mesh_node->first_node("mesh")->value()));

		new_mesh->setShader(g_resourceManager.getShader(mesh_node->first_node("shader")->value()));
		new_mesh->setTexture(g_resourceManager.getTexture(mesh_node->first_node("texture")->value()));
		new_mesh->setSpeeds((float)atof(mesh_node->first_node("pitchspeed")->value()), (float)atof(mesh_node->first_node("rollspeed")->value()));

		if (strcmp(mesh_node->first_node("transform")->name(), "transform") == 0) {
			for (xml_node<> *child = mesh_node->first_node("transform")->first_node(); child; child = child->next_sibling()) {
				if (strcmp(child->name(), "scale") == 0) {
					new_mesh->m.scale((float)atof(child->first_node("x")->value()), (float)atof(child->first_node("y")->value()), (float)atof(child->first_node("z")->value()));
				}
				if (strcmp(child->name(), "translate") == 0) {
					new_mesh->m.traslate((float)atof(child->first_node("x")->value()), (float)atof(child->first_node("y")->value()), (float)atof(child->first_node("z")->value()));
				}
				if (strcmp(child->name(), "rotate") == 0) {
					float angle = (float)atof(child->first_node("angle")->value());
					int x = (int)atof(child->first_node("axis")->first_node("x")->value());
					int y = (int)atof(child->first_node("axis")->first_node("y")->value());
					int z = (int)atof(child->first_node("axis")->first_node("z")->value());
					new_mesh->m.rotate(angle, Vector3(x, y, z));
				}
			}
		}

		new_mesh->obb = new_mesh->mesh->findOBB();
		new_mesh->mesh->createOBBGeometry();
		new_mesh->shouldDrawOBB = false;
		new_mesh->mesh->setCollisionModel();

//		new_mesh->mesh->setCollisionModel();
		parent->addChild(new_mesh);
		for (xml_node<> *child = mesh_node->first_node("player"); child; child = child->next_sibling()) {
			loadPlayer(child, new_mesh);
		}
		
	}
	
	//Recursive function to load an enemy into the scene
	void loadEnemy(xml_node<> *mesh_node, GameObject* parent) {
		GameObjectEnemy* new_mesh = new GameObjectEnemy();
		//Find name, texture, shader and mesh of enemy
		new_mesh->setName(mesh_node->first_node("name")->value());
		new_mesh->setMesh(g_resourceManager.getMesh(mesh_node->first_node("mesh")->value()));

		new_mesh->setShader(g_resourceManager.getShader(mesh_node->first_node("shader")->value()));
		new_mesh->setTexture(g_resourceManager.getTexture(mesh_node->first_node("texture")->value()));
		//Find speeds of enemy
		new_mesh->setSpeeds((float)atof(mesh_node->first_node("pitchspeed")->value()), (float)atof(mesh_node->first_node("rollspeed")->value()));
		//Find transformations in y
		if (strcmp(mesh_node->first_node("transform")->name(), "transform") == 0) {
			for (xml_node<> *child = mesh_node->first_node("transform")->first_node(); child; child = child->next_sibling()) {
				if (strcmp(child->name(), "scale") == 0) {
					new_mesh->m.scale((float)atof(child->first_node("x")->value()), (float)atof(child->first_node("y")->value()), (float)atof(child->first_node("z")->value()));
				}
				if (strcmp(child->name(), "translate") == 0) {
					new_mesh->m.traslate((float)atof(child->first_node("x")->value()), (float)atof(child->first_node("y")->value()), (float)atof(child->first_node("z")->value()));
				}
				if (strcmp(child->name(), "rotate") == 0) {
					float angle = (float)atof(child->first_node("angle")->value());
					int x = (int)atof(child->first_node("axis")->first_node("x")->value());
					int y = (int)atof(child->first_node("axis")->first_node("y")->value());
					int z = (int)atof(child->first_node("axis")->first_node("z")->value());
					new_mesh->m.rotate(angle, Vector3(x, y, z));
				}
			}
		}

		new_mesh->obb = new_mesh->mesh->findOBB();
		new_mesh->mesh->createOBBGeometry();
		new_mesh->shouldDrawOBB = true;
		new_mesh->mesh->setCollisionModel();

		parent->addChild(new_mesh);
		for (xml_node<> *child = mesh_node->first_node("enemy"); child; child = child->next_sibling()) {
			loadEnemy(child, new_mesh);
		}
		
	}

	//Recursive function to load an enemy into the scene
	void loadMinion(xml_node<> *mesh_node, GameObject* parent) {
		GameObjectMinion* new_mesh = new GameObjectMinion();
		//Find name, texture, shader and mesh of enemy
		new_mesh->setName(mesh_node->first_node("name")->value());
		new_mesh->setMesh(g_resourceManager.getMesh(mesh_node->first_node("mesh")->value()));

		new_mesh->setShader(g_resourceManager.getShader(mesh_node->first_node("shader")->value()));
		new_mesh->setTexture(g_resourceManager.getTexture(mesh_node->first_node("texture")->value()));
		//Find speeds of enemy
		new_mesh->setSpeeds((float)atof(mesh_node->first_node("pitchspeed")->value()), (float)atof(mesh_node->first_node("rollspeed")->value()));
		//Find transformations in y
		if (strcmp(mesh_node->first_node("transform")->name(), "transform") == 0) {
			for (xml_node<> *child = mesh_node->first_node("transform")->first_node(); child; child = child->next_sibling()) {
				if (strcmp(child->name(), "scale") == 0) {
					new_mesh->m.scale((float)atof(child->first_node("x")->value()), (float)atof(child->first_node("y")->value()), (float)atof(child->first_node("z")->value()));
				}
				if (strcmp(child->name(), "translate") == 0) {
					new_mesh->m.traslate((float)atof(child->first_node("x")->value()), (float)atof(child->first_node("y")->value()), (float)atof(child->first_node("z")->value()));
				}
				if (strcmp(child->name(), "rotate") == 0) {
					float angle = (float)atof(child->first_node("angle")->value());
					int x = (int)atof(child->first_node("axis")->first_node("x")->value());
					int y = (int)atof(child->first_node("axis")->first_node("y")->value());
					int z = (int)atof(child->first_node("axis")->first_node("z")->value());
					new_mesh->m.rotate(angle, Vector3(x, y, z));
				}
			}
		}

		new_mesh->obb = new_mesh->mesh->findOBB();
		new_mesh->mesh->createOBBGeometry();
		new_mesh->shouldDrawOBB = true;
		new_mesh->isCollider = true;
		new_mesh->mesh->setCollisionModel();

		parent->addChild(new_mesh);
		for (xml_node<> *child = mesh_node->first_node("minion"); child; child = child->next_sibling()) {
			loadMinion(child, new_mesh);
		}

	}

	//Recursive function to load an object into the scene
	void loadObject(xml_node<> *mesh_node, GameObject* parent) {
		GameObjectMesh* new_mesh = new GameObjectMesh();

		new_mesh->setName(mesh_node->first_node("name")->value());
		new_mesh->setMesh(g_resourceManager.getMesh(mesh_node->first_node("mesh")->value()));

		new_mesh->setShader(g_resourceManager.getShader(mesh_node->first_node("shader")->value()));
		new_mesh->setTexture(g_resourceManager.getTexture(mesh_node->first_node("texture")->value()));
		if (strcmp(mesh_node->first_node("transform")->name(), "transform") == 0) {
			for (xml_node<> *child = mesh_node->first_node("transform")->first_node(); child; child = child->next_sibling()) {
				if (strcmp(child->name(), "scale") == 0) {
					new_mesh->m.scale((float)atof(child->first_node("x")->value()), (float)atof(child->first_node("y")->value()), (float)atof(child->first_node("z")->value()));
				}
				if (strcmp(child->name(), "translate") == 0) {
					new_mesh->m.traslate((float)atof(child->first_node("x")->value()), (float)atof(child->first_node("y")->value()), (float)atof(child->first_node("z")->value()));
				}
				if (strcmp(child->name(), "rotate") == 0) {
					float angle = (float)atof(child->first_node("angle")->value());
					int x = (int)atof(child->first_node("axis")->first_node("x")->value());
					int y = (int)atof(child->first_node("axis")->first_node("y")->value());
					int z = (int)atof(child->first_node("axis")->first_node("z")->value());
					new_mesh->m.rotate(angle, Vector3(x, y, z));
				}
			}
		}

		new_mesh->obb = new_mesh->mesh->findOBB();
		new_mesh->mesh->createOBBGeometry();
		new_mesh->shouldDrawOBB = false;
		new_mesh->isCollider = true;
		new_mesh->mesh->setCollisionModel();

//		new_mesh->mesh->setCollisionModel();
		parent->addChild(new_mesh);
		for (xml_node<> *child = mesh_node->first_node("node"); child; child = child->next_sibling()) {
			loadObject(child, new_mesh);
		}
	}

	//Load a level from a xml file
	void loadLevel(char* level_url) {
		rapidxml::file<> xmlFile(level_url);
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());

		xml_node<> *scene = doc.first_node();

		//***********Load resources***********//
		xml_node<> *resources = scene->first_node("resources");
		//Load resource meshes
		xml_node<> *resource_meshes = resources->first_node("meshes");
		for(xml_node<> *child = resource_meshes->first_node("mesh"); child; child = child->next_sibling()){
			try {
				g_resourceManager.loadMesh(child->first_node("name")->value(), child->first_node("url")->value());
			}
			catch (exception) {
				std::cout << "Mesh not found or error" << endl;
				exit(0);
			}
		}
		//Load resource textures
		xml_node<> *resource_textures = resources->first_node("textures");
		for(xml_node<> *child = resource_textures->first_node("texture"); child; child = child->next_sibling()){
			try {
				g_resourceManager.loadTexture(child->first_node("name")->value(), child->first_node("url")->value());
			}
			catch(exception){
				std::cout << "Texture not found or error" << endl;
				exit(0);
			}
		}
		xml_node<> *resource_audios = resources->first_node("audios");
		for (xml_node<> *child = resource_audios->first_node("audio"); child; child = child->next_sibling()) {
			try {
				if (strcmp(child->first_node("loop")->value(), "true") == 0) {
					g_resourceManager.loadSample(child->first_node("name")->value(), child->first_node("url")->value(), true);
				}
				else {
					g_resourceManager.loadSample(child->first_node("name")->value(), child->first_node("url")->value(), false);
				}
			}
			catch (exception) {
				std::cout << "Texture not found or error" << endl;
				exit(0);
			}
		}
		
		
		xml_node<> *root = scene->first_node("root");

		//***********Load environment***********//
		xml_node<> *env = root->first_node("environment");
		g_scene.environment_go = new GameObjectEnvironment();
		g_scene.environment_go->setName(env->first_node("name")->value());
		g_scene.environment_go->setMesh(g_resourceManager.getMesh(env->first_node("mesh")->value()));
		g_scene.environment_go->setShader(g_resourceManager.getShader(env->first_node("shader")->value()));
		g_scene.environment_go->setTexture(g_resourceManager.getTexture(env->first_node("texture")->value()));


		if (strcmp(env->first_node("transform")->name(), "transform") == 0) {
			for (xml_node<> *child = env->first_node("transform")->first_node(); child; child = child->next_sibling()) {
				if (strcmp(child->name(), "scale") == 0) {
					g_scene.environment_go->m.scale((float)atof(child->first_node("x")->value()), (float)atof(child->first_node("y")->value()), (float)atof(child->first_node("z")->value()));
				}
				if (strcmp(child->name(), "translate") == 0) {
					g_scene.environment_go->m.traslate((float)atof(child->first_node("x")->value()), (float)atof(child->first_node("y")->value()), (float)atof(child->first_node("z")->value()));
				}
				if (strcmp(child->name(), "rotate") == 0) {
					float angle = (float)atof(child->first_node("angle")->value());
					int x = (int)atof(child->first_node("axis")->first_node("x")->value());
					int y = (int)atof(child->first_node("axis")->first_node("y")->value());
					int z = (int)atof(child->first_node("axis")->first_node("z")->value());
					g_scene.environment_go->m.rotate(angle, Vector3(x, y, z));
				}
			}
		}

		//***********Load lights***********//
		xml_node<> *lights = root->first_node("lights");
		for (xml_node<> *child = lights->first_node("light"); child; child = child->next_sibling()) {
			GameObjectLight* new_light = new GameObjectLight(Vector3((float)atof(child->first_node("position")->first_node("x")->value()), (float)atof(child->first_node("position")->first_node("y")->value()), (float)atof(child->first_node("position")->first_node("z")->value())));
			g_scene.root->addChild((GameObject*)new_light);
			g_scene.lights.push_back(new_light->getLight());
		}
		//***********Load players***********//
		xml_node<> *players = root->first_node("players");
		for (xml_node<> *child = players->first_node("player"); child; child = child->next_sibling()) {
			loadPlayer(child, g_scene.root);
		}
		//***********Load meshes***********//
		xml_node<> *nodes = root->first_node("nodes");
		for (xml_node<> *child = nodes->first_node("node"); child; child = child->next_sibling()) {
			loadObject(child, g_scene.root);
		}
		//***********Load enemies***********//
		xml_node<> *enemies = root->first_node("enemies");
		for (xml_node<> *child = enemies->first_node("enemy"); child; child = child->next_sibling()) {
			loadEnemy(child, g_scene.root);
		}
		//***********Load minions***********//
		xml_node<> *minions = root->first_node("minions");
		for (xml_node<> *child = minions->first_node("minion"); child; child = child->next_sibling()) {
			loadMinion(child, g_scene.root);
		}
	}

	void loadBasics() {
		rapidxml::file<> xmlFile("data/levels/basics.xml");
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());

		xml_node<> *scene = doc.first_node();

		//***********Load resources***********//
		xml_node<> *resources = scene->first_node("resources");
		//Load resource meshes
		xml_node<> *resource_meshes = resources->first_node("meshes");
		for (xml_node<> *child = resource_meshes->first_node("mesh"); child; child = child->next_sibling()) {
			try {
				g_resourceManager.loadMesh(child->first_node("name")->value(), child->first_node("url")->value());
			}
			catch (exception) {
				std::cout << "Mesh not found or error" << endl;
				exit(0);
			}
		}
		//Load resource textures
		xml_node<> *resource_textures = resources->first_node("textures");
		for (xml_node<> *child = resource_textures->first_node("texture"); child; child = child->next_sibling()) {
			try {
				g_resourceManager.loadTexture(child->first_node("name")->value(), child->first_node("url")->value());
			}
			catch (exception) {
				std::cout << "Texture not found or error" << endl;
				exit(0);
			}
		}
		//Load resource shaders
		xml_node<> *resource_shaders = resources->first_node("shaders");
		for (xml_node<> *child = resource_shaders->first_node("shader"); child; child = child->next_sibling()) {
			if (!g_resourceManager.loadShader(child->first_node("name")->value(), child->first_node("url")->value(), child->first_node("url")->next_sibling()->value())) {
				std::cout << "Shader not found or error" << endl;
				exit(0);
			}
		}
		//Load resource audios
		xml_node<> *resource_audios = resources->first_node("audios");
		for (xml_node<> *child = resource_audios->first_node("audio"); child; child = child->next_sibling()) {
			try {
				if (strcmp(child->first_node("loop")->value(), "true") == 0) {
					g_resourceManager.loadSample(child->first_node("name")->value(), child->first_node("url")->value(), true);
				}
				else {
					g_resourceManager.loadSample(child->first_node("name")->value(), child->first_node("url")->value(), false);
				}
			}
			catch (exception) {
				std::cout << "Texture not found or error" << endl;
				exit(0);
			}
		}

	}
};