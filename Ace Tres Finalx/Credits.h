#include <d3d9.h>
#include <d3dx9.h>
#include "particlesystem.h"

class Credits
{
	LPDIRECT3DTEXTURE9 bigreykTexture[5];
	LPDIRECT3DTEXTURE9 creditsTexture[39];
	CParticleSystem *g_pParticleSystem;

	float bigreykX;
	float bigreykY;
	int bigreykframe;
	int animationframe;
	int delay;
	int starttime;
	LPD3DXFONT font;

	float fireR;
	float fireG;
	float fireB;
	int firestate;

	float creditsY[39];

	int begintime;

public:
	Credits();
	void animate();
	void update();
	void draw();
	void drawFire();
};