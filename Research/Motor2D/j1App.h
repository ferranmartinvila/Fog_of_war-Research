#ifndef __j1APP_H__
#define __j1APP_H__

#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"
#include <list>

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1FileSystem;
class j1Scene;
class j1Menu;
class j1Map;
class j1Module;
class j1EntitiesManager;
class j1Player;
class j1FogOfWar;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;
	float GetDT() const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();


public:

	// Modules
	j1Window*			win = NULL;
	j1Input*			input = NULL;
	j1Render*			render = NULL;
	j1Textures*			tex = NULL;
	j1Scene*			scene = NULL;
	j1Menu*				menu = NULL;
	j1FileSystem*		fs = NULL;
	j1Map*				map = NULL;
	j1EntitiesManager*	entities_manager = NULL;
	j1Player*			player = NULL;
	j1FogOfWar*			fog_of_war = NULL;

	// Add a new module to handle
	void		AddModule(j1Module* module);
	//Get Module pointer by name
	j1Module*	GetModule(const  std::string* module_name);
	//Get number of modules
	uint		GetModulesNum()const;
	//Get module of position x in array
	j1Module*	GetModuleAt(uint index)const;

public:

	//Get config xml file
	pugi::xml_node GetConfigXML()const;

private:

	pugi::xml_node config_node;

	std::list<j1Module*>	modules;
	int						argc = 0;
	char**					args = nullptr;

	std::string				title;
	std::string				organization;

	j1PerfTimer			ptimer;
	uint64				frame_count = 0;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	float				dt = 0.0f;
	int					capped_ms = -1;

	bool				want_to_quit = false;

public:

	void SetQuit();

	bool	debug_mode = false;

};

extern j1App* App;

#endif