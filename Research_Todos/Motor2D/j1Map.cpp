#define _CRT_SECURE_NO_WARNINGS

#include "j1Map.h"

#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1EntitiesManager.h"
#include "j1FogOfWar.h"

#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name = "map";
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder = (config.child("folder").child_value());

	return ret;
}

void j1Map::Draw(bool debug)
{
	//Collect the tiles inside the viewport
	std::vector<iPoint> tiles_in_view;
	map_quadtree.CollectCandidates(tiles_in_view, App->render->camera_viewport);

	uint size = tiles_in_view.size();

	//Get default tileset from default tile id
	TileSet* tileset = GetTilesetFromTileId(DEFAULT_TILE);
	
	//Get default tile texture from default tile id
	SDL_Rect r = tileset->GetTileRect(DEFAULT_TILE);

	for (uint k = 0; k < size; k++)
	{
		iPoint world_loc = MapToWorld(tiles_in_view[k].x, tiles_in_view[k].y);
		
		// TODO 2:	Map Optimization!
		//			Call the improved GetFogID method here to get tile FOG_TYPE.
		//			The tiles_in_view vector are in map coordinates so you can pass them to the GetFogID method without any transformation. 
		//			If the FOG_TYPE is DARK_FOG the tile is covered of opaque fog so its not necessary to draw it.
		// 			More Info: https://ferranmartinvila.github.io/Fog_of_war-Research/

		//Blit the current tile
		App->render->TileBlit(tileset->texture, world_loc.x, world_loc.y, &r);

	}

	//Draw map tiles net
	if (debug)
	{
		//Tiles size to locate the debug lines in the tiles vertex
		uint tile_h_2 = floor(data.tile_height * 0.5);
		uint tile_w = floor(data.tile_width);

		//X axis lines
		for (uint x = 0; x < data.width; x++)
		{
			iPoint init = MapToWorld(x, 0);
			iPoint end = MapToWorld(x, data.height);

			App->render->DrawLine(init.x + tile_w, init.y + tile_h_2, end.x + tile_w, end.y + tile_h_2, 0, 250, 0);
		}

		//Y axis lines
		for (uint y = 0; y < data.height; y++)
		{
			iPoint init = MapToWorld(0, y);
			iPoint end = MapToWorld(data.width, y);

			App->render->DrawLine(init.x, init.y + tile_h_2, end.x, end.y + tile_h_2, 0, 250, 0);
		}

	}
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	std::list<TileSet*>::const_iterator item = data.tilesets.begin();
	TileSet* set = item._Ptr->_Myval;

	while (item != data.tilesets.end())
	{
		if (id < item._Ptr->_Myval->firstgid)
		{
			set = item._Ptr->_Prev->_Myval;
			break;
		}
		set = item._Ptr->_Myval;
		item++;
	}

	return set;
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = (x - y) * (int)(data.tile_width * 0.5f) - data.tile_width * 0.5f;
	ret.y = (x + y) * (int)(data.tile_height * 0.5f) + (x + y);

	return ret;
}

iPoint j1Map::MapToWorldCenter(int x, int y) const
{
	iPoint ret = MapToWorld(x, y);

	ret.x += data.tile_width * 0.5f;
	ret.y += data.tile_height * 0.5f - MARGIN;

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(x + data.tile_width * 0.5f, y);

	float half_width = data.tile_width * 0.5f;
	float half_height = (data.tile_height + MARGIN) * 0.5f;

	float pX = (((ret.x / half_width) + (ret.y / half_height)) * 0.5f);
	float pY = (((ret.y / half_height) - (ret.x / half_width)) * 0.5f);

	ret.x = (pX > (floor(pX) + 0.5f)) ? ceil(pX) : floor(pX);
	ret.y = (pY > (floor(pY) + 0.5f)) ? ceil(pY) : floor(pY);

	if (ret.x <= 0)ret.x = 1;
	else if (ret.x >= 120)ret.x = 119;
	if (ret.y <= 0)ret.y = 0;
	else if (ret.y >= 120)ret.y = 119;

	return ret;
}

