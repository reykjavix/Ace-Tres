#include "MyDirectX.h"
#include "Reyk.h"
#include "ApeBoss.h"
#include "PlatformHandler.h"
#include "Enemy.h"

extern int apehandleftstatus;
extern float apefaceX;
extern float apefaceY;

extern Reyk* players[];
extern ApeBoss *aboss;
extern PlatformHandler *ph;

bool detectCollision(RECT r1, RECT r2);
void FindMinimum( float *myArray, int arraySize, int *minIndex);

extern LPDIRECT3DTEXTURE9 smallapeTexture;
extern LPDIRECT3DTEXTURE9 smallapeheadTexture;

extern CSound *apehit;

/*========================================================================


========================================================================*/

SmallApe::SmallApe(float x, float y)
{
	droprate = 50;

	smallapeTexture = LoadTexture("smallape.png");
	smallapeheadTexture = LoadTexture("smallapehead.png");
	smallapeframe = 0;
	status = 1;
	smallapecounter = 0;
	smallapecountermax = 30;
	smallapeX = x;
	smallapeY = y;
	smallapevelX = 0.0;
	smallapevelY = 0.0;
	smallaperotation = 0.0;
	smallapescale = 1.0;

	smallapeheadframe;
	smallapeheadX = 0.0;
	smallapeheadY = 0.0;
	smallapeheadvelX = 0.0;
	smallapeheadvelY = 0.0;
	smallapeheadrotation = 0.0;
	hp = 4;

	smallapebleeding = false;


	/* initialize particle system */
	g_pParticleSystem = new CParticleSystem();

    g_pParticleSystem->SetTexture( "particle.bmp" );
    g_pParticleSystem->SetMaxParticles( 0 );
    g_pParticleSystem->SetNumToRelease( 30 );
    g_pParticleSystem->SetReleaseInterval( 0.05f );
    g_pParticleSystem->SetLifeCycle( 2.0f );
    g_pParticleSystem->SetSize( 0.1f );
    g_pParticleSystem->SetColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, 0.8f ));
    g_pParticleSystem->SetPosition( D3DXVECTOR3( 0.0, 0.0, 10.0f) );
    g_pParticleSystem->SetVelocity( D3DXVECTOR3( 0.0f, 1.3f, 0.0f) );
    g_pParticleSystem->SetGravity( D3DXVECTOR3( 0.0f, -1.0f, 0.0f) );
    g_pParticleSystem->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
    g_pParticleSystem->SetVelocityVar( 0.5f );

    g_pParticleSystem->Init( d3ddev );

	smallapepowerup = new Powerup(2+(rand()%5));

	trailing = false;
	trailsize = 0;
	headtrailing = false;
	headtrailsize = 0;

	for (int i = 0; i < ENEMYTRAILMAX; i++)
	{
		trailX[i] = 0.0;
		trailY[i] = 0.0;
		trailrotation[i] = 0.0;
		trailframe[i] = 0;

		headtrailX[i] = 0.0;
		headtrailY[i] = 0.0;
		headtrailrotation[i] = 0.0;
	}
}

/*========================================================================


========================================================================*/

