#define _CRT_SECURE_NO_WARNINGS

#include <iostream>		
#include <sstream> 
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1FileSystem.h"
#include "j1Map.h"
#include "j1EntitiesManager.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1FogOfWar.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{

	PERF_START(ptimer);

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	scene = new j1Scene();
	fs = new j1FileSystem();
	map = new j1Map();
	entities_manager = new j1EntitiesManager();
	player = new j1Player();
	fog_of_war = new j1FogOfWar();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(fs);
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(map);

	AddModule(entities_manager);


	// scene last
	AddModule(scene);

	AddModule(player);
	AddModule(fog_of_war);

	// render last to swap buffer
	AddModule(render);

	

	PERF_PEEK(ptimer);

}

// Destructor
j1App::~j1App()
{
	// release modules
	for(std::list<j1Module*>::iterator item = modules.end(); item != modules.end(); item--)
	{
		RELEASE(item._Ptr->_Myval);
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool j1App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;
		
	config = LoadConfig(config_file);

	//Load App config data
	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title = app_config.child("title").child_value();
		organization = app_config.child("organization").child_value();

		int cap = app_config.attribute("framerate_cap").as_int(-1);

		if(cap > 0)
		{
			capped_ms = 1000 / cap;
		}
	}

	//Call modules Awake method
	if(ret == true)
	{
		for(std::list<j1Module*>::iterator item = modules.begin(); item != modules.end() && ret == true; item++)
		{
			ret = item._Ptr->_Myval->Awake(config.child(item._Ptr->_Myval->name.c_str()));
			modules.end();
		}
	}


	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	PERF_START(ptimer);
	bool ret = true;
	std::list<j1Module*>::iterator item = modules.begin();

	while(item != modules.end() != NULL && ret == true)
	{
		ret = item._Ptr->_Myval->Start();
		item++;
	}
	startup_time.Start();

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	//Activate/Deactivate debug mode
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debug_mode = !debug_mode;
	}

	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
	{
		ret = PreUpdate();
	}

	if (ret == true)
	{
		ret = DoUpdate();
	}

	if (ret == true)
	{
		ret = PostUpdate();
	}

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	char* buf;
	int size = App->fs->Load("config.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if(result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	dt = frame_time.ReadSec();
	frame_time.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	// Framerate calculations --

	if(last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %lu ",
			  avg_fps, (unsigned long)last_frame_ms, frames_on_last_update, dt, seconds_since_startup, frame_count);
	App->win->SetTitle(title);

	if(capped_ms > 0 && last_frame_ms < capped_ms)
	{
		j1PerfTimer t;
		SDL_Delay(capped_ms - last_frame_ms);
	}
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	std::list<j1Module*>::const_iterator item = modules.begin();

	while(item != modules.end())
	{
		if(item._Ptr->_Myval->active) ret = item._Ptr->_Myval->PreUpdate();

		item++;
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	std::list<j1Module*>::const_iterator item = modules.begin();

	while(item != modules.end())
	{
		if(item._Ptr->_Myval->active)ret = item._Ptr->_Myval->Update(dt);

		item++;
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	std::list<j1Module*>::const_iterator item = modules.begin();

	while(item != modules.end() && ret)
	{
		if(item._Ptr->_Myval->active)ret = item._Ptr->_Myval->PostUpdate();

		item++;
	}

	if(ret)ret = !want_to_quit;

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	PERF_START(ptimer);
	bool ret = true;
	std::list<j1Module*>::const_iterator item = modules.end();
	item--;

	while(item != modules.begin() && ret == true)
	{
		ret = item._Ptr->_Myval->CleanUp();
		item--;
	}

	PERF_PEEK(ptimer);
	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.c_str();
}

// ---------------------------------------
float j1App::GetDT() const
{
	return dt;
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.c_str();
}

j1Module * j1App::GetModule(const std::string* module_name)
{
	std::list<j1Module*>::const_iterator item = modules.begin();
	
	while (item != modules.end())
	{
		if (*module_name == item._Ptr->_Myval->name.c_str())
		{
			return item._Ptr->_Myval;
		}

		item++;
	}
	return nullptr;
}

uint j1App::GetModulesNum() const
{
	return modules.size();
}

j1Module * j1App::GetModuleAt(uint index)const
{
	std::list<j1Module*>::const_iterator item = modules.begin();
	
	for (uint k = 0; k < index; k++)
			item++;

	return item._Ptr->_Myval;
}

pugi::xml_node j1App::GetConfigXML() const
{
	return config_node;
}

void j1App::SetQuit()
{
	want_to_quit = true;
}


