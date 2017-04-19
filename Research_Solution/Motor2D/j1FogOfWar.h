#ifndef _FOG_OF_WAR_
#define _FOG_OF_WAR_

#include "j1Module.h"
#include "Iso_Primitives.h"
#include "Quadtree.h"

#define ALPHA_LIMIT 255
#define MID_ALPHA 150
#define AREA_RADIANCY 0.3			/* Values between -1 & 1 */
#define	DIVISIONS_PER_PIXELS 130	/* Values between 20 & 70 in debug for >60 fps */ /* Values between 20 & 130 in release for >60 fps */
#define RENDER_MARGIN 80

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

	FOG_TYPE*				fog_layer = nullptr;	/*Layer that contains fog types*/
	AlphaCell*				alpha_layer = nullptr;	/*Layer that contains fog alpha values*/
	QuadTree<AlphaCell*>	fog_quadtree;			/*Contains the same data as the alpha layer but organized in a quadtree*/

	uint alpha_layer_width = 0;		/*Number of cells in the fog width*/
	uint alpha_layer_height = 0;	/*Number of cells in the fog height*/
	
	uint alpha_cell_size = 0;

	float light_blow = 0.0f;

public:

	bool		glow_mode = false;

	void		GenerateFogOfWar();

	FOG_TYPE	GetFogID(int x, int y)const;

	void		ClearAlphaLayer(const Circle zone, unsigned short alpha = 0, bool radiancy = false);
	void		ClearFogLayer(const Circle zone, FOG_TYPE type);
};

#endif