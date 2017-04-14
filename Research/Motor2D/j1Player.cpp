#include "j1Player.h"

#include "p2Log.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"

#include "j1App.h"
#include "j1Window.h"
#include "j1EntitiesManager.h"




//j1Player Constructor ============================================================
j1Player::j1Player()
{
	name = "player";
}

//j1Player Destructor ============================================================
j1Player::~j1Player()
{
}

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player");
	bool ret = true;


	return ret;
}

bool j1Player::Start()
{

	return true;
}

bool j1Player::PreUpdate()
{

	return true;
}

bool j1Player::PostUpdate()
{

	return true;
}

bool j1Player::CleanUp()
{

	return true;
}