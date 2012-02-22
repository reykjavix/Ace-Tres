#include <d3d9.h>
#include <d3dx9.h>
#include "MyMesh.h"

class Platform
{
	/*
	LPDIRECT3DTEXTURE9 platformTexture;
	LPDIRECT3DTEXTURE9 rampTexture;
	LPDIRECT3DTEXTURE9 squareTexture;
	LPDIRECT3DTEXTURE9 blueTexture;
	LPDIRECT3DTEXTURE9 honeytop1Texture;
	LPDIRECT3DTEXTURE9 honeytop2Texture;
	LPDIRECT3DTEXTURE9 honey1Texture;
	LPDIRECT3DTEXTURE9 honey2Texture;
	LPDIRECT3DTEXTURE9 conveyerrightTexture;
	LPDIRECT3DTEXTURE9 conveyerrightendleftTexture;
	LPDIRECT3DTEXTURE9 conveyerrightendrightTexture;
	LPDIRECT3DTEXTURE9 conveyerleftTexture;
	LPDIRECT3DTEXTURE9 conveyerleftendleftTexture;
	LPDIRECT3DTEXTURE9 conveyerleftendrightTexture;
	*/
	double platformX;
	double platformY;

	int platformwidth;
	int platformheight;
	int ramp;
	bool visible;
	int type;

	int frame;
	int endframe;
	int delay;
	int starttime;

	float velX;
	float velY;

	float toprange;
	float bottomrange;

	MyMesh *m_mesh;

public:
	Platform(float, float, int, int, int, boolean, float, float);
	float getX();
	float getY();
	float getCenterX();
	float getCenterY();
	void movePlatform(float, float);
	int getwidth();
	int getheight();
	bool detectPlatformCollision(RECT);
	bool detectRampCollision(RECT);
	void drawPlatform();
	void drawPlatform3D();
	void setvisible(bool);
	bool getvisible();
	int getramp();
	long getYInt(long);
	float getvelX();
	float getvelY();

	void animate();
	void settoprange(float);
	void setbottomrange(float);
	void setvelY(float);
};