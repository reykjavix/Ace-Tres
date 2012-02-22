#include "MyDirectX.h"
#include "Reyk.h"
#include "Enemy.h"

extern Reyk* players[];
bool detectCollision(RECT r1, RECT r2);

const int DECAWALK[16] = {0, 1, 2, 3, 4, 3, 2, 1, 0, 5, 6, 7, 8, 7, 6, 5};
const float DECAHEADY[16] = {-117.0, -124.0, -130.0, -137.0, -145.0, -137.0, -130.0, -124.0,
							-117.0, -124.0, -130.0, -137.0, -145.0, -137.0, -130.0, -124.0};

extern LPDIRECT3DTEXTURE9 decapedeTexture;
extern LPDIRECT3DTEXTURE9 decapedeRTexture;
extern LPDIRECT3DTEXTURE9 decapedeheadTexture;
extern LPDIRECT3DTEXTURE9 decapedeheadRTexture;
extern LPDIRECT3DTEXTURE9 decapedeeyeTexture;
extern LPDIRECT3DTEXTURE9 decapedeeyeRTexture;

extern CSound *decapedehit;

/*========================================================================


========================================================================*/

Decapede::Decapede(float x, float y, float l, float r, int d)
{
	droprate = 90;

	decapedeTexture = LoadTexture("decapede.png");
	decapedeRTexture = LoadTexture("decapedeR.png");
	decapedeheadTexture = LoadTexture("decapedehead.png");
	decapedeheadRTexture = LoadTexture("decapedeheadR.png");
	decapedeeyeTexture = LoadTexture("decapedeeye.png");
	decapedeeyeRTexture = LoadTexture("decapedeeyeR.png");

	maxhp = 8;
	hp = maxhp;
	status = 1;
	movespeed = 0.5;
	direction = d;
	headshake = 0;
	headshaking = false;
	decapedeframecount = 0;

	decapedeX = x;
	decapedeY = y;
	decapedeheadX = 0.0;
	decapedeheadY = 0.0;
	decapedeheadvelX = 0.0;
	decapedeheadvelY = 0.0;
	decapedeheadrotation = 0.0;
	for (int i = 0; i < 4; i++)
	{
		decaeyeX[i] = 0.0;
		decaeyeY[i] = 0.0;
		decaeyevelX[i] = 0.0;
		decaeyevelY[i] = 0.0;
		decaeyeactive[i] = false;
		decaeyerotation[i] = 0.0;
	}

	frame = 0;
	headframe = 0;
	startframe = 0;
	endframe = 15;
	delay = 80;

	leftrange = l;
	rightrange = r;

	/* initialize particle system */
	for (int i = 0; i < 4; i++){
		g_pParticleSystem[i] = new CParticleSystem();
		g_pParticleSystem[i]->SetTexture( "particle.bmp" );
		g_pParticleSystem[i]->SetMaxParticles( 500 );
		g_pParticleSystem[i]->SetNumToRelease( 20 );
		g_pParticleSystem[i]->SetReleaseInterval( 0.05f );
		g_pParticleSystem[i]->SetLifeCycle( 1.0f );
		g_pParticleSystem[i]->SetSize( 0.1f );
		g_pParticleSystem[i]->SetColor( D3DXCOLOR( 0.8f, 0.0f, 0.8f, 0.8f ));
		g_pParticleSystem[i]->SetPosition( D3DXVECTOR3( 0.0, 0.0, 10.0f) );
		g_pParticleSystem[i]->SetVelocity( D3DXVECTOR3( 0.0f, 1.3f, 0.0f) );
		g_pParticleSystem[i]->SetGravity( D3DXVECTOR3( 0.0f, -1.0f, 0.0f) );
		g_pParticleSystem[i]->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
		g_pParticleSystem[i]->SetVelocityVar( 0.4f );

		g_pParticleSystem[i]->Init( d3ddev );
	}

	g_pParticleSystem[4] = new CParticleSystem();
	g_pParticleSystem[4]->SetTexture( "particle.bmp" );
	g_pParticleSystem[4]->SetMaxParticles( 500 );
	g_pParticleSystem[4]->SetNumToRelease( 40 );
	g_pParticleSystem[4]->SetReleaseInterval( 0.05f );
	g_pParticleSystem[4]->SetLifeCycle( 2.0f );
	g_pParticleSystem[4]->SetSize( 0.2f );
	g_pParticleSystem[4]->SetColor( D3DXCOLOR( 0.8f, 0.0f, 0.8f, 0.8f ));
	g_pParticleSystem[4]->SetPosition( D3DXVECTOR3( 0.0, 0.0, 10.0f) );
	g_pParticleSystem[4]->SetVelocity( D3DXVECTOR3( 0.0f, 1.3f, 0.0f) );
	g_pParticleSystem[4]->SetGravity( D3DXVECTOR3( 0.0f, -1.0f, 0.0f) );
	g_pParticleSystem[4]->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	g_pParticleSystem[4]->SetVelocityVar( 0.8f );

	g_pParticleSystem[4]->Init( d3ddev );

	decapedepowerup = new Powerup(2+(rand()%5));

	headtrailing = false;
	headtrailsize = 0;

	for (int i = 0; i < ENEMYTRAILMAX; i++)
	{
		headtrailX[i] = 0.0;
		headtrailY[i] = 0.0;
		headtrailrotation[i] = 0.0;

		for (int j = 0; j < 4; j++)
		{
			eyetrailsize[j] = 0;
			eyetrailX[j][i] = 0.0;
			eyetrailY[j][i] = 0.0;
			eyetrailrotation[j][i] = 0.0;
		}
	}
}

