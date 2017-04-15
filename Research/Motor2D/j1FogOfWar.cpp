#include "j1FogOfWar.h"

#include "j1App.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Window.h"

//Constructors ========================
j1FogOfWar::j1FogOfWar()
{
	name = "fog_of_war";
}

//Destructors =========================
j1FogOfWar::~j1FogOfWar()
{

}

//Game Loop ===========================
bool j1FogOfWar::Start()
{
	//Calculate fog alpha layer size with the map data & the divisions per tile
	uint size = App->map->data.width * App->map->data.height * DIVISIONS_PER_TILE;
	alpha_layer_width = App->map->data.width * DIVISIONS_PER_TILE;
	alpha_layer_height = App->map->data.height * DIVISIONS_PER_TILE;
	int mid_map_lenght = (App->map->data.width * App->map->data.tile_width) * -0.5;

	//Build fog quadtree boundaries & limit
	fog_quadtree.SetBoundaries({ mid_map_lenght,0,(int)App->map->data.width * (int)App->map->data.tile_width,((int)App->map->data.height + MARGIN) * (int)App->map->data.tile_height });
	fog_quadtree.SetMaxObjects(20);
	fog_quadtree.SetDebugColor({ 255,255,0,255 });

	//Build fog alpha layer
	//Allocate alpha layer cells
	alpha_layer = new AlphaCell[alpha_layer_width * alpha_layer_height];

	//Set cells position
	uint divided_tile_width = floor(App->map->data.tile_width / DIVISIONS_PER_TILE);
	uint divided_tile_height = floor((App->map->data.tile_height + MARGIN) / DIVISIONS_PER_TILE);



	for (uint y = 0; y < alpha_layer_height; y++)
	{
		for (uint x = 0; x < alpha_layer_width; x++)
		{
			AlphaCell* current_cell = &alpha_layer[y * alpha_layer_width + x];

			current_cell->position = { mid_map_lenght + (int)divided_tile_width * (int)x, (int)divided_tile_height * (int)y };
			current_cell->alpha = rand() % 255;
			if(!fog_quadtree.Insert(current_cell, &current_cell->position))
			{
				LOG("Fail");
			}
		}
	}
	

	//Initialize alpha layer with the max alpha value


	//Allocate an array of booleans with the size of the map
	fog_layer.reserve(size);

	//Initialize the array filled of false (all covered of fog)
	fog_layer.assign(size, DARK_FOG);


	//Create a SDL_Surface for the fog
	fog_surface = SDL_CreateRGBSurface(0, 200, 200, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	//Load fog eraser surface
	fog_eraser = IMG_Load("data/maps/fog_eraser.png");

	//Fill the fog surface with black color
	SDL_Rect rect = { 0, 0, fog_surface->w, fog_surface->h };
	if (SDL_FillRect(fog_surface, &rect, 0xff000090) != 0)
	{
		LOG("Error filling fog surface");
	}



	return true;
}

bool j1FogOfWar::PostUpdate()
{
	//Paste fog zone at the screen surface to render it
	fog_quadtree.Draw();

	std::vector<AlphaCell*> in_view_cells;
	SDL_Rect viewport = { -App->render->camera.x - App->map->data.tile_width, -App->render->camera.y - App->map->data.tile_height, App->render->camera.w + App->map->data.tile_width * 2, App->render->camera.h + App->map->data.tile_height * 2 };

	uint divided_tile_width = floor(App->map->data.tile_width / DIVISIONS_PER_TILE);
	uint divided_tile_height = floor((App->map->data.tile_height + MARGIN) / DIVISIONS_PER_TILE);

	uint size = fog_quadtree.CollectCandidates(in_view_cells, viewport);
	for (uint k = 0; k < size; k++)
	{
		App->render->FogBlit(in_view_cells[k]->position, divided_tile_width, divided_tile_height, in_view_cells[k]->alpha);
	}

	if (SDL_BlitSurface(fog_surface, NULL, App->win->screen_surface, NULL) != 0)
	{
		LOG("Error in fog surface blit: %s", SDL_GetError());
	}


	/*
	//Update the current fog zone
	SDL_LockSurface(fog_surface);

	Uint32* destPixels = (Uint32*)fog_surface->pixels;

	for (uint x = 0; x < fog_surface->w; x++)
	{
	for (uint y = 0; y < fog_surface->h; y++)
	{

	Uint32* destPixel = destPixels + (y * fog_surface->w) + x;
	unsigned char* destAlpha = (unsigned char*)destPixel + 3;

	if (*destAlpha < 0x05)
	{
	*destAlpha = 0x02;
	}
	}
	}

	SDL_UnlockSurface(fog_surface);
	*/


	return true;
}

//Functionality =======================
FOG_TYPE j1FogOfWar::GetFogID(int x, int y) const
{
	return fog_layer[y * App->map->data.width + x];
}

void j1FogOfWar::ClearFogZone(Circle zone)
{

	//Collect all the tiles in the unit vision area
	iPoint pos = zone.GetPosition();
	zone.SetPosition(iPoint(pos.x - App->map->data.tile_width * 0.5f, pos.y - App->map->data.tile_height * 0.5f)); /*TEMP CASE*/

	std::vector<iPoint> tiles;
	uint size = App->map->map_quadtree.CollectCandidates(tiles, zone);

	//Clear fog in vision area
	for (uint k = 0; k < size; k++)
	{
		iPoint map_loc = App->map->WorldToMap(tiles[k].x, tiles[k].y);

		iPoint world_loc = App->map->MapToWorld(map_loc.x, map_loc.y);
		fog_layer[map_loc.y * App->map->data.width + map_loc.x] = NO_FOG;
		ClearFogTile(world_loc.x, world_loc.y);

	}

}

void j1FogOfWar::ClearFogTile(int in_x, int in_y)
{

	SDL_Rect sourceRect = { 0, 0, fog_eraser->w, fog_eraser->h };
	SDL_Rect destRect = { in_x, in_y, fog_eraser->w, fog_eraser->h };
	//destRect.x += (App->map->data.tile_width * App->map->data.width) * 0.5;
	destRect.x += App->render->camera.x;
	destRect.y += App->render->camera.y;

	SDL_Rect fog_rect = { 0,0,fog_surface->w,fog_surface->h };



	Uint32* destPixels = (Uint32*)fog_surface->pixels;
	Uint32* sourcePixels = (Uint32*)fog_eraser->pixels;

	for (uint x = 0; x < destRect.w; x++)
	{
		for (uint y = 0; y < destRect.h; y++)
		{
			SDL_Point pixel_loc = { destRect.x + x,destRect.y + y };
			if (!SDL_PointInRect(&pixel_loc, &fog_rect))continue;

			Uint32* destPixel = destPixels + ((y + destRect.y) * fog_surface->w) + (destRect.x + x);
			Uint32* srcPixel = sourcePixels + ((y + sourceRect.y) * fog_eraser->w) + (sourceRect.x + x);

			unsigned char* destAlpha = (unsigned char*)destPixel + 3; // fetch alpha channel
			unsigned char* sourceAlpha = (unsigned char*)srcPixel + 3; // fetch alpha channel

			if (*sourceAlpha > *destAlpha)
			{
				continue;
			}

			if (*sourceAlpha < 0x10)*destAlpha = 0x01;
			else *destAlpha = *sourceAlpha;

		}
	}



}
