#ifndef _BASE_ENTITIES_
#define _BASE_ENTITIES_

#include "p2Defs.h"
#include "p2Point.h"
#include "Iso_Primitives.h"

enum ENTITY_TYPE
{
	NO_TY = 0,
	NEUTRAL,
	ENEMY,
	ALLY
};

class MyEntity
{
public:

	MyEntity();
	~MyEntity();

private:

	Rectngle	sprite;
	ENTITY_TYPE	type = NO_TY;
	fPoint		position = { 0,0 };
	Circle		vision_area;
	Circle		render_area;

public:

	//Game Loop Methods ---------------
	void	Draw();
	void	DebugDraw();
	void	CheckVisionArea();

	//Set Methods ---------------------
	void	SetSpriteSize(uint x, uint y);
	void	SetEntityColor(const SDL_Color& color);
	void	SetType(ENTITY_TYPE new_type);
	void	SetPosition(float x, float y);
	void	SetVisionRange(uint range);

	//Get Methods ---------------------
	fPoint	GetPosition()const;

};

#endif // _ENTITY_