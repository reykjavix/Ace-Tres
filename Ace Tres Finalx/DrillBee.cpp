#include "MyDirectX.h"
#include "Reyk.h"
#include "Enemy.h"

extern Reyk* players[];
bool detectCollision(RECT r1, RECT r2);

extern LPDIRECT3DTEXTURE9 drillbeeTexture;

extern CSound *drill;
extern CSound *explosion;

/*========================================================================


========================================================================*/

DrillBee::DrillBee(float x, float y, float l, float r, int d, bool b)
{
	droprate = 25;

	srand(GetTickCount());
	drillbeeTexture = LoadTexture("drillbee.png");
	
	frame = 0;
	endframe = 10;
	delay = 20;

	hp = 2;
	status = 1;
	direction = d;
	drillbeeframecount = 0;
	shakeamount = 0;
	shaking = false;

	velX = 3.0;
	velY = 0.0;
	drillbeeX = x;
	drillbeeY = y;
	savedX = x;
	drillbeerotation = 0.0;
	rotationvel = 0.0;
	Ybase = y;

	bossbee = b;
	initialhp = hp;

	leftrange = l;
	rightrange = r;

	/* initialize particle system */
	g_pParticleSystem = new CParticleSystem();

    g_pParticleSystem->SetTexture( "particle.bmp" );
    g_pParticleSystem->SetMaxParticles( 0 );
    g_pParticleSystem->SetNumToRelease( 100 );
    g_pParticleSystem->SetReleaseInterval( 0.0001f );
    g_pParticleSystem->SetLifeCycle( 0.25f );
    g_pParticleSystem->SetSize( 0.2f );
    g_pParticleSystem->SetColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 0.8f ));
    g_pParticleSystem->SetPosition( D3DXVECTOR3( 0.0, 0.0, 10.0f) );
    g_pParticleSystem->SetVelocity( D3DXVECTOR3( 0.0f, 1.3f, 0.0f) );
    g_pParticleSystem->SetGravity( D3DXVECTOR3( 0.0f, -1.0f, 0.0f) );
    g_pParticleSystem->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
    g_pParticleSystem->SetVelocityVar( 2.0f );

    g_pParticleSystem->Init( d3ddev );

	drillbeepowerup = new Powerup(2+(rand()%5));

	trailing = true;
	trailsize = 0;

	for (int i = 0; i < ENEMYTRAILMAX; i++)
	{
		trailX[i] = 0.0;
		trailY[i] = 0.0;
		trailrotation[i] = 0.0;
	}
}

void DrillBee::drawParticle()
{
	Reyk* player1 = players[clientIndex];

	particlePosition = screenToWorld(drillbeeX+150 - player1->getXmod(), drillbeeY+150 - player1->getYmod(), 5.0f);

	d3ddev->SetTexture( 0, g_pParticleSystem->GetTextureObject() );
	g_pParticleSystem->Render( d3ddev );

	g_pParticleSystem->SetMaxParticles(0);
}

/*========================================================================


========================================================================*/

void DrillBee::animate()
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
			frame = 0;
		}
	}
}

/*========================================================================


========================================================================*/

