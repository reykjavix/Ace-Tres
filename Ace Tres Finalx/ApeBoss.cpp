#include "MyDirectX.h"
#include "ApeBoss.h"
#include "SmallApe.h"
#include "Reyk.h"
#include "EnemyHandler.h"

#include "SongPlayer.h"
#include "fmod.h"
#include "fmod_errors.h"

extern Reyk* players[];

extern EnemyHandler *eh; 

extern float sunY;
extern int gamestatus;
extern int currentlevel;

bool detectCollision(RECT r1, RECT r2);

extern CSound *apebosshit;
extern CSound *apebossfinalhit;
extern CSound *handslide;
extern CSound *snotrocket;
extern CSound *apefist;

extern FSOUND_STREAM * Song;
extern void SwitchSong(FSOUND_STREAM * & SongData, char *p_FileName);

void loadLevel2();

/*========================================================================


========================================================================*/

ApeBoss::ApeBoss()
{
	apefaceTexture = LoadTexture("apeface.png");
	apehandleftTexture = LoadTexture("apehandleft.png");
	apehandrightTexture = LoadTexture("apehandright.png");
	apenoseTexture = LoadTexture("apenose.png");
	boogerTexture = LoadTexture("booger.png");
	apebodyTexture = LoadTexture("apebossbody.png");

	apefaceframe = 0;
	apefaceshake = 0;
	apehp = 50;
	apefacestatus = 0;
	apefaceX = 240.0;
	apefaceY = -120.0;
	apefacevel = 0.0;
	apeframecount = 0;
	apecount = 0;
	apecountmax = 100;
	apedurationmod = 1.0;
	apespeedmod = 1.0;
	apefacevelX = 0.0;
	apefacevelY = 0.0;
	apefacerotation = 0.0;
	apehandleftrotation = 0.0;
	apehandrightrotation = 0.0;

	apehandleftframe = 0;
	apehandleftstatus = 0;
	apehandleftshake = 0;
	apehandleftnumstomps = 0;
	apehandleftwhichattack = 0;
	apehandleftsavedX = 0.0;

	apebodyX = 0.0;
	apebodyY = -90.0;

	apehandleftX = -65.0;
	apehandleftY = 250.0;
	apehandleftvelX = 0.0;
	apehandleftvelY = 0.0;

	apehandrightframe = 0;
	apehandrightstatus = 0;
	apehandrightshake = 0;
	apehandrightnumstomps = 0;
	apehandrightwhichattack = 0;
	apehandrightsavedX = 0.0;

	apenosestatus = 1;
	apenoseshake = 0;
	apenoseX = 545.0;
	apenoseY = 238.0;
	apenosevelX = 0.0;
	apenosevelY = 0.0;
	apenoserotation = 0.0;
	apenosebleedtimer = 0;

	apehandrightX = 805.0;
	apehandrightY = 250.0;
	apehandrightvelX = 0.0;
	apehandrightvelY = 0.0;

	numboogers = 0;
	for (int i = 0; i < 5; i++)
	{
		boogerstatus[i] = 0;
		boogerX[i] = 0.0;
		boogerY[i] = 0.0;
		boogervelX[i] = 0.0;
		boogervelY[i] = 0.0;
	}
	boogerrotation[0] = 0.0;
	boogerrotation[1] = 0.5;
	boogerrotation[2] = -0.5;
	boogerrotation[3] = 1.0;
	boogerrotation[4] = -1.0;

	aperising = false;
	apefalling = false;
	apenosebleeding = false;

/* initialize particle system */
	for (int i = 0; i < 2; i++){
		g_pParticleSystem[i] = new CParticleSystem();
		g_pParticleSystem[i]->SetTexture( "particle.bmp" );
		g_pParticleSystem[i]->SetMaxParticles( 00 );
		g_pParticleSystem[i]->SetNumToRelease( 20 );
		g_pParticleSystem[i]->SetReleaseInterval( 0.05f );
		g_pParticleSystem[i]->SetLifeCycle( 1.0f );
		g_pParticleSystem[i]->SetSize( 0.1f );
		g_pParticleSystem[i]->SetColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, 0.8f ));
		g_pParticleSystem[i]->SetPosition( D3DXVECTOR3( 0.0, 0.0, 10.0f) );
		g_pParticleSystem[i]->SetVelocity( D3DXVECTOR3( 0.0f, -0.5f, 0.0f) );
		g_pParticleSystem[i]->SetGravity( D3DXVECTOR3( 0.0f, -1.0f, 0.0f) );
		g_pParticleSystem[i]->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
		g_pParticleSystem[i]->SetVelocityVar( 0.4f );

		g_pParticleSystem[i]->Init( d3ddev );
	}

	g_pParticleSystem[2] = new CParticleSystem();
	g_pParticleSystem[2]->SetTexture( "particle.bmp" );
	g_pParticleSystem[2]->SetMaxParticles( 00 );
	g_pParticleSystem[2]->SetNumToRelease( 20 );
	g_pParticleSystem[2]->SetReleaseInterval( 0.05f );
	g_pParticleSystem[2]->SetLifeCycle( 1.0f );
	g_pParticleSystem[2]->SetSize( 0.5f );
	g_pParticleSystem[2]->SetColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, 0.8f ));
	g_pParticleSystem[2]->SetPosition( D3DXVECTOR3( 0.0, 0.0, 10.0f) );
	g_pParticleSystem[2]->SetVelocity( D3DXVECTOR3( 0.0f, 1.3f, 0.0f) );
	g_pParticleSystem[2]->SetGravity( D3DXVECTOR3( 0.0f, -1.0f, 0.0f) );
	g_pParticleSystem[2]->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	g_pParticleSystem[2]->SetVelocityVar( 2.0f );

	g_pParticleSystem[2]->Init( d3ddev );

	lefthandtrailing = false;
	lefthandtrailsize = 0;
	righthandtrailing = false;
	righthandtrailsize = 0;
	nosetrailing = false;
	nosetrailsize = 0;
	headtrailing = false;
	headtrailsize = 0;
	boogertrailsize = 0;

	for (int i = 0; i < APEBOSSTRAILMAX; i++)
	{
		lefthandtrailX[i] = 0.0;
		lefthandtrailY[i] = 0.0;
		lefthandtrailframe[i] = 0;

		righthandtrailX[i] = 0.0;
		righthandtrailY[i] = 0.0;
		righthandtrailframe[i] = 0;

		nosetrailX[i] = 0.0;
		nosetrailY[i] = 0.0;
		nosetrailrotation[i] = 0.0;

		headtrailX[i] = 0.0;
		headtrailY[i] = 0.0;
		headtrailrotation[i] = 0.0;

		for (int j = 0; j < 5; j++)
		{
			boogertrailX[j][i] = 0.0;
			boogertrailY[j][i] = 0.0;
		}
	}
}

