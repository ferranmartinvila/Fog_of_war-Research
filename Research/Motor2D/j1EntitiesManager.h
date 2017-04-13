#ifndef _ENTITIES_MANAGER_
#define _ENTITIES_MANAGER_

#include "j1Module.h"
#include "BaseEntities.h"
#include "Quadtree.h"

class j1EntitiesManager : public j1Module
{
public:

	j1EntitiesManager();
	~j1EntitiesManager();

public:

	// Called before render is available
	bool Awake(pugi::xml_node& config_node);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	//Draw entities
	bool Draw()const;

	// Called before quitting
	bool CleanUp();

	//Handle Console Input ----------------------
	void Console_Command_Input(Command* command, Cvar* cvar, std::string* input);
	void Console_Cvar_Input(Cvar* cvar, Command* command_type, std::string* input);

public:

	//Lists of current game entities
	std::list<Unit*>		units;
	std::list<Resource*>	resources;
	std::list<Building*>	buildings;

	std::list<MyEntity*>	entities_list;
	m_QuadTree<MyEntity*>	entities_quadtree;

	//Vector where all the killed/destroyed entities are placed
	std::vector<Entity*>	wasted_units;

	//Vector of predefined units
	std::vector<Unit*>		units_defs;
	std::vector<Resource*>	resources_defs;
	std::vector<Building*>	buildings_defs;

	// Cvar that defines the console unit generator unit type
	Cvar* unit_cvar;

public:

	//Functionality -------------------
	//Factory Methods -------
	MyEntity*	GenerateUnit();

	//Delete Methods --------
	bool		DeleteEntity(Entity* entity);

};
#endif // _ENTITIES_MANAGER_
