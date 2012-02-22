#include <d3d9.h>
#include <d3dx9.h>
#include "particlesystem.h"

#pragma once
#ifndef REYK_H
#define REYK_H

// Animation arrays
const int WALK[8] = {1, 2, 1, 0, 3, 4, 3, 0};
const int IDLE[16] = {0, 5, 6, 7, 8, 9, 10, 11, 12, 11, 10, 9, 8, 7, 6, 5};
const int ATTACK[13] = {0, 15, 16, 17, 18, 19, 19, 20, 21, 21, 21, 21, 21};
const int JUMP[9] = {22, 23, 24, 25, 26, 27, 28, 29, 30};
const int LICK[25] = {31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56};
const int BARFFIRE[13] = {32, 33, 33, 33, 33, 33, 50, 51, 52, 53, 54, 55, 56};

const float lickrightX[14] = { 201.0, 234.0, 302.0, 348.0, 348.0, 348.0, 348.0, 338.0, 334.0, 333.0, 302.0, 243.0, 209.0, 173.0 };
const float lickleftX[14] = { 148.0, 122.0, 50.0, 3.0, 3.0, 3.0, 3.0, 13.0, 16.0, 16.0, 50.0, 240.0, 208.0, 171.0 };
const float lickY[14] = { 116.0, 97.0, 137.0, 179.0, 161.0, 158.0, 136.0, 121.0, 114.0, 103.0, 135.0, 101.0, 127.0, 129.0 };

// Used to pick the correct frame when player changes direction
const int DIRDIFF = 60;

const int TRAILMAX = 35;

extern LPD3DXSPRITE spriteobj;

class Reyk
{
	LPDIRECT3DTEXTURE9 reykTexture;
	LPDIRECT3DTEXTURE9 hpbarTexture;
	LPDIRECT3DTEXTURE9 lickTexture;
	LPDIRECT3DTEXTURE9 fireballTexture;
	LPDIRECT3DTEXTURE9 tinyTexture;
	LPDIRECT3DTEXTURE9 headTexture;

	int curframe;
	int frame;
	int startframe;
	int starttime;
	int endframe;
	int delay;
	int poweruptype;
	int framecount;
	int direction;
	int status;
	int jumpcounter;
	int reykWidth;
	int reykHeight;
	float reykX;
	float reykY;
	float jumpVel;
	float velX;
	float velY;
	float rotation;
	bool slowed;
	int slowcounter;
	int hp;
	int energy;
	bool invincible;
	int invinciblecounter;
	bool grounded;
	float platformleft;
	float platformright;
	
	float oldX;
	float newX;
	float oldY;
	float newY;
	
	float mapX;
	float mapY;

	bool lockedleft;
	bool lockedright;
	bool lockedtop;
	bool lockedbottom;
	float lockedleftamount;
	float lockedrightamount;
	float lockedtopamount;
	float lockedbottomamount;


	bool collidingright;
	bool collidingleft;
	int zvalue;

	bool licking;
	bool fallingforward;

	bool pressingleft;
	bool pressingright;

	bool fireballactive;
	int fireballcounter;
	int fireballframe;
	float fireballvel;
	float fireballX;
	float fireballY;
	float fireballrotation;

	float oldmapY;

	float groundedvelX;
	float groundedvelY;

	float modelDepth;

	bool finallick;

	float headX;
	float headY;
	float headvelX;
	float headvelY;
	float headrotation;
	float headrotationvel;
	int headframe;

	int deathcounter;
	int playerAssignedIndex;
	bool sendPacket;

	bool trailing;
	int trailsize;
	float trailX[TRAILMAX];
	float trailY[TRAILMAX];
	int trailframe[TRAILMAX];

	bool headtrailing;
	int headtrailsize;
	float headtrailX[TRAILMAX];
	float headtrailY[TRAILMAX];
	float headtrailrotation[TRAILMAX];

	bool reykbleeding;
	CParticleSystem *g_pParticleSystem;
	D3DXVECTOR3 particlePosition;

public:
	Reyk(int index);
	void Reyk_Animate();
	void Reyk_Draw_Frame(int, int, int, int, int, int);
	void Reyk_Transform_Draw();
	void Reyk_Draw3D();
	void Reyk_Move();
	void Reyk_Jump();
	void Reyk_Attack();
	void Reyk_GetHit(int);
	void Reyk_Update();
	void Reyk_GetSlowed();
	void Reyk_GetInvincible();
	void Reyk_Land(float, float, float);
	void Reyk_Lick();
	void Reyk_Die();
	void Reyk_BarfFireball();
	void Reyk_StartSliding(float, float);

	void setTexture(LPDIRECT3DTEXTURE9);
	int getStatus();
	void setStatus(int);
	int getDirection();
	void setDirection(int);
	float getJumpVel();
	void setJumpVel(float);
	void setFrame(int);
	float getX();
	float getY();
	void setX(float);
	float getCenterX();
	float getCenterY();
	void checkAxeCollision();
	float getvelX();
	float getvelY();
	void setgrounded(bool);
	bool getgrounded();
	int gethp();

	float getXchange();
	void setNewX();
	void setOldX();
	float getYchange();
	void setNewY();
	void setOldY();

	void setMapX(float);
	float getMapX();
	float getMapY();
	void setcollidingleft(bool);
	void setcollidingright(bool);

	float getplatformleft();
	float getplatformright();
	void setplatformleft(float);
	void setplatformright(float);

	void setY(float);

	RECT getHitbox();
	RECT getJumpHitbox();
	RECT getLeftHitbox();
	RECT getRightHitbox();
	RECT getFeetHitbox();
	RECT getFeetPoint();
	RECT getFireballHitbox();

	void checkLickCollision();
	void setLickTexture(LPDIRECT3DTEXTURE9, int);
	void setLick(MODEL*, float, int);
	void setfallingforward(bool);

	void setpressingleft(bool);
	void setpressingright(bool);

	void setMapY(float);

	float getXmod();
	float getYmod();

	bool getlockedleft();
	bool getlockedbottom();
	bool getlockedright();
	
	void setoldmapY();

	bool getpressingleft();
	bool getpressingright();

	void setgroundedvelX(float);
	void setgroundedvelY(float);

	void setlockedbottom(bool);
	void setlockedbottomamount(float);

	float getlockedbottomamount();

	void setlockedleft(bool);
	void setlockedright(bool);
	
	int getAssignedIndex();

	void setSendPacket(bool b);
	int getenergy();

	bool getinvincible();
	void setFinalLick(bool b);

	void Reyk_DrawParticle();
};

#endif /* REYK_H */