/*========================================================================


========================================================================*/

ApeBoss::~ApeBoss()
{
	apefaceTexture->Release();
	apehandleftTexture->Release();
	apehandrightTexture->Release();
	apenoseTexture->Release();
	boogerTexture->Release();
	apebodyTexture->Release();
}

/*========================================================================


========================================================================*/

void ApeBoss::drawParticle() {

	if (apenosebleeding == false)
		return;
	
	Reyk* player1 = players[clientIndex];

	particlePosition[0] = screenToWorld(10685 + 45 + apenoseX - player1->getXmod(), 110 + apenoseY - player1->getYmod(), 5.0f);
	particlePosition[1] = screenToWorld(10685 + 158 + apenoseX - player1->getXmod(), 110 + apenoseY - player1->getYmod(), 5.0f);
	particlePosition[2] = screenToWorld(10865 + 470 + apebodyX - player1->getXmod(), 400 + apebodyY - player1->getYmod(), 5.0f);

	for (int i = 0; i < 3; i++) {
		d3ddev->SetTexture( 0, g_pParticleSystem[i]->GetTextureObject() );
		g_pParticleSystem[i]->Render( d3ddev );
	}
}

void ApeBoss::updateApe(int rand1, int rand2, int rand3)
{
	g_pParticleSystem[2]->SetMaxParticles(0);

	apeframecount++;
	if (aperising)
	{
		aperisingoffset -= 6.0;
		if (aperisingoffset <= 0.0)
		{
			aperisingoffset = 0.0;
			aperising = false;
		}
	}
	else if (apefalling)
	{
		g_pParticleSystem[2]->SetMaxParticles(500);
		apebodyY += 2.0;
		apehandleftY += 1.7;
		apehandrightY += 1.7;
		if (aperisingoffset >= 1500.0)
		{
			aperisingoffset = 1500.0;
			apefalling = false;
		}
	}
	if (apefacestatus == 0 && apehandleftstatus == 0 && apehandrightstatus == 0)
	{
		apecount++;
		if (apecount >= apecountmax)
		{
			apecount = 0;
			apeAttack(rand1, rand2, rand3);
		}
	}
	else if (apehandrightstatus == 11)
	{
		apecount++;
		if (apecount >= apecountmax/4)
		{
			apecount = 0;
			apeAttack2(rand1);
		}
	}
	switch (apefacestatus)
	{
	case 0:
		apefaceY = (float)(-120 + 20.0 * sin((double)apeframecount/16.0));
		break;
	case 1:
		if (apefaceY > -150.0)
		{
			apefaceY -= 2.5;
		}
		else
		{
			apefaceY = -150.0;
		}
		break;
	case 2:
		if (apehandleftstatus != 0)
		{
			apefaceX = (float)(240.0 + 40.0 * (1.0 - ((double)apehandleftshake/35.0)) * sin((double)apeframecount));
			apehandleftX = (float)(75.0 + 40.0 * (1.0 - ((double)apehandleftshake/35.0)) * sin((double)apeframecount));
			apehandleftshake++;
			if (apehandleftshake == 35)
			{
				if (apehandleftwhichattack == 0)
				{
					shootBoogers(5);
				}
				else
				{
					shootSmallApe();
				}
				apefaceX = 240.0;
				apefacestatus = 3;
				apefaceframe = 0;
				apefacevel = 20.0;
			}
		}
		else if (apehandrightstatus != 0)
		{
			apefaceX = (float)(240.0 + 40.0 * (1.0 - ((double)apehandrightshake/35.0)) * sin((double)apeframecount));
			apehandrightX = (float)(660.0 + 40.0 * (1.0 - ((double)apehandrightshake/35.0)) * sin((double)apeframecount));
			apehandrightshake++;
			if (apehandrightshake == 35)
			{
				if (apehandrightwhichattack == 0)
				{
					shootBoogers(5);
				}
				else
				{
					shootSmallApe();
				}
				apefaceX = 240.0;
				apefacestatus = 3;
				apefaceframe = 0;
				apefacevel = 20.0;
			}
		}
		break;
	case 3:
		apefaceY += apefacevel;
		if (apehandleftstatus != 0)
		{
			apehandleftY += apefacevel;
		}
		else
		{
			apehandrightY += apefacevel;
		}
		apefacevel -= 1.25;
		if (apefaceY <= -120.0 && apefacevel < 0.0)
		{
			apefacestatus = 0;
			if (apehandleftstatus != 0)
			{
				apehandleftstatus = 10;
			}
			else
			{
				apehandrightstatus = 10;
			}
		}
		break;
	case 4:
		if (apefaceY < -120.0)
		{
			apefaceY += 25.0;
			if (apefaceY >= -120.0)
			{
				apefaceY = -120.0;
			}
		}
		else if (apefaceY > -120.0)
		{
			apefaceY -= 25.0;
			if (apefaceY <= -120.0)
			{
				apefaceY = -120.0;
			}
		}
		apefaceX = (float)(240.0 + 80.0 * (1.0 - ((double)apefaceshake/20.0)) * sin((double)apeframecount));
		apehandleftX = (float)(-65.0 + 40.0 * (1.0 - ((double)apefaceshake/20.0)) * sin((double)apeframecount));
		apehandrightX = (float)(805.0 + 40.0 * (1.0 - ((double)apefaceshake/20.0)) * sin((double)apeframecount));
		apefaceshake++;
		if (apefaceshake >= 20)
		{
			apefaceX = 240.0;
			apehandleftX = -65.0;
			apehandrightX = 805.0;
			apehandleftY = 250.0;
			apehandrightY = 250.0;
			apefacestatus = 5;
			apehandleftstatus = 11;
			apehandleftframe = 3;
			apefaceframe = 3;
		}
		break;
	case 5:
		break;
	case 6:
		apefaceY = (float)(-120 + 20.0 * sin((double)apeframecount/16.0));
		apehandleftY = (float)(apefaceY + 288.0);
		break;
	case 7:
		apefaceY = (float)(-120 + 20.0 * sin((double)apeframecount/16.0));
		apehandleftY = (float)(apefaceY + 288.0);
		apefaceX = (float)(240.0 + 60.0 * (1.0 - ((double)apefaceshake/20.0)) * sin((double)apeframecount));
		apehandleftX = (float)(apefaceX + 50.0);
		apefaceshake++;
		if (apefaceshake >= 20)
		{
			apefacestatus = 6;
			apefaceframe = 3;
		}
		break;
	case 8:
		apefaceX += apefacevelX;
		apefaceY += apefacevelY;
		apefacevelY += 0.8F;
		apehandleftrotation = -1.0;
		apehandrightrotation = 1.0;
		apehandleftX += 50.0 * sin(double(apeframecount));
		apehandrightX += 50.0 * sin(double(apeframecount));
		apefacerotation += 0.4F;
		if (IS_SERVER && apeframecount >= 500 )
		{
			gamestatus = 1;

			SwitchSong(Song, "level2.mp3");

			currentlevel = 2;
			loadLevel2();

			Reyk* player1;
			for(int i = 0; i < MAX_CLIENTS; i++)
			{
				player1 = players[i];
				player1->setlockedbottomamount(1085.0);
				player1->setlockedbottom(false);
				player1->setlockedleft(false);
				player1->setlockedright(false);
				player1->setX(415.0 + i*50);
				player1->setY(415.0);
				player1->setMapX(11080.0);
				player1->setMapY(300.0);
			}
		}
		break;
	}

	if (apenosestatus == 1)
	{
		apenoseX = apefaceX + 300.0F;
		apenoseY = apefaceY + 356.0F;
	}
	else if (apenosestatus == 2)
	{
		apenoseX = (float)((apefaceX + 300.0) + 30.0 * (1.0 - ((double)apenoseshake/24.0)) * sin((double)apeframecount));
		apenoseY = apefaceY + 356.0F;
		apenoseshake++;
		if (apenoseshake >= 24)
		{
			apenoseX = apefaceX + 300.0F;
			apenosestatus = 1;
			if (apefaceframe == 1)
			{
				apefaceframe = 0;
			}
		}
	}
	else if (apenosestatus == 3)
	{
		apenoseX += apenosevelX;
		apenoseY += apenosevelY;
		apenosevelY += 1.0F;
		if (apenoseX <= -250.0 || apenoseX >= 1330.0)
		{
			apenosestatus = 0;
		}
		if (apenosevelX < 0)
		{
			apenoserotation -= 0.3F;
		}
		else
		{
			apenoserotation += 0.3F;
		}
		if (apenoseY >= 3000.0)
		{
			nosetrailing = false;
		}
	}

	apenosebleedtimer++;
	if (apenosebleedtimer > 20) {
		g_pParticleSystem[0]->SetMaxParticles(0);
		g_pParticleSystem[1]->SetMaxParticles(0);
		apenosebleedtimer = 0;
	}

	g_pParticleSystem[0]->SetPosition( particlePosition[0] );
	g_pParticleSystem[0]->Update(0.01);

	g_pParticleSystem[1]->SetPosition( particlePosition[1] );
	g_pParticleSystem[1]->Update(0.01);

	g_pParticleSystem[2]->SetPosition( particlePosition[2] );
	g_pParticleSystem[2]->Update(0.01);

	// LEFT

	if (!lefthandtrailing)
	{
		if (lefthandtrailsize > 0)
			lefthandtrailsize--;
	}
	else
	{
		if (lefthandtrailsize < APEBOSSTRAILMAX)
			lefthandtrailsize++;
	}

	for (int i = APEBOSSTRAILMAX-1; i > 0; i--)
	{
		lefthandtrailX[i] = lefthandtrailX[i-1];
		lefthandtrailY[i] = lefthandtrailY[i-1];
		lefthandtrailframe[i] = lefthandtrailframe[i-1];
	}

	lefthandtrailX[0] = apehandleftX;
	lefthandtrailY[0] = apehandleftY;
	lefthandtrailframe[0] = apehandleftframe;

	// RIGHT

	if (!righthandtrailing)
	{
		if (righthandtrailsize > 0)
			righthandtrailsize--;
	}
	else
	{
		if (righthandtrailsize < APEBOSSTRAILMAX)
			righthandtrailsize++;
	}

	for (int i = APEBOSSTRAILMAX-1; i > 0; i--)
	{
		righthandtrailX[i] = righthandtrailX[i-1];
		righthandtrailY[i] = righthandtrailY[i-1];
		righthandtrailframe[i] = righthandtrailframe[i-1];
	}

	righthandtrailX[0] = apehandrightX;
	righthandtrailY[0] = apehandrightY;
	righthandtrailframe[0] = apehandrightframe;

	// NOSE
	if (apenosestatus == 3 || nosetrailsize > 0)
	{
		if (!nosetrailing)
		{
			if (nosetrailsize > 0)
				nosetrailsize--;
		}
		else
		{
			if (nosetrailsize < APEBOSSTRAILMAX)
				nosetrailsize++;
		}

		for (int i = APEBOSSTRAILMAX-1; i > 0; i--)
		{
			nosetrailX[i] = nosetrailX[i-1];
			nosetrailY[i] = nosetrailY[i-1];
			nosetrailrotation[i] = nosetrailrotation[i-1];
		}

		nosetrailX[0] = apenoseX;
		nosetrailY[0] = apenoseY;
		nosetrailrotation[0] = apenoserotation;
	}

	// HEAD

	if (apehp <= 0)
	{
		if (!headtrailing)
		{
			if (headtrailsize > 0)
				headtrailsize--;
		}
		else
		{
			if (headtrailsize < APEBOSSTRAILMAX)
				headtrailsize++;
		}

		for (int i = APEBOSSTRAILMAX-1; i > 0; i--)
		{
			headtrailX[i] = headtrailX[i-1];
			headtrailY[i] = headtrailY[i-1];
			headtrailrotation[i] = headtrailrotation[i-1];
		}

		headtrailX[0] = apefaceX;
		headtrailY[0] = apefaceY;
		headtrailrotation[0] = apefacerotation;
	}

	// BOOGERS

	if (boogertrailsize < APEBOSSTRAILMAX)
		boogertrailsize++;

	for (int i = APEBOSSTRAILMAX-1; i > 0; i--)
	{
		for (int j = 0; j < 5; j++)
		{
			boogertrailX[j][i] = boogertrailX[j][i-1];
			boogertrailY[j][i] = boogertrailY[j][i-1];
		}
	}

	for (int j = 0; j < 5; j++)
	{
		boogertrailX[j][0] = boogerX[j];
		boogertrailY[j][0] = boogerY[j];
	}
}

