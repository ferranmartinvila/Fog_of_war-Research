#include "j1Scene.h"

#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1EntitiesManager.h"

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


	// Entities Build -------------------------------------
	player = App->entities_manager->GenerateUnit(); 
	player->SetPosition(350, 280);
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
	//MAP MOVEMENT-----------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		App->render->camera.y += SDL_ceil(500 * dt);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		App->render->camera.y -= SDL_ceil(500 * dt);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->render->camera.x += SDL_ceil(500 * dt);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->render->camera.x -= SDL_ceil(500 * dt);
	}



	//Test unit animations --------------
	fPoint pos = player->GetPosition();
	float speed = 95;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{

	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{

	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{

	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{

	}


	///Movement
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
	{
		player->SetPosition(pos.x, pos.y + -speed * dt);
	}
	else if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	{
		player->SetPosition(pos.x, pos.y + speed * dt);
	}
	else if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
	{
		player->SetPosition(pos.x + speed * dt, pos.y );
	}
	else if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
	{
		player->SetPosition(pos.x + -speed * dt, pos.y);
	}

	// ------------------------------------------

	App->map->Draw(App->debug_mode);
	App->entities_manager->Draw();

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
