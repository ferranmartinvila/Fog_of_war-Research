#include "Entity.h"

#include "j1App.h"
#include "j1Render.h"
#include "j1Map.h"
#include "p2Log.h"
#include "j1EntitiesManager.h"
#include "j1FogOfWar.h"

///Class MyEntity -------------------------------
//Constructors ========================
MyEntity::MyEntity()
{

}

//Destructors =========================
MyEntity::~MyEntity()
{

}

//Game Loop Methods ===================
void MyEntity::Draw()
{
	
	iPoint map_loc = App->map->WorldToMap(position.x - App->map->data.tile_width * 0.5, position.y - App->map->data.tile_height * 0.5);
	FOG_TYPE fog_type = App->fog_of_war->GetFogID(map_loc.x, map_loc.y);

	if (type == ENEMY && fog_type != NO_FOG)return;
	else if (type == NEUTRAL && fog_type == DARK_FOG)return;
	else if (type == ALLY)
	{
		App->fog_of_war->ClearAlphaLayer(vision_area, 0, App->fog_of_war->glow_mode);
	}

	App->render->CallBlit(sprite.GetRect(), iPoint(-sprite.GetRect().w * 0.5, -sprite.GetRect().h ), sprite.GetColor(), -position.y);
}

void MyEntity::DebugDraw()
{
	vision_area.Draw();
	render_area.Draw();
}

void MyEntity::CheckVisionArea()
{
	std::vector<MyEntity*> entities_vec;
	uint size = App->entities_manager->entities_quadtree.CollectCandidates(entities_vec, vision_area);

	for (uint k = 0; k < size; k++)
	{
		if (entities_vec[k]->position != this->position && entities_vec[k]->type == NEUTRAL)
		{
			App->fog_of_war->ClearAlphaLayer(entities_vec[k]->vision_area,MID_ALPHA);
			App->fog_of_war->ClearFogLayer(entities_vec[k]->render_area, GRAY_FOG);
		}
	}
}

//Set Methods =========================
void MyEntity::SetSpriteSize(uint x, uint y)
{
	sprite.SetRect({ 0,0,(int)x,(int)y });
}

void MyEntity::SetEntityColor(const SDL_Color & color)
{
	sprite.SetColor(color);
	vision_area.SetColor(color);
	render_area.SetColor(color);
}

void MyEntity::SetType(ENTITY_TYPE new_type)
{
	type = new_type;
}

void MyEntity::SetPosition(float x, float y)
{
	
	if (!App->map->IsInMap(x, y))return;

	//Extract the units to push it with the new position later
	App->entities_manager->entities_quadtree.Exteract(this, &position);

	//Set unit position
	position.x = x;
	position.y = y;

	//Set Spite position /*This have to change bro*/
	sprite.SetPosition(x, y);

	//Set unit vision position
	vision_area.SetPosition(iPoint(x, y));
	render_area.SetPosition(iPoint(x, y));

	if (type == ALLY)
	{
		App->fog_of_war->ClearFogLayer(render_area, GRAY_FOG);
		App->fog_of_war->ClearFogLayer(vision_area, NO_FOG);
	}

	//Add the unit with the correct position in the correct quad tree
	App->entities_manager->entities_quadtree.Insert(this, &position);
}

void MyEntity::SetVisionRange(uint range)
{
	vision_area.SetRad(range);
	render_area.SetRad(range + RENDER_MARGIN);
}

fPoint MyEntity::GetPosition() const
{
	return position;
}
///----------------------------------------------