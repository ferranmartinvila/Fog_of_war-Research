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
	//Player entity build -----------------------
	player = App->entities_manager->GenerateUnit();
	player->SetPosition(350, 280);
	player->SetSpriteColor({ 255,255,25,255 });
	player->SetSpriteRect({ 350, 280,50,150 });
	player->SetType(ALLY);
	// ------------------------------------------
	return true;
}

bool j1Player::Update(float dt)
{
	//Test unit animations --------------
	fPoint pos = player->GetPosition();
	float speed = 500;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		player->SetPosition(pos.x, ceil(pos.y + -speed * dt));
		pos = player->GetPosition();
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		player->SetPosition(pos.x, pos.y + speed * dt);
		pos = player->GetPosition();
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		player->SetPosition(ceil(pos.x + -speed * dt), pos.y);
		pos = player->GetPosition();
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		player->SetPosition(pos.x + speed * dt, pos.y);
	}

	return true;
}

bool j1Player::CleanUp()
{

	return true;
}