void SmallApe::update()
{
	int numPlayersOutOfBounds = 0;
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if (abs((int)(smallapeX - players[i]->getCenterX())) > 640.0 && status <= 1)
		{
			numPlayersOutOfBounds++;
		}
	}

	if(numPlayersOutOfBounds == MAX_CLIENTS)
		return;

	if(IS_SERVER)
	{
		Reyk* player1;
		for(int i = 0; i < MAX_CLIENTS; i++)
		{
			player1 = players[i];
			if (status != 0 && status != 3 && status != 5 && player1->getStatus() != 5 && status != 0)
			{
				if ( detectCollision(getHitbox(), player1->getHitbox()))
				{
					if (smallapeX+160.0 > player1->getCenterX())
					{
						player1->Reyk_GetHit(1);
				
					}
					else
					{
						player1->Reyk_GetHit(0);
				
					}
				}
			}
		}
	}

	switch (status)
	{
	case 0:
		smallapebleeding = false;
		return;
		break;
	case 1:
		{
			if (smallapeX+160.0 < players[0]->getCenterX())
			{
				smallapeframe = 0;
			}
			else
			{
				smallapeframe = 4;
			}
			smallapecounter++;
			if (smallapecounter >= smallapecountermax)
			{
				smallapecounter = 0;
				smallapeAttack(players[0]);
			}
			break;
		}
	case 2:
		smallapeX += smallapevelX;
		smallapeY += smallapevelY;
		smallapevelY += 1.2;
		
		if (ph->enemyCheckAllPlatformsCollision(apeGetFeetHitbox()) && smallapevelY > 0.0)
		{
			status = 1;
			trailing = false;
		}
		break;
	case 3:
		smallapeX += smallapevelX;
		smallapeY += smallapevelY;
		smallapevelY += 1.8;
		if (ph->enemyCheckAllPlatformsCollision(apeGetFeetHitbox()) && smallapevelY > 0.0)
		{
			status = 1;
			trailing = false;
		}
		break;
	case 4:
		smallapeY += smallapevelY;
		smallapescale = (smallapeY / 670.0);
		if (aboss->getlefthandstatus() != 0)
		{
			smallaperotation += 0.4;
		}
		else
		{
			smallapeX -= 1.2;
			smallaperotation -= 0.4;
		}
		if (smallapeY >= 670.0)
		{
			smallapeY = 670.0;
			smallapescale = 1.0;
			smallaperotation = 0.0;

			if (smallapeX+160.0 > players[0]->getCenterX())
			{
				smallapevelX = 10.0;
				smallapeframe = 7;
			}
			else
			{
				smallapevelX = -10.0;
				smallapeframe = 3;
			}
			smallapevelY = -18.0;
			status = 3;
		}
		break;
	case 5:
		if (!ph->enemyCheckAllPlatformsCollision(apeGetFeetHitbox()))
		{
			smallapeY += 10.0;
		}
		smallapeheadX += smallapeheadvelX;
		smallapeheadY += smallapeheadvelY;
		smallapeheadvelY += 1.0;
		smallapeheadrotation +=  0.4;
		smallapecounter++;
		smallapepowerup->setPosition(powerupX, powerupY);
		powerupY += powerupvelY;
		powerupvelY += 0.4;
		g_pParticleSystem->SetColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, (1.0-((float)smallapecounter/200.0f))*1.2f ));
		if (smallapecounter >= 200)
		{
			status = 0;
		}
		break;
	}

	g_pParticleSystem->SetPosition( particlePosition );
	g_pParticleSystem->Update(0.02);

	if (!trailing)
	{
		if (trailsize > 0)
			trailsize--;
	}
	else
	{
		if (trailsize < ENEMYTRAILMAX)
			trailsize++;
	}

	for (int i = ENEMYTRAILMAX-1; i > 0; i--)
	{
		trailX[i] = trailX[i-1];
		trailY[i] = trailY[i-1];
		trailframe[i] = trailframe[i-1];
		trailrotation[i] = trailrotation[i-1];
		trailscale[i] = trailscale[i-1];
	}

	trailX[0] = smallapeX;
	trailY[0] = smallapeY;
	trailframe[0] = smallapeframe;
	trailrotation[0] = smallaperotation;
	trailscale[0] = smallapescale;

	if (!headtrailing)
	{
		if (headtrailsize > 0)
			headtrailsize--;
	}
	else
	{
		if (headtrailsize < ENEMYTRAILMAX)
			headtrailsize++;
	}

	for (int i = ENEMYTRAILMAX-1; i > 0; i--)
	{
		headtrailX[i] = headtrailX[i-1];
		headtrailY[i] = headtrailY[i-1];
		headtrailrotation[i] = headtrailrotation[i-1];
	}

	headtrailX[0] = smallapeheadX;
	headtrailY[0] = smallapeheadY;
	headtrailrotation[0] = smallapeheadrotation;
}

/*========================================================================


========================================================================*/

void SmallApe::drawParticle()
{
	Reyk* player1 = players[clientIndex];
	if (abs((int)(smallapeX - player1->getCenterX())) > 1280.0
			|| abs((int)(smallapeY - player1->getCenterY())) > 1600.0
			|| status == 0)
		{
			return;
		}

	if (smallapebleeding)
		g_pParticleSystem->SetMaxParticles(500);
	else g_pParticleSystem->SetMaxParticles(0);

	particlePosition = screenToWorld(smallapeX+150 - player1->getXmod(), smallapeY+150 - player1->getYmod(), 5.0f);

	d3ddev->SetTexture( 0, g_pParticleSystem->GetTextureObject() );
	g_pParticleSystem->Render( d3ddev );
}

