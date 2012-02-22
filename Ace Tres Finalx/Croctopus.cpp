#include "MyDirectX.h"
#include "Croctopus.h"
#include "Reyk.h"
#include "SongPlayer.h"
#include "fmod.h"
#include "fmod_errors.h"


const float EYEOFFSETX[7] = {763.0, 653.0, 872.0, 587.0, 968.0, 540.0, 1023.0};
const float EYEOFFSETY[7] = {405.0, 397.0, 397.0, 379.0, 379.0, 340.0, 340.0};
//										4     3     5     2     6     1     7
const float EYEPARTICLEOFFSETSX[7] = {75.0, 65.0, 80.0, 50.0, 80.0, 45.0, 70.0};
const float EYEPARTICLEOFFSETSY[7] = {110.0, 95.0, 95.0, 75.0, 80.0, 70.0, 70.0};
const int MOUTHOPEN[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
const int MOUTHCLOSE[9] = {8, 7, 6, 5, 4, 3, 2, 1, 0};

extern Reyk* players[];

bool detectCollision(RECT r1, RECT r2);

extern ofstream debugfile;

extern int gamestatus;
extern int currentlevel;

extern FSOUND_STREAM * Song;
extern void SwitchSong(FSOUND_STREAM * & SongData, char *p_FileName);

extern CSound *crochit;
extern CSound *crocopen;
extern CSound *crocattack;
extern CSound *crunch;

void loadLevel3();

/*========================================================================


========================================================================*/

Croctopus::Croctopus()
{
	crocRTexture = LoadTexture("crocfaceR.png");
	crocTexture = LoadTexture("crocface.png");
	
	tentacleRTexture = LoadTexture("croctentacleR.png");
	tentacleTexture = LoadTexture("croctentacle.png");
	
	bottomTexture = LoadTexture("croctopusbottom.png");
	topTexture = LoadTexture("croctopustop.png");
	
	eyeoneTexture = LoadTexture("croceyeone.png");
	eyetwoTexture = LoadTexture("croceyetwo.png");
	eyetwoRTexture  = LoadTexture("croceyetwoR.png");
	eyethreeTexture = LoadTexture("croceyethree.png");
	eyethreeRTexture = LoadTexture("croceyethreeR.png");
	eyefourTexture = LoadTexture("croceyefour.png");
	eyefourRTexture = LoadTexture("croceyefourR.png");

	croctopuscounter = 0;
	attackcounter = 0;
	attacking = false;
	whichspot = 0;
	attackorder[0] = 0;
	attackorder[1] = 1;
	attackorder[2] = 2;

	crocX = 0.0 + 12300.0;
	crocY = 0.0 - 5800.0;

	bottomX = 200.0 + 12300.0;
	bottomY = 354.0 - 5800.0;
	
	topX = 512.0 + 12300.0;
	topY = -37.0 - 5800.0;

	savedtopX = topX;
	savedbottomX = bottomX;

	for (int i = 0; i < 3; i++)
	{
		crocfaceX[i] = -1000.0 + 12300.0;
		savedcrocfaceX[i] = -1000.0 + 12300.0;
		crocfaceY[i] = -1000.0;
		crocfacevelY[i] = 0.0;
		crocfaceframe[i] = 0;
		startframe[i] = 0;
		endframe[i] = 8;
		crocfacecounter[i] = 0;
		crocfacestatus[i] = 0;
		playercaught[i] = false;
		whichplayer[i] = -1;
	}

	for (int i = 0; i < 7; i++)
	{
		eyeX[i] = EYEOFFSETX[i];
		eyeY[i] = EYEOFFSETY[i];
		eyevelX[i] = 0.0;
		eyevelY[i] = 0.0;
		eyerotation[i] = 0.0;
		eyeframe[i] = 0;
		eyeshake[i] = 30;
		eyehp[i] = 30;
	}
	
	hp = 210;

	starttime = 0;
	delay = 50;

	dying = false;


	/* initialize particle system */
	for (int i = 0; i < 7; i++){
		g_pParticleSystem[i] = new CParticleSystem();
		g_pParticleSystem[i]->SetTexture( "particle.bmp" );
		g_pParticleSystem[i]->SetMaxParticles( 500 );
		g_pParticleSystem[i]->SetNumToRelease( 20 );
		g_pParticleSystem[i]->SetReleaseInterval( 0.05f );
		g_pParticleSystem[i]->SetLifeCycle( 1.0f );
		g_pParticleSystem[i]->SetSize( 0.1f );
		g_pParticleSystem[i]->SetColor( D3DXCOLOR( 0.8f, 0.0f, 0.8f, 0.8f ));
		g_pParticleSystem[i]->SetPosition( D3DXVECTOR3( 0.0, 0.0, 10.0f) );
		g_pParticleSystem[i]->SetVelocity( D3DXVECTOR3( 0.0f, 0.5f, 0.0f) );
		g_pParticleSystem[i]->SetGravity( D3DXVECTOR3( 0.0f, -1.0f, 0.0f) );
		g_pParticleSystem[i]->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
		g_pParticleSystem[i]->SetVelocityVar( 0.5f );

		g_pParticleSystem[i]->Init( d3ddev );
	}

	for (int i = 0; i < CROCTRAILMAX; i++)
	{
		for (int j = 0; j < 7; j++)
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

Croctopus::~Croctopus()
{
	crocRTexture->Release();
	crocTexture->Release();
	
	tentacleRTexture->Release();
	tentacleTexture->Release();
	
	bottomTexture->Release();
	topTexture->Release();
	
	eyeoneTexture->Release();
	eyetwoTexture->Release();
	eyetwoRTexture->Release();
	eyethreeTexture->Release();
	eyethreeRTexture->Release();
	eyefourTexture->Release();
	eyefourRTexture->Release();
}

/*========================================================================


========================================================================*/

void Croctopus::draw()
{
	Reyk* player1 = players[clientIndex];

	float xmod = player1->getXmod();
	float ymod = player1->getYmod();

	Sprite_Transform_Draw(bottomTexture, bottomX - xmod, bottomY - ymod, 1280, 670);
	
	if (eyehp[5] > 0)
		Sprite_Transform_Draw(eyefourTexture, eyeX[5] - xmod, eyeY[5] - ymod, 110, 110, eyeframe[5], 6);
	if (eyehp[6] > 0)
		Sprite_Transform_Draw(eyefourRTexture, eyeX[6] - xmod, eyeY[6] - ymod, 110, 110, eyeframe[6], 6);
	if (eyehp[3] > 0)
		Sprite_Transform_Draw(eyethreeTexture, eyeX[3] - xmod, eyeY[3] - ymod, 120, 120, eyeframe[3], 6);
	if (eyehp[4] > 0)
		Sprite_Transform_Draw(eyethreeRTexture, eyeX[4] - xmod, eyeY[4] - ymod, 120, 120, eyeframe[4], 6);
	if (eyehp[2] > 0)
		Sprite_Transform_Draw(eyetwoRTexture, eyeX[2] - xmod, eyeY[2] - ymod, 150, 150, eyeframe[2], 6);
	if (eyehp[1] > 0)
		Sprite_Transform_Draw(eyetwoTexture, eyeX[1] - xmod, eyeY[1] - ymod, 150, 150, eyeframe[1], 6);
	if (eyehp[0] > 0)
		Sprite_Transform_Draw(eyeoneTexture, eyeX[0] - xmod, eyeY[0] - ymod, 150, 150, eyeframe[0], 6);

	Sprite_Transform_Draw(topTexture, topX - xmod, topY - ymod, 650, 530);

	for (int j = 0; j < 7; j++)
	{
		for (int i = 0; i < eyetrailsize[j]; i++)
		{
			LPDIRECT3DTEXTURE9 eyetexture;
			int eyesize;
			switch(j)
			{
			case 0:
				eyetexture = eyeoneTexture;
				eyesize = 150;
				break;
			case 1:
				eyetexture = eyetwoTexture;
				eyesize = 150;
				break;
			case 2:
				eyetexture = eyetwoRTexture;
				eyesize = 150;
				break;
			case 3:
				eyetexture = eyethreeTexture;
				eyesize = 120;
				break;
			case 4:
				eyetexture = eyethreeRTexture;
				eyesize = 120;
				break;
			case 5:
				eyetexture = eyefourTexture;
				eyesize = 110;
				break;
			case 6:
				eyetexture = eyefourRTexture;
				eyesize = 110;
				break;
			}
			Sprite_Transform_Draw(eyetexture, eyetrailX[j][i] - xmod, eyetrailY[j][i] - ymod, eyesize, eyesize, eyeframe[j], 6, eyetrailrotation[j][i], 1.0F, 1.0F,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(CROCTRAILMAX - i)/(float)CROCTRAILMAX));
		}
	}

	if (eyehp[5] <= 0)
		Sprite_Transform_Draw(eyefourTexture, eyeX[5] - xmod, eyeY[5] - ymod, 110, 110, eyeframe[5], 6, eyerotation[5]);
	if (eyehp[6] <= 0)
		Sprite_Transform_Draw(eyefourRTexture, eyeX[6] - xmod, eyeY[6] - ymod, 110, 110, eyeframe[6], 6, eyerotation[6]);
	if (eyehp[3] <= 0)
		Sprite_Transform_Draw(eyethreeTexture, eyeX[3] - xmod, eyeY[3] - ymod, 120, 120, eyeframe[3], 6, eyerotation[3]);
	if (eyehp[4] <= 0)
		Sprite_Transform_Draw(eyethreeRTexture, eyeX[4] - xmod, eyeY[4] - ymod, 120, 120, eyeframe[4], 6, eyerotation[4]);
	if (eyehp[2] <= 0)
		Sprite_Transform_Draw(eyetwoRTexture, eyeX[2] - xmod, eyeY[2] - ymod, 150, 150, eyeframe[2], 6, eyerotation[2]);
	if (eyehp[1] <= 0)
		Sprite_Transform_Draw(eyetwoTexture, eyeX[1] - xmod, eyeY[1] - ymod, 150, 150, eyeframe[1], 6, eyerotation[1]);
	if (eyehp[0] <= 0)
		Sprite_Transform_Draw(eyeoneTexture, eyeX[0] - xmod, eyeY[0] - ymod, 150, 150, eyeframe[0], 6, eyerotation[0]);

}

void Croctopus::drawParticle()
{
	Reyk* player1 = players[clientIndex];

	for (int i = 0; i < 7; i++)
	{
		if (eyehp[i] <= 0)
			g_pParticleSystem[i]->SetMaxParticles(500);
		else g_pParticleSystem[i]->SetMaxParticles(0);

		particlePositions[i] = screenToWorld(bottomX - 200.0 + EYEOFFSETX[i] - player1->getXmod() + EYEPARTICLEOFFSETSX[i], bottomY - 354.0 + EYEOFFSETY[i] - player1->getYmod() + EYEPARTICLEOFFSETSY[i], 5.0f);
		d3ddev->SetTexture( 0, g_pParticleSystem[i]->GetTextureObject() );
		g_pParticleSystem[i]->Render( d3ddev );
	}
}

/*========================================================================


========================================================================*/

void Croctopus::drawcrocs()
{
	Reyk* player1 = players[clientIndex];

	float xmod = player1->getXmod();
	float ymod = player1->getYmod();

	for (int i = 0; i < 3; i++)
	{
		LPDIRECT3DTEXTURE9 currentface = crocTexture;
		LPDIRECT3DTEXTURE9 currenttentacle = tentacleTexture;
		float tentaclemodX = 0.0;
		if (crocfaceX[i] > crocX + 640.0)
		{
			currentface = crocRTexture;
			currenttentacle = tentacleRTexture;
			tentaclemodX = 60.0;
		}

		int curframe = 0;
		if (crocfacestatus[i] == 3)
		{
			curframe = MOUTHOPEN[crocfaceframe[i]];
		}
		else if (crocfacestatus[i] == 4)
		{
			curframe = 8;
		}
		else if (crocfacestatus[i] == 5)
		{
			curframe = MOUTHCLOSE[crocfaceframe[i]];
		}
		Sprite_Transform_Draw(currenttentacle, crocfaceX[i] + 83.0 + tentaclemodX - xmod, crocfaceY[i] - 1824.0 - ymod, 460, 650);
		Sprite_Transform_Draw(currenttentacle, crocfaceX[i] + 83.0 + tentaclemodX - xmod, crocfaceY[i] - 1216.0 - ymod, 460, 650);
		Sprite_Transform_Draw(currenttentacle, crocfaceX[i] + 83.0 + tentaclemodX - xmod, crocfaceY[i] - 608.0 - ymod, 460, 650);
		Sprite_Transform_Draw(currentface, crocfaceX[i] - xmod, crocfaceY[i] - ymod, 650, 650, curframe, 9);
	}
}

/*========================================================================


========================================================================*/

void Croctopus::move(float x, float y)
{
	crocX += x;
	crocY += y;
}

/*========================================================================


========================================================================*/

void Croctopus::animate()
{
	if ((int)GetTickCount() > starttime + delay)
	{
		starttime = GetTickCount();

		for (int i = 0; i < 3; i++)
		{
			crocfaceframe[i] += 1;

			if (crocfaceframe[i] > endframe[i])
			{
				if (crocfacestatus[i] == 5)
				{
					startframe[i] = 8;
				}
				crocfaceframe[i] = startframe[i];
			}
			if (crocfaceframe[i] < startframe[i]) crocfaceframe[i] = endframe[i];
		}
	}
}

/*========================================================================


========================================================================*/

void Croctopus::update(int rand4, int* randarr, int rand3)
{
	croctopuscounter++;

	for (int j = 0; j < 7; j++)
	{
		if (eyevelX[j] != 0.0 && eyetrailsize[j] < CROCTRAILMAX)
			eyetrailsize[j]++;
	}

	for (int i = CROCTRAILMAX-1; i > 0; i--)
	{
		for (int j = 0; j < 7; j++)
		{
			eyetrailX[j][i] = eyetrailX[j][i-1];
			eyetrailY[j][i] = eyetrailY[j][i-1];
		}
	}

	for (int j = 0; j < 7; j++)
	{
		eyetrailX[j][0] = eyeX[j];
		eyetrailY[j][0] = eyeY[j];
		eyetrailrotation[j][0] = eyerotation[j];
	}

	for (int i = 0; i < 7; i++)
	{
		g_pParticleSystem[i]->SetPosition(particlePositions[i]);
		g_pParticleSystem[i]->Update(0.01);
	}

	if (dying)
	{
		bottomX = savedbottomX + 30 * sin((double)croctopuscounter/10.0);
		topX = savedtopX + 30 * sin((double)croctopuscounter/10.0);
		bottomY += 2.0;
		topY += 2.0;

		for (int i = 0; i < 3; i++)
		{
			crocfaceY[i] -= 10.0;
		}

		for (int i = 0; i < 7; i++)
		{
			if (eyehp[i] <= 0)
			{
				eyeX[i] += eyevelX[i];
				eyeY[i] += eyevelY[i];
				eyevelY[i] += 1.0;
				eyerotation[i] += 0.4;
			}
			else
			{
				eyehp[i] = -1;
			}
		}

		if (IS_SERVER && croctopuscounter >= 500)
		{
			gamestatus = 1;

			SwitchSong(Song, "level3.mp3");

			loadLevel3();
			currentlevel = 3;

			for(int i = 0; i < MAX_CLIENTS; i++)
			{
				players[i]->setlockedbottomamount(1400.0 - 6700.0);
				players[i]->setlockedbottom(false);
			}
		}

		return;
	}

	crocY = (float)(-5750.0 - 20.0 * sin((double)croctopuscounter/60.0));

	bottomX = crocX + 200.0;
	bottomY = crocY + 354.0;

	topX = crocX + 512.0;
	topY = crocY - 37.0;

	if (attacking && !dying)
	{
		attackcounter++;
		if (attackcounter == 40)
		{
			crocfacestatus[attackorder[0]] = 1;
		}
		else if (attackcounter == 80)
		{
			crocfacestatus[attackorder[1]] = 1;
		}
		else if (attackcounter == 120)
		{
			crocfacestatus[attackorder[2]] = 1;
		}
		if (crocfacestatus[0] == 0 && crocfacestatus[1] == 0 && crocfacestatus[2] == 0)
		{
			attacking = false;
			attackcounter = 0;
		}
	}
	else
	{
		startattack(rand4, randarr, rand3);
	}

	for (int i = 0; i < 7; i++)
	{
		if (eyehp[i] <= 0)
		{
			eyeX[i] += eyevelX[i];
			eyeY[i] += eyevelY[i];
			eyevelY[i] += 1.0;
			eyerotation[i] += 0.4;
		}
		else
		{
			eyeY[i] = crocY + EYEOFFSETY[i];
			eyeX[i] = crocX + EYEOFFSETX[i];
			if (eyeshake[i] < 30)
			{
				eyeX[i] = crocX + EYEOFFSETX[i] + 20.0 * (1.0 - (double)eyeshake[i]/30.0) * sin((double)croctopuscounter);
				eyeshake[i]++;
				if (eyeshake[i] >= 30)
				{
					eyeshake[i] = 30;
					eyeX[i] = crocX + EYEOFFSETX[i];
				}
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		crocfaceX[i] = crocX + savedcrocfaceX[i] + (float)(15.0 * sin((double)croctopuscounter/12.0));
		if (playercaught[i])
		{
			players[whichplayer[i]]->setSendPacket(false);
			players[whichplayer[i]]->setMapY(crocfaceY[i] + 100.0);
			players[whichplayer[i]]->setSendPacket(false);
		}
		switch(crocfacestatus[i])
		{
		case 0:
			crocfaceframe[i] = 0;
			break;
		case 1:
			delay = 50;
			startframe[i] = 0;
			crocfaceframe[i] = 0;
			crocfaceY[i] += 4.0;
			if (crocfaceY[i] >= -150.0 + crocY)
			{
				crocfaceY[i] = -150.0 + crocY;
				crocfacestatus[i] = 2;
				crocfacecounter[i] = 0;
			}
			break;
		case 2:
			crocfaceframe[i] = 0;
			crocfacecounter[i]++;
			if (crocfacecounter[i] >= 50)
			{
				crocfacecounter[i] = 0;
				crocfacestatus[i] = 3;
			}
			break;
		case 3:
			endframe[i] = 8;
			if (crocfaceframe[i] == 8)
			{
				crocfacecounter[i] = 0;
				crocfacestatus[i] = 4;
				if(!IS_SERVER) PlaySound(crocopen);
			}
			break;
		case 4:
			crocfaceframe[i] = 8;
			crocfaceX[i] = crocX + savedcrocfaceX[i] + 20.0 * (1.0 - (double)crocfacecounter[i]/30.0) * sin((double)croctopuscounter);
			crocfacecounter[i]++;
			if (crocfacecounter[i] >= 30)
			{
				startframe[i] = 0;
				endframe[i] = 8;
				crocfaceframe[i] = 0;
				crocfacestatus[i] = 5;
				crocfacevelY[i] = 50.0;
				if(!IS_SERVER) PlaySound(crocattack);
			}
			break;
		case 5:
			delay = 15;
			crocfaceY[i] += crocfacevelY[i];
			crocfacevelY[i] -= 0.8;
			if(IS_SERVER)
				detectcroccollision();
			if (crocfaceY[i] <= - 700.0 + crocY)
			{
				delay = 50;
				crocfacevelY[i] = 0;
				crocfaceframe[i] = 0;
				crocfacestatus[i] = 0;
				startframe[i] = 0;
				endframe[i] = 0;
				if (playercaught[i])
				{
					players[whichplayer[i]]->setSendPacket(false);
					players[whichplayer[i]]->setMapX(400.0 + 12300.0);//don't send packets
					players[whichplayer[i]]->setMapY(-5500.0);
					players[whichplayer[i]]->setX(415.0);
					players[whichplayer[i]]->Reyk_Die();
					players[whichplayer[i]]->setY(415.0);
					players[whichplayer[i]]->setSendPacket(true);
				}
				playercaught[i] = false;
				whichplayer[i] = -1;
			}
			break;
		}
	}
}

/*========================================================================


========================================================================*/

RECT Croctopus::geteyeHitbox(int which)
{
	RECT hb;

	hb.left = eyeX[which];
	hb.top = eyeY[which];

	switch (which)
	{
	case 0:
	case 1:
	case 2:
		hb.right = hb.left + 150.0;
		hb.bottom = hb.top + 150.0;
		break;
	case 3:
	case 4:
		hb.right = hb.left + 120.0;
		hb.bottom = hb.top + 120.0;
		break;
	case 5:
	case 6:
		hb.right = hb.left + 110.0;
		hb.bottom = hb.top + 110.0;
		break;
	}

	return hb;
}

/*========================================================================


========================================================================*/

void Croctopus::eyegethit(int which, Reyk* player1)
{
	if (eyeshake[which] < 30 || eyehp[which] < 0)
	{
		return;
	}

	if(!IS_SERVER) PlaySound(crochit);

	eyehp[which]--;
	hp--;

	if (hp <= 0)
	{
		dying = true;
		topY = crocY - 37.0;
		croctopuscounter = 0;
		return;
	}

	if (eyehp[which] % 5 == 0)
	{
		eyeframe[which]++;
	}
	if (eyehp[which] <= 0)
	{
		eyeframe[which] = 5;
		if (player1->getCenterX() > eyeX[which] + 50.0)
		{
			eyevelX[which] = -10.0;
		}
		else
		{
			eyevelX[which] = 10.0;
		}
		eyevelY[which] = -20.0;
	}
	else
	{
		eyeshake[which] = 0;
	}
}

/*========================================================================


========================================================================*/
int* Croctopus::randomizeAttack(){
	static int randarr[3];
	int i;
	for (i = 0; i < 100; i++)
	{
		int randindex1 = rand() % 3;
		int randindex2 = rand() % 3;
		int temp = attackorder[randindex1];
		attackorder[randindex1] = attackorder[randindex2];
		attackorder[randindex2] = temp;
	}

	for (i = 0; i < 3; i++)
		randarr[i] = attackorder[i];
	
	return randarr;
}

/*========================================================================


========================================================================*/

void Croctopus::startattack(int rand4, int* randarr, int rand3)
{
	attackcounter = 0;
	attacking = true;
	whichspot = rand()%4;

	for (int i = 0; i < 100; i++)
	{
		int randindex1 = rand() % 3;
		int randindex2 = rand() % 3;
		int temp = attackorder[randindex1];
		attackorder[randindex1] = attackorder[randindex2];
		attackorder[randindex2] = temp;
	}

	switch(whichspot)
	{
	case 0:
		crocfaceX[0] = 150.0;
		savedcrocfaceX[0] = 150.0;
		crocfaceY[0] = -800.0 + crocY;

		crocfaceX[1] = 850.0;
		savedcrocfaceX[1] = 850.0;
		crocfaceY[1] = -800.0 + crocY;

		crocfaceX[2] = 1200.0;
		savedcrocfaceX[2] = 1200.0;
		crocfaceY[2] = -800.0 + crocY;
		break;
	case 1:
		crocfaceX[0] = -200.0;
		savedcrocfaceX[0] = -200.0;
		crocfaceY[0] = -800.0 + crocY;

		crocfaceX[1] = 850.0;
		savedcrocfaceX[1] = 850.0;
		crocfaceY[1] = -800.0 + crocY;

		crocfaceX[2] = 1200.0;
		savedcrocfaceX[2] = 1200.0;
		crocfaceY[2] = -800.0 + crocY;
		break;
	case 2:
		crocfaceX[0] = -200.0;
		savedcrocfaceX[0] = -200.0;
		crocfaceY[0] = -800.0 + crocY;

		crocfaceX[1] = 150.0;
		savedcrocfaceX[1] = 150.0;
		crocfaceY[1] = -800.0 + crocY;

		crocfaceX[2] = 1200.0;
		savedcrocfaceX[2] = 1200.0;
		crocfaceY[2] = -800.0 + crocY;
		break;
	case 3:
		crocfaceX[0] = -200.0;
		savedcrocfaceX[0] = -200.0;
		crocfaceY[0] = -800.0 + crocY;

		crocfaceX[1] = 150.0;
		savedcrocfaceX[1] = 150.0;
		crocfaceY[1] = -800.0 + crocY;

		crocfaceX[2] = 850.0;
		savedcrocfaceX[2] = 850.0;
		crocfaceY[2] = -800.0 + crocY;
		break;
	}
	for (int i = 0; i < 3; i++)
	{
		crocfacestatus[i] = 1;
	}
}

/*========================================================================


========================================================================*/

void Croctopus::detectcroccollision()
{
	int numPlayersParalyzed = 0;
	for(int i = 0; i < MAX_CLIENTS; i++)
		if (players[i]->getStatus() == 8)
		{
			numPlayersParalyzed++;
		}

	if(numPlayersParalyzed == MAX_CLIENTS)
		return;

	for (int i = 0; i < 3; i++)
	{
		RECT mouthhb;

		mouthhb.left = crocfaceX[i] + 175.0;
		mouthhb.right = mouthhb.left + 300.0;
		mouthhb.top = crocfaceY[i] + 300.0;
		mouthhb.bottom = mouthhb.top + 300.0;

		if(IS_SERVER)
			for(int j = 0; j < MAX_CLIENTS; j++)
				if (detectCollision(players[j]->getHitbox(), mouthhb) && players[j]->getStatus() != 8)
				{
					players[j]->setStatus(8);
					players[j]->setMapX(crocfaceX[i] + 135.0);

					playercaught[i] = true;
					whichplayer[i] = j;
					return;
				}
		//else
			//PlaySound(crunch);

		RECT tentaclehb1;

		tentaclehb1.left = crocfaceX[i] + 83.0;
		tentaclehb1.right = tentaclehb1.left + 460.0;
		tentaclehb1.top = crocfaceY[i] - 1824.0;
		tentaclehb1.bottom = tentaclehb1.top + 650.0;

		RECT tentaclehb2;

		tentaclehb2.left = crocfaceX[i] + 83.0;
		tentaclehb2.right = tentaclehb2.left + 460.0;
		tentaclehb2.top = crocfaceY[i] - 1216.0;
		tentaclehb2.bottom = tentaclehb2.top + 650.0;

		RECT tentaclehb3;

		tentaclehb3.left = crocfaceX[i] + 83.0;
		tentaclehb3.right = tentaclehb3.left + 460.0;
		tentaclehb3.top = crocfaceY[i] - 608.0;
		tentaclehb3.bottom = tentaclehb3.top + 650.0;

		if(IS_SERVER)
			for(int j = 0; j < MAX_CLIENTS; j++)
				if (detectCollision(players[j]->getHitbox(), tentaclehb1)
					|| detectCollision(players[j]->getHitbox(), tentaclehb2)
					|| detectCollision(players[j]->getHitbox(), tentaclehb3))
				{
					if (players[j]->getCenterX() > tentaclehb1.left + 230.0)
					{
						players[j]->Reyk_GetHit(0);
					}
					else
					{
						players[j]->Reyk_GetHit(1);
					}
				}
	}
}

/*========================================================================


========================================================================*/

void Croctopus::setplayercaught(int i, bool b, int which){
	playercaught[i] = b;
	whichplayer[i] = which;
}

/*========================================================================


========================================================================*/

int Croctopus::gethp()
{
	return hp;
}
