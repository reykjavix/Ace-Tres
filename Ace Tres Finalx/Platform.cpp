#include "MyDirectX.h"
#include "Platform.h"
#include "Reyk.h"

extern Reyk* players[];
extern MODEL *platMesh;
extern MODEL *honeycombMesh;
extern int currentlevel;

extern LPDIRECT3DTEXTURE9 platformTexture;
extern LPDIRECT3DTEXTURE9 rampTexture;
extern LPDIRECT3DTEXTURE9 squareTexture;
extern LPDIRECT3DTEXTURE9 blueTexture;
extern LPDIRECT3DTEXTURE9 honeytop1Texture;
extern LPDIRECT3DTEXTURE9 honeytop2Texture;
extern LPDIRECT3DTEXTURE9 honey1Texture;
extern LPDIRECT3DTEXTURE9 honey2Texture;
extern LPDIRECT3DTEXTURE9 conveyerrightTexture;
extern LPDIRECT3DTEXTURE9 conveyerrightendleftTexture;
extern LPDIRECT3DTEXTURE9 conveyerrightendrightTexture;
extern LPDIRECT3DTEXTURE9 conveyerleftTexture;
extern LPDIRECT3DTEXTURE9 conveyerleftendleftTexture;
extern LPDIRECT3DTEXTURE9 conveyerleftendrightTexture;

/*========================================================================


========================================================================*/

Platform::Platform(float x, float y, int w, int h, int r, boolean v, float vx, float vy)
{
	platformX = x;
	platformY = y;
	platformwidth = w;
	platformheight = h;
	ramp = r;
	visible = v;
	velX = vx;
	velY = vy;

	frame = 0;
	endframe = 9;
	delay = 60;

	toprange = 0.0;
	bottomrange = 0.0;

	m_mesh = new MyMesh();
}


/*========================================================================


========================================================================*/

float Platform::getX()
{
	return platformX;
}

/*========================================================================


========================================================================*/

float Platform::getY()
{
	return platformY;
}

float Platform::getCenterX()
{
	return platformX + platformwidth/2.0;
}


float Platform::getCenterY()
{
	return platformY + platformheight/2.0;
}

/*========================================================================


========================================================================*/

int Platform::getwidth()
{
	return platformwidth;
}

/*========================================================================


========================================================================*/

int Platform::getheight()
{
	return platformheight;
}

/*========================================================================


========================================================================*/

int Platform::getramp()
{
	return ramp;
}

/*========================================================================


========================================================================*/

bool Platform::detectPlatformCollision(RECT r2)
{
	RECT r1;
	//if (ramp == 0)
	//{
		r1.left = platformX; //- 700.0;
		r1.right = r1.left+platformwidth;
		r1.top = platformY;
		r1.bottom = r1.top+platformheight;
//	}

	return !(r1.left > r2.right || r1.right < r2.left || r1.top > r2.bottom || r1.bottom < r2.top);
}

/*========================================================================


========================================================================*/

void Platform::drawPlatform()
{
	Reyk* player1 = players[clientIndex];
	/*if (ramp == 0)
	{
		Sprite_Transform_Draw(blockTexture, platformX - (player1->getXmod()), platformY  - (player1->getYmod()), platformwidth, platformheight);
	}
	else if (ramp == 1 || ramp == 2)
	{
		Sprite_Transform_Draw(squareTexture, platformX - (player1->getXmod()), platformY - (player1->getYmod()), platformwidth, platformheight);
	}
	/*else if (ramp == 3)
	{
		Sprite_Transform_Draw(squareTexture, platformX - (player1->getXmod()), platformY - (player1->getYmod()), platformwidth, platformheight);
	}
	else if (ramp == 4)
	{
		Sprite_Transform_Draw(blueTexture, platformX - (player1->getXmod()), platformY - (player1->getYmod()), platformwidth, platformheight);
	}
	else if (ramp == 5)
	{
		Sprite_Transform_Draw(blueTexture, platformX - (player1->getXmod()), platformY - (player1->getYmod()), platformwidth, platformheight);
	} */
	if (ramp == 6)
	{
		Sprite_Transform_Draw(honeytop1Texture, platformX - (player1->getXmod()), platformY - (player1->getYmod()), platformwidth, platformheight);
	}
	else if (ramp == 7)
	{
		Sprite_Transform_Draw(honeytop2Texture, platformX - (player1->getXmod()), platformY - (player1->getYmod()), platformwidth, platformheight);
	}
	else if (ramp == 8)
	{
		Sprite_Transform_Draw(honey1Texture, platformX - (player1->getXmod()), platformY - (player1->getYmod()), platformwidth, platformheight);
	}
	else if (ramp == 9)
	{
		Sprite_Transform_Draw(honey2Texture, platformX - (player1->getXmod()), platformY - (player1->getYmod()), platformwidth, platformheight);
	}
	else if (ramp == 10)
	{
		Sprite_Transform_Draw(conveyerrightTexture, platformX - (player1->getXmod()), platformY - (player1->getYmod()), platformwidth, platformheight, frame, 10);
	}
	else if (ramp == 11)
	{
		Sprite_Transform_Draw(conveyerrightendleftTexture, platformX - (player1->getXmod()), platformY - (player1->getYmod()), platformwidth, platformheight, frame, 10);
	}
	else if (ramp == 12)
	{
		Sprite_Transform_Draw(conveyerrightendrightTexture, platformX - (player1->getXmod()), platformY - (player1->getYmod()), platformwidth, platformheight, frame, 10);
	}
	else if (ramp == 13)
	{
		Sprite_Transform_Draw(conveyerleftTexture, platformX - (player1->getXmod()), platformY - (player1->getYmod()), platformwidth, platformheight, frame, 10);
	}
	else if (ramp == 14)
	{
		Sprite_Transform_Draw(conveyerleftendleftTexture, platformX - (player1->getXmod()), platformY - (player1->getYmod()), platformwidth, platformheight, frame, 10);
	}
	else if (ramp == 15)
	{
		Sprite_Transform_Draw(conveyerleftendrightTexture, platformX - (player1->getXmod()), platformY - (player1->getYmod()), platformwidth, platformheight, frame, 10);
	}
}

