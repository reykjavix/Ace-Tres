#include "MyDirectX.h"
#include "Reyk.h"
#include "PlatformHandler.h"
#include "EnemyHandler.h"

#include "ApeBoss.h"
#include "Croctopus.h"
#include "QueenBee.h"
#include "XBoss.h"

#include "Credits.h"

#include "SongPlayer.h"
#include "fmod.h"
#include "fmod_errors.h"

extern int currentlevel;

bool detectCollision(RECT r1, RECT r2);

extern ApeBoss *aboss;
extern Croctopus *croc;
extern QueenBee *qbee;
extern XBoss *xboss;

extern Credits *creds;

extern PlatformHandler *ph;
extern EnemyHandler *eh;

const float REYK_MOVE_SPEED = 6.0;
const float REYK_JUMP_VEL = -25.0;
const float GRAVITY = 1.0;

const int HURTAMOUNT = 4;

extern int gamestatus;

extern ofstream debugfile;

extern FSOUND_STREAM * Song;
extern void SwitchSong(FSOUND_STREAM * & SongData, char *p_FileName);

extern bool paused;

MyMesh *reyk_mesh;

void loadBoss1();
void loadBoss2();
void loadBoss3();
void loadBoss4();
void loadCredits();

/*========================================================================


========================================================================*/

Reyk::Reyk(int index)
{
	//load the sprite sheet
	reykTexture = NULL;
	lickTexture = LoadTexture("shit2.png");
	fireballTexture = LoadTexture("orangefire.png");
	tinyTexture = LoadTexture("xbosstiny.png");
	headTexture = LoadTexture("reykhead.png");

	frame = 0;
	startframe = 0;
	endframe = 0;
	starttime = 0;
	delay = 30;
	framecount = 0;
	poweruptype = 0;

	direction = 0;
	status = 3;
	jumpcounter = 0;
	
	reykWidth = 450;
	reykHeight = 450;

	reykX = 415.0 + index*50;
	reykY = 415.0;
	if (currentlevel == 1)
	{
		mapX = 415.0;
		mapY = 415.0;
		lockedbottom = true;
		lockedbottomamount = 415.0;
	}
	else if (currentlevel == 2)
	{
		mapX = 13000.0;
		mapY = 415.0;
		lockedbottom = false;
		lockedbottomamount = 1085.0;
	}
	else if (currentlevel == 3)
	{
		mapX = 11500.0;
		mapY = -5900.0;
		lockedbottom = false;
		lockedbottomamount = 1400.0 - 6700.0;
	}
	else if (currentlevel == 4)
	{
		mapX = 3000.0;
		mapY = -10500.0;
		lockedbottom = false;
		lockedbottomamount = -10415.0;
	}
	//mapX = 5000.0;
	mapY = 415.0;
	//mapY = 1100.0;

	jumpVel = -10.0;
	rotation = 0.0;
	velX = 0.0;
	velY = 0.0;
	
	slowed = false;
	slowcounter = 0;
	hp = 16;
	energy = 40;
	invincible = false;
	invinciblecounter = 0;
	grounded = false;
	trailing = true;

	lockedleft = false;
	lockedright = false;
	lockedtop = false;
	

	lockedleftamount = 0.0;
	lockedrightamount = 0.0;
	lockedtopamount = 0.0;
	//lockedbottomamount = 1400.0;
	

	licking = false;

	pressingleft = false;
	pressingright = false;

	fireballactive = false;
	fireballcounter = 0;
	fireballframe = 0;
	fireballvel = 0.0;
	fireballX = 0.0;
	fireballY = 0.0;
	fireballrotation = 0.0;

	groundedvelX = 0.0;
	groundedvelY = 0.0;
	reyk_mesh = new MyMesh();

	finallick = false;

	headX = 0.0;
	headY = 0.0;
	headvelX = 0.0;
	headvelY = 0.0;
	headrotation = 0.0;
	headrotationvel = 0.0;
	headframe = 0;

	deathcounter = 0;
	playerAssignedIndex = index;

	trailing = true;
	trailsize = 0;

	headtrailing = false;
	headtrailsize = 0;

	for (int i = 0; i < TRAILMAX; i++)
	{
		trailX[i] = 0.0;
		trailY[i] = 0.0;
		trailframe[i] = 0;

		headtrailX[i] = 0.0;
		headtrailY[i] = 0.0;
		headtrailrotation[i] = 0.0;
	}

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
}

/*========================================================================


========================================================================*/

void Reyk::Reyk_Animate()
{
	if ((int)GetTickCount() > starttime + delay)
	{
		starttime = GetTickCount();

		frame += 1;

		if ((status == 6 || status == 7) && frame == 19)
		{
			if (licking)
			{
				if (poweruptype == 1 || poweruptype == 3)
				{
					hp++;
					if (hp > 16)
					{
						hp = 16;
					}
				}
				else if (poweruptype == 2)
				{
					hp += 4;
					if (hp > 16)
					{
						hp = 16;
					}
				}
				else if (poweruptype == 5)
				{
					energy += 10;
					if (energy > 100)
					{
						energy = 100;
					}
				}
				else if (poweruptype == 4)
				{
					energy += 40;
					if (energy > 100)
					{
						energy = 100;
					}
				}
				else if (poweruptype == 6)
				{
					Reyk_GetInvincible();
				}
				else
				{
					hp--;
				}
			}
					
			licking = false;
			if (finallick)
			{
				finallick = false;
				loadCredits();
			}
			
		}
		if (frame > endframe)
		{
			if (status == 2)
			{
				status = 0;
				startframe = 0;
				endframe = 15;
				if (grounded)
					trailing = false;
			}
			else if (status == 4)
			{
				startframe = endframe;
			}
			else if (status == 6)
			{
				status = 0;
				startframe = 0;
				endframe = 15;
			}
			else if (status == 7)
			{
				status = 3;
				startframe = 0;
				endframe = 9;
			}
			else if (status == 10)
			{
				status = 0;
				startframe = 0;
				endframe = 15;
			}
			else if (status == 11)
			{
				status = 3;
				startframe = 0;
				endframe = 9;
			}
			
			frame = startframe;
		}
		if (frame < startframe)
		{
			frame = endframe;
		}

		if (fireballactive)
		{
			fireballframe += 1;
			if (fireballframe > 4)
			{
				fireballframe = 0;
			}
		}
	}
}

