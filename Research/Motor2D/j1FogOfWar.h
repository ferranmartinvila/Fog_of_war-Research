#ifndef _FOG_OF_WAR_
#define _FOG_OF_WAR_

#include "j1Module.h"
#include "Iso_Primitives.h"
#include "Quadtree.h"

#define ALPHA_LIMIT 255
#define	DIVISIONS_PER_TILE 3

enum FOG_TYPE
{
	NO_FOG = 0,
	GRAY_FOG,
	DARK_FOG
};

class j1FogOfWar : public j1Module
{
public:

	j1FogOfWar();
	~j1FogOfWar();


	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PostUpdate();

private:

	/*Fog Layer*/					std::vector<FOG_TYPE> fog_layer;
	/*Fog Surface*/					SDL_Surface* fog_surface = nullptr;
	/*Stamp to erase fog surface*/	SDL_Surface* fog_eraser = nullptr;
	SDL_Texture* eraser = nullptr;

	AlphaCell* alpha_layer = nullptr;
	uint alpha_layer_width = 0;
	uint alpha_layer_height = 0;
	QuadTree<AlphaCell*> fog_quadtree;

public:

	FOG_TYPE	GetFogID(int x, int y)const;
	void		ClearFogZone(Circle zone);
	void		ClearFogTile(int x, int y);

};

#endif