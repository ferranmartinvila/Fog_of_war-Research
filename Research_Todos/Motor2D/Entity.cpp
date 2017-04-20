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
	//Get fog type from the entity tile coordinates
	iPoint map_loc = App->map->WorldToMap(position.x - App->map->data.tile_width * 0.5, position.y - App->map->data.tile_height * 0.5);
	FOG_TYPE fog_type = App->fog_of_war->GetFogID(map_loc.x, map_loc.y); /* Use this in TODO 3 to know the fog type*/

	//	TODO 3:	Entities draw optimization!
	//			Draw the entity only when is necessary.
	//			ALLY entities are always drawn.
	//			NEUTRAL entities are not drawn when FOG_TYPE is DARK_FOG.
	//			ENEMY entities are only drawn when there’s NO_FOG.

	//if (type == ENEMY && fog_type != NO_FOG)return; /* Enemy entities are only drawn when there's no fog */
	//else if (type == NEUTRAL && fog_type == DARK_FOG)return; /* Neutral entities aren't drawn under dark fog */
	
	// TODO 5:	Make ally entities clear alpha layer!
	//			Check if this is an ally entity.
	//			If is an ally, clear alpha layer around vision area with 0 alpha value.
	//			To clear alpha layer use ClearAlphaLayer method from fog of war module.
	//			More Info: LINK_AKA

	/*
	else if (type == ALLY)
	{
		App->fog_of_war->ClearAlphaLayer(vision_area, 0);
	}
	*/

	//Draw entity sprite
	App->render->CallBlit(sprite.GetRect(), iPoint(-sprite.GetRect().w * 0.5, -sprite.GetRect().h ), sprite.GetColor(), -position.y);
}

void MyEntity::DebugDraw()
{
	vision_area.Draw();
	render_area.Draw();
}

void MyEntity::CheckVisionArea()
{
	// TODO 6:	Make player check his vision area!
	//			Use entities_quadtree from entities_manager to collect the candidates inside the vision area.
	//			CollectCandidates quadtree method fill the vector you pass with the entities inside the specified area.
	//			More Info: LINK_AKA

	/*
	std::vector<MyEntity*> entities_vec;
	uint size = App->entities_manager->entities_quadtree.CollectCandidates(entities_vec, vision_area);
	*/

	// TODO 7:	Clear fog of war area when a neutral entity is detected!
	//			Iterate the entities collected in the last TODO and check if there's a neutral entity.
	//			If there’s one, clear alpha layer around its vision area and fog layer around its render area.
	//			More Info: LINK_AKA

	/*if (entities_vec[k]->type == NEUTRAL)
	{
		App->fog_of_war->ClearAlphaLayer(entities_vec[k]->vision_area, MID_ALPHA);
		App->fog_of_war->ClearFogLayer(entities_vec[k]->render_area, GRAY_FOG); 
	}*/
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