void DrillBee::update(int rand2, int rand800, int rand20)
{
	drillbeeframecount++;

	if (shaking)
	{
		drillbeeX = (float)(savedX + 30.0 * (1.0 - ((double)shakeamount/10.0)) * sin((double)drillbeeframecount));

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
			drillbeeX = savedX;
		}
	}

	g_pParticleSystem->SetPosition(particlePosition);
	g_pParticleSystem->Update(0.01);

	switch(status)
	{
	case 0:
		if (bossbee)
		{
			drillbeeframecount++;
			if (drillbeeframecount >= 350)
			{
				int randside = rand2;
				if (randside == 0)
				{
					drillbeeX = 1600.0;
					direction = 1;
				}
				else
				{
					drillbeeX = -200.0;
					direction = 0;
				}
				int Yoffset = rand800;
				drillbeeY = 0.0 + (float)Yoffset;
				Ybase = drillbeeY;
				hp = initialhp;
				drillbeeframecount = rand20;
				drillbeerotation = 0.0;
				status = 1;
				velX = 3.0;
				shaking = false;
				shakeamount = 0;
				frame = 0;
			}
		}
		if (drillbeepowerup->getStatus() == 1)
		{
			drillbeepowerup->setPosition(powerupX, powerupY);
			powerupY += powerupvelY;
			powerupvelY += 0.4;
		}
		break;
	case 1:
		if(IS_SERVER)
			for(int i = 0; i < MAX_CLIENTS; i++)
				if ( detectCollision(players[i]->getHitbox(), getHitbox()))
				{
					if(!IS_SERVER) PlaySound(drill);

					if (players[i]->getCenterX() > drillbeeX + 140.0)
					{
						players[i]->Reyk_GetHit(0);
					}
					else
					{
						players[i]->Reyk_GetHit(1);
					}
				}

		if (direction == 0)
		{
			drillbeeX += velX;
			if (drillbeeX >= rightrange)
			{
				direction = 1;
			}
		}
		else
		{
			drillbeeX -= velX;
			if (drillbeeX <= leftrange)
			{
				direction = 0;
			}
		}
		drillbeeY = Ybase + (150.0 * sin((double)drillbeeframecount/30.0));
		break;
	case 2:
		drillbeeX += velX;
		drillbeeY += velY;
		velY += 1.5;
		drillbeerotation += rotationvel;

		drillbeepowerup->setPosition(powerupX, powerupY);
		powerupY += powerupvelY;
		powerupvelY += 0.4;

		if (drillbeeframecount >= 100)
		{
			drillbeeframecount = 0;
			status = 0;
		}
	}

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
		trailrotation[i] = trailrotation[i-1];
	}

	trailX[0] = drillbeeX;
	trailY[0] = drillbeeY;
	trailrotation[0] = drillbeerotation;
}

/*========================================================================


========================================================================*/

void DrillBee::draw()
{
	Reyk* player1 = players[clientIndex];

	if (abs((int)(drillbeeX + 140 - player1->getCenterX())) > 2560
			|| abs((int)(drillbeeY + 140 - player1->getCenterY())) > 2048)
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
		curframe += 11;
	}

	for (int i = 0; i < trailsize; i++)
	{
		Sprite_Transform_Draw(drillbeeTexture, trailX[i] - xmod, trailY[i] - ymod, 280, 280, curframe, 11, trailrotation[i], 1.0F, 1.0F,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(ENEMYTRAILMAX - i)/(float)ENEMYTRAILMAX));
	}

	Sprite_Transform_Draw(drillbeeTexture, drillbeeX - xmod, drillbeeY - ymod, 280, 280, curframe, 11, drillbeerotation);
}

/*========================================================================


========================================================================*/

void DrillBee::gethit(Reyk* player1)
{
	g_pParticleSystem->SetMaxParticles(100);

	if (shaking)
	{
		return;
	}
	hp--;
	if (hp <= 0)
	{
		trailing = true;

		if (player1->getCenterX() > drillbeeX + 140.0)
		{
			velX = -4.0;
			rotationvel = -0.45;
		}
		else
		{
			velX = 4.0;
			rotationvel = 0.45;
		}

		velY = -35.0;
		drillbeeframecount = 0;
		drillbeerotation = 0.0;
		status = 2;

		if (rand()%100 < droprate && drillbeepowerup->getStatus() != 1)
		{
			powerupX = drillbeeX + 150;
			powerupY = drillbeeY + 150;
			powerupvelY = -20.0;

			drillbeepowerup->setStatus(1);
			drillbeepowerup->setPosition(powerupX, powerupY);
		}
	}
	else
	{
		shaking = true;
		shakeamount = 0;
		savedX = drillbeeX;
	}
	return;
}

/*========================================================================


========================================================================*/

int DrillBee::getstatus()
{
	return status;
}

/*========================================================================


========================================================================*/

RECT DrillBee::getHitbox()
{
	RECT hb;
	
	if (direction == 0)
	{
		hb.left = drillbeeX + 67.0;
	}
	else
	{
		hb.left = drillbeeX + 83.0;
	}
	hb.right = hb.left + 130.0;
	hb.top = drillbeeY + 50.0;
	hb.bottom = hb.top + 227.0;

	return hb;
}

void DrillBee::drawPowerup()
{
	drillbeepowerup->draw();
}

RECT DrillBee::getPowerupHitbox()
{
	return drillbeepowerup->getHitbox();
}

float DrillBee::getPowerupDepth()
{
	return drillbeepowerup->getDepth();
}

MODEL* DrillBee::getPowerupModel()
{
	return drillbeepowerup->getModel();
}

int DrillBee::getPowerupStatus()
{
	return drillbeepowerup->getStatus();
}

int DrillBee::getPowerupType()
{
	return drillbeepowerup->getType();
}

void DrillBee::eatPowerup()
{
	drillbeepowerup->setStatus(2);
}