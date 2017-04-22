#ifndef __j1TEXTURES_H__
#define __j1TEXTURES_H__

#include "j1Module.h"

struct SDL_Texture;
struct SDL_Surface;

class j1Textures : public j1Module
{
public:

	j1Textures();
	~j1Textures();

public:

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Load Texture
	SDL_Texture* const	Load(const char* path);

	// Load Surface
	SDL_Texture* const	LoadSurface(SDL_Surface* surface);

public:

	std::list<SDL_Texture*>	textures;

};


#endif // __j1TEXTURES_H__