/*========================================================================


========================================================================*/

void Decapede::animate()
{
	if ((int)GetTickCount() > starttime + delay)
	{
		starttime = GetTickCount();

		if (status == 0)
		{
			return;
		}
		else if (status == 2)
		{
			frame = 0;
			return;
		}
		frame += 1;

		if (frame > endframe)
		{
			frame = startframe;
		}
		if (frame < startframe) frame = endframe;
	}
}

/*========================================================================


========================================================================*/

void Decapede::update()
{
	int numPlayersOutOfBounds = 0;
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if ((abs((int)(decapedeX - players[i]->getCenterX())) > 1280.0
			|| abs((int)(decapedeY - players[i]->getCenterY())) > 1024.0)
			&& status <= 1)
		{
			numPlayersOutOfBounds++;
		}
	}

	if(numPlayersOutOfBounds == MAX_CLIENTS)
		return;

	decapedeframecount++;

	if(IS_SERVER)
		for(int i = 0; i < MAX_CLIENTS; i++)
			if ( status != 2 && status != 0 && detectCollision(getbodyHitbox(), players[i]->getHitbox()))
			{
				if (decapedeX + 400.0 > players[i]->getCenterX())
				{
					players[i]->Reyk_GetHit(1);
				}
				else
				{
					players[i]->Reyk_GetHit(0);
				}
			}

	switch(status)
	{
	case 0:
		break;
	case 1:
		if (direction == 0)
		{
			decapedeX += movespeed;
			if (decapedeX >= rightrange)
			{
				direction = 1;
			}
		}
		else
		{
			decapedeX -= movespeed;
			if (decapedeX <= leftrange)
			{
				direction = 0;
			}
		}
		break;
	case 2:
		decapedeheadX += decapedeheadvelX;
		decapedeheadY += decapedeheadvelY;
		decapedeheadvelY += 1.0;
		decapedeheadrotation += 0.4;

		decapedepowerup->setPosition(powerupX, powerupY);
		powerupY += powerupvelY;
		powerupvelY += 0.4;

		if (decapedeframecount >= 200)
		{
			status = 0;
		}
		break;
	}

	if (status != 2)
	{
		float offset = 263.0;
		if (direction == 1)
		{
			offset = 144.0;
		}
		if (headshaking)
		{		
			decapedeheadX = (float)((decapedeX + offset) + 30.0 * (1.0 - ((double)headshake/25.0)) * sin((double)decapedeframecount));
			headshake++;
			if (headshake >= 30)
			{
				headshake = 0;
				decapedeheadX = decapedeX + offset;
				headshaking = false;
			}
			decapedeheadY = decapedeY + DECAHEADY[frame];
		}
		else
		{
			decapedeheadX = decapedeX + offset;
			decapedeheadY = decapedeY + DECAHEADY[frame];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (decaeyeactive[i])
		{
			decaeyeX[i] += decaeyevelX[i];
			decaeyeY[i] += decaeyevelY[i];
			decaeyevelY[i] += 0.8;
			decaeyerotation[i] += 0.4;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		g_pParticleSystem[i]->SetPosition(particlePositions[i]);
		g_pParticleSystem[i]->Update(0.1);
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

	headtrailX[0] = decapedeheadX;
	headtrailY[0] = decapedeheadY;
	headtrailrotation[0] = decapedeheadrotation;

	for (int j = 0; j < 4; j++)
	{
		if (decaeyeactive[j] == true && eyetrailsize[j] < ENEMYTRAILMAX)
			eyetrailsize[j]++;
	}

	for (int i = ENEMYTRAILMAX-1; i > 0; i--)
	{
		for (int j = 0; j < 4; j++)
		{
			eyetrailX[j][i] = eyetrailX[j][i-1];
			eyetrailY[j][i] = eyetrailY[j][i-1];
		}
	}

	for (int j = 0; j < 4; j++)
	{
		eyetrailX[j][0] = decaeyeX[j];
		eyetrailY[j][0] = decaeyeY[j];
		eyetrailrotation[j][0] = decaeyerotation[j];
	}

}

/*========================================================================


========================================================================*/

void Decapede::draw()
{

	if (status == 0)
	{
		return;
	}

	Reyk* player1 = players[clientIndex];

	if (abs((int)(decapedeX + 400 - player1->getCenterX())) > 1600
			|| abs((int)(decapedeY + 230 - player1->getCenterY())) > 1500)
		{
			return;
		}
	LPDIRECT3DTEXTURE9 body = decapedeTexture;
	LPDIRECT3DTEXTURE9 head = decapedeheadTexture;
	LPDIRECT3DTEXTURE9 eye = decapedeeyeTexture;
	if (direction == 1)
	{
		body = decapedeRTexture;
		head = decapedeheadRTexture;
		eye = decapedeeyeRTexture;
	}
	int curframe = DECAWALK[frame];

	//float xmod = player1->getMapX() - 415.0;
	//float ymod = player1->getMapY() - 415.0;

	float xmod = player1->getXmod();
	float ymod = player1->getYmod();

	if (status == 2)
	{
		if (decapedeframecount < 100)
		{
			Sprite_Transform_Draw(body, decapedeX - xmod, decapedeY - ymod, 800, 460, curframe, 9);
		}
		else if (decapedeframecount < 150)
		{
			if (decapedeframecount % 2 == 0)
			{
				Sprite_Transform_Draw(body, decapedeX - xmod, decapedeY - ymod, 800, 460, curframe, 9);
			}
		}
		else
		{
			if (decapedeframecount % 3 == 0)
			{
				Sprite_Transform_Draw(body, decapedeX - xmod, decapedeY - ymod, 800, 460, curframe, 9);
			}
		}
	}
	else if (status != 0)
	{
		Sprite_Transform_Draw(body, decapedeX - xmod, decapedeY - ymod, 800, 460, curframe, 9);
	}

	for (int i = 0; i < headtrailsize; i++)
	{
		Sprite_Transform_Draw(head, headtrailX[i] - xmod, headtrailY[i] - ymod, 400, 400, headframe, 5, headtrailrotation[i], 1.0F, 1.0F,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(ENEMYTRAILMAX - i)/(float)ENEMYTRAILMAX));
	}

	Sprite_Transform_Draw(head, decapedeheadX - xmod, decapedeheadY - ymod, 400, 400, headframe, 5, decapedeheadrotation);

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < eyetrailsize[j]; i++)
		{
			Sprite_Transform_Draw(eye, eyetrailX[j][i] - xmod, eyetrailY[j][i] - ymod, 120, 120, 0, 1, eyetrailrotation[j][i], 1.0F, 1.0F,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(ENEMYTRAILMAX - i)/(float)ENEMYTRAILMAX));
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (decaeyeactive[i])
		{
			Sprite_Transform_Draw(eye, decaeyeX[i] - xmod, decaeyeY[i] - ymod, 120, 120, 0, 1, decaeyerotation[i]);
		}
	}
}

