#define _CRT_SECURE_NO_WARNINGS

#include "j1EntitiesManager.h"

#include "j1App.h"
#include "j1Render.h"
#include "j1Animator.h"
#include "j1Input.h"
#include "j1FileSystem.h"
#include "j1Console.h"
#include "j1Pathfinding.h"
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
	//Update all the units
	std::list<Unit*>::const_iterator item = units.begin();
	bool ret = true;

	while (item != units.end())
	{

		ret = item._Ptr->_Myval->Update();

		item++;
	}

	//Clean all the wasted entities
	if (wasted_units.size() > 0) wasted_units.clear();

	return ret;
}

bool j1EntitiesManager::Draw() const
{
	bool ret = true;

	//Draw all units
	std::list<Unit*>::const_iterator unit_item = units.begin();
	while (unit_item != units.end() && ret)
	{
		ret = unit_item._Ptr->_Myval->Draw(App->debug_mode);
		unit_item++;
	}
	//Draw all Resources
	std::list<Resource*>::const_iterator resource_item = resources.begin();
	while (resource_item != resources.end() && ret)
	{
		ret = resource_item._Ptr->_Myval->Draw(App->debug_mode);
		resource_item++;
	}
	//Draw all buildings
	std::list<Building*>::const_iterator building_item = buildings.begin();
	while (building_item != buildings.end() && ret)
	{
		ret = building_item._Ptr->_Myval->Draw(App->debug_mode);
		building_item++;
	}

	return ret;
}

bool j1EntitiesManager::CleanUp()
{
	//Clean Up Units List
	std::list<Unit*>::iterator units_item = units.begin();
	while (units_item != units.end())
	{
		RELEASE(units_item._Ptr->_Myval);
		units_item++;
	}
	units.clear();

	//Clean Up resources list
	std::list<Resource*>::iterator resources_item = resources.begin();
	while (resources_item != resources.end())
	{
		RELEASE(resources_item._Ptr->_Myval);
		resources_item++;
	}
	resources.clear();

	//Clean Up buildings list
	std::list<Building*>::iterator buildings_item = buildings.begin();
	while (buildings_item != buildings.end())
	{
		RELEASE(buildings_item._Ptr->_Myval);
		buildings_item++;
	}
	buildings.clear();

	//Clean Up units_defs vector
	units_defs.clear();

	//Clean Up resoureces_defs vector
	resources_defs.clear();

	//Clean Up buildings_defs vector
	buildings_defs.clear();

	return true;
}

void j1EntitiesManager::Console_Command_Input(Command * command, Cvar * cvar, std::string * input)
{

}

void j1EntitiesManager::Console_Cvar_Input(Cvar * cvar, Command * command_type, std::string * input)
{
	if (*command_type->GetCommandStr() == "get")
	{
		App->console->GenerateConsoleLabel("%s is [%s]", unit_cvar->GetCvarName()->c_str(), unit_cvar->GetValueString()->c_str());
	}
	else if (*command_type->GetCommandStr() == "set")
	{
		unit_cvar->SetValue(input->c_str());
		LOG("Now %s is [%s]", unit_cvar->GetCvarName()->c_str(), unit_cvar->GetValueString()->c_str());
	}
}

MyEntity * j1EntitiesManager::GenerateUnit()
{
	MyEntity* new_entity = new MyEntity();
	
	entities_list.push_back(new_entity);

	return new_entity;
}


bool j1EntitiesManager::DeleteEntity(Entity * entity)
{
	//Check if the entity is really defined
	if (entity == nullptr || entity->GetEntityType() == NO_ENTITY)
	{
		return false;
	}
	
	//Get the entity type
	ENTITY_TYPE type = entity->GetEntityType();
	
	//Remove the entity from the correct list
	if (type == RESOURCE) resources.remove((Resource*)entity);
	else if (type == BUILDING) buildings.remove((Building*)entity);
	else units.remove((Unit*)entity);

	//Add the entity at the wasted entities list 
	wasted_units.push_back(entity);

	return true;
}