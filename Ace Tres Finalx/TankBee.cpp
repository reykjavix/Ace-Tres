#include "MyDirectX.h"
#include "Reyk.h"
#include "Enemy.h"

bool detectCollision(RECT r1, RECT r2);

extern LPDIRECT3DTEXTURE9 tankbeeTexture;
extern LPDIRECT3DTEXTURE9 tankbeeheadTexture;

extern CSound *explosion;
extern Reyk* players[];

/*========================================================================


========================================================================*/

TankBee::TankBee(float x, float y, float l, float r, int d, bool b)
{
	droprate = 80;

	tankbeeTexture = LoadTexture("tankbee.png");
	tankbeeheadTexture = LoadTexture("tankbeehead.png");
	
	frame = 0;
	headframe = 0;
	startframe = 0;
	endframe = 8;
	starttime = 0;
	delay = 20;

	status = 1;
	hp = 6;
	direction = d;
	tankbeeframecount = 0;
	shakeamount = 0;
	shaking = false;

	velX = 4.0;
	velY = 0.0;
	tankbeeX = x;
	tankbeeY = y;
	savedX = 0.0;
	headX = 0.0;
	headY = 0.0;
	headvelX = 0.0;
	headvelY = 0.0;
	headrotation = 0.0;
	rotationvel = 0.0;

	leftrange = l;
	rightrange = r;

	bossbee = b;

	initialX = x;
	initialY = y;
	initialdirection = d;
	initialhp = hp;


	/* initialize particle system */
	g_pParticleSystem = new CParticleSystem();

    g_pParticleSystem->SetTexture( "particle.bmp" );
    g_pParticleSystem->SetMaxParticles( 0 );
    g_pParticleSystem->SetNumToRelease( 100 );
    g_pParticleSystem->SetReleaseInterval( 0.0001f );
    g_pParticleSystem->SetLifeCycle( 3.0f );
    g_pParticleSystem->SetSize( 0.5f );
    g_pParticleSystem->SetColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, 0.8f ));
    g_pParticleSystem->SetPosition( D3DXVECTOR3( 0.0, 0.0, 10.0f) );
    g_pParticleSystem->SetVelocity( D3DXVECTOR3( 0.0f, 1.3f, 0.0f) );
    g_pParticleSystem->SetGravity( D3DXVECTOR3( 0.0f, -1.0f, 0.0f) );
    g_pParticleSystem->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
    g_pParticleSystem->SetVelocityVar( 1.2f );

    g_pParticleSystem->Init( d3ddev );

	tankbeepowerup = new Powerup(2+(rand()%5));

	headtrailing = true;
	headtrailsize = 0;

	for (int i = 0; i < ENEMYTRAILMAX; i++)
	{
		headtrailX[i] = 0.0;
		headtrailY[i] = 0.0;
		headtrailrotation[i] = 0.0;
	}
}

/*========================================================================


========================================================================*/

void TankBee::animate()
{
	if ((int)GetTickCount() > starttime + delay)
	{
		starttime = GetTickCount();

		if (status == 0)
		{
			return;
		}
		frame += 1;

		if (frame > endframe)
		{
			frame = startframe;
		}
	}
}

/*========================================================================


========================================================================*/

void TankBee::update()
{
	tankbeeframecount++;

	if (shaking)
	{
		tankbeeX = (float)(savedX + 30.0 * (1.0 - ((double)shakeamount/10.0)) * sin((double)tankbeeframecount));

		if (direction == 0)
		{
			savedX += velX;
		}
		else
		{
			savedX -= velX;
		}
		shakeamount++;
		
		if (shakeamount > 10)
		{
			shaking = false;
			shakeamount = 0;
			tankbeeX = savedX;
		}
	}

	g_pParticleSystem->SetPosition(particlePosition);
	g_pParticleSystem->Update(0.01);

	switch (status)
	{
	case 0:
		if (bossbee)
		{
			if (tankbeeframecount >= 500)
			{
				tankbeeframecount = 0;
				tankbeeX = initialX;
				tankbeeY = initialY;
				direction = initialdirection;
				status = 1;
				hp = initialhp;
				frame = 0;
				startframe = 0;
				endframe = 8;
			}
		}
		if (tankbeepowerup->getStatus() == 1)
		{
			tankbeepowerup->setPosition(powerupX, powerupY);
			powerupY += powerupvelY;
			powerupvelY += 0.4;
		}
		break;
	case 1:
		if(IS_SERVER)
		{
			for(int i = 0; i < MAX_CLIENTS; i++)
			{
				if (detectCollision(players[i]->getHitbox(), getHitbox()))
				{
					if (players[i]->getCenterX() > tankbeeX + 140.0)
					{
						players[i]->Reyk_GetHit(0);
				
					}
					else
					{
						players[i]->Reyk_GetHit(1);
				
					}
				}
			}
		}

		if (direction == 0)
		{
			tankbeeX += velX;
			if (tankbeeX >= rightrange)
			{
				direction = 1;
			}
		}
		else
		{
			tankbeeX -= velX;
			if (tankbeeX <= leftrange)
			{
				direction = 0;
			}
		}
		break;
	case 2:
		tankbeeX = (float)(savedX + 50.0 * (1.0 - (1.0 - ((double)shakeamount/100.0))) * sin((double)tankbeeframecount));
		shakeamount++;
		if (shakeamount == 75)
			g_pParticleSystem->SetMaxParticles(100);
		if (shakeamount >= 100)
		{
			if(!IS_SERVER) PlaySound(explosion);

			shakeamount = 0;
			status = 3;
			tankbeeX = savedX;

			if(IS_SERVER)
				for(int i = 0; i < MAX_CLIENTS; i++)
					if (detectCollision(players[i]->getHitbox(), getExplosionHitbox()))
					{
						if (players[i]->getCenterX() > tankbeeX + 140.0)
						{
							players[i]->Reyk_GetHit(0);
					
						}
						else
						{
							players[i]->Reyk_GetHit(1);
					
						}
					}
		}

		headX += headvelX;
		headY += headvelY;
		headrotation += rotationvel;
		headvelY += 1.5;
		break;
	case 3:
		g_pParticleSystem->SetMaxParticles(000);

		tankbeepowerup->setPosition(powerupX, powerupY);
		powerupY += powerupvelY;
		powerupvelY += 0.4;

		if (tankbeeframecount >= 200)
		{
			status = 0;
			tankbeeframecount = 0;
		}
		break;
	}

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

	headtrailX[0] = headX;
	headtrailY[0] = headY;
	headtrailrotation[0] = headrotation;
}

