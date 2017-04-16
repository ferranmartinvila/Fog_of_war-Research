#include "j1Scene.h"

#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1EntitiesManager.h"
#include "j1FogOfWar.h"

j1Scene::j1Scene() : j1Module()
{
	name = "scene";
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	for (pugi::xml_node map_tmx = config.child("map_folder"); map_tmx; map_tmx = map_tmx.next_sibling("map_folder")) {

		map_folder = (map_tmx.child_value());

	}

	bool ret = true;
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	//Map build -------------------------------------------
	Load_Map();
	// ----------------------------------------------------
	
	//Fog of War build ------------------------------------
	App->fog_of_war->GeneratFogOfWar();
	// ----------------------------------------------------

	//Generate Scene Ally , Neutral & Enemy entities ------
	//Neutral entity definition -------
	MyEntity* entity = App->entities_manager->GenerateUnit();
	iPoint location = App->map->MapToWorld(20, 20);
	entity->SetEntityColor({ 0,0,255,255 });
	entity->SetSpriteSize(15,30);
	entity->SetType(ENTITY_TYPE::NEUTRAL);
	entity->SetVisionRange(300);
	entity->SetPosition(location.x, location.y);
	//Ally entity definition ----------
	entity = App->entities_manager->GenerateUnit();
	location = App->map->MapToWorld(25, 5);
	entity->SetEntityColor({ 0,255,0,255 });
	entity->SetSpriteSize(15, 30);
	entity->SetType(ENTITY_TYPE::ALLY);
	entity->SetVisionRange(150);
	entity->SetPosition(location.x, location.y);
	//Enemy entity definition ---------
	entity = App->entities_manager->GenerateUnit();
	location = App->map->MapToWorld(5, 15);
	entity->SetEntityColor({ 255,0,0,255 });
	entity->SetSpriteSize(15, 30);
	entity->SetType(ENTITY_TYPE::ENEMY);
	entity->SetVisionRange(100);
	entity->SetPosition(location.x, location.y);
	// ----------------------------------------------------

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	App->map->Draw(App->debug_mode);

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;
	
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool j1Scene::Load_Map()
{
	return App->map->Load(map_folder.c_str());
	return false;
}