/*========================================================================


========================================================================*/

void ApeBoss::apeAttack(int rand1, int rand2, int rand3)
{
	int whichhand = rand()%2;
	int whichattack = rand()%3;
	if (eh->getstatus(91) == 0)
	{
		whichattack = rand()%4;
	}

	if (whichhand == 0)
	{
		switch(whichattack)
		{
		case 0:
			lefthandtrailing = true;
			apehandleftstatus = 1;
			apehandleftwhichattack = 0;
			break;
		case 1:
			lefthandtrailing = true;
			apehandleftstatus = 1;
			apehandleftwhichattack = 1;
			break;
		case 2:
			apehandleftstatus = 8;
			apehandleftframe = 2;
			apefacestatus = 1;
			apehandleftwhichattack = 0;
			break;
		case 3:
			apehandleftstatus = 8;
			apehandleftframe = 2;
			apefacestatus = 1;
			apehandleftwhichattack = 1;
			break;
		}
	}
	else
	{
		switch(whichattack)
		{
		case 0:
			righthandtrailing = true;
			apehandrightframe = 0;
			apehandrightstatus = 1;
			apehandrightwhichattack = 0;
			break;
		case 1:
			righthandtrailing = true;
			apehandrightframe = 0;
			apehandrightstatus = 1;
			apehandrightwhichattack = 1;
			break;
		case 2:
			apehandrightstatus = 8;
			apehandrightframe = 2;
			apefacestatus = 1;
			apehandrightwhichattack = 0;
			break;
		case 3:
			apehandrightstatus = 8;
			apehandrightframe = 2;
			apefacestatus = 1;
			apehandrightwhichattack = 1;
			break;
		}
	}
}

