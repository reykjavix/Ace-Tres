#include "MyDirectX.h"
#include "Reyk.h"
#include "Enemy.h"

extern Reyk* players[];
bool detectCollision(RECT r1, RECT r2);

extern ofstream debugfile;

extern LPDIRECT3DTEXTURE9 lightningbeeTexture;
extern LPDIRECT3DTEXTURE9 lightningTexture;

extern CSound *explosion;
extern CSound *lasercharge;
extern CSound *lightning;

/*========================================================================


========================================================================*/

LightningBee::LightningBee(float x, float y, float l, float r, int d, bool b)
{
	droprate = 50;

	lightningbeeTexture = LoadTexture("lightningbee.png");
	lightningTexture = LoadTexture("lightning.png");
	
	frame = 0;
	lightningframe = 0;
	endframe = 4;
	starttime = 0;
	delay = 10;

	status = 1;
	hp = 3;
	direction = d;
	lightningbeeframecount = 0;
	shakeamount = 0;
	shaking = false;

	velX = 1.5;
	velY = 0.0;
	lightningbeeX = x;
	lightningbeeY = y;
	savedX = 0.0;
	lightningbeerotation = 0.0;
	rotationvel = 0.0;
	Ybase = y;
	lightningX = 0.0;
	lightningY = 0.0;

	initialhp = hp;
	initialdirection = d;
	initialX = x;
	initialY = y;
	initialvelX = velX;

	leftrange = l;
	rightrange = r;

	bossbee = b;

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

	lightningbeepowerup = new Powerup(2+(rand()%5));

	trailing = true;
	trailsize = 0;

	for (int i = 0; i < ENEMYTRAILMAX; i++)
	{
		trailX[i] = 0.0;
		trailY[i] = 0.0;
		trailrotation[i] = 0.0;
	}
}

void LightningBee::drawParticle()
{
	Reyk* player1 = players[clientIndex];

	particlePosition = screenToWorld(lightningbeeX+150 - player1->getXmod(), lightningbeeY+150 - player1->getYmod(), 5.0f);

	d3ddev->SetTexture( 0, g_pParticleSystem->GetTextureObject() );
	g_pParticleSystem->Render( d3ddev );

	g_pParticleSystem->SetMaxParticles(0);
}

/*========================================================================


========================================================================*/

void LightningBee::animate()
{
	if (status != 3)
	{
		return;
	}
	if ((int)GetTickCount() > starttime + delay)
	{
		starttime = GetTickCount();

		if (status == 0)
		{
			return;
		}
		lightningframe += 1;

		if (lightningframe > endframe)
		{
			lightningframe = 0;
		}
	}
}

/*========================================================================


========================================================================*/

