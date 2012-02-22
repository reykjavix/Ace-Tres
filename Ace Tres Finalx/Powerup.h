#include "MyMesh.h"
#include "MyDirectX.h"

#ifndef POWERUP_
#define POWERUP_

class Powerup
{
	float X;
	float Y;
	float depth;
	int type; // 0 lose hp, 1 gain hp, 
	int status; // 0 not active, 1 active, 2 already eaten
	MyMesh *m_mesh;

public:
	Powerup(int);
	Powerup(float, float, int);
	void setPosition(float, float);
	void draw();
	RECT getHitbox();
	int getType();
	float getX();
	float getY();
	float getDepth();
	int getStatus();
	MODEL* getModel();
	void setStatus(int);
	void setType(int);
};

#endif