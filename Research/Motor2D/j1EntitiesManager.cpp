#define _CRT_SECURE_NO_WARNINGS

#include "j1EntitiesManager.h"

#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1FileSystem.h"
#include "j1Map.h"
#include "p2Log.h"

//Testing purposes only should be erased
#include "j1Scene.h"

//Constructors ========================
j1EntitiesManager::j1EntitiesManager()
{
	name = "entities_manager";
}

//Destructors =========================
j1EntitiesManager::~j1EntitiesManager()
{

}

//Game Loop ===========================
bool j1EntitiesManager::Awake(pugi::xml_node & config_node)
{
	return true;
}

bool j1EntitiesManager::Start()
{
	entities_quadtree.SetMaxObjects(8);
	entities_quadtree.SetBoundaries(App->render->viewport);

	return true;
}

bool j1EntitiesManager::Update(float dt)
{

	return true;
}

bool j1EntitiesManager::PostUpdate()
{
	//Clean all the wasted entities
	uint size = wasted_entities.size();
	for (uint k = 0; k < size; k++)
	{
		RELEASE(wasted_entities[k]);
	}
	wasted_entities.clear();
	
	return true;
}

bool j1EntitiesManager::Draw() const
{
	return true;
}

bool j1EntitiesManager::CleanUp()
{
	//Clean Up Units List
	std::list<MyEntity*>::iterator entity_item = entities_list.begin();
	while (entity_item != entities_list.end())
	{
		RELEASE(entity_item._Ptr->_Myval);
		entity_item++;
	}
	entities_list.clear();

	return true;
}

MyEntity * j1EntitiesManager::GenerateUnit()
{
	MyEntity* new_entity = new MyEntity();
	
	entities_list.push_back(new_entity);

	return new_entity;
}


bool j1EntitiesManager::DeleteEntity(MyEntity * entity)
{

	entities_list.remove(entity);
	entities_quadtree.Exteract(entity, &entity->GetPosition());

	wasted_entities.push_back(entity);

	return true;
}