/*========================================================================


========================================================================*/

void Decapede::drawParticle() {

	Reyk* player1 = players[clientIndex];

	for(int i = 0; i < 4; i++) {
		if (status == 1 && decaeyeactive[i])
			g_pParticleSystem[i]->SetMaxParticles(500);
		else g_pParticleSystem[i]->SetMaxParticles(0);
	}

	if (direction == 0) {
		g_pParticleSystem[1]->SetVelocity( D3DXVECTOR3( 1.0f, 0.0f, 0.0f) );
		g_pParticleSystem[3]->SetVelocity( D3DXVECTOR3( -1.0f, 0.0f, 0.0f) );
	}
	else {
		g_pParticleSystem[1]->SetVelocity( D3DXVECTOR3( -1.0f, 0.0f, 0.0f) );
		g_pParticleSystem[3]->SetVelocity( D3DXVECTOR3( 1.0f, 0.0f, 0.0f) );
	}

	if (status == 2)
		g_pParticleSystem[4]->SetMaxParticles(500);
	else g_pParticleSystem[4]->SetMaxParticles(0);

	if(direction == 0) {
		particlePositions[0] = screenToWorld(decapedeX+430 - player1->getXmod(), decapedeY-30 - player1->getYmod(), 5.0f);
		particlePositions[1] = screenToWorld(decapedeX+590 - player1->getXmod(), decapedeY+25 - player1->getYmod(), 5.0f);
		particlePositions[2] = screenToWorld(decapedeX+520 - player1->getXmod(), decapedeY-50 - player1->getYmod(), 5.0f);
		particlePositions[3] = screenToWorld(decapedeX+375 - player1->getXmod(), decapedeY+40 - player1->getYmod(), 5.0f);
		particlePositions[4] = screenToWorld(decapedeX+475 - player1->getXmod(), decapedeY+100 - player1->getYmod(), 5.0f);

	}

	else {
		particlePositions[0] = screenToWorld(decapedeX+380 - player1->getXmod(), decapedeY-30 - player1->getYmod(), 5.0f);
		particlePositions[1] = screenToWorld(decapedeX+220 - player1->getXmod(), decapedeY+25 - player1->getYmod(), 5.0f);
		particlePositions[2] = screenToWorld(decapedeX+290 - player1->getXmod(), decapedeY-50 - player1->getYmod(), 5.0f);
		particlePositions[3] = screenToWorld(decapedeX+425 - player1->getXmod(), decapedeY+40 - player1->getYmod(), 5.0f);
		particlePositions[4] = screenToWorld(decapedeX+325 - player1->getXmod(), decapedeY+100 - player1->getYmod(), 5.0f);
	}

	for (int i = 0; i < 5; i++) {
		d3ddev->SetTexture( 0, g_pParticleSystem[i]->GetTextureObject() );
		g_pParticleSystem[i]->Render( d3ddev );
	}
}