/*========================================================================


========================================================================*/

void Reyk::Reyk_Draw_Frame(int destx, int desty, int framenum, int framew, int frameh, int columns)
{
	D3DXVECTOR3 position( (float)destx, (float)desty, 0 );
	D3DCOLOR white = D3DCOLOR_XRGB(255,255,255);

	RECT rect;
 	rect.left = (framenum % columns) * framew;
	rect.top = (framenum / columns) * frameh;
	rect.right = rect.left + framew;
	rect.bottom = rect.top + frameh;

	spriteobj->Draw(reykTexture, &rect, NULL, &position, white);
}

/*========================================================================


========================================================================*/

void Reyk::Reyk_Transform_Draw()
{
	//float scaling = 1.0;
	//int columns = 12;
    //create a scale vector
    //D3DXVECTOR2 scale(scaling, scaling);

    //create a translate vector
    //D3DXVECTOR2 trans((int)reykX, (int)reykY);

    //set center by dividing width and height by two
    //D3DXVECTOR2 center((float)(reykWidth * scaling)/2, (float)(reykHeight * scaling)/2);

    //create 2D transformation matrix
    //D3DXMATRIX mat;
    //D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, &center, rotation, &trans);
    
    //tell sprite object to use the transform
    //spriteobj->SetTransform(&mat);
    //calculate frame location in source image
	curframe = 0;
	
	switch (status)
	{
	case 0:
		curframe = IDLE[frame];
		break;
	case 1:
		curframe = WALK[frame];
		break;
	case 2:
		curframe = ATTACK[frame];
		/*if (IS_SERVER && curframe == 17)
		{
			checkAxeCollision();
		}*/
		break;
	case 3:
		curframe = JUMP[frame];
		break;
	case 4:
		curframe = ATTACK[frame];
		/*if (IS_SERVER && curframe == 17)
		{
			checkAxeCollision();
		}*/
		break;
	case 5:
		curframe = 57;
		break;
	case 6:
	case 7:
		curframe = LICK[frame];
		break;
	case 8:
		curframe = 57;
		break;
	case 9:
		curframe = 58;
		break;
	case 10:
	case 11:
		curframe = BARFFIRE[frame];
		break;
	case 12:
		curframe = 59;
		break;
	}

	if (direction == 1)
	{
		curframe += DIRDIFF;
	}

    //int fx = (curframe % columns) * 450;
    //int fy = (curframe / columns) * 450;
    //RECT srcRect = {fx, fy, fx + 450, fy + 450};

	//D3DXMatrixIdentity( &mat );
	//spriteobj->SetTransform( &mat );

	if (finallick)
	{
		if (direction == 0)
		{
			Sprite_Transform_Draw(tinyTexture, reykX + lickrightX[frame-6], reykY + lickY[frame-6], 124, 100);
		}
		else
		{
			Sprite_Transform_Draw(tinyTexture, reykX + lickleftX[frame-6], reykY + lickY[frame-6], 124, 100);
		}
	}

	if (fireballactive)
	{
		float xmod = getXmod();
		float ymod = getYmod();
		Sprite_Transform_Draw(fireballTexture, fireballX - xmod, fireballY - ymod, 150, 180, fireballframe, 5, fireballrotation);
	}

	if (trailsize > 0 && !slowed)
	{
		for (int i = 0; i < trailsize; i++)
		{
				Sprite_Transform_Draw(reykTexture, trailX[i] - getXmod(), trailY[i] - getYmod(), 450, 450, trailframe[i], 12, 0.0f, 1.0f, 1.0f,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(TRAILMAX - i)/(float)TRAILMAX));
		}
	}

	D3DCOLOR color;

	if (slowed)
	{
		color = D3DCOLOR_XRGB(0,255,0);
	}
	else
	{
		color = D3DCOLOR_XRGB(255,255,255);
	}
	if (invincible)
	{
		if (invinciblecounter % 2 == 0)
		{
			Sprite_Transform_Draw(reykTexture, reykX, reykY, 450, 450, curframe, 12, 0.0f, 1.0f, 1.0f, color);
		}
	}
	else
	{
		Sprite_Transform_Draw(reykTexture, reykX, reykY, 450, 450, curframe, 12, 0.0f, 1.0f, 1.0f, color);
	}

	for (int i = 0; i < headtrailsize; i++)
		{
			Sprite_Transform_Draw(headTexture, headtrailX[i], headtrailY[i], 100, 100, headframe, 2, headtrailrotation[i], 1.0f, 1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(TRAILMAX - i)/(float)TRAILMAX));
		}

	if (status == 12)
	{
		Sprite_Transform_Draw(headTexture, headX, headY, 100, 100, headframe, 2, headrotation);
	}
}

void Reyk::Reyk_Draw3D()
{
	if (!licking) reyk_mesh->setMesh(NULL);

	if (direction == 0)
		{
			reyk_mesh->setPosition(screenToWorld(reykX + lickrightX[frame-6] + 50, reykY + lickY[frame-6] + 50, modelDepth));
		}
		else
		{
			reyk_mesh->setPosition(screenToWorld(reykX + lickleftX[frame-6] + 50, reykY + lickY[frame-6] + 50, modelDepth));
		}

	reyk_mesh->Draw();
}


/*========================================================================


========================================================================*/