/*========================================================================


========================================================================*/

void ApeBoss::apeAttack2(int rand)
{
	if (rand == 0)
	{
		righthandtrailing = true;
		apehandrightframe = 0;
		apehandrightstatus = 1;
		apehandrightwhichattack = 0;
		apehandrightnumstomps = 0;
	}
	else
	{
		righthandtrailing = true;
		apehandrightframe = 0;
		apehandrightstatus = 1;
		apehandrightwhichattack = 1;
	}
}

/*========================================================================


========================================================================*/

void ApeBoss::updateLeftHand()
{
	if (aperising)
	{
		return;
	}

	if(IS_SERVER)
		for(int i = 0; i < MAX_CLIENTS; i++)
			if (detectCollision(lefthandGetHitbox(), players[i]->getHitbox()) && players[i]->getStatus() != 5 && apefacestatus < 6)
			{
				if (apehandleftX + 270.0 > players[i]->getCenterX())
				{
					players[i]->Reyk_GetHit(1);
				}
				else
				{
					players[i]->Reyk_GetHit(0);
				}
			}

	switch (apehandleftstatus)
	{
	case 0:
		apehandleftY = (float)(250.0 + 45.0 * sin((double)((7+apeframecount)/16.0)));
		lefthandtrailing = false;
		break;
	case 1:
		apehandleftY -= 10.0F;
		if (apehandleftY <= 0.0)
		{
			apehandleftstatus = 2;
			apehandleftshake = 0;
			//PlaySound(apefist);
		}
		break;
	case 2:
		apehandleftX = (float)(-65.0 + 50.0 * (1.0 - ((double)apehandleftshake/30.0)) * sin((double)apeframecount));
		apehandleftshake++;
		if (apehandleftshake == 30)
		{
			apehandleftX = -65.0F;
			apehandleftstatus = 3;
		}
		break;
	case 3:
		apehandleftY += 120.0F;
		if (apehandleftY >= 630.0)
		{
			//PlaySound(apefist);
			apehandleftY = 630.0F;
			apehandleftstatus = 4;
			apehandleftshake = 0;
		}
		break;
	case 4:
		apehandleftX = (float)(-65.0 + (apehandleftnumstomps * 420.0) + 50.0 * (1.0 - ((double)apehandleftshake/32.0)) * sin((double)apeframecount));
		apehandleftshake++;
		if (apehandleftshake == 16 && apehandleftwhichattack == 0)
		{
			if (apehandleftY >= 630.0)
			{
				apehandleftY = 550.0F;
			}
			apehandleftX = (float)(-65.0 + (apehandleftnumstomps * 420.0));
			apehandleftstatus = 5;
			apehandleftvelX = 12.0F;
			apehandleftvelY = -100.0F;
			apehandleftnumstomps++;
			if (apehandleftnumstomps > 2)
			{
				apehandleftstatus = 6;
			}
		}
		else if (apehandleftshake == 32 && apehandleftwhichattack == 1)
		{
				apehandleftstatus = 7;
				apehandleftframe = 1;
		}
		break;
	case 5:
		apehandleftX += apehandleftvelX;
		apehandleftY += apehandleftvelY;
		apehandleftvelY += 5.4F;
		if (apehandleftY >= 630.0)
		{
			//PlaySound(apefist);
			apehandleftstatus = 4;
			apehandleftshake = 0;
		}
		break;
	case 6:
		if (apehandleftX > -65.0)
		{
			apehandleftX -= 40.0F;
		}
		else
		{
			apehandleftX = -65.0F;
		}
		if (apehandleftY > 250.0)
		{
			apehandleftY -= 20.0F;
		}
		else
		{
			apehandleftY = 250.0F;
		}
		if (apehandleftX <= -65.0 && apehandleftY <= 250.0)
		{
			apehandleftX = -65.0F;
			apehandleftY = 250.0F;
			apehandleftstatus = 0;
			apehandleftnumstomps = 0;
		}
		break;
	case 7:
		//PlaySound(handslide);
		apehandleftX += 50.0F;
		if (apehandleftX >= 750.0)
		{
			apehandleftstatus = 6;
			apehandleftframe = 0;
		}
		break;
	case 8:
		if (apehandleftX < 75.0)
		{
			apehandleftX += 7.0F;
		}
		else
		{
			apehandleftX = 75.0F;
		}
		if (apehandleftY > 190.0)
		{
			apehandleftY -= 8.0F;
		}
		else
		{
			apehandleftY = 190.0F;
		}
		if (apehandleftX == 75.0 && apehandleftY == 190.0)
		{
			apehandleftstatus = 9;
			apefacestatus = 2;
			apefaceframe = 2;
			apehandleftshake = 0;
		}
		break;
	case 9:

		break;
	case 10:
		if (apehandleftX > -65.0)
		{
			apehandleftX -= 15.5F;
		}
		else
		{
			apehandleftX = -65.0F;
		}
		if (apehandleftY > 250.0)
		{
			apehandleftY -= 15.5F;
			if (apehandleftY <= 250.0)
			{
				apehandleftY = 250.0F;
			}
		}
		else if (apehandleftY < 250.0)
		{
			apehandleftY += 15.5F;
			if (apehandleftY >= 250.0)
			{
				apehandleftY = 250.0F;
			}
		}
		if (apehandleftX == -65.0 && apehandleftY == 250.0)
		{
			apehandleftstatus = 0;
			apehandleftframe = 0;
		}
		break;
	case 11:
		if (apehandleftX < apefaceX + 50.0)
		{
			apehandleftX += 25.0F;
			if (apehandleftX >= apefaceX + 50.0)
			{
				apehandleftX = apefaceX + 50.0F;
			}
		}
		if (apehandleftY > apefaceY + 288.0)
		{
			apehandleftY -= 20.0F;
			if (apehandleftY <= apefaceY + 288.0)
			{
				apehandleftY = apefaceY + 288.0F;
			}
		}
		if (apehandleftX == apefaceX + 50.0 && apehandleftY == apefaceY + 288.0)
		{
			apehandleftstatus = 9;
			apefacestatus = 6;
			apehandrightframe = 0;
			apehandrightstatus = 11;
			apespeedmod = 1.5;
			apedurationmod = 0.75;
			apecountmax = 300;
		}
		break;
	}
}

