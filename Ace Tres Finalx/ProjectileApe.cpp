#include "MyDirectX.h"
#include "Reyk.h"
#include "PlatformHandler.h"
#include "Enemy.h"

const double PI = 3.1415926535;

int THROW_ANIM[10] = { 2, 3, 4, 4, 4, 5, 6, 6, 6, 6};
const int dirdiff = 8;

MyMesh *m_mesh;

extern Reyk* players[];
extern PlatformHandler *ph;
extern MODEL* projectile0;
extern MODEL* honeycombMesh;
extern MODEL* asteriskFruit;
extern MODEL* ringBerry;
extern MODEL* XFruit;
extern MODEL* OFruit;
extern MODEL* spikedAcorn;

extern ofstream debugfile;

bool detectCollision(RECT r1, RECT r2);
void FindMinimum( float *myArray, int arraySize, int *minIndex);

extern LPDIRECT3DTEXTURE9 projectileapeTexture;
extern LPDIRECT3DTEXTURE9 projectileapeheadTexture;
extern LPDIRECT3DTEXTURE9 projectileTexture;

extern CSound *apehit;
extern CSound *apecatch;
extern CSound *apethrow;

/*========================================================================


========================================================================*/

ProjectileApe::ProjectileApe(float x, float y)
{
	droprate = 75;

	projectileapeTexture = LoadTexture("projectileape.png");
	projectileapeheadTexture = LoadTexture("projectileapehead.png");
	projectileTexture = LoadTexture("shit2.png");

	direction = 1;
	projectileapeframe = 0;
	projectileapestatus = 1;
	projectileapecounter = 0;
	projectileapecountermax = 100;
	projectileapeX = x;
	projectileapeY = y;
	projectileapevelX = 0.0;
	projectileapevelY = 0.0;

	projectileapeheadframe = 1;
	projectileapeheadX = 0.0;
	projectileapeheadY = 0.0;
	projectileapeheadvelX = 0.0;
	projectileapeheadvelY = 0.0;
	projectileapeheadrotation = 0.0;
	hp = 2;

	projectilestatus = 0;
	projectileX = 0.0;
	projectileY = 0.0;
	projectilerotation = 0.0;

	starttime = 0;
	startframe = 0;
	endframe = 9;
	delay = 20;

	projectileapebleeding = false;


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

	m_mesh = new MyMesh(projectile0);
	m_mesh->setRotation(rand(), rand(), rand());

	projectileapepowerup = new Powerup(2+(rand()%5));

	trailing = false;
	trailsize = 0;
	headtrailing = false;
	headtrailsize = 0;

	for (int i = 0; i < ENEMYTRAILMAX; i++)
	{
		trailX[i] = 0.0;
		trailY[i] = 0.0;
		trailframe[i] = 0;
	
		headtrailX[i] = 0.0;
		headtrailY[i] = 0.0;
		headtrailrotation[i] = 0.0;
	}
}

/*========================================================================


========================================================================*/

void ProjectileApe::animate()
{
	if ((int)GetTickCount() > starttime + delay)
	{
		starttime = GetTickCount();

		projectileapeframe += 1;
		if (projectileapeframe > endframe)
		{
			if (projectileapestatus == 3)
			{
				projectileapestatus = 1;
				projectileapecounter = 0;
			}
			projectileapeframe = startframe;
		}
		if (projectileapeframe < startframe) projectileapeframe = endframe;
	}	
}

/*========================================================================


========================================================================*/

