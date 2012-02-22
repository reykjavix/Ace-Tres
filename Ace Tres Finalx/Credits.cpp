#include "MyDirectX.h"
#include "Credits.h"
#include "SongPlayer.h"

const int WALK[8] = { 0, 1, 2, 1, 0, 3, 4, 3 };
const int CREDSY[39] = {
	18, 457, 883, 1424, 1874, 
	2054, 2234, 2413, 2580, 2744,
	2909, 3255, 3450, 3610, 3805,
	0, 192, 334, 514, 658,
	814, 964, 1114, 1234, 1414,
	1534, 1714, 1864, 2298, 2434,
	2554, 2683, 2824, 2953, 3109,
	3274, 3445, 3604, 3904
};

extern FSOUND_STREAM * Song;
extern void SwitchSong(FSOUND_STREAM * & SongData, char *p_FileName);

/*========================================================================


========================================================================*/

Credits::Credits()
{
	bigreykTexture[0] = LoadTexture("bigreyk1.png");
	bigreykTexture[1] = LoadTexture("bigreyk2.png");
	bigreykTexture[2] = LoadTexture("bigreyk3.png");
	bigreykTexture[3] = LoadTexture("bigreyk4.png");
	bigreykTexture[4] = LoadTexture("bigreyk5.png");
	
	creditsTexture[0] = LoadTexture("credits002.png");
	creditsTexture[1] = LoadTexture("credits003.png");
	creditsTexture[2] = LoadTexture("credits004.png");
	creditsTexture[3] = LoadTexture("credits005.png");
	creditsTexture[4] = LoadTexture("credits006.png");
	creditsTexture[5] = LoadTexture("credits007.png");
	creditsTexture[6] = LoadTexture("credits008.png");
	creditsTexture[7] = LoadTexture("credits009.png");
	creditsTexture[8] = LoadTexture("credits010.png");
	creditsTexture[9] = LoadTexture("credits0011.png");
	creditsTexture[10] = LoadTexture("credits0012.png");
	creditsTexture[11] = LoadTexture("credits0013.png");
	creditsTexture[12] = LoadTexture("credits0014.png");
	creditsTexture[13] = LoadTexture("credits0015.png");
	creditsTexture[14] = LoadTexture("credits0016.png");
	creditsTexture[15] = LoadTexture("credits0017.png");
	creditsTexture[16] = LoadTexture("credits0018.png");
	creditsTexture[17] = LoadTexture("credits0020.png");
	creditsTexture[18] = LoadTexture("credits0021.png");
	creditsTexture[19] = LoadTexture("credits0022.png");
	creditsTexture[20] = LoadTexture("credits0023.png");
	creditsTexture[21] = LoadTexture("credits0024.png");
	creditsTexture[22] = LoadTexture("credits0025.png");
	creditsTexture[23] = LoadTexture("credits0026.png");
	creditsTexture[24] = LoadTexture("credits0027.png");
	creditsTexture[25] = LoadTexture("credits0028.png");
	creditsTexture[26] = LoadTexture("credits0029.png");
	creditsTexture[27] = LoadTexture("credits0030.png");
	creditsTexture[28] = LoadTexture("credits0031.png");
	creditsTexture[29] = LoadTexture("credits0032.png");
	creditsTexture[30] = LoadTexture("credits0033.png");
	creditsTexture[31] = LoadTexture("credits0034.png");
	creditsTexture[32] = LoadTexture("credits0035.png");
	creditsTexture[33] = LoadTexture("credits0036.png");
	creditsTexture[34] = LoadTexture("credits0037.png");
	creditsTexture[35] = LoadTexture("credits0038.png");
	creditsTexture[36] = LoadTexture("credits0039.png");
	creditsTexture[37] = LoadTexture("credits0040.png");
	creditsTexture[38] = LoadTexture("credits0041.png");

	for (int i = 0; i < 39; i ++)
	{
		creditsY[i] = (float)CREDSY[i] + 1024.0;
		if (i > 8)
		{
			creditsY[i] -= 150.0;
		}
		if (i > 14)
		{
			creditsY[i] += 4000.0;
		}
	}

	bigreykX = 0.0;
	bigreykY = 0.0;
	bigreykframe = 0;
	animationframe = 0;
	delay = 70;
	starttime = 0;

	font = MakeFont("Arial Bold", 24);

	g_pParticleSystem = new CParticleSystem();
	g_pParticleSystem->SetTexture( "particle.bmp" );
	g_pParticleSystem->SetMaxParticles( 10000000 );
	g_pParticleSystem->SetNumToRelease( 300 );
	g_pParticleSystem->SetReleaseInterval( 0.02f );
	g_pParticleSystem->SetLifeCycle( 5.0f );
	g_pParticleSystem->SetSize( 0.5f );
	g_pParticleSystem->SetColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ));
	g_pParticleSystem->SetPosition( D3DXVECTOR3( 0.0, 0.0, 10.0f) );
	g_pParticleSystem->SetVelocity( D3DXVECTOR3( 0.0f, 1.3f, 0.0f) );
	g_pParticleSystem->SetGravity( D3DXVECTOR3( 0.0f, -1.0f, 0.0f) );
	g_pParticleSystem->SetWind( D3DXVECTOR3( 0.0f, 0.0f, 0.0f) );
	g_pParticleSystem->SetVelocityVar( 30.0f );

	g_pParticleSystem->Init( d3ddev );

	fireR = 255.0;
	fireG = 0.0;
	fireB = 0.0;
	
	firestate = 0;

	SwitchSong(Song, "credits.mp3");

	begintime = GetTickCount();
}

/*========================================================================


========================================================================*/

void Credits::animate()
{
	if ((int)GetTickCount() > starttime + delay)
	{
		starttime = GetTickCount();

		animationframe += 1;

		if (animationframe > 7)
		{
			animationframe = 0;
		}
	}
}

/*========================================================================


========================================================================*/

void Credits::update()
{
	switch(firestate)
	{
	case 0:
		fireR -= 1.0;
		fireG += 1.0;
		if (fireR <= 0.0)
		{
			firestate = 1;
		}
		break;
	case 1:
		fireG -= 1.0;
		fireB += 1.0;
		if (fireG <= 0.0)
		{
			firestate = 2;
		}
		break;
	case 2:
		fireB -= 1.0;
		fireR += 1.0;
		if (fireB <= 0.0)
		{
			firestate = 0;
		}
	}

	g_pParticleSystem->SetColor( D3DXCOLOR( fireR/225.0f, fireG/225.0f, fireB/225.0f, 1.0f ));

	for (int i = 0; i < 39; i++)
	{
		creditsY[i] -= 2.5;
		if (creditsY[38] < 457.0)
		{
			creditsY[38] = 457.0;
		}
	}

	return;
}

/*========================================================================


========================================================================*/

void Credits::draw()
{
	Sprite_Transform_Draw(bigreykTexture[WALK[animationframe]], bigreykX, bigreykY, 750, 1024);

	for (int i = 0; i < 39; i++)
	{
		if (creditsY[i] > -150.0 && creditsY[i] < 1024.0)
		{
			Sprite_Transform_Draw(creditsTexture[i], 760, creditsY[i], 500, 150);
		}
	}
}

/*========================================================================


========================================================================*/

void Credits::drawFire() {

	g_pParticleSystem->SetPosition( screenToWorld(470, 700, 5.0f) );
	g_pParticleSystem->Update(0.01);

	d3ddev->SetTexture( 0, g_pParticleSystem->GetTextureObject() );
	g_pParticleSystem->Render( d3ddev );
}

