#ifndef __j1MAP_H__
#define __j1MAP_H__

#define MARGIN 1
#define DEFAULT_TILE 3

#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "j1Module.h"
#include "Quadtree.h"
#include "SDL/include/SDL_rect.h"
struct SDL_Texture;

// ----------------------------------------------------
struct MapLayer
{
	std::string	name;
	int			width;
	int			height;
	std::string	encoding;
	uint*		data;

	MapLayer() : data(NULL)
	{
	
	}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y*width) + x];
	}
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	std::string			name;
	int					firstgid = 0;
	int					margin = 0;
	int					spacing = 0;
	int					tile_width = 0;
	int					tile_height = 0;
	SDL_Texture*		texture = nullptr;
	int					tex_width = 0;
	int					tex_height = 0;
	int					num_tiles_width = 0;
	int					num_tiles_height = 0;
	int					offset_x = 0;
	int					offset_y = 0;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// ----------------------------------------------------
struct MapData
{
	uint				width = 0;
	uint				height = 0;
	uint				tile_width = 0;
	uint				tile_height = 0;
	SDL_Color			background_color = { 0,0,0,0 };
	MapTypes			type = MAPTYPE_UNKNOWN;
	std::list<TileSet*>	tilesets;
	std::list<MapLayer*>layers;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();
	~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw(bool debug);

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

private:

	//Load Map
	bool LoadMap();
	//Load Tiles Id
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	//Load Tiles Texture
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	//Load Map Layer
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);

public:

	//Class that allocate all the map data
	MapData data;

	//Get Tileset from tile id
	TileSet* GetTilesetFromTileId(int id) const;
	
	//Transform map coordinates to world coordinates
	iPoint MapToWorld(int x, int y) const;
	
	//Transform map coordinates to word coordinates (in tile center)
	iPoint MapToWorldCenter(int x, int y)const;
	
	//Transform world coordinates to map coordinates
	iPoint WorldToMap(int x, int y) const;
	
	//Return true if the coordinates are in the map area
	bool	IsInMap(float x, float y)const;

private:

	pugi::xml_document	map_file;
	std::string			folder;
	bool				map_loaded = false;

public:

	QuadTree<iPoint>	map_quadtree;

};
#endif // __j1MAP_H__