void ProjectileApe::draw()
{
	Reyk* player1 = players[clientIndex];
	if ((abs((int)(projectileapeX - player1->getCenterX())) > 1280.0
		|| abs((int)(projectileapeY - player1->getCenterY())) > 1024.0))
		{
			return;
		}

	float xmod = player1->getXmod();
	float ymod = player1->getYmod();

	int curframe = 0;
	switch (projectileapestatus)
	{
	case 0:
		return;
		break;
	case 1:
		curframe = 0;
		break;
	case 2:
		curframe = 2;
		break;
	case 3:
		curframe = THROW_ANIM[projectileapeframe];
		if (curframe == 3)
		{
			projectileX = projectileapeX + 74.0;
			if (direction == 0)
			{
				projectileX = projectileapeX + 147.0;
			}
			projectileY = projectileapeY + -38.0;
		}
		else if (curframe == 4)
		{
			projectileX = projectileapeX + 166.0;
			if (direction == 0)
			{
				projectileX = projectileapeX + 53.0;
			}
			projectileY = projectileapeY + -38.0;
		}
		else if (curframe == 5)
		{		
			throwProjectile(players[0]);
		}
		break;
	case 4:
		curframe = 7;
		break;
	case 5:
		curframe = 1;
		startframe = 0;
		endframe = 0;
		projectileapeframe = 0;
		break;
	}

	if (direction == 1)
	{
		curframe += dirdiff;
	}

	for (int i = 0; i < trailsize; i++)
	{
		Sprite_Transform_Draw(projectileapeTexture, trailX[i] - xmod, trailY[i] - ymod, 320, 320, trailframe[i], 16, 0.0F, 1.0F, 1.0F,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(TRAILMAX - i)/(float)TRAILMAX));
	}

	if (projectileapestatus == 5)
	{
		if (projectileapecounter < 75)
			{
				Sprite_Transform_Draw(projectileapeTexture, projectileapeX - xmod, projectileapeY - ymod, 320, 320, curframe, 16);
			}
		else if (projectileapecounter < 150)
			{
				if (projectileapecounter%2 == 0)
				{
					Sprite_Transform_Draw(projectileapeTexture, projectileapeX - xmod, projectileapeY - ymod, 320, 320, curframe, 16);
				}
			}
		else
			{
				if (projectileapecounter == 0)
				{
					Sprite_Transform_Draw(projectileapeTexture, projectileapeX - xmod, projectileapeY - ymod, 320, 320, curframe, 16);
				}
			}

		for (int i = 0; i < headtrailsize; i++)
		{
			Sprite_Transform_Draw(projectileapeheadTexture, headtrailX[i] - xmod, headtrailY[i] - ymod, 120, 120, projectileapeheadframe, 2, headtrailrotation[i], 1.0F, 1.0F,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(TRAILMAX - i)/(float)TRAILMAX)); 
		}

		Sprite_Transform_Draw(projectileapeheadTexture, projectileapeheadX - xmod, projectileapeheadY - ymod, 120, 120, projectileapeheadframe, 2, projectileapeheadrotation); 

	}
	else if (projectileapestatus != 0)
	{
		Sprite_Transform_Draw(projectileapeTexture, projectileapeX - xmod, projectileapeY - ymod, 320, 320, curframe, 16);
	}
}

/*========================================================================


========================================================================*/

void ProjectileApe::move(float x, float y)
{
	projectileapeX += x;
	projectileapeY += y;

	projectileapeheadX += x;
	projectileapeheadY += y;

	projectileX += x;
	projectileY += y;
}

/*========================================================================


========================================================================*/


void ProjectileApe::draw3D()
{
	Reyk* player1 = players[clientIndex];
	if (projectileapestatus == 0 || (abs((int)(projectileapeX - player1->getCenterX())) > 1280.0
			|| abs((int)(projectileapeY - player1->getCenterY())) > 1024.0))
	{
		return;
	}

	if (projectilestatus == 0)
		m_mesh->setMesh(NULL);
	else if(projectileframe == 0)
		m_mesh->setMesh(projectile0);
	else if(projectileframe == 1)
		m_mesh->setMesh(ringBerry);
	else if(projectileframe == 2)
		m_mesh->setMesh(asteriskFruit);
	else if(projectileframe == 3)
		m_mesh->setMesh(OFruit);
	else if(projectileframe == 4)
		m_mesh->setMesh(XFruit);
	else if(projectileframe == 5)
		m_mesh->setMesh(spikedAcorn);


	m_mesh->setPosition(screenToWorld(projectileX+50 - player1->getXmod(), projectileY+50 - player1->getYmod(), 80.0f));
	
	if (projectileapestatus == 2)
	{
		m_mesh->setRotation(D3DXVECTOR3(0.0F, 1.57079633F, 0.0F));
	}
	else if (projectilestatus == 3) 
	{
		D3DXVECTOR3 temp = m_mesh->getRotation();
		temp.z += 0.07;
		//temp.y += 0.05;
		//temp.x += 0.05;
		
		m_mesh->setRotation(temp);
	}
	
	m_mesh->Draw();
}

void ProjectileApe::drawParticle()
{
	Reyk* player1 = players[clientIndex];

	if (projectileapestatus ==0 || (abs((int)(projectileapeX - player1->getCenterX())) > 1280.0
			|| abs((int)(projectileapeY - player1->getCenterY())) > 1024.0))
	{
		return;
	}

	if (projectileapebleeding)
		g_pParticleSystem->SetMaxParticles(500);
	else g_pParticleSystem->SetMaxParticles(0);

	particlePosition = screenToWorld(projectileapeX+160 - player1->getXmod(), projectileapeY+150 - player1->getYmod(), 5.0f);

	d3ddev->SetTexture( 0, g_pParticleSystem->GetTextureObject() );

	g_pParticleSystem->Render( d3ddev );
}