/*========================================================================


========================================================================*/

void SmallApe::gethit(Reyk* player1)
{
	hp--;
	trailing = true;
	if (hp <= 0)
	{
		smallapeDie(player1);
		return;
	}
	if (smallapeX+160.0 > player1->getCenterX())
		{
			smallapevelX = 10.0;
			smallapeframe = 7;
		}
		else
		{
			smallapevelX = -10.0;
			smallapeframe = 3;
		}
		smallapevelY = -15.0;
		status = 3;
		smallapecounter = 0;
}

/*========================================================================


========================================================================*/

void SmallApe::smallapeAttack(Reyk* player1)
{
	status = 2;
	trailing = true;
	if (smallapeX+160.0 > player1->getCenterX())
	{
		smallapevelX = -10.0;
			smallapeframe = 6;
	}
	else
	{
		smallapevelX = 10.0;
		smallapeframe = 2;
	}
	smallapevelY = -15.0;
}

/*========================================================================


========================================================================*/

void SmallApe::smallapeDie(Reyk* player1)
{
	headtrailing = true;
	status = 5;
	smallapecounter = 0;
	smallapeheadX = smallapeX + 105.0;
	smallapeheadY = smallapeY + 60.0;
	if (smallapeX+160.0 > player1->getCenterX())
	{
		smallapeframe = 5;
		smallapeheadframe = 1;
		smallapeheadvelX = 6.0;
	}
	else
	{
		smallapeframe = 1;
		smallapeheadframe = 0;
		smallapeheadvelX = -6.0;
	}
	smallapeheadvelY = -22.5;

	smallapebleeding = true;

	if (rand()%100 < droprate)
	{

		powerupvelY = -18.0;
		powerupX = smallapeX + 150;
		powerupY = smallapeY + 180;

		smallapepowerup->setStatus(1);
		smallapepowerup->setPosition(powerupX, powerupY);
	}
}

/*========================================================================


========================================================================*/

RECT SmallApe::getHitbox()
{
	RECT hitbox;
	
	hitbox.left = smallapeX + 93.0;
	hitbox.right = smallapeX + 225.0;
	hitbox.top = smallapeY + 83.0;
	hitbox.bottom = smallapeY + 263.0;

	return hitbox;
}

/*========================================================================


========================================================================*/

