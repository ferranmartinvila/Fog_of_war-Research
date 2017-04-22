#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class MyEntity;

class j1Scene : public j1Module
{
public:

	j1Scene();
	~j1Scene();

public:

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:



public:

	//Map Folders
	std::string map_folder;

	//Load the selected Map
	bool Load_Map();

};

#endif // __j1SCENE_H__