/*========================================================================


========================================================================*/

void ProjectileApe::update(int rand2)
{
	if (projectilestatus == 1)
	{
		if(IS_SERVER)
		{
			Reyk* player1;
			for(int i = 0; i < MAX_CLIENTS; i++)
			{
				player1 = players[i];
				if (detectCollision(player1->getHitbox(), projectileGetHitbox()))
				{
					if (projectileX + 50.0 > player1->getCenterX())
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

		projectileY += projectilevelY;
		projectilevelY += 0.25;
		projectilerotation += 0.5;

		if (projectileY >= projectileapeY - 11.0 && projectileapestatus == 2)
		{
			//PlaySound(apecatch);
			projectileY = projectileapeY - 11.0;
			projectilerotation = 0.0;
			projectilestatus = 2;
			projectileapestatus = 3;
			projectileapeframe = 0;
		}
		else if (projectileY > projectileapeY + 800.0)
		{
			projectilestatus = 0;
		}
	}
	else if (projectilestatus == 3)
	{
		if(IS_SERVER)
		{
			Reyk* player1;
			for(int i = 0; i < MAX_CLIENTS; i++)
			{
				player1 = players[i];
				if (detectCollision(player1->getHitbox(), projectileGetHitbox()))
				{
					if (projectileX + 50.0 > player1->getCenterX())
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
		projectileX += projectilevelX;
		projectileY += projectilevelY;
		projectilevelY += 1.0;
		projectilerotation -= 0.3;
	}

	switch (projectileapestatus)
	{
	case 0:
		projectileapebleeding = false;
		return;
		break;
	case 1:
		{
			if (projectileapeX + 160.0 >  players[0]->getCenterX())
			{
				direction = 1;
			}
			else
			{
				direction = 0;
			}

			projectileapecounter++;

			int numPlayersOutOfBounds = 0;
			for(int i = 0; i < MAX_CLIENTS; i++)
			{
				if ((abs((int)(projectileapeX - players[i]->getCenterX())) > 1280.0
					|| abs((int)(projectileapeY - players[i]->getCenterY())) > 1024.0)
					&& projectileapestatus <= 1)
					{
						numPlayersOutOfBounds++;
					}
			}

			if(numPlayersOutOfBounds == MAX_CLIENTS)
				return;

			if (projectileapecounter > 100)
			{
				projectileframe = rand2;
				projectileapecounter = 0;
				projectileapestatus = 2;
				if (direction == 1)
				{
					projectileX = projectileapeX + 8.0;
				}
				else
				{
					projectileX = projectileapeX + 208.0;
				}
				projectilestatus = 1;
				projectileY = projectileapeY - 1124.0;
				projectilevelX = 0.0;
				projectilevelY = 8.0;
				projectileapeframe = 0;
			}
			break;
		}
	case 2:
		break;
	case 3:
		break;
	case 4:
		projectileapeframe = 7;
		projectileapeX += projectileapevelX;
		projectileapeY += projectileapevelY;
		projectileapevelY += 1.8;
		if (ph->enemyCheckAllPlatformsCollision(projectileapeGetFeetHitbox()) && projectileapevelY > 0.0)
		{
			projectileapeframe = 0;
			projectileapestatus = 1;
			trailing = false;
		}
		break;
	case 5:
		if (!ph->enemyCheckAllPlatformsCollision(projectileapeGetFeetHitbox()))
		{
			projectileapeY += 10.0;
		}
		projectileapeheadX += projectileapeheadvelX;
		projectileapeheadY += projectileapeheadvelY;
		projectileapeheadvelY += 1.0;
		projectileapeheadrotation +=  0.4;
		projectileapecounter++;

		projectileapepowerup->setPosition(powerupX, powerupY);
		powerupY += powerupvelY;
		powerupvelY += 0.4;
		g_pParticleSystem->SetColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, (1.0-((float)projectileapecounter/200.0f))*1.2f ));
		if (projectileapecounter >= 200)
		{
			projectileapestatus = 0;
			projectileapebleeding = false;
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
	}

	trailX[0] = projectileapeX;
	trailY[0] = projectileapeY;
	trailframe[0] = projectileapeframe;
	if (direction == 1)
		trailframe[0] += dirdiff;

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

	headtrailX[0] = projectileapeheadX;
	headtrailY[0] = projectileapeheadY;
	headtrailrotation[0] = projectileapeheadrotation;
}


/*========================================================================


========================================================================*/

void ProjectileApe::throwProjectile(Reyk* player1)
{
	projectileY = projectileapeY + 67.0;

	if (direction == 1)
	{
		projectileX = projectileapeX - 15.0;
		projectilevelX = -25.0;
	}
	else
	{
		projectileX = projectileapeX + 185.0;
		projectilevelX = 25.0;
	}
	float Ydiff = abs(player1->getCenterY() - projectileapeY);
	projectilevelY = -8.0 - 10.0 * (1.0 - (Ydiff/200));
	projectilestatus = 3;
	//PlaySound(apethrow);
}

/*========================================================================


========================================================================*/

void ProjectileApe::gethit(Reyk* player1)
{
	hp--;
	trailing = true;
	
	//PlaySound(apehit);

	if (hp <= 0)
	{
		projectileapeDie(player1);
		return;
	}
	if (projectileapeX+160.0 > player1->getCenterX())
		{
			projectileapevelX = 10.0;
			projectileapeframe = 7;
		}
		else
		{
			projectileapevelX = -10.0;
			projectileapeframe = 3;
		}
		projectileapevelY = -15.0;
		projectileapestatus = 4;
		projectileapecounter = 0;
}

/*========================================================================


========================================================================*/

void ProjectileApe::projectileapeDie(Reyk* player1)
{
	headtrailing = true;
	trailing = false;
	projectileapestatus = 5;
	projectileapecounter = 0;
	projectileapeheadX = projectileapeX + 105.0;
	projectileapeheadY = projectileapeY + 60.0;
	if (projectileapeX+160.0 > player1->getCenterX())
	{
		projectileapeframe = 8;
		projectileapeheadframe = 1;
		projectileapeheadvelX = 6.0;
	}
	else
	{
		projectileapeframe = 1;
		projectileapeheadframe = 0;
		projectileapeheadvelX = -6.0;
	}
	projectileapeheadvelY = -22.5;

	projectileapebleeding = true;

	if (rand()%100 < droprate)
	{
		powerupX = projectileapeX + 150;
		powerupY = projectileapeY + 180;
		powerupvelY = -18.0;

		projectileapepowerup->setStatus(1);
		projectileapepowerup->setPosition(powerupX, powerupY);
	}
}

/*========================================================================


========================================================================*/

RECT ProjectileApe::projectileapeGetFeetHitbox()
{
	RECT hb;

	hb.left = projectileapeX + 93.0;
	hb.right = hb.left + 130.0;
	hb.top = projectileapeY + 240.0;
	hb.bottom = hb.top + 20.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT ProjectileApe::getfeetpoint()
{
	RECT hb;

	hb.left = projectileapeX + 158.0;
	hb.right = hb.left + 1.0;
	hb.top = projectileapeY + 230.0;
	hb.bottom = hb.top + 1.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT ProjectileApe::getHitbox()
{
	RECT hitbox;
	
	hitbox.left = projectileapeX + 93.0;
	hitbox.right = projectileapeX + 225.0;
	hitbox.top = projectileapeY + 83.0;
	hitbox.bottom = projectileapeY + 263.0;

	return hitbox;
}

/*========================================================================


========================================================================*/

RECT ProjectileApe::projectileGetHitbox()
{
	RECT hitbox;
	
	hitbox.left = projectileX;
	hitbox.right = hitbox.left + 100.0;
	hitbox.top = projectileY;
	hitbox.bottom = hitbox.top + 100.0;

	return hitbox;
}

/*========================================================================


========================================================================*/

int ProjectileApe::getstatus()
{
	return projectileapestatus;
}

/*========================================================================


========================================================================*/

int ProjectileApe::getprojectilestatus()
{
	return projectilestatus;
}

/*========================================================================


========================================================================*/

LPDIRECT3DTEXTURE9* ProjectileApe::getTexture()
{
	return (LPDIRECT3DTEXTURE9 *)projectileTexture;
}

/*========================================================================


========================================================================*/

void ProjectileApe::reset()
{
	projectileapecounter = 0;
	projectileapestatus = 1;
	projectilestatus = 0;
}

/*========================================================================


========================================================================*/

int ProjectileApe::getProjectileFrame()
{
	return projectileframe;
}

void ProjectileApe::drawPowerup()
{
	if (projectileapestatus == 0)
		return;
	projectileapepowerup->draw();
}

RECT ProjectileApe::getPowerupHitbox()
{
	return projectileapepowerup->getHitbox();
}

float ProjectileApe::getPowerupDepth()
{
	return projectileapepowerup->getDepth();
}

MODEL* ProjectileApe::getPowerupModel()
{
	return projectileapepowerup->getModel();
}

int ProjectileApe::getPowerupStatus()
{
	return projectileapepowerup->getStatus();
}

int ProjectileApe::getPowerupType()
{
	return projectileapepowerup->getType();
}

void ProjectileApe::eatPowerup()
{
	projectileapepowerup->setStatus(2);
}