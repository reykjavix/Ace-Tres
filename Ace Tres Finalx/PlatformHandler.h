#include <d3d9.h>
#include <d3dx9.h>
#include "Platform.h"
#include "Reyk.h"

class PlatformHandler
{
	Platform *platformarray[1500];
	int numvisible;
public:
	PlatformHandler();
	void animatePlatforms();
	PlatformHandler(MODEL*);
	void drawPlatforms();
	void drawPlatforms3D();
	void movePlatforms();
	void checkAllPlatformsCollision(Reyk *);
	bool enemyCheckAllPlatformsCollision(RECT);
	void end();
	int getnumvisible();
	bool checkAllPlatformsRightCollision(Reyk *);
	bool checkAllPlatformsLeftCollision(Reyk *);
	
};