/*========================================================================


========================================================================*/

void TankBee::draw()
{
	Reyk* player1 = players[clientIndex];

	if (abs((int)(tankbeeX + 140 - player1->getCenterX())) > 2560
			|| abs((int)(tankbeeY + 140 - player1->getCenterY())) > 2048)
	{
		return;
	}
	if (status == 0)
	{
		return;
	}

	float xmod = player1->getXmod();
	float ymod = player1->getYmod();

	int curframe = frame;
	if (direction == 1)
	{
		curframe += 10;
	}
	if (status == 3)
	{
		if (tankbeeframecount < 75)
		{
			Sprite_Transform_Draw(tankbeeTexture, tankbeeX - xmod, tankbeeY - ymod, 280, 280, curframe, 10);
		}
		else if (tankbeeframecount < 150)
		{
			if (tankbeeframecount % 2 == 0)
			{
				Sprite_Transform_Draw(tankbeeTexture, tankbeeX - xmod, tankbeeY - ymod, 280, 280, curframe, 10);
			}
		}
	}
	else
	{
		Sprite_Transform_Draw(tankbeeTexture, tankbeeX - xmod, tankbeeY - ymod, 280, 280, curframe, 10);
	}

	for (int i = 0; i < headtrailsize; i++)
	{
		Sprite_Transform_Draw(tankbeeheadTexture, headtrailX[i] - xmod, headtrailY[i] - ymod, 110, 110, headframe, 2, headtrailrotation[i], 1.0F, 1.0F,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(ENEMYTRAILMAX - i)/(float)ENEMYTRAILMAX));
	}

	if (status == 2)
	{
		Sprite_Transform_Draw(tankbeeheadTexture, headX - xmod, headY - ymod, 110, 110, headframe, 2, headrotation);
	}
}

void TankBee::drawParticle()
{
	Reyk* player1 = players[clientIndex];

	particlePosition = screenToWorld(tankbeeX+150 - player1->getXmod(), tankbeeY+150 - player1->getYmod(), 5.0f);
	d3ddev->SetTexture( 0, g_pParticleSystem->GetTextureObject() );
	g_pParticleSystem->Render( d3ddev );
}

/*========================================================================


========================================================================*/

void TankBee::gethit(Reyk* player1)
{
	if (shaking || status == 2 || status == 3)
	{
		return;
	}
	hp--;
	if (hp <= 0)
	{
		headtrailing = true;
		if (player1->getCenterX() > tankbeeX + 140.0)
		{
			headvelX = -8.0;
			rotationvel = -0.45;
		}
		else
		{
			headvelX = 8.0;
			rotationvel = 0.45;
		}

		if (direction == 0)
		{
			headX = tankbeeX + 94.0;
		}
		else
		{
			headX = tankbeeX + 76.0;
		}

		headY = tankbeeY + 50.0;
		headvelY = -35.0;
		tankbeeframecount = 0;
		headrotation = 0.0;
		status = 2;
		frame = 9;
		startframe = 9;
		endframe = 9;
		savedX = tankbeeX;

		if (rand()%100 < droprate && tankbeepowerup->getStatus() != 1)
		{
			powerupX = tankbeeX + 150;
			powerupY = tankbeeY + 150;
			powerupvelY = -25.0;

			tankbeepowerup->setStatus(1);
			tankbeepowerup->setPosition(powerupX, powerupY);
		}
	}
	else
	{
		shaking = true;
		shakeamount = 0;
		savedX = tankbeeX;
	}
	return;
}

/*========================================================================


========================================================================*/

int TankBee::getstatus()
{
	return status;
}

/*========================================================================


========================================================================*/

RECT TankBee::getHitbox()
{
	RECT hb;
	
	if (direction == 0)
	{
		hb.left = tankbeeX + 67.0;
	}
	else
	{
		hb.left = tankbeeX + 83.0;
	}
	hb.right = hb.left + 130.0;
	hb.top = tankbeeY + 50.0;
	hb.bottom = hb.top + 227.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT TankBee::getExplosionHitbox()
{
	RECT hb;
	
	hb.left = tankbeeX - 100.0;
	hb.right = tankbeeX + 380.0;
	hb.top = tankbeeY - 100.0;
	hb.bottom = tankbeeY + 380.0;

	return hb;
}


void TankBee::drawPowerup()
{
	tankbeepowerup->draw();
}

RECT TankBee::getPowerupHitbox()
{
	return tankbeepowerup->getHitbox();
}

float TankBee::getPowerupDepth()
{
	return tankbeepowerup->getDepth();
}

MODEL* TankBee::getPowerupModel()
{
	return tankbeepowerup->getModel();
}

int TankBee::getPowerupStatus()
{
	return tankbeepowerup->getStatus();
}

int TankBee::getPowerupType()
{
	return tankbeepowerup->getType();
}

void TankBee::eatPowerup()
{
	tankbeepowerup->setStatus(2);
}