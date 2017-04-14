#include "BaseEntities.h"

#include "j1App.h"
#include "j1Render.h"
#include "j1Map.h"
#include "p2Log.h"
#include "j1EntitiesManager.h"

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
	App->render->CallBlit(sprite.GetRect(), iPoint(-sprite.GetRect().w * 0.5, sprite.GetRect().h * 0.5), sprite.GetColor(), position.y);
}

//Set Methods =========================
void MyEntity::SetSpriteRect(const SDL_Rect & rect)
{
	sprite.SetRect(rect);
}

void MyEntity::SetSpriteColor(const SDL_Color & color)
{
	sprite.SetColor(color);
}

void MyEntity::SetType(ENTITY_TY new_type)
{
	type = new_type;
}

void MyEntity::SetPosition(float x, float y)
{
	//Extract the units to push it with the new position later
	App->entities_manager->entities_quadtree.Exteract(this, &position);

	//Set unit position
	position.x = x;
	position.y = y;

	//Set Spite position /*This have to change bro*/
	sprite.SetPosition(x, y);

	//Set unit vision position
	vision_area.SetPosition(iPoint(position.x, position.y));

	//Add the unit with the correct position in the correct quad tree
	App->entities_manager->entities_quadtree.Insert(this, &position);
}

void MyEntity::SetVisionRange(uint range)
{
	vision_area.SetRad(range);
}

fPoint MyEntity::GetPosition() const
{
	return position;
}
///----------------------------------------------