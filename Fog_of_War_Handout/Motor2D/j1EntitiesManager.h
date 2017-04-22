#ifndef _ENTITIES_MANAGER_
#define _ENTITIES_MANAGER_

#include "j1Module.h"
#include "Entity.h"
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

	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:

	std::list<MyEntity*>	entities_list;
	m_QuadTree<MyEntity*>	entities_quadtree;

	//Vector where all the killed/destroyed entities are placed
	std::vector<MyEntity*>	wasted_entities;

public:

	//Functionality -------------------
	//Factory Methods -------
	MyEntity*	GenerateUnit();

	//Delete Methods --------
	bool		DeleteEntity(MyEntity* entity);

};
#endif // _ENTITIES_MANAGER_