bool j1Map::IsInMap(float x,float y) const
{
	float mid_map_height = (data.height * (data.tile_height + MARGIN)) * 0.5f;
	float mid_map_width = (data.width * (data.tile_width)) * 0.5f;
	float min_y = mid_map_height - ((mid_map_width - abs(x)) * 0.5f);
	float max_y = mid_map_height + ((mid_map_width - abs(x)) * 0.5f);

	if (y < min_y)
	{
		return false;
	}

	if (y > max_y)
	{
		return false;
	}

	if (x < ((data.width * data.tile_width) * -0.5))
	{
		return false;
	}
		
	if (x > ((data.width*data.tile_width)*0.5))
	{
		return false;
	}

	if (y < 0)
	{
		return false;
	}
		
	if (y > (data.height * (data.tile_height + MARGIN)))
	{
		return false;
	}
		
	return true;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	std::list<TileSet*>::iterator item = data.tilesets.begin();

	while (item != data.tilesets.end())
	{
		RELEASE(item._Ptr->_Myval);
		item++;
	}
	data.tilesets.clear();

	// Remove all layers
	std::list<MapLayer*>::iterator item2 = data.layers.begin();

	while (item2 != data.layers.end())
	{
		RELEASE(item2._Ptr->_Myval);
		item2++;
	}
	data.layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	std::string tmp = folder + std::string(file_name);

	char* buf;
	int size = App->fs->Load(tmp.c_str(), &buf);
	pugi::xml_parse_result result = map_file.load_buffer(buf, size);

	RELEASE(buf);

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info -----------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.push_back(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.layers.push_back(lay);
	}

	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		std::list<TileSet*>::iterator item = data.tilesets.begin();
		while (item != data.tilesets.end())
		{
			TileSet* s = item._Ptr->_Myval;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.c_str(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item++;
		}

		std::list<MapLayer*>::iterator item_layer = data.layers.begin();
		while (item_layer != data.layers.end())
		{
			MapLayer* l = item_layer._Ptr->_Myval;
			LOG("Layer ----");
			LOG("name: %s", l->name.c_str());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer++;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		std::string bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.length() > 0)
		{
			std::string red, green, blue;
			red = bg_color.substr(1, 2);
			green = bg_color.substr(3, 2);
			blue = bg_color.substr(5, 2);

			int v = 0;

			sscanf_s(red.c_str(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.c_str(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.c_str(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}


		data.type = MAPTYPE_ISOMETRIC;
		

		//Define map area 
		SDL_Rect map_area;
		map_area.x = ((data.width) * data.tile_width) * -0.5;
		map_area.y = -App->render->camera.y;
		map_area.w = data.width * data.tile_width;
		map_area.h = data.height * data.tile_height + data.height;

		// Define entities quadtree area 
		App->entities_manager->entities_quadtree.SetBoundaries(map_area);

		// Define map quadtree area
		map_quadtree.SetBoundaries(map_area);
		map_quadtree.SetMaxObjects(10);

		//Fill the draw quad tree with all the tiles coordinates
		uint fails = 0;
		for (uint y = 0; y < data.height; y++)
		{
			for (uint x = 0; x < data.width; x++)
			{
				iPoint loc = MapToWorldCenter(x, y);
				if (!map_quadtree.Insert(iPoint(x, y), &loc)) fails++;
			}
		}

		LOG("Map QuadTree generated with: %i errors", fails);

	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name = (tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.c_str(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	//Get layer name
	layer->name = node.attribute("name").as_string();
	LOG("Layer Name: %s", layer->name.c_str());
	//Get layer width
	layer->width = node.attribute("width").as_int();
	LOG("Layer Width: %u", layer->width);
	//Get layer height
	layer->height = node.attribute("height").as_int();
	LOG("Layer Height: %u", layer->height);
	//Get layer data encoding
	layer->encoding = node.child("data").attribute("encoding").as_string();
	LOG("Layer Data Encoding: %s", layer->encoding.c_str());


	pugi::xml_node layer_data = node.child("data");

	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{

		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;

		if (layer->encoding == "") {

			for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
			{
				layer->data[i++] = tile.attribute("gid").as_int(0);
			}
		}

		else if (layer->encoding == "csv") {

			i = 0;

			std::string layer_tiles = layer_data.child_value();

			char* test = strtok((char*)layer_tiles.c_str(), ",");

			while (test != NULL) {
				test = strtok(NULL, ",");
				if (test == NULL)continue;
				if (*test == '\n')test++;
				layer->data[i++] = atoi(test);
			}

		}
	}

	return ret;
}
