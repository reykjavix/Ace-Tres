#include <d3d9.h>
#include <d3dx9.h>
#include "particlesystem.h"
#include "Reyk.h"

const int CROCTRAILMAX = 34;

class Croctopus
{
	LPDIRECT3DTEXTURE9 bottomTexture;
	LPDIRECT3DTEXTURE9 topTexture;

	LPDIRECT3DTEXTURE9 crocTexture;
	LPDIRECT3DTEXTURE9 crocRTexture;

	LPDIRECT3DTEXTURE9 tentacleTexture;
	LPDIRECT3DTEXTURE9 tentacleRTexture;

	LPDIRECT3DTEXTURE9 eyeoneTexture;
	LPDIRECT3DTEXTURE9 eyetwoTexture;
	LPDIRECT3DTEXTURE9 eyetwoRTexture;
	LPDIRECT3DTEXTURE9 eyethreeTexture;
	LPDIRECT3DTEXTURE9 eyethreeRTexture;
	LPDIRECT3DTEXTURE9 eyefourTexture;
	LPDIRECT3DTEXTURE9 eyefourRTexture;

	CParticleSystem *g_pParticleSystem[7];

	int croctopuscounter;
	int attackcounter;
	bool attacking;
	int whichspot;
	int attackorder[3];

	float crocX;
	float crocY;

	float bottomX;
	float bottomY;
	
	float topX;
	float topY;

	float crocfaceX[3];
	float savedcrocfaceX[3];
	float crocfaceY[3];
	float crocfacevelY[3];
	int crocfaceframe[3];
	int endframe[3];
	int startframe[3];
	int crocfacecounter[3];
	int crocfacestatus[3];
	bool playercaught[3];
	int whichplayer[3];

	float eyeX[7];
	float eyeY[7];
	float eyevelX[7];
	float eyevelY[7];
	float eyerotation[7];

	int eyehp[7];
	int eyeframe[7];
	int eyeshake[7];

	int starttime;
	int delay;

	int hp;

	bool dying;
	float savedtopX;
	float savedbottomX;

	D3DXVECTOR3 particlePositions[7];

	int eyetrailsize[7];
	float eyetrailX[7][CROCTRAILMAX];
	float eyetrailY[7][CROCTRAILMAX];
	float eyetrailrotation[7][CROCTRAILMAX];

public:
	Croctopus();
	~Croctopus();
	void update(int rand4, int* randarr, int rand3);
	void move(float, float);
	void draw();
	void drawcrocs();
	RECT geteyeHitbox(int);
	void eyegethit(int, Reyk*);
	void animate();
	void startattack(int rand4, int* randarr, int rand3);
	void detectcroccollision();
	int* randomizeAttack();
	void setplayercaught(int i, bool b, int which);

	void drawParticle();
	int gethp();
};