void LightningBee::update(int enemyIndex)
{
	lightningbeeframecount++;

	if (shaking)
	{
		lightningbeeX = (float)(savedX + 30.0 * (1.0 - ((double)shakeamount/10.0)) * sin((double)lightningbeeframecount));

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
			lightningbeeX = savedX;
		}
	}

	g_pParticleSystem->SetPosition(particlePosition);
	g_pParticleSystem->Update(0.01);

	switch(status)
	{
	case 0:
		if (bossbee)
		{
			if (lightningbeeframecount >= 700)
			{
				lightningbeeX = initialX;
				lightningbeeY = initialY;
				Ybase = lightningbeeY;
				hp = initialhp;
				direction = initialdirection;
				lightningbeerotation = 0.0;
				status = 1;
				velX = initialvelX;
				shaking = false;
				shakeamount = 0;
				frame = 0;
				lightningbeeframecount = 0;
			}
		}
		break;
	case 1:
		if(IS_SERVER)
			for(int i = 0; i < MAX_CLIENTS; i++)
				if (detectCollision(players[i]->getHitbox(), getHitbox()))
				{
					if (players[i]->getCenterX() > lightningbeeX + 140.0)
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
			lightningbeeX += velX;
			if (lightningbeeX >= rightrange)
			{
				direction = 1;
			}
		}
		else
		{
			lightningbeeX -= velX;
			if (lightningbeeX <= leftrange)
			{
				direction = 0;
			}
		}
		lightningbeeY = Ybase + (80.0 * sin((double)lightningbeeframecount/30.0));
		
		if(!IS_SERVER) 
			PlaySound(lasercharge);
		else
			for(int i = 0; i < MAX_CLIENTS; i++)
				if (detectCollision(players[i]->getHitbox(), getLightningHitbox()))
				{
					shakeamount = 0;
					status = 2;
					savedX = lightningbeeX;
					savedframecount = lightningbeeframecount;
				}
		break;
	case 2:
		lightningbeeX = (float)(savedX + 20.0 * (1.0 - ((double)shakeamount/45.0)) * sin((double)lightningbeeframecount));
		shakeamount++;
		if (shakeamount >= 45)
		{
			if(!IS_SERVER) PlaySound(lightning);
			shakeamount = 0;
			status = 3;
			lightningframe = 0;
			if (direction == 0)
			{
				lightningX = lightningbeeX + 15.0;
			}
			else
			{
				lightningX = lightningbeeX - 15.0;
			}
			lightningY = lightningbeeY + 240.0;
		}
		break;
	case 3:
		if(IS_SERVER)
			for(int i = 0; i < MAX_CLIENTS; i++)
			{	
				if ( detectCollision(players[i]->getHitbox(), getHitbox()))
				{
					if (players[i]->getCenterX() > lightningbeeX + 140.0)
					{
						players[i]->Reyk_GetHit(0);
				
					}
					else
					{
						players[i]->Reyk_GetHit(1);
				
					}
				}
				if ( detectCollision(players[i]->getHitbox(), getLightningHitbox()))
				{
					if (players[i]->getCenterX() > lightningbeeX + 140.0)
					{
						players[i]->Reyk_GetHit(0);
				
					}
					else
					{
						players[i]->Reyk_GetHit(1);
				
					}
				}
			}

		if (shakeamount <= 75)
		{
			lightningbeeX = (float)(savedX + 20.0 * (1.0 - (1.0 - ((double)shakeamount/75.0))) * sin((double)lightningbeeframecount));
		}
		else
		{
			lightningbeeX = (float)(savedX + 30.0 * (1.0 - ((double)shakeamount/150.0)) * sin((double)lightningbeeframecount));
		}
		shakeamount++;
		if (shakeamount >= 150)
		{
			lightningbeeX = savedX;
			status = 1;
			lightningbeeframecount = savedframecount;
		}
		break;
	case 4:
		lightningbeeX += velX;
		lightningbeeY += velY;
		velY += 1.5;
		lightningbeerotation += rotationvel;

		lightningbeepowerup->setPosition(powerupX, powerupY);
		powerupY += powerupvelY;
		powerupvelY += 0.4;

		if (bossbee)
		{
			if (lightningbeeframecount >= 100)
			{
				status = 0;
			}
		}
		else
		{
			if (lightningbeeframecount >= 200)
			{
				status = 0;
			}
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

	trailX[0] = lightningbeeX;
	trailY[0] = lightningbeeY;
	trailrotation[0] = lightningbeerotation;
}

/*========================================================================


========================================================================*/

void LightningBee::draw()
{
	Reyk* player1 = players[clientIndex];

	if (abs((int)(lightningbeeX + 140 - player1->getCenterX())) > 2560
			|| abs((int)(lightningbeeY + 140 - player1->getCenterY())) > 2048)
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
		curframe += 1;
	}

	if (status == 3)
	{
		Sprite_Transform_Draw(lightningTexture, lightningX - xmod, lightningY - ymod, 250, 780, lightningframe, 5);
	}

	for (int i = 0; i < trailsize; i++)
	{
		Sprite_Transform_Draw(lightningbeeTexture, trailX[i] - xmod, trailY[i] - ymod, 280, 280, curframe, 2, trailrotation[i], 1.0F, 1.0F,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(ENEMYTRAILMAX - i)/(float)ENEMYTRAILMAX));
	}

	Sprite_Transform_Draw(lightningbeeTexture, lightningbeeX - xmod, lightningbeeY - ymod, 280, 280, curframe, 2, lightningbeerotation);
}

/*========================================================================


========================================================================*/

void LightningBee::gethit(Reyk* player1)
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
		if(!IS_SERVER) PlaySound(explosion);

		if (player1->getCenterX() > lightningbeeX + 140.0)
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
		lightningbeeframecount = 0;
		lightningbeerotation = 0.0;
		status = 4;

		if (rand()%100 < droprate && lightningbeepowerup->getStatus() != 1)
		{
			powerupX = lightningbeeX + 150;
			powerupY = lightningbeeY + 150;
			powerupvelY = -20.0;

			lightningbeepowerup->setStatus(1);
			lightningbeepowerup->setPosition(powerupX, powerupY);
		}
	}
	else
	{
		shaking = true;
		shakeamount = 0;
		savedX = lightningbeeX;
	}
	return;
}

/*========================================================================


========================================================================*/

int LightningBee::getstatus()
{
	return status;
}

/*========================================================================


========================================================================*/

RECT LightningBee::getHitbox()
{
	RECT hb;
	
	if (direction == 0)
	{
		hb.left = lightningbeeX + 67.0;
	}
	else
	{
		hb.left = lightningbeeX + 83.0;
	}
	hb.right = hb.left + 130.0;
	hb.top = lightningbeeY + 50.0;
	hb.bottom = hb.top + 227.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT LightningBee::getLightningHitbox()
{
	RECT hb;
	
	if (direction == 0)
	{
		hb.left = lightningbeeX + 61.0;
	}
	else
	{
		hb.left = lightningbeeX + 31.0;
	}
	hb.right = hb.left + 172.0;
	hb.top = lightningbeeY + 253.0;
	hb.bottom = hb.top + 750.0;

	return hb;
}


void LightningBee::drawPowerup()
{
	lightningbeepowerup->draw();
}

RECT LightningBee::getPowerupHitbox()
{
	return lightningbeepowerup->getHitbox();
}

float LightningBee::getPowerupDepth()
{
	return lightningbeepowerup->getDepth();
}

MODEL* LightningBee::getPowerupModel()
{
	return lightningbeepowerup->getModel();
}

int LightningBee::getPowerupStatus()
{
	return lightningbeepowerup->getStatus();
}

int LightningBee::getPowerupType()
{
	return lightningbeepowerup->getType();
}

void LightningBee::eatPowerup()
{
	lightningbeepowerup->setStatus(2);
}

void LightningBee::setSomeShit(int w, int x, float y, int z)
{
	shakeamount = w;
	status = x;
	savedX = y;
	savedframecount = z;
}