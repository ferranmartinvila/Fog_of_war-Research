#include "j1Render.h"

#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Map.h"

///Class Blit_Call ------------------------------
//Constructors ========================
Blit_Call::Blit_Call()
{

}

Blit_Call::Blit_Call(const SDL_Rect& rect, const iPoint& pivot, const SDL_Color& color, int priority) :rect(rect), pivot(pivot), color(color), priority(priority)
{

}

//Destructors =========================
Blit_Call::~Blit_Call()
{

}

iPoint Blit_Call::GetPivot() const
{
	return pivot;
}

SDL_Rect Blit_Call::GetRect() const
{
	return rect;
}

SDL_Color Blit_Call::GetColor() const 
{
	return color;
}

bool Blit_Call::operator<(const Blit_Call & target) const
{
	if (priority < target.priority)return true;
	return false;
}

//Operator to compare blit calls in the priority queue

/// ---------------------------------------------

j1Render::j1Render() : j1Module()
{
	name = "renderer";
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
j1Render::~j1Render()
{

}

// Called before render is available
bool j1Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = App->win->screen_surface->w;
		camera.h = App->win->screen_surface->h;
		camera.x = 0;
		camera.y = 0;
	}

	return ret;
}

// Called before the first frame
bool j1Render::Start()
{
	LOG("Render start");

	// back background
	SDL_RenderGetViewport(renderer, &viewport);


	return true;
}

// Called each loop iteration
bool j1Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool j1Render::Update(float dt)
{
	uint size = blit_queue.size();
	for (uint k = 0; k < size; k++)
	{
		const Blit_Call* blit = &blit_queue.top();
		Blit(&blit->GetRect(),blit->GetPivot(),&blit->GetColor());
		blit_queue.pop();
	}
	return true;
}

bool j1Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool j1Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

void j1Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void j1Render::CalculateCameraViewport()
{
	camera_viewport = { -App->render->camera.x - (int)App->map->data.tile_width, -App->render->camera.y - (int)App->map->data.tile_height, App->render->camera.w + (int)App->map->data.tile_width * 2, App->render->camera.h + (int)App->map->data.tile_height * 2 };
}

bool j1Render::CallBlit(const SDL_Rect rect, const iPoint pivot, const SDL_Color color, int priority)
{
	bool ret = false;
	blit_queue.emplace(rect, pivot, color, priority);
	return true;
}

bool j1Render::Blit(const SDL_Rect * rect, const iPoint pivot, const SDL_Color * color) const
{
	if (rect == nullptr || color == nullptr)return false;

	SDL_Rect rect_to_blit = { rect->x + pivot.x,rect->y + pivot.y, rect->w,rect->h };
	App->render->DrawQuad(rect_to_blit, color->r, color->g, color->b, color->a);

	return true;
}

bool j1Render::TileBlit(SDL_Texture * texture, int x, int y, const SDL_Rect * section)
{
	uint scale = App->win->GetScale();

	SDL_Rect rect = { camera.x + x * scale ,camera.y + y * scale ,section->w * scale ,section->h * scale };

	SDL_Point p = { 0,0 };

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, 0, &p, SDL_RendererFlip::SDL_FLIP_NONE))
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		return false;
	}

	return true;
}

bool j1Render::FogBlit(const iPoint & position, uint w, uint h, Uint8 alpha)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);

	SDL_Rect alpha_rect;
	alpha_rect.x = position.x + camera.x;
	alpha_rect.y = position.y + camera.y;
	alpha_rect.w = w;
	alpha_rect.h = h;
	
	SDL_RenderFillRect(renderer, &alpha_rect);

	return false;
}

bool j1Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, float x_angle, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[720];

	float factor = (float)M_PI / 180.0f;

	//In case that the circle is fixed at camera coordinates
	float angle = 0.0f;

	if (!use_camera)
	{
		for (uint i = 0; i < 720; ++i)
		{
			points[i].x = (int)(x + radius * cos(angle * factor));
			points[i].y = (int)((y + radius * sin(angle * factor) * sin(x_angle)));
			angle += 0.5f;
		}
	}
	//Else if the circle is not fixed
	else
	{
		for (uint i = 0; i < 720; ++i)
		{
			points[i].x = (int)(x + radius * cos(angle * factor)) + camera.x;
			points[i].y = (int)((y + radius * sin(angle * factor) * sin(x_angle))) + camera.y;
			angle += 0.5f;
		}
	}

	result = SDL_RenderDrawPoints(renderer, points, 720);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