/*========================================================================


========================================================================*/

void Decapede::move(float x, float y)
{
	decapedeX += x;
	decapedeY += y;

	decapedeheadX += x;
	decapedeheadY += y;

	for (int i = 0; i < 4; i++)
	{
		if (decaeyeactive[i])
		{
			decaeyeX[i] += x;
			decaeyeY[i] += y;
		}
	}
}

/*========================================================================


========================================================================*/

void Decapede::gethit(Reyk* player1)
{
	if (headshaking)
	{
		return;
	}

	if(!IS_SERVER) PlaySound(decapedehit);

	headshaking = true;
	headshake = 0;
	hp--;
	if (hp == (maxhp*4)/5 && !decaeyeactive[0])
	{
		headframe++;
		if (direction == 0)
		{
			decaeyeX[0] = decapedeheadX + 43.0;
		}
		else
		{
			decaeyeX[0] = decapedeheadX + 303.0;
		}
		decaeyeY[0] = decapedeheadY + 12.0;
		if (decapedeX + 400.0 > player1->getCenterX())
		{
			decaeyevelX[0] = 6.0;
		}
		else
		{
			decaeyevelX[0] = -6.0;
		}
		decaeyevelY[0] = -20.0;
		decaeyeactive[0] = true;
		movespeed *= 1.5;
		delay = 70;
	}
	else if (hp == (maxhp*3)/5 && !decaeyeactive[1])
	{
		headframe++;
		if (direction == 0)
		{
			decaeyeX[1] = decapedeheadX + 290.0;
		}
		else
		{
			decaeyeX[1] = decapedeheadX;
		}
		decaeyeY[1] = decapedeheadY + 138.0;
		if (decapedeX + 400.0 > player1->getCenterX())
		{
			decaeyevelX[1] = 6.0;
		}
		else
		{
			decaeyevelX[1] = -6.0;
		}
		decaeyevelY[1] = -20.0;
		decaeyeactive[1] = true;
		movespeed *= 1.5;
		delay = 60;
	}
	else if (hp == (maxhp*2)/5 && !decaeyeactive[2])
	{
		headframe++;
		if (direction == 0)
		{
			decaeyeX[2] = decapedeheadX + 272.0;
		}
		else
		{
			decaeyeX[2] = decapedeheadX + 20.0;
		}
		decaeyeY[2] = decapedeheadY + 7.0;
		if (decapedeX + 400.0 > player1->getCenterX())
		{
			decaeyevelX[2] = 6.0;
		}
		else
		{
			decaeyevelX[2] = -6.0;
		}
		decaeyevelY[2] = -20.0;
		decaeyeactive[2] = true;
		movespeed *= 1.5;
		delay = 50;
	}
	else if (hp == maxhp/5 && !decaeyeactive[3])
	{
		headframe++;
		if (direction == 0)
		{
			decaeyeX[3] = decapedeheadX + 17.0;
		}
		else
		{
			decaeyeX[3] = decapedeheadX + 277.0;
		}
		decaeyeY[3] = decapedeheadY + 138.0;
		if (decapedeX + 400.0 > player1->getCenterX())
		{
			decaeyevelX[3] = 6.0;
		}
		else
		{
			decaeyevelX[3] = -6.0;
		}
		decaeyevelY[3] = -20.0;
		decaeyeactive[3] = true;
		movespeed *= 1.5;
		delay = 40;
	}
	else if (hp <= 0)
	{
		headtrailing = true;
		status = 2;
		decapedeframecount = 0;
		if (decapedeX + 400.0 > player1->getCenterX())
		{
			decapedeheadvelX = 10.0;
		}
		else
		{
			decapedeheadvelX = -10.0;
		}
		decapedeheadvelY = -35.0;

		if (direction == 0)
		{
			powerupX = decapedeX + 600.0;
		}
		else
		{
			powerupX = decapedeX + 300.0;
		}

		if (rand()%100 < droprate)
		{
			powerupY = decapedeY + 170.0;
			powerupvelY = -22.0;

			decapedepowerup->setStatus(1);
			decapedepowerup->setPosition(powerupX, powerupY);
		}
	}
}

