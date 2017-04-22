#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Module.h"
#include <queue>

///Class Blit_Call ------------------------------
//Little class to save all blit calls and organize them in a priority queue
class Blit_Call
{
public:

	Blit_Call();
	Blit_Call(const SDL_Rect& rect, const iPoint& pivot, const SDL_Color& color, int priority = 0);
	~Blit_Call();

private:

	iPoint				pivot = { 0,0 };
	SDL_Rect			rect = { 0,0,0,0 };
	SDL_Color			color = { 0,0,0,0 };
	int					priority = 0;

public:

	//Get Methods -----------
	iPoint				GetPivot()const;
	SDL_Rect			GetRect()const;
	SDL_Color			GetColor()const;

	//Operator to compare blit calls in the priority queue
	bool operator<(const Blit_Call& target) const;

};
/// ---------------------------------------------

class j1Render : public j1Module
{
public:

	j1Render();
	~j1Render();

public:

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:

	//Priority queue to organize all the blits
	std::priority_queue<Blit_Call> blit_queue;

public:

	SDL_Renderer*	renderer = nullptr;
	SDL_Rect		camera;
	SDL_Rect		viewport;
	SDL_Rect		camera_viewport;
	SDL_Color		background;

public:

	//Calculate the current camera viewport used to collect the elements in camera
	void	CalculateCameraViewport();

	//Add blit call ---------
	bool	CallBlit(const SDL_Rect rect, const iPoint pivot, const SDL_Color color, int priority = 0);
	
	// Draw & Blit ----------
	bool Blit(const SDL_Rect* rect, const iPoint pivot, const SDL_Color* color) const;
	bool TileBlit(SDL_Texture* texture, int x, int y, const SDL_Rect* section);
	bool FogBlit(const iPoint& position, uint cell_size , Uint8 alpha);

	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, float x_angle = 0, bool use_camera = true) const;

	// Set background color -
	void SetBackgroundColor(SDL_Color color);

};

#endif // __j1RENDER_H__