void Reyk::Reyk_Move()
{
	setOldX();
	
	if (status == 9)
	{
		mapY += velY;

		if (lockedbottom)
		{
			reykY += velY;
			
		}
		if (velY < 12.0)
		{
			velY += 0.4;
			if (velY > 12.0)
			{
				velY = 12.0;
			}
		}
		return;
	}

	if (status == 3)
	{
		mapX += velX;

		if (lockedright || lockedleft)
		{
			reykX += velX;
			
		}
		if (velX < 0.0)
		{
			velX += 0.5;
			if (velX > 0.0)
			{
				velX = 0.0;
			}
		}
		else if (velX > 0.0)
		{
			velX -= 0.5;
			if (velX < 0.0)
			{
				velX = 0.0;
			}
		}
	}

	/*=======================================================
	
						GOING RIGHT

	========================================================*/
	if (direction == 0)
	{
		if (reykX >= 980.0)
		{
			return;
		}
		if (status == 5)
		{
			if (!ph->checkAllPlatformsLeftCollision(this) && reykX + velX > -180.0)
			{
				if (lockedleft || lockedright)
				{
					mapX += velX;
					reykX += velX;
					
				}
				else
				{
					mapX += velX;
				}
				
			}
			//reykY += velY;
			mapY += velY;
			
			if (lockedbottom)
			{
				reykY += velY;
				
			}
			velY += 1.0;
		}
		if (ph->checkAllPlatformsRightCollision(this))
		{
			return;
		}
		
		if (status != 5)
		{
			if (lockedright || lockedleft)
			{
				if (!slowed)
				{
					reykX += REYK_MOVE_SPEED;
					mapX += REYK_MOVE_SPEED;
				}
				else
				{
					reykX += REYK_MOVE_SPEED/2;
					mapX += REYK_MOVE_SPEED/2;
				}
				
			}
			else
			{
				if (!slowed)
				{
					mapX += REYK_MOVE_SPEED;
				}
				else
				{
					mapX += REYK_MOVE_SPEED/2;
				}
				
			}
		}

		if (IS_SERVER && mapX >= 11100.0 && gamestatus != 0 && currentlevel == 1)
		{
			loadBoss1();
			gamestatus = 0;

			SwitchSong(Song, "boss1.mp3");

			aboss->startBossBattle();

			setlockedleft(true);
			setlockedright(true);
		}
		
		/*
		if (reykX >= 415.0 && lockedleft)
		{
			reykX = 415.0;
			lockedleft = false;
		}

		if (mapY >= -1070.0 && mapY <= 1240.0 && mapX >= 3940.0 && !lockedright)
		{
			lockedright = true;
		}
		
		if (gamestatus == 3 && mapX >= 830.0)
		{
			lockedright = true;
		}

		if (gamestatus == 4 && mapX >= 700.0)
		{
			lockedright = true;
		}
		*/
	}
	/*=======================================================
	
						GOING LEFT

	========================================================*/
	else
	{
		if (status == 5)
		{
			if (!ph->checkAllPlatformsRightCollision(this) && reykX + velX < 980.0)
			{
				if (lockedleft || lockedright)
				{
					reykX += velX;
					mapX += velX;
					
				}
				else
				{
					mapX += velX;
				}

			}
			//reykY += velY;
			mapY += velY;
			
			if (lockedbottom)
				{
					reykY += velY;
					
				}
			velY += 1.0;
		}
		if (ph->checkAllPlatformsLeftCollision(this) && currentlevel != 1)
		{
			if (pressingleft && status != 1 && status != 5 && velX <= 0.0)
			{
				status = 9;
				
				velX = 0.0;
				velY = 2.0;
				
			}
			return;
		}
		if (reykX <= -180.0)
		{
			return;
		}

		if (status != 5)
		{
			if (lockedleft || lockedright)
			{
				if (!slowed)
				{
					reykX -= REYK_MOVE_SPEED;
					mapX -= REYK_MOVE_SPEED;
				}
				else
				{
					reykX -= REYK_MOVE_SPEED/2;
					mapX -= REYK_MOVE_SPEED/2;
				}

			}
			else
			{
				if (!slowed)
				{
					mapX -= REYK_MOVE_SPEED;
				}
				else
				{
					mapX -= REYK_MOVE_SPEED/2;
				}

			}
		}

		/*
		if (mapX < 415.0)
		{
			lockedleft = true;
		}
		if (reykX <= 415.0 && lockedright)
		{
			reykX = 415.0;
			lockedright = false;
		}

		if (lockedright && reykX <= 415.0)
		{
			lockedright = false;
		}
		*/
	}

	setNewX();

	if (getXchange() < 0.0)
	{
		if (reykX >= 415.0 && lockedleft)
		{
			reykX = 415.0;
			lockedleft = false;

		}

		/*
		if (mapY >= -1070.0 && mapY <= 1240.0 && mapX >= 3940.0 && !lockedright)
		{
			lockedright = true;
		}
		*/

		if (mapX >= 5600.0 && !lockedright && currentlevel == 3)
		{
			//lockedright = true;
		}

		if (gamestatus == 3 && mapX >= 830.0 + 12300.0)
		{
			lockedright = true;
		}

		if (gamestatus == 4 && mapX >= 700.0)
		{
			//lockedright = true;
		}

		if (gamestatus == 5 && mapX >= 2830.0)
		{
			lockedright = true;
		}
	}
	else if (getXchange() > 0.0)
	{
		if (mapX < 12720.0 && gamestatus == 3)
		{
			lockedleft = true;
		}
		if (reykX <= 415.0 && lockedright && gamestatus != 0)
		{
			reykX = 415.0;
			lockedright = false;

		}

		if (mapX < 415.0 && currentlevel == 1)
		{
			lockedleft = true;
		}

		if (mapX <= 2330.0 && gamestatus == 5)
		{
			lockedleft = true;
		}
	}

	if (IS_SERVER && mapX <= 500.0 + 12300.0 && mapY <= -5000.0 && currentlevel == 2 && gamestatus != 3)
	{
		loadBoss2();
		gamestatus = 3;
		SwitchSong(Song, "boss2.mp3");
	}
}

/*========================================================================


========================================================================*/

void Reyk::Reyk_Jump()
{
	if (status == 5 || status == 2 || status == 12)
	{
		return;
	}

	groundedvelX = 0.0;
	groundedvelY = 0.0;

	if (status == 9)
	{
		if (direction == 0)
		{
			velX = -15.0;
		}
		else
		{
			velX = 15.0;
		}
	}
	endframe = 8;
	status = 3;

	velY = REYK_JUMP_VEL;
	grounded = false;
	trailing = true;
}

