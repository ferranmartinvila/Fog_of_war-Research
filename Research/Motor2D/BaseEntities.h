#ifndef _BASE_ENTITIES_
#define _BASE_ENTITIES_

#include "p2Defs.h"
#include "p2Point.h"
#include "Iso_Primitives.h"

enum ENTITY_TY
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

	fPoint	position = { 0,0 };
	Circle	vision_area;

public:

	//Game Loop Methods ---------------
	void	Draw();

	//Set Methods ---------------------
	void	SetPosition(float x, float y);
	void	SetVisionRange(uint range);

	//Get Methods ---------------------
	fPoint	GetPosition()const;

};

#endif // _ENTITY_