void Platform::drawPlatform3D()
{
	if (currentlevel == 3 || currentlevel == 4) 
	{
		if(ramp == 3 || ramp == 4|| ramp == 5)
			m_mesh->setMesh(honeycombMesh); 
	}
	else if (ramp == 3)
			m_mesh->setMesh(platMesh);
	
	else m_mesh->setMesh(NULL);

	if(m_mesh->getModel() == NULL) return;

	Reyk* player1 = players[clientIndex];

	if (m_mesh->getModel() == platMesh)
		m_mesh->setPosition(screenToWorld(getCenterX() - player1->getXmod(), getCenterY() - player1->getYmod(), 15.0f));
	else m_mesh->setPosition(screenToWorld(getCenterX() - player1->getXmod(), getCenterY() - player1->getYmod(), 100.0f));
	m_mesh->Draw();
}

/*========================================================================


========================================================================*/

void Platform::movePlatform(float x, float y)
{
	platformX += x;
	platformY += y;
	if (ramp == 5)
	{
		if (platformY <= toprange)
		{
			velY *= -1.0;
		}
		else if (platformY >= bottomrange)
		{
			velY *= -1.0;
		}
	}
}

/*========================================================================


========================================================================*/

bool Platform::getvisible()
{
	return visible;
}

/*========================================================================


========================================================================*/

bool Platform::detectRampCollision(RECT r)
{
		long yint = getYInt(r.left);
		long x = r.left;

		if (x < platformX || x > platformX + platformwidth)
		{
			return false;
		}
		else if (r.top  >= yint || r.bottom >= yint)
		{
			return true;
		}
	return false;
}

/*========================================================================


========================================================================*/

long Platform::getYInt(long x)
{
	long p1x = platformX;
	long p1y = platformY + platformheight;
	long p2x = platformX + platformwidth;
	long p2y = platformY;

	double slope;
	long intercept;
	if (ramp == 1)
	{
		slope = -1 * ((double)p1y - (double)p2y) / ((double)p2x - (double)p1x);
		intercept = p1y - (slope * (double)p1x);
	}
	else
	{
		slope = ((double)p1y - (double)p2y) / ((double)p2x - (double)p1x);
		intercept = p1y - (slope * (double)p1x) - platformheight;
	}
	

	long yint = slope * (double)x + (double)intercept;

	return yint;
}

/*========================================================================


========================================================================*/

float Platform::getvelX()
{
	return velX;
}

/*========================================================================


========================================================================*/

float Platform::getvelY()
{
	return velY;
}

/*========================================================================


========================================================================*/

void Platform::animate()
{
	if (ramp < 10 || ramp > 15)
	{
		return;
	}
	if ((int)GetTickCount() > starttime + delay)
	{
		starttime = GetTickCount();

		frame += 1;

		if (frame > endframe)
		{
			frame = 0;
		}
	}
}

/*========================================================================


========================================================================*/

void Platform::settoprange(float t)
{
	toprange = t;
}

/*========================================================================


========================================================================*/

void Platform::setbottomrange(float b)
{
	bottomrange = b;
}

/*========================================================================


========================================================================*/

void Platform::setvelY(float vy)
{
	velY = vy;
}