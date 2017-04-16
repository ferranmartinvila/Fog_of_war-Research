#include "j1FogOfWar.h"

#include "j1App.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Input.h"

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
	return true;
}

bool j1FogOfWar::PostUpdate()
{
	//Paste fog zone at the screen surface to render it
	//fog_quadtree.Draw();

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		for (uint y = 0; y < alpha_layer_height; y++)
		{
			for (uint x = 0; x < alpha_layer_width; x++)
			{
				alpha_layer[y * alpha_layer_width + x].alpha = ALPHA_LIMIT;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		{

			for (uint y = 0; y < App->map->data.height; y++)
			{
				for (uint x = 0; x < App->map->data.width; x++)
				{
					fog_layer[y * App->map->data.width + x] = DARK_FOG;
				}
			}
		}
	}

	if (App->debug_mode)return true;

	std::vector<AlphaCell*> in_view_cells;
	uint divided_tile_width = floor(App->map->data.tile_width / DIVISIONS_PER_TILE);
	uint divided_tile_height = floor((App->map->data.tile_height + MARGIN) / DIVISIONS_PER_TILE);

	uint size = fog_quadtree.CollectCandidates(in_view_cells, App->render->camera_viewport);
	for (uint k = 0; k < size; k++)
	{
		App->render->FogBlit(in_view_cells[k]->position, divided_tile_width, divided_tile_height, in_view_cells[k]->alpha);
		
		if (in_view_cells[k]->alpha < MID_ALPHA)in_view_cells[k]->alpha = MID_ALPHA;
	}

	return true;
}

//Functionality =======================
void j1FogOfWar::GeneratFogOfWar()
{
	//Calculate fog alpha layer size with the map data & the divisions per tile
	alpha_layer_width = App->map->data.width * DIVISIONS_PER_TILE;
	alpha_layer_height = App->map->data.height * DIVISIONS_PER_TILE;
	int mid_map_lenght = (App->map->data.width * App->map->data.tile_width) * -0.5;

	//Build fog quadtree boundaries & limit
	fog_quadtree.SetBoundaries({ mid_map_lenght,0,(int)App->map->data.width * (int)App->map->data.tile_width,((int)App->map->data.height + MARGIN) * (int)App->map->data.tile_height });
	fog_quadtree.SetMaxObjects(DIVISIONS_PER_TILE * 20);
	fog_quadtree.SetDebugColor({ 255,255,0,255 });

	//Build fog alpha layer
	//Allocate alpha layer cells
	alpha_layer = new AlphaCell[alpha_layer_width * alpha_layer_height];

	//Set cells position
	float divided_tile_width = App->map->data.tile_width / DIVISIONS_PER_TILE;
	float divided_tile_height = (App->map->data.tile_height + MARGIN) / DIVISIONS_PER_TILE;

	for (uint y = 0; y < alpha_layer_height; y++)
	{
		for (uint x = 0; x < alpha_layer_width; x++)
		{
			AlphaCell* current_cell = &alpha_layer[y * alpha_layer_width + x];

			current_cell->position = { (int)ceil(mid_map_lenght + (int)divided_tile_width * (int)x), (int)ceil((int)divided_tile_height * (int)y) };
			current_cell->alpha = 255;
			if (!fog_quadtree.Insert(current_cell, &iPoint(current_cell->position.x + divided_tile_width * 0.5, current_cell->position.y + divided_tile_height * 0.5)))
			{
				LOG("Fail");
			}
		}
	}

	//Build fog type layer
	fog_layer = new FOG_TYPE[App->map->data.width * App->map->data.height];

	for (uint y = 0; y < App->map->data.height; y++)
	{
		for (uint x = 0; x < App->map->data.width; x++)
		{
			fog_layer[y * App->map->data.width + x] = DARK_FOG;
		}
	}
}

FOG_TYPE j1FogOfWar::GetFogID(int x, int y) const
{
	return fog_layer[y * App->map->data.width + x];
}

void j1FogOfWar::ClearAlphaLayer(const Circle zone, unsigned short alpha)
{
	std::vector<AlphaCell*> fog_cells;
	uint size = fog_quadtree.CollectCandidates(fog_cells, zone);

	//Clear fog in vision area
	for (uint k = 0; k < size; k++)
	{
		if(fog_cells[k]->alpha > alpha)fog_cells[k]->alpha = alpha;
	}

}

void j1FogOfWar::ClearFogLayer(const Circle zone, FOG_TYPE type)
{

	std::vector<iPoint> tiles_in;
	uint size = App->map->map_quadtree.CollectCandidates(tiles_in, zone);

	for (uint k = 0; k < size; k++)
	{
		fog_layer[tiles_in[k].y * App->map->data.width + tiles_in[k].x] = type;
	}

}