/*========================================================================


========================================================================*/

void ApeBoss::updateRightHand()
{
	if (aperising)
	{
		return;
	}
	if(IS_SERVER)
		for(int i = 0; i < MAX_CLIENTS; i++)
			if (detectCollision(righthandGetHitbox(), players[i]->getHitbox()) && players[i]->getStatus() != 5 && apehp > 0)
			{
				if (apehandrightX + 270.0 > players[i]->getCenterX())
				{
					players[i]->Reyk_GetHit(1);
				}
				else
				{
					players[i]->Reyk_GetHit(0);
				}
			}

	switch (apehandrightstatus)
	{
	case 0:
		apehandrightY = (float)(250.0 + 45.0 * sin((double)((7+apeframecount)/16.0)));
		righthandtrailing = false;
		break;
	case 1:
		if (apefacestatus < 6)
		{
			apehandrightY -= 10.0F;
		}
		else
		{
			apehandrightY -= 15.0F;
		}
		if (apehandrightY <= 0.0)
		{
			//PlaySound(apefist);
			apehandrightstatus = 2;
			apehandrightshake = 0;
		}
		break;
	case 2:
		apehandrightX = (float)(805.0 + 50.0 * (1.0 - ((double)apehandrightshake/(30.0))) * sin((double)apeframecount));
		apehandrightshake++;
		if (apehandrightshake >= 30)
		{
			apehandrightX = 805.0F;
			apehandrightstatus = 3;
		}
		break;
	case 3:
		if (apefacestatus < 6)
		{
			apehandrightY += 120.0F;
		}
		else
		{
			apehandrightY += 180.0F;
		}
		if (apehandrightY >= 630.0)
		{
			//PlaySound(apefist);
			apehandrightY = 630.0;
			apehandrightstatus = 4;
			apehandrightshake = 0;
		}
		break;
	case 4:
		apehandrightX = (float)(805.0 + (apehandrightnumstomps * -420.0) + 50.0 * (1.0 - ((double)apehandrightshake/(32.0))) * sin((double)apeframecount));
		apehandrightshake++;
		if (apehandrightshake == 16 && apehandrightwhichattack == 0)
		{
			if (apehandrightY >= 630.0)
			{
				apehandrightY = 550.0F;
			}
			apehandrightstatus = 5;
			if (apefacestatus < 6)
			{
				apehandrightvelX = -12.0F;
				apehandrightvelY = -100.0F;
			}
			else
			{
				apehandrightvelX = -12.0F;
				apehandrightvelY = -150.0F;
			}
			apehandrightnumstomps++;
			if (apehandrightnumstomps > 2)
			{
				apehandrightstatus = 6;
			}
		}

		else if (apehandrightshake == 32 && apehandrightwhichattack == 1)
		{
				apehandrightstatus = 7;
				apehandrightframe = 1;
		}
		break;
	case 5:
		apehandrightX += apehandrightvelX;
		apehandrightY += apehandrightvelY;

		if (apefacestatus < 6)
		{
			apehandrightvelY += 5.4F;
		}
		else
		{
			apehandrightvelY += 8.1F;
		}
		if (apehandrightY >= 630.0)
		{
			//PlaySound(apefist);
			apehandrightstatus = 4;
			apehandrightshake = 0;
		}
		break;
	case 6:
		if (apehandrightX < 805.0)
		{
			apehandrightX += 40.0F;
		}
		else
		{
			apehandrightX = 805.0F;
		}
		if (apehandrightY > 250.0)
		{
			apehandrightY -= 20.0F;
		}
		else
		{
			apehandrightY = 250.0F;
		}
		if (apehandrightX >= 805.0 && apehandrightY <= 250.0)
		{
			apehandrightX = 805.0F;
			apehandrightY = 250.0F;
			if (apefacestatus >= 6)
			{
				apehandrightstatus = 11;
			}
			else
			{
				apehandrightstatus = 0;
			}
			apehandrightnumstomps = 0;
		}
		break;
	case 7:
		//PlaySound(handslide);
		apehandrightX -= 50.0;
		if (apehandrightX <= 10.0 && apefacestatus < 6)
		{
			apehandrightstatus = 6;
			apehandrightframe = 0;
		}
		else if (apehandrightX <= -250.0 && apefacestatus >= 6)
		{
			apehandrightstatus = 6;
			apehandrightframe = 0;
		}
		break;
	case 8:
		if (apehandrightX > 660.0)
		{
			apehandrightX -= 7.0;
		}
		else
		{
			apehandrightX = 660.0;
		}
		if (apehandrightY > 190.0)
		{
			apehandrightY -= 8.0;
		}
		else
		{
			apehandrightY = 190.0;
		}
		if (apehandrightX == 660.0 && apehandrightY == 190.0)
		{
			apehandrightstatus = 9;
			apefacestatus = 2;
			apefaceframe = 2;
			apehandrightshake = 0;
		}
		break;
	case 9:

		break;
	case 10:
		if (apehandrightX < 805.0)
		{
			apehandrightX += 15.5;
		}
		else
		{
			apehandrightX = 805.0;
		}
		if (apehandrightY > 250.0)
		{
			apehandrightY -= 15.5;
			if (apehandrightY <= 250.0)
			{
				apehandrightY = 250.0;
			}
		}
		else if (apehandrightY < 250.0)
		{
			apehandrightY += 15.5;
			if (apehandrightY >= 250.0)
			{
				apehandrightY = 250.0;
			}
		}
		if (apehandrightX == 805.0 && apehandrightY == 250.0)
		{
			apehandrightstatus = 0;
			apehandrightframe = 0;
		}
		break;
	case 11:
		apehandrightY = (float)(250.0 + 45.0 * sin((double)((7+apeframecount)/8.0)));
		break;
	}
}