/*========================================================================


========================================================================*/

int Decapede::getstatus()
{
	return status;
}

/*========================================================================


========================================================================*/

RECT Decapede::getHitbox()
{
	RECT hb;

	hb.left = decapedeheadX;
	hb.right = hb.left + 400.0;
	hb.top = decapedeheadY;
	hb.bottom = hb.top + 250.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT Decapede::getbodyHitbox()
{
	RECT hb;
	if (direction == 0)
	{
		hb.left = decapedeX + 264.0;
	}
	else
	{
		hb.left = decapedeX + 127.0;
	}
	hb.right = hb.left + 407.0;
	hb.top = decapedeY;
	hb.bottom = hb.top + 391.0;

	return hb;	
}


void Decapede::drawPowerup()
{
	decapedepowerup->draw();
}

RECT Decapede::getPowerupHitbox()
{
	return decapedepowerup->getHitbox();
}

float Decapede::getPowerupDepth()
{
	return decapedepowerup->getDepth();
}

MODEL* Decapede::getPowerupModel()
{
	return decapedepowerup->getModel();
}

int Decapede::getPowerupStatus()
{
	return decapedepowerup->getStatus();
}

int Decapede::getPowerupType()
{
	return decapedepowerup->getType();
}

void Decapede::eatPowerup()
{
	decapedepowerup->setStatus(2);
}