RECT SmallApe::apeGetFeetHitbox()
{
	RECT hb;

	hb.left = smallapeX + 93.0;
	hb.right = hb.left + 130.0;
	hb.top = smallapeY + 240.0;
	hb.bottom = hb.top + 20.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT SmallApe::getfeetpoint()
{
	RECT hb;

	hb.left = smallapeX + 158.0;
	hb.right = hb.left + 1.0;
	hb.top = smallapeY + 250.0;
	hb.bottom = hb.top + 1.0;

	return hb;
}

/*========================================================================


========================================================================*/

int SmallApe::getstatus()
{
	return status;
}

/*========================================================================


========================================================================*/

int SmallApe::getX()
{
	return smallapeX;
}


/*========================================================================


========================================================================*/

int SmallApe::getY()
{
	return smallapeY;
}

/*========================================================================


========================================================================*/

int SmallApe::getSWX()
{
	Reyk* player1 = players[clientIndex];
	D3DXVECTOR3 world = screenToWorld(smallapeX+150 - player1->getXmod(), smallapeY+150 - player1->getYmod(), 5.0f);
	return world.x;
}

/*========================================================================


========================================================================*/

int SmallApe::getSWY()
{
	Reyk* player1 = players[clientIndex];
	D3DXVECTOR3 world = screenToWorld(smallapeX+150 - player1->getXmod(), smallapeY+150 - player1->getYmod(), 5.0f);
	return world.y;
}

/*========================================================================


========================================================================*/

int SmallApe::getframe()
{
	return smallapeframe;
}

/*========================================================================


========================================================================*/

float SmallApe::getrotation()
{
	return smallaperotation;
}

/*========================================================================


========================================================================*/

float SmallApe::getscale()
{
	return smallapescale;
}

/*========================================================================


========================================================================*/

void SmallApe::getShot()
{
	status = 4;
	hp = 4;
	smallapevelY = 20.0;
	smallapebleeding = false;
	trailing = true;
	trailsize = 0;
	if (aboss->getlefthandstatus() != 0)
	{
		smallapeX = aboss->getX() + 400.0 + 10685.0;
		smallapeframe = 7;
	}
	else
	{
		smallapeX = aboss->getX() + 260.0 + 10685.0;
		smallapeframe = 3;
	}
	smallapeY = aboss->getY() + 400.0;
}

/*========================================================================


========================================================================*/

int SmallApe::getcounter()
{
	return smallapecounter;
}

/*========================================================================


========================================================================*/

void SmallApe::draw()
{
	Reyk* player1 = players[clientIndex];

	if (abs((int)(smallapeX - player1->getCenterX())) > 1280.0
			|| abs((int)(smallapeY - player1->getCenterY())) > 1600.0
			|| status == 0)
		{
			return;
		}

		D3DCOLOR color;
		color = D3DCOLOR_XRGB(255,255,255);

		//float xmod = player1->getMapX() - 415.0;
		//float ymod = player1->getMapY() - 415.0;

		float xmod = player1->getXmod();
		float ymod = player1->getYmod();

		for (int i = 0; i < trailsize; i++)
		{
			Sprite_Transform_Draw(smallapeTexture, trailX[i] - xmod, trailY[i] - ymod, 320, 320, trailframe[i], 8, trailrotation[i], trailscale[i], trailscale[i], 
				D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(ENEMYTRAILMAX - i)/(float)ENEMYTRAILMAX));
		}

		if (status == 5)
		{
			if (smallapecounter < 75)
			{
				Sprite_Transform_Draw(smallapeTexture, smallapeX - xmod, smallapeY - ymod, 320, 320, smallapeframe, 8, smallaperotation, smallapescale, smallapescale, color);
			}
			else if (smallapecounter < 150)
			{
				if (smallapecounter%2 == 0)
				{
					Sprite_Transform_Draw(smallapeTexture, smallapeX - xmod, smallapeY - ymod, 320, 320, smallapeframe, 8, smallaperotation, smallapescale, smallapescale, color);
				}
			}
			else
			{
				if (smallapecounter == 0)
				{
					Sprite_Transform_Draw(smallapeTexture, smallapeX - xmod, smallapeY - ymod, 320, 320, smallapeframe, 8, smallaperotation, smallapescale, smallapescale, color);
				}
			}

			for (int i = 0; i < headtrailsize; i++)
			{
				Sprite_Transform_Draw(smallapeheadTexture, headtrailX[i] - xmod, headtrailY[i] - ymod, 120, 120, smallapeheadframe, 2, headtrailrotation[i], 1.0F, 1.0F, 
					D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(ENEMYTRAILMAX - i)/(float)ENEMYTRAILMAX));
			}

			Sprite_Transform_Draw(smallapeheadTexture, smallapeheadX - xmod, smallapeheadY - ymod, 120, 120, smallapeheadframe, 2, smallapeheadrotation, 1.0F, 1.0F, color);
		}
		else if (status != 0)
		{
			Sprite_Transform_Draw(smallapeTexture, smallapeX - xmod, smallapeY - ymod, 320, 320, smallapeframe, 8, smallaperotation, smallapescale, smallapescale, color);
		}
}

/*========================================================================


========================================================================*/

void SmallApe::move(float x, float y)
{
	smallapeX += x;
	smallapeY += y;

	smallapeheadX += x;
	smallapeheadY += y;
}

/*========================================================================


========================================================================*/

void SmallApe::setstatus(int s)
{
	status = s;
}

void SmallApe::drawPowerup()
{
	if (status == 0)
		return;
	smallapepowerup->draw();
}

RECT SmallApe::getPowerupHitbox()
{
	return smallapepowerup->getHitbox();
}

float SmallApe::getPowerupDepth()
{
	return smallapepowerup->getDepth();
}

MODEL* SmallApe::getPowerupModel()
{
	return smallapepowerup->getModel();
}

int SmallApe::getPowerupStatus()
{
	return smallapepowerup->getStatus();
}

int SmallApe::getPowerupType()
{
	return smallapepowerup->getType();
}

void SmallApe::eatPowerup()
{
	smallapepowerup->setStatus(2);
}