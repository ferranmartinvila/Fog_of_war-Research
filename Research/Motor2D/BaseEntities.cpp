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

}

//Set Methods =========================
void MyEntity::SetPosition(float x, float y)
{
	//Extract the units to push it with the new position later
	App->entities_manager->entities_quadtree.Exteract(this, &position);

	//Set unit position
	position.x = x;
	position.y = y;

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