/*========================================================================


========================================================================*/

void ApeBoss::shootBoogers(int b)
{
	numboogers = b;
	boogertrailsize = 0;
	for (int i = 0; i < b; i++)
	{
		if (apehandleftstatus != 0)
		{
			boogerX[i] = apefaceX + 380.0F;
		}
		else
		{
			boogerX[i] = apefaceX + 265.0F;
		}
		boogerY[i] = apefaceY + 450.0F;
		boogerstatus[i] = 1;
		switch(i)
		{
		case 0:
			boogervelY[i] = 15.0;
			break;
		case 1:
			boogervelX[i] = -6.0;
			boogervelY[i] = 9.0;
			break;
		case 2:
			boogervelX[i] = 6.0;
			boogervelY[i] = 9.0;
			break;
		case 3:
			boogervelX[i] = -7.5;
			boogervelY[i] = 4.5;
			break;
		case 4:
			boogervelX[i] = 7.5;
			boogervelY[i] = 4.5;
			break;
		}
	}
}

/*========================================================================


========================================================================*/

void ApeBoss::updateBoogers()
{
	for (int i = 0; i < numboogers; i++)
	{
		if (boogerstatus[i] == 1)
		{
			RECT hb;
			hb.left = (long)(boogerX[i] + 47.0F + 10685.0F);
			hb.right = (long)(hb.left + 60.0F);
			hb.top = (long)(boogerY[i] + 81.0F);
			hb.bottom = (long)(hb.top + 60.0F);

			if(IS_SERVER)
				for(int i = 0; i < MAX_CLIENTS; i++)
					if ( players[i]->getStatus() != 5 && detectCollision(hb, players[i]->getHitbox()))
					{
						if (boogerX[i] + 75.0 > players[i]->getCenterX())
						{
							players[i]->Reyk_GetHit(1);
						}
						else
						{
							players[i]->Reyk_GetHit(0);
						}

						players[i]->Reyk_GetSlowed();
					}

			boogerX[i] += boogervelX[i];
			boogerY[i] += boogervelY[i];
			if (boogerY[i] >= 1024.0)
			{
				boogerstatus[i] = 0;
			}
		}
	}
}

/*========================================================================


========================================================================*/

void ApeBoss::shootSmallApe()
{
	eh->shootApe();
}

/*========================================================================


========================================================================*/

RECT ApeBoss::lefthandGetHitbox()
{
	RECT hb;
	hb.left = (long)(apehandleftX + 98.0F + 50.0F + 10685.0F);
	hb.right = (long)(hb.left + 300.0F - 100.0F);
	hb.top = (long)(apehandleftY + 68.0F + 50.0F);
	hb.bottom = (long)(hb.top + 340.0F - 100.0F);

	return hb;
}

/*========================================================================


========================================================================*/

RECT ApeBoss::righthandGetHitbox()
{
	RECT hb;
	hb.left = (long)(apehandrightX + 136.0F + 50.0F + 10685.0F);
	hb.right = (long)(hb.left + 300.0F - 100.0F);
	hb.top = (long)(apehandrightY + 68.0F + 50.0F);
	hb.bottom = (long)(hb.top + 340.0F - 100.0F);

	return hb;
}

/*========================================================================


========================================================================*/

RECT ApeBoss::apenoseGetHitbox()
{
	RECT hb;
	hb.left = (long)apenoseX + 10685.0F;
	hb.right = (long)(hb.left + 200.0F);
	hb.top = (long)(apenoseY);
	hb.bottom = (long)(hb.top + 150.0F);

	return hb;
}

/*========================================================================


========================================================================*/

void ApeBoss::apenoseHit()
{
	if (apenosestatus != 1 || aperising)
	{
		return;
	}

	Reyk* player1 = players[clientIndex];

	g_pParticleSystem[0]->SetMaxParticles(400);
	g_pParticleSystem[1]->SetMaxParticles(400);
	apenosebleeding = true;
	apenosebleedtimer = 0;
	apefaceframe = 1;
	apenosestatus = 2;
	apenoseshake = 0;
	apehp--;
	if (apehp <= 15)
	{
		nosetrailing = true;
		apefaceframe = 4;
		apenosestatus = 3;
		apefacestatus = 4;
		apenoserotation = 0.0;
		apehandleftstatus = 9;
		apehandrightstatus = 9;
		apehandleftframe = 1;
		apehandrightframe = 1;
		apehandleftY = 250.0;
		apehandrightY = 250.0;
		if (apenoseX + 100.0 > player1->getCenterX())
		{
			apenosevelX = 10.0;
		}
		else
		{
			apenosevelX = -10.0;
		}
		apenosevelY = -30.0;
	}
}

/*========================================================================


========================================================================*/

void ApeBoss::apefaceHit()
{
	if (apefacestatus == 7)
	{
		return;
	}
	
	Reyk* player1 = players[clientIndex];

	apefaceshake = 0;
	apefacestatus = 7;
	apefaceframe = 4;
	apehp--;
	if (apehp <= 0)
	{
		headtrailing = true;
		apehandleftX = -65.0;
		apehandrightX = 805.0;
		apehandleftstatus = 9;
		apehandrightstatus = 9;
		apehandleftframe = 1;
		apehandrightframe = 1;
		apehandleftY = 250.0;
		apehandrightY = 250.0;
		apefacestatus = 8;
		apeframecount = 0;
		if (apefaceX + 400.0 > player1->getCenterX())
		{
			apefacevelX = 5.0;
		}
		else
		{
			apefacevelX = -5.0;
		}
		apefacevelY = -30.0;
		apehandleftvelY = -25.0;
		apehandleftvelX = -0.25;
		apehandrightvelY = -25.0;
		apehandrightvelX = 0.25;
		apefalling = true;

		//PlaySound(apebossfinalhit);
	}
	else
	{
		//PlaySound(apebosshit);
	}
}

/*========================================================================


========================================================================*/

RECT ApeBoss::apefaceGetHitbox()
{
	RECT hb;

	hb.left = (long)(apefaceX + 131.0F + 10685.0F);
	hb.right = (long)(hb.left + 536.0F);
	hb.top = (long)(apefaceY + 222.0F);
	hb.bottom = (long)(hb.top + 434.0F);

	return hb;
}

/*========================================================================


========================================================================*/

int ApeBoss::getstatus()
{
	return apefacestatus;
}

/*========================================================================


========================================================================*/

int ApeBoss::getlefthandstatus()
{
	return apehandleftstatus;
}

/*========================================================================


========================================================================*/

int ApeBoss::gethp()
{
	return apehp;
}

/*========================================================================


========================================================================*/

float ApeBoss::getX()
{
	return apefaceX;
}

/*========================================================================


========================================================================*/

float ApeBoss::getY()
{
	return apefaceY;
}

/*========================================================================


========================================================================*/

void ApeBoss::ApeFace_Sprite_Transform_Draw()
{
	D3DCOLOR color;
	color = D3DCOLOR_XRGB(255,255,255);

	if (headtrailsize > 0)
	{
		for (int i = 0; i < headtrailsize; i++)
			Sprite_Transform_Draw(apefaceTexture, headtrailX[i], headtrailY[i], 800, 800, apefaceframe, 5, headtrailrotation[i], 1.0F, 1.0F, 
				D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(TRAILMAX - i)/(float)TRAILMAX));
	}

	Sprite_Transform_Draw(apefaceTexture, (int)apefaceX, (int)apefaceY+(int)aperisingoffset, 800, 800, apefaceframe, 5, apefacerotation, 1.0, 1.0, color);
}

/*========================================================================


========================================================================*/

void ApeBoss::ApeBody_Sprite_Transform_Draw()
{
	Sprite_Transform_Draw(apebodyTexture, apebodyX, apebodyY+(int)aperisingoffset, 1280, 1024);
}


/*========================================================================


========================================================================*/

void ApeBoss::ApeNose_Sprite_Transform_Draw()
{
	D3DCOLOR color;
	color = D3DCOLOR_XRGB(255,255,255);
		
	if (nosetrailsize > 0)
	{
		for (int i = 0; i < nosetrailsize; i++)
			Sprite_Transform_Draw(apenoseTexture, nosetrailX[i], nosetrailY[i], 200, 200, 0, 1, nosetrailrotation[i], 1.0F, 1.0F, 
				D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(TRAILMAX - i)/(float)TRAILMAX));
	}

	switch (apenosestatus)
		{
		case 1:
			Sprite_Transform_Draw(apenoseTexture, (int)apenoseX, (int)apenoseY+(int)aperisingoffset, 200, 200, 0, 1, 0.0F, 1.0F, 1.0F, color);
			break;
		case 2:
			Sprite_Transform_Draw(apenoseTexture, (int)apenoseX, (int)apenoseY+(int)aperisingoffset, 200, 200, 0, 1, 0.0F, 1.0F, 1.0F, color);
			break;
		case 3:
			Sprite_Transform_Draw(apenoseTexture, (int)apenoseX, (int)apenoseY+(int)aperisingoffset, 200, 200, 0, 1, apenoserotation, 1.0F, 1.0F, color);
			break;
		}
}

/*========================================================================


========================================================================*/

void ApeBoss::ApeHandLeft_Sprite_Transform_Draw()
{
	D3DCOLOR color;
	color = D3DCOLOR_XRGB(255,255,255);
	
	for (int i = 0; i < lefthandtrailsize; i++)
	{
		Sprite_Transform_Draw(apehandleftTexture, lefthandtrailX[i], lefthandtrailY[i]+(int)aperisingoffset, 540, 540, lefthandtrailframe[i], 4, 0.0F, 1.0F, 1.0F, 
			D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(APEBOSSTRAILMAX - i)/(float)APEBOSSTRAILMAX));
	}

	Sprite_Transform_Draw(apehandleftTexture, (int)apehandleftX, (int)apehandleftY+(int)aperisingoffset, 540, 540, apehandleftframe, 4, apehandleftrotation, 1.0F, 1.0F, color);
}

/*========================================================================


========================================================================*/

void ApeBoss::ApeHandRight_Sprite_Transform_Draw()
{
	D3DCOLOR color;
	color = D3DCOLOR_XRGB(255,255,255);
	
	if (apefacestatus < 7)
	{
		for (int i = 0; i < righthandtrailsize; i++)
		{
			Sprite_Transform_Draw(apehandrightTexture, righthandtrailX[i], righthandtrailY[i]+(int)aperisingoffset, 540, 540, righthandtrailframe[i], 4, 0.0F, 1.0F, 1.0F, 
				D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(APEBOSSTRAILMAX - i)/(float)APEBOSSTRAILMAX));
		}
	}

	Sprite_Transform_Draw(apehandrightTexture, (int)apehandrightX, (int)apehandrightY+(int)aperisingoffset, 540, 540, apehandrightframe, 4, apehandrightrotation, 1.0F, 1.0F, color);
}

/*========================================================================


========================================================================*/

void ApeBoss::Boogers_Sprite_Transform_Draw()
{
	D3DCOLOR color;
	color = D3DCOLOR_XRGB(255,255,255);
		
	for (int i = 0; i < boogertrailsize; i++)
	{
		for (int j = 0; j < numboogers; j++)
		{
			Sprite_Transform_Draw(boogerTexture, (int)boogertrailX[j][i], (int)boogertrailY[j][i]+(int)aperisingoffset, 150, 150, 0, 1, boogerrotation[j], 1.0F, 1.0F, 
				D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(APEBOSSTRAILMAX - i)/(float)APEBOSSTRAILMAX));
		}
	}

	for (int i = 0; i < numboogers; i++)
		{
			Sprite_Transform_Draw(boogerTexture, (int)boogerX[i], (int)boogerY[i]+(int)aperisingoffset, 150, 150, 0, 1, boogerrotation[i], 1.0F, 1.0F, color);
		}
}

/*========================================================================


========================================================================*/

void ApeBoss::startBossBattle()
{
	aperising = true;
	aperisingoffset = 2000.0;
}