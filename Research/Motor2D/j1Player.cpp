#include "j1Player.h"

#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
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

bool j1Player::Update(float dt)
{
	//Move player entity ------------------------
	fPoint pos = player->GetPosition();
	float speed = 300;
	bool moved = false;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		player->SetPosition(pos.x, pos.y + -speed * dt);
		pos = player->GetPosition();
		moved = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		player->SetPosition(pos.x, pos.y + speed * dt);
		pos = player->GetPosition();
		moved = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		player->SetPosition(pos.x + -speed * dt, pos.y);
		pos = player->GetPosition();
		moved = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		player->SetPosition(pos.x + speed * dt, pos.y);
		moved = true;
	}

	if (moved)
	{
		App->render->camera.x = -pos.x + App->render->camera.w * 0.5;
		App->render->camera.y = -pos.y + App->render->camera.h * 0.5;
		App->render->CalculateCameraViewport();
	}
	// ------------------------------------------


	// Check player vision area -----------------
	player->CheckVisionArea();
	// ------------------------------------------

	return true;
}

void j1Player::GeneratePlayerEntity()
{
	//Player entity build -----------------------
	player = App->entities_manager->GenerateUnit();
	player->SetVisionRange(300);
	player->SetEntityColor({ 255,255,25,255 });
	player->SetSpriteSize(20, 50);
	player->SetType(ALLY);
	player->SetPosition(350, 280);
	// ------------------------------------------

	//Center camera at player entity ------------
	fPoint player_pos = player->GetPosition();
	App->render->camera.x = -player_pos.x + App->render->camera.w * 0.5;
	App->render->camera.y = -player_pos.y + App->render->camera.h * 0.5;
	App->render->CalculateCameraViewport();
	// ------------------------------------------
}