/*========================================================================


========================================================================*/

void Reyk::setTexture(LPDIRECT3DTEXTURE9 t)
{
	reykTexture = t;
}

/*========================================================================


========================================================================*/

int Reyk::getStatus()
{
	return status;
}

/*========================================================================


========================================================================*/

void Reyk::setStatus(int s)
{
	status = s;
}

/*========================================================================


========================================================================*/

int Reyk::getDirection()
{
	return direction;
}

/*========================================================================


========================================================================*/

void Reyk::setDirection(int d)
{
	direction = d;
}

/*========================================================================


========================================================================*/

float Reyk::getJumpVel()
{
	return jumpVel;
}

/*========================================================================


========================================================================*/

void Reyk::setJumpVel(float j)
{
	jumpVel = j;
}

/*========================================================================


========================================================================*/

void Reyk::setFrame(int f)
{
	frame = f;
}

/*========================================================================


========================================================================*/

float Reyk::getX()
{
	return reykX;
}

/*========================================================================


========================================================================*/

float Reyk::getY()
{
	return reykY;
}

/*========================================================================


========================================================================*/

float Reyk::getCenterX()
{
	return mapX + 212.0f;
}

/*========================================================================


========================================================================*/

float Reyk::getCenterY()
{
	return mapY + 270.0f;
}

/*========================================================================


========================================================================*/

void Reyk::checkAxeCollision()
{
	RECT p1a;
	if (direction == 0)
	{
		//p1a.left = reykX+96.0;
		p1a.left = mapX + 96.0;
	}
	else
	{
		//p1a.left = reykX+42.0;
		p1a.left = mapX + 42.0;	
	}
	p1a.right = p1a.left + 311.0;
	//p1a.top = reykY+12.0;
	p1a.top = mapY + 12.0;
	p1a.bottom = p1a.top + 129.0;

	RECT p1b;
	if (direction == 0)
	{
		//p1b.left = reykX+307.0;
		p1b.left = mapX + 307.0;
	}
	else
	{
		//p1b.left = reykX+6.0;
		p1b.left = mapX + 6.0;
	}
	p1b.right = p1b.left + 136.0;
	//p1b.top = reykY+141.0;
	p1b.top = mapY + 141.0;
	p1b.bottom = p1b.top + 181.0;

	eh->checkEnemyAxeCollision(p1a, p1b, this);

	if (gamestatus == 3)
	{
		for (int i = 0; i < 7; i++)
		{
			if (detectCollision(p1a, croc->geteyeHitbox(i)) || detectCollision(p1b, croc->geteyeHitbox(i)))
			{
				croc->eyegethit(i, this);
			}
		}
	}

	if (gamestatus == 4)
	{
		if (detectCollision(p1a, qbee->getHitbox()) || detectCollision(p1b, qbee->getHitbox()))
		{
			qbee->gethit();
		}
	}

	if (gamestatus == 5)
	{
		if ((xboss->getstatus() == 1) && 
			(detectCollision(p1a, xboss->getHitbox()) || detectCollision(p1b, xboss->getHitbox())))
		{
			int rand670 = rand()%670;
			int rand2 = rand()%2;
			int rand100 = rand()%100;
			xboss->gethit(rand670, rand2, rand100);
		}
		else if ((xboss->getstatus() == 3) &&
			(detectCollision(p1a, xboss->getFaceHitbox()) || detectCollision(p1b, xboss->getFaceHitbox())))
		{
			int rand670 = rand()%670;
			int rand2 = rand()%2;
			int rand100 = rand()%100;
			xboss->gethit(rand670, rand2, rand100);
		}
	}
	
	if (gamestatus == 0)
	{
		if (aboss->getstatus() != 6)
		{
			RECT nose = aboss->apenoseGetHitbox();

			if (detectCollision(p1a, nose) || detectCollision(p1b, nose))
			{
				aboss->apenoseHit();
			}
		}
		else
		{
			RECT face = aboss->apefaceGetHitbox();
			if (detectCollision(p1a, face) || detectCollision(p1b, face))
			{
				aboss->apefaceHit();
			}
		}
	}
}

/*========================================================================


========================================================================*/

void Reyk::Reyk_Attack()
{
	if (status == 5 || status == 9 || status == 12)
	{
		return;
	}
	frame = 0;

	endframe = 12;
	if (status == 3)
	{
		status = 4;
	}
	else
	{
		status = 2;
	}
	trailing = true;
}

/*========================================================================


========================================================================*/

void Reyk::Reyk_Lick()
{
	if (status == 5 || status == 6 || status == 7 || status == 12)
	{
		return;
	}
	frame = 0; 
	endframe = 24;
	if (status == 3)
	{
		status = 7;
		velX = 0.0;
	}
	else
	{
		status = 6;
	}
}

/*========================================================================


========================================================================*/

void Reyk::Reyk_Update()
{
	setOldX();

	bool r = ph->checkAllPlatformsRightCollision(this);
	bool l = ph->checkAllPlatformsLeftCollision(this);

	if (reykY >= 1050.0)
	{
		setSendPacket(false);
		Reyk_Die();
		setSendPacket(true);
	}
	if (reykX <= -180.0)
	{
		reykX = -180.0;
	}
	else if (reykX >= 980.0)
	{
		reykX = 980.0;
	}
	switch(status)
	{
	case 0:
		startframe = 0;
		endframe = 15;
		delay = 30;
		break;
	case 1:
		startframe = 0;
		endframe = 7;
		delay = 45;
		break;
	case 2:
		endframe = 12;
		delay = 30;
		if (IS_SERVER && ATTACK[frame] == 17)
		{
			checkAxeCollision();
		}
		break;
	case 3:
		groundedvelX = 0.0;
		groundedvelY = 0.0;
		startframe = 0;
		endframe = 8;
		delay = 15;
	case 4:
		//reykY += velY;
		mapY += velY;

		if (lockedbottom)
		{
			reykY += velY;
		}
		velY += GRAVITY;
		if (status == 4)
		{
			delay = 25;
			if (fallingforward)
			{
				setSendPacket(false);
				Reyk_Move();
				setSendPacket(true);
			}
		}
		if (IS_SERVER && ATTACK[frame] == 17 && status == 4)
		{
			checkAxeCollision();
		}
		break;
	case 5:
		startframe = 0;
		endframe = 0;
		break;
	case 6:
		delay = 18;
		if(IS_SERVER)
			checkLickCollision();
		break;
	case 7:
		delay = 20;
		//reykY += velY;
		mapY += velY;

		if (lockedbottom)
		{
			reykY += velY;
		}
		velY += GRAVITY;
		if (fallingforward)
		{
			setSendPacket(false);
			Reyk_Move();
			setSendPacket(true);
		}
		if(IS_SERVER)
			checkLickCollision();
		break;
	case 8:
		break;
	case 9:
		startframe = 0;
		endframe = 0;

		if (velY < 0.0)
		{
			velY = 0.0;
		}

		if ((direction == 0 && (!pressingright || !r))
			|| (direction == 1 && (!pressingleft || !l)))
		{
			status = 3;

			velY = 0.0;
		}
		else if (pressingright && r)
		{
			if (groundedvelX != 0.0)
			{
				if (lockedright || lockedleft)
				{
					reykX += groundedvelX;
				}
				mapX += groundedvelX;
			}
		}
		else if (pressingleft && l)
		{
			if (groundedvelX != 0.0)
			{
				if (lockedright || lockedleft)
				{
					reykX += groundedvelX;
				}
				mapX += groundedvelX;
			}
		}

		break;
	case 10:
		break;
	case 11:
		mapY += velY;

		if (lockedbottom)
		{
			reykY += velY;
		}
		velY += GRAVITY;
		if (fallingforward)
		{
			setSendPacket(false);
			Reyk_Move();
			setSendPacket(true);
		}
		break;
	case 12:
		deathcounter++;

		headX += headvelX;
		headY += headvelY;
		headvelY += 1.0;
		headrotation += headrotationvel;

		if (headY > 2000.0)
		{
			if (direction == 0)
			{
				headX = reykX + 152.0;
			}
			else
			{
				headX = reykX + 198.0;
			}
			headY = -200.0;
			headvelX = 0.0;
			headvelY = -10.0;
		}

		if (!ph->enemyCheckAllPlatformsCollision(getJumpHitbox()))
		{
			mapY += 10.0;

			if (lockedbottom)
			{
				reykY += 10.0;
			}
		}

		if (headvelX == 0.0 && headY >= reykY + 108.0)
		{
			hp = 16;
			status = 3;
			Reyk_GetInvincible();
			headtrailing = false;
			slowed = false;
		}
	}

	if (grounded)
	{
		if (groundedvelY != 0.0)
		{
			if (lockedtop || lockedbottom)
			{
				reykY += groundedvelY;
			}
			mapY += groundedvelY;
		}

		if (groundedvelX > 0.0)
		{
			if (!ph->checkAllPlatformsRightCollision(this) && reykX < 980.0)
			{
				if (lockedright || lockedleft)
				{
					reykX += groundedvelX;
				}
				mapX += groundedvelX;
			}
		}
		else
		{
			if (!ph->checkAllPlatformsLeftCollision(this) && reykX > -180.0)
			{
				if (lockedright || lockedleft)
				{
					reykX += groundedvelX;
				}
				mapX += groundedvelX;
			}
		}

		ph->checkAllPlatformsCollision(this);
	}

	if (slowed)
	{
		slowcounter++;
		delay = 55;
		if (slowcounter >= 800)
		{
			slowed = false;
		}
	}

	if (invincible)
	{
		invinciblecounter++;

		if (invinciblecounter >= 150)
		{
			invincible = false;
		}
	}

	if (fireballactive)
	{
		fireballX += fireballvel;

		if(IS_SERVER)
			eh->checkEnemyFireballCollision(getFireballHitbox(), this);

		if (gamestatus == 3)
		{
			for (int i = 0; i < 7; i++)
			{
				if (detectCollision(getFireballHitbox(), croc->geteyeHitbox(i)))
				{
					croc->eyegethit(i, this);
				}
			}
		}

		if (gamestatus == 4)
		{
			if (IS_SERVER && detectCollision(getFireballHitbox(), qbee->getHitbox()))
			{
				qbee->gethit();
			}
		}

		if (IS_SERVER && gamestatus == 5)
		{
			if (((xboss->getstatus() == 1) && (detectCollision(getFireballHitbox(), xboss->getHitbox())))
				|| ((xboss->getstatus() == 3) && (detectCollision(getFireballHitbox(), xboss->getFaceHitbox()))))
			{
				int rand670 = rand()%670;
				int rand2 = rand()%2;
				int rand100 = rand()%100;
				xboss->gethit(rand670, rand2, rand100);
			}
		}

		fireballcounter++;
		if (fireballcounter > 150)
		{
			fireballactive = false;
		}
	}

	if (mapY >= lockedbottomamount && !lockedbottom)
	{
		lockedbottom = true;
		float Ychange = mapY - oldmapY;
		reykY += Ychange;
		
	}

	if (mapY >= 0.0 && mapY < lockedbottomamount && lockedbottom && currentlevel == 2)
	{
		float Ychange = 415.0 - reykY;
		mapY += Ychange;

		reykY = 415.0;

		lockedbottom = false;
	}
	else if (mapY < lockedbottomamount && lockedbottom && (currentlevel == 3 || currentlevel == 4))
	{
		float Ychange = 415.0 - reykY;
		mapY += Ychange;

		reykY = 415.0;
		lockedbottom = false;
	}

	if (mapY <= -5300.0 && !lockedbottom && currentlevel == 2)
	{
		lockedbottomamount = -5300.0;
		lockedbottom = true;
	}

	setNewX();

	if (getXchange() < 0.0)
	{
		if (reykX >= 415.0 && lockedleft)
		{
			reykX = 415.0;

			lockedleft = false;
		}

		if (mapY >= -1070.0 && mapY <= 1240.0 && mapX >= 3940.0 && !lockedright)
		{
			//lockedright = true;
		}
		
		if (gamestatus == 3 && mapX >= 830.0)
		{
			//lockedright = true;
		}

		if (gamestatus == 4 && mapX >= 700.0)
		{
			//lockedright = true;
		}
	}
	else if (getXchange() > 0.0)
	{
		if (mapX < -980.0)
		{
			lockedleft = true;
		}
		if (reykX <= 415.0 && lockedright && gamestatus != 0)
		{
			reykX = 415.0;

			lockedright = false;
		}

	}

	if (IS_SERVER && currentlevel == 3 && mapX <= 5800.0 && mapY <= -10000.0 && gamestatus != 4)
	{
		SwitchSong(Song, "boss3.mp3");
		loadBoss3();
		gamestatus = 4;
	}

	if (currentlevel == 3 && mapY <= -10415.0)
	{
		lockedbottomamount = -10415.0;
	}

	if (gamestatus == 4 && mapX <= 4820.0 && !lockedleft)
	{
		lockedleft = true;
	}

	if (gamestatus == 4 && mapX >= 5120.0 && !lockedright)
	{
		lockedright = true;
	}

	if (IS_SERVER && currentlevel == 4 && mapX <= 2330.0 && gamestatus == 1)
	{
		SwitchSong(Song, "boss4.mp3");
		loadBoss4();
		gamestatus = 5;
	}

	if (!trailing)
	{
		if (trailsize > 0)
			trailsize--;
	}
	else
	{
		if (trailsize < TRAILMAX)
			trailsize++;
	}

	for (int i = TRAILMAX-1; i > 0; i--)
	{
		trailX[i] = trailX[i-1];
		trailY[i] = trailY[i-1];
		trailframe[i] = trailframe[i-1];
	}

	trailX[0] = mapX;
	trailY[0] = mapY;
	trailframe[0] = curframe;

	if (!headtrailing)
	{
		if (headtrailsize > 0)
			headtrailsize--;
	}
	else
	{
		if (headtrailsize < TRAILMAX)
			headtrailsize++;
	}

	for (int i = TRAILMAX-1; i > 0; i--)
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

RECT Reyk::getHitbox()
{
	RECT hb;
	if (direction == 0)
	{
		//hb.left = reykX + 172.0;
		hb.left = mapX + 172.0;
	}
	else
	{
		//hb.left = reykX + 202.0;
		hb.left = mapX + 202.0;
	}
	hb.right = hb.left + 75.0;

	//hb.top = reykY + 140.0;
	hb.top = mapY + 140.0;
	hb.bottom = hb.top + 140.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT Reyk::getFeetHitbox()
{
	RECT hb;
	if (direction == 0)
	{
		//hb.left = reykX + 172.0;
		hb.left = mapX + 172.0;
	}
	else
	{
		//hb.left = reykX + 202.0;
		hb.left = mapX + 202.0;
	}
	hb.right = hb.left + 75.0;
	//hb.top = reykY + 255.0;
	hb.top = mapY + 255.0;
	hb.bottom = hb.top + 20.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT Reyk::getFeetPoint()
{
	RECT hb;
	if (direction == 0)
	{
		hb.left = mapX + 208.0;
	}
	else
	{
		hb.left = mapX + 238.0;
	}
	hb.right = hb.left + 1.0;
	hb.top = mapY + 274.0;
	hb.bottom = hb.top + 10.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT Reyk::getJumpHitbox()
{
	RECT hb;
	if (direction == 0)
	{
		//hb.left = reykX + 172.0;
		//hb.right = hb.left + 76.0;
		hb.left = mapX + 172.0;
		hb.right = hb.left + 76.0;
	}
	else
	{
		//hb.left = reykX + 202.0;
		//hb.right = hb.left + 76.0;
		hb.left = mapX + 202.0;
		hb.right = hb.left + 76.0;
	}
	//hb.top = reykY + 250.0;
	//hb.bottom = hb.top + 20.0;
	hb.top = mapY + 250.0;
	hb.bottom = hb.top + 20.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT Reyk::getLeftHitbox()
{
	RECT hb;
	if (direction == 0)
	{
		hb.left = mapX + 151.0;
		hb.right = hb.left + 20.0;
	}
	else
	{
		hb.left = mapX + 181.0;
		hb.right = hb.left + 20.0;
	}
	hb.top = mapY + 120.0;
	hb.bottom = hb.top + 105.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT Reyk::getRightHitbox()
{
	RECT hb;
	if (direction == 0)
	{
		hb.left = mapX + 249.0;
		hb.right = hb.left + 20.0;
	}
	else
	{
		hb.left = mapX + 280.0;
		hb.right = hb.left + 20.0;
	}
	hb.top = mapY + 120.0;
	hb.bottom = hb.top + 105.0;

	return hb;
}

/*========================================================================


========================================================================*/

void Reyk::Reyk_GetHit(int dir)
{
	if (invincible || status == 12)
	{
		return;
	}
	
	groundedvelX = 0.0;
	groundedvelY = 0.0;
	
	licking = false;
	grounded = false;
	trailing = true;

	if (dir == 0)
	{
		direction = 1;
		frame = 117;
		velX = 8.0;
	}
	else
	{
		direction = 0;
		frame = 57;
		velX = -8.0;
	}

	velY = -15.0;
	status = 5;

	hp-=HURTAMOUNT;

	if (hp <= 0)
	{
		status = 12;
		headtrailing = true;
		if (direction == 0)
		{
			headX = reykX + 152.0;
			headY = reykY + 108.0;
			headframe = 0;
		}
		else
		{
			headX = reykX + 198.0;
			headY = reykY + 108.0;
			headframe = 1;
		}
		if (dir == 0)
		{
			headvelX = 6.0;
			headrotationvel = 0.4;
		}
		else
		{
			headvelX = -6.0;
			headrotationvel = -0.4;
		}
		headvelY = -30.0;
		deathcounter = 0;
	}
	else
	{
		Reyk_GetInvincible();
	}
}

/*========================================================================


========================================================================*/

void Reyk::Reyk_GetSlowed()
{
	slowed = true;
	slowcounter = 0;
}

/*========================================================================


========================================================================*/

void Reyk::Reyk_Land(float left, float right, float y)
{
	//reykY = y - 270.0;
	//mapY = y - 20.0;

	float oldmapY = mapY;
	mapY = y - 270.0;

	float mapchange = mapY - oldmapY;
	if (lockedbottom)
	{
		reykY = 415.0 + (mapY - lockedbottomamount);
	}

	velY = 0.0;
	velX = 0.0;
	platformleft = left;
	platformright = right;
	fallingforward = false;
	if (status == 7)
	{
		status = 6;
	}
	else if (status == 4)
	{
		status = 2;
	}
	else if (status == 11)
	{
		status = 10;
	}
	else
	{
		frame = 0;
		endframe = 15;
		status = 0;
	}
	grounded = true;
	trailing = false;
}

/*========================================================================


========================================================================*/

int Reyk::gethp()
{
	return hp;
}

/*========================================================================


========================================================================*/

void Reyk::Reyk_GetInvincible()
{
	invincible = true;
	invinciblecounter = 0;
}

/*========================================================================


========================================================================*/

float Reyk::getvelX()
{
	return velX;
}

/*========================================================================


========================================================================*/

float Reyk::getvelY()
{
	return velY;
}

/*========================================================================


========================================================================*/

void Reyk::setgrounded(bool g)
{
	grounded = g;
}

/*========================================================================


========================================================================*/

bool Reyk::getgrounded()
{
	return grounded;
}

/*========================================================================


========================================================================*/

void Reyk::setX(float x)
{
	reykX = x;
}

/*========================================================================


========================================================================*/

void Reyk::setNewX()
{
	//newX = reykX;
	newX = mapX;
}

/*========================================================================


========================================================================*/

void Reyk::setOldX()
{
	//oldX = reykX;
	oldX = mapX;
}

/*========================================================================


========================================================================*/

float Reyk::getXchange()
{
	return oldX - newX;
}

/*========================================================================


========================================================================*/

void Reyk::setNewY()
{
	//newY = reykY;
	newY = mapY;
}

/*========================================================================


========================================================================*/

void Reyk::setOldY()
{
	//oldY = reykY;
	oldY = mapY;
}

/*========================================================================


========================================================================*/

float Reyk::getYchange()
{
	return oldY - newY;
}

/*========================================================================


========================================================================*/

float Reyk::getplatformleft()
{
	return platformleft;
}

/*========================================================================


========================================================================*/

float Reyk::getplatformright()
{
	return platformright;
}

/*========================================================================


========================================================================*/

void Reyk::setplatformleft(float left)
{
	platformleft = left;
}

/*========================================================================


========================================================================*/

void Reyk::setplatformright(float right)
{
	platformright = right;
}

/*========================================================================


========================================================================*/

void Reyk::setMapX(float x)
{
	mapX = x;
}

/*========================================================================


========================================================================*/

float Reyk::getMapX()
{
	return mapX;
}

/*========================================================================


========================================================================*/

float Reyk::getMapY()
{
	return mapY;
}


/*========================================================================


========================================================================*/

void Reyk::setcollidingright(bool r)
{
	collidingright = r;
}

/*========================================================================


========================================================================*/

void Reyk::checkLickCollision()
{
	if (frame < 6 || frame > 15 || licking || finallick)
	{
		return;
	}
	RECT hba;
	switch(frame)
	{
	case 6:
		if (direction == 0)
		{
			hba.left = mapX + 226;
		}
		else
		{
			hba.left = mapX + 175;
		}
		hba.right = hba.left + 50;
		hba.top = mapY + 167;
		hba.bottom = hba.top + 50;
		break;
	case 7:
		if (direction == 0)
		{
			hba.left = mapX + 226;
		}
		else
		{
			hba.left = mapX + 135;
		}
		hba.right = hba.left + 88;
		hba.top = mapY + 140;
		hba.bottom = hba.top + 75;
		break;
	case 8:
		if (direction == 0)
		{
			hba.left = mapX + 280;
		}
		else
		{
			hba.left = mapX + 50;
		}
		hba.right = hba.left + 120;
		hba.top = mapY + 140;
		hba.bottom = hba.top + 95;
		break;
	case 9:
		if (direction == 0)
		{
			hba.left = mapX + 353;
		}
		else
		{
			hba.left = mapX + 4;
		}
		hba.right = hba.left + 93;
		hba.top = mapY + 181;
		hba.bottom = hba.top + 78;
		break;
	case 10:
		if (direction == 0)
		{
			hba.left = mapX + 360;
		}
		else
		{
			hba.left = mapX + 4;
		}
		hba.right = hba.left + 88;
		hba.top = mapY + 166;
		hba.bottom = hba.top + 89;
		break;
	case 11:
		if (direction == 0)
		{
			hba.left = mapX + 360;
		}
		else
		{
			hba.left = mapX + 4;
		}
		hba.right = hba.left + 79;
		hba.top = mapY + 151;
		hba.bottom = hba.top + 81;
		break;
	case 12:
		if (direction == 0)
		{
			hba.left = mapX + 351;
		}
		else
		{
			hba.left = mapX + 13;
		}
		hba.right = hba.left + 74;
		hba.top = mapY + 149;
		hba.bottom = hba.top + 61;
		break;
	case 13:
		if (direction == 0)
		{
			hba.left = mapX + 350;
		}
		else
		{
			hba.left = mapX + 25;
		}
		hba.right = hba.left + 71;
		hba.top = mapY + 142;
		hba.bottom = hba.top + 60;
		break;
	case 14:
		if (direction == 0)
		{
			hba.left = mapX + 350;
		}
		else
		{
			hba.left = mapX + 30;
		}
		hba.right = hba.left + 71;
		hba.top = mapY + 127;
		hba.bottom = hba.top + 65;
		break;
	case 15:
		if (direction == 0)
		{
			hba.left = mapX + 280;
		}
		else
		{
			hba.left = mapX + 50;
		}
		hba.right = hba.left + 120;
		hba.top = mapY + 140;
		hba.bottom = hba.top + 95;
		break;
	}

	if (eh->checkLickCollision(hba, this))
	{
	}
	if (frame >= 8 && frame <= 14)
	{
		RECT hbb;
	
		if (direction == 0)
		{
			hbb.left = mapX + 230;
		}
		else
		{
			hbb.left = mapX + 63;
		}
		hbb.right = hbb.left + 160;
		hbb.top = mapY + 155;
		hbb.bottom = hbb.top + 23;
		
		if (eh->checkLickCollision(hbb, this))
		{
		}

		if (gamestatus == 5)
		{
			if (detectCollision(hbb, xboss->getTinyHitbox()) && xboss->getstatus() == 7)
			{
				finallick = true;
				xboss->setstatus(8);
			}
		}
	}

	if (gamestatus == 5)
	{
		if (detectCollision(hba, xboss->getTinyHitbox()) && xboss->getstatus() == 7)
		{
			finallick = true;
			xboss->setstatus(8);
		}
	}
}

/*========================================================================


========================================================================*/

void Reyk::setLick(MODEL* model, float depth, int type)
{
	reyk_mesh->setMesh(model);
	modelDepth = depth;
	licking = true;
	poweruptype = type;
}

/*========================================================================


========================================================================*/

void Reyk::setfallingforward(bool b)
{
	fallingforward = b;
}

/*========================================================================


========================================================================*/

void Reyk::setY(float y)
{
	reykY = y;
}

/*========================================================================


========================================================================*/

void Reyk::setMapY(float y)
{
	mapY = y;
}


/*========================================================================


========================================================================*/

void Reyk::Reyk_Die()
{
	slowed = false;
	reykY = 0.0;
	if (lockedbottom)
	{
		mapY = lockedbottomamount - 200.0;
		reykY = 215.0;

	}

	velY = 0.0;
	status = 3;
	hp--;

	if (direction == 0)
		direction = 1;
	else
		direction = 0;

	headtrailsize = 0;

	Reyk_GetInvincible();
}

/*========================================================================


========================================================================*/

void Reyk::setpressingleft(bool l)
{
	pressingleft = l;
}

/*========================================================================


========================================================================*/

void Reyk::setpressingright(bool r)
{
	pressingright = r;
}

/*========================================================================


========================================================================*/

void Reyk::Reyk_BarfFireball()
{
	if (fireballactive || status == 5 || status == 12 || energy < 20)
	{
		return;
	}

	energy -= 20;

	startframe = 0;
	endframe = 12;
	if (status == 3)
	{
		status = 11;
	}
	else
	{
		status = 10;
	}

	frame = 0;

	fireballactive = true;

	fireballcounter = 0;
	fireballframe = 0;

	if (direction == 0)
	{
		fireballvel = 8.0;
		fireballX = mapX + 175.0;
		fireballrotation = -1.575;
		
	}
	else
	{
		fireballvel = -8.0;
		fireballX = mapX + 128.0;
		fireballrotation = 1.575;
	}

	fireballY = mapY + 105.0;
}


/*========================================================================


========================================================================*/

RECT Reyk::getFireballHitbox()
{
	RECT hb;

	hb.left = fireballX;
	hb.right = fireballX + 180.0;
	hb.top = fireballY;
	hb.bottom = fireballY + 150.;

	return hb;
}

/*========================================================================


========================================================================*/

float Reyk::getXmod()
{
	float xoffset = 0.0;
	if (lockedleft || lockedright)
	{
		xoffset = reykX - 415.0;
	}
	return (mapX - 415.0) - xoffset;
}

/*========================================================================


========================================================================*/

float Reyk::getYmod()
{
	float yoffset = 0.0;
	if (lockedbottom)
	{
		yoffset = mapY - lockedbottomamount;
	}
	return (mapY - 415.0) - yoffset;
}

/*========================================================================


========================================================================*/

bool Reyk::getlockedleft()
{
	return lockedleft;
}

/*========================================================================


========================================================================*/

bool Reyk::getlockedbottom()
{
	return lockedbottom;
}

/*========================================================================


========================================================================*/

bool Reyk::getlockedright()
{
	return lockedright;
}

/*========================================================================


========================================================================*/

void Reyk::setoldmapY()
{
	oldmapY = mapY;
}

/*========================================================================


========================================================================*/

void Reyk::setgroundedvelX(float x)
{
	groundedvelX = x;
}

/*========================================================================


========================================================================*/

void Reyk::setgroundedvelY(float y)
{
	groundedvelY = y;
}


/*========================================================================


========================================================================*/

bool Reyk::getpressingleft()
{
	return pressingleft;
}

/*========================================================================


========================================================================*/

bool Reyk::getpressingright()
{
	return pressingright;
}

/*========================================================================


========================================================================*/

void Reyk::Reyk_StartSliding(float velX, float velY)
{
	
	if (currentlevel == 1)
	{
		return;
	}
	groundedvelX = velX;
	groundedvelY = velY;
	status = 9;

	velY = 2.0;
	velX = 0.0;
}

/*========================================================================


========================================================================*/

void Reyk::setlockedbottomamount(float lb)
{
	lockedbottomamount = lb;
}

/*========================================================================


========================================================================*/

void Reyk::setlockedbottom(bool lb)
{
	lockedbottom = lb;
}

/*========================================================================


========================================================================*/

float Reyk::getlockedbottomamount()
{
	return lockedbottomamount;
}

/*========================================================================


========================================================================*/

void Reyk::setlockedleft(bool l)
{
	lockedleft = l;
}

/*========================================================================


========================================================================*/

void Reyk::setlockedright(bool r)
{
	lockedright = r;
}

int Reyk::getAssignedIndex(){
	return playerAssignedIndex;
}

void Reyk::setSendPacket(bool b){
	sendPacket = b;
}

int Reyk::getenergy()
{
	return energy;
}

/*========================================================================


========================================================================*/

bool Reyk::getinvincible()
{
	return invincible;
}

void Reyk::setFinalLick(bool lickmyass)
{
	finallick = lickmyass;
}