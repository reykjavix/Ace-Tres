#include "MyDirectX.h"
#include "QueenBee.h"
#include "Reyk.h"
#include "EnemyHandler.h"

extern Reyk* players[];
bool detectCollision(RECT r1, RECT r2);

const float EYESINITIALX[10] = { 250.0, 367.0, 287.0, 380.0, 307.0, 371.0, 367.0, 317.0, 214.0, 431.0 };
const float EYESINITIALY[10] = { 242.0, 300.0, 277.0, 220.0, 313.0, 343.0, 257.0, 357.0, 312.0, 262.0 };

extern int currentlevel;
extern int gamestatus;

extern CSound *beehit;
extern CSound *beefinal;
extern CSound *crack;

extern EnemyHandler *eh;

/*========================================================================


========================================================================*/

QueenBee::QueenBee()
{
	queenbeebodyTexture = LoadTexture("queenbeebody.png");
	queenbeeheadTexture = LoadTexture("queenbeehead.png");
	queenbeevisorTexture = LoadTexture("queenbeevisor.png");
	queenbeeeyesTexture = LoadTexture("queenbeeeyes.png");
	eyeTexture = LoadTexture("queenbeeeye.png");

	queenbeecounter = 0;
	status = 1;
	shakeamount = 0;
	hp = 40;
	shaking = false;

	bodyX = 4300.0;
	bodyY = -11280.0;
	headX = bodyX + 140.0;
	headY = bodyY - 311.0;
	headvelX = 6.0;
	headvelY = -40.0;
	savedX = headX;
	headrotation = 0.0;
	rotationvel = 0.0;

	visorframe = 0;
	visorX = headX + 150.0;
	visorY = headY + 121.0;
	visorvelX = -5.0;
	visorvelY = -30.0;
	visorrotation = 0.0;
	visorstatus = 1;
	savedvisorX = visorX;

	eyesframe = 0;

	for (int i = 0; i < 10; i++)
	{
		eyesX[i] = headX + EYESINITIALX[i];
		eyesY[i] = headY + EYESINITIALY[i];
		eyesvelX[i] = 0.0;
		eyesvelY[i] = 0.0;
		eyesrotation[i] = 0.0;
		eyesactive[i] = false;
	}

	visortrailing = false;
	visortrailsize = 0;
	headtrailing = false;
	headtrailsize = 0;

	for (int i = 0; i < QBEETRAILMAX; i++)
	{
		visortrailX[i] = 0.0;
		visortrailY[i] = 0.0;
		visortrailrotation[i] = 0.0;

		headtrailX[i] = 0.0;
		headtrailY[i] = 0.0;
		headtrailrotation[i] = 0.0;

		for (int j = 0; j < 10; j++)
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

QueenBee::~QueenBee()
{
	queenbeebodyTexture->Release();
	queenbeeheadTexture->Release();
	queenbeevisorTexture->Release();
	queenbeeeyesTexture->Release();
	eyeTexture->Release();
}

/*========================================================================


========================================================================*/

void QueenBee::update()
{
	if (status == 0)
	{
		return;
	}
	queenbeecounter++;

	if (shaking && status != 2)
	{
		headX = savedX + (30 * (1.0 - (shakeamount/20.0)) * sin((double)queenbeecounter));
		shakeamount++;
		if (shakeamount >= 20)
		{
			headX = savedX;
			shaking = false;
			shakeamount = 0;
		}
	}

	if (visorshaking)
	{
		visorX = savedvisorX + (30 * (1.0 - (shakeamount/20.0)) * sin((double)queenbeecounter));
		shakeamount++;
		if (shakeamount >= 20)
		{
			visorX = savedvisorX;
			visorshaking = false;
			shakeamount = 0;
		}
	}

	switch (status)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		headX += headvelX;
		headY += headvelY;
		headrotation += 0.45;
		headvelY += 1.5;
		if (queenbeecounter >= 400)
		{
			eh->killAllEnemies();
			status = 0;
			
			gamestatus = 1;
			currentlevel = 4;
		}
		break;
	}

	switch(visorstatus)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		visorX += visorvelX;
		visorY += visorvelY;
		visorvelY += 1.5;
		visorrotation -= 0.45;
		if (queenbeecounter >= 200)
		{
			visorstatus = 0;
		}
		break;
	}

	for (int i = 0; i < 10; i++)
	{
		if (eyesactive[i])
		{
			eyesX[i] += eyesvelX[i];
			eyesY[i] += eyesvelY[i];
			eyesvelY[i] += 1.0;
			eyesrotation[i] += 0.45;
			if (eyesY[i] >= -9000.0)
			{
				//eyesactive[i] = false;
			}
		}
	}

	// VISOR

	if (!visortrailing)
	{
		if (visortrailsize > 0)
			visortrailsize--;
	}
	else
	{
		if (visortrailsize < QBEETRAILMAX)
			visortrailsize++;
	}

	for (int i = QBEETRAILMAX-1; i > 0; i--)
	{
		visortrailX[i] = visortrailX[i-1];
		visortrailY[i] = visortrailY[i-1];
		visortrailrotation[i] = visortrailrotation[i-1];
	}

	visortrailX[0] = visorX;
	visortrailY[0] = visorY;
	visortrailrotation[0] = visorrotation;

	// HEAD

	if (!headtrailing)
	{
		if (headtrailsize > 0)
			headtrailsize--;
	}
	else
	{
		if (headtrailsize < QBEETRAILMAX)
			headtrailsize++;
	}

	for (int i = QBEETRAILMAX-1; i > 0; i--)
	{
		headtrailX[i] = headtrailX[i-1];
		headtrailY[i] = headtrailY[i-1];
		headtrailrotation[i] = headtrailrotation[i-1];
	}

	headtrailX[0] = headX;
	headtrailY[0] = headY;
	headtrailrotation[0] = headrotation;

	// EYES

	for (int j = 0; j < 10; j++)
	{
		if (eyesactive[j] == true && eyetrailsize[j] < QBEETRAILMAX)
			eyetrailsize[j]++;
	}

	for (int i = QBEETRAILMAX-1; i > 0; i--)
	{
		for (int j = 0; j < 10; j++)
		{
			eyetrailX[j][i] = eyetrailX[j][i-1];
			eyetrailY[j][i] = eyetrailY[j][i-1];
		}
	}

	for (int j = 0; j < 10; j++)
	{
		eyetrailX[j][0] = eyesX[j];
		eyetrailY[j][0] = eyesY[j];
		eyetrailrotation[j][0] = eyesrotation[j];
	}
}

/*========================================================================


========================================================================*/

void QueenBee::draw()
{
	Reyk* player1 = players[clientIndex];

	float xmod = player1->getXmod();
	float ymod = player1->getYmod();

	if (status == 2)
	{
		if (queenbeecounter >= 300)
		{
			if (rand()%2 == 0)
			{
				Sprite_Transform_Draw(queenbeebodyTexture, bodyX - xmod, bodyY - ymod, 1200, 1450);
			}
		}
		else
		{
			Sprite_Transform_Draw(queenbeebodyTexture, bodyX - xmod, bodyY - ymod, 1200, 1450);
		}
	}
	else
	{
		Sprite_Transform_Draw(queenbeebodyTexture, bodyX - xmod, bodyY - ymod, 1200, 1450);
	}

	for (int i = 0; i < headtrailsize; i++)
	{
		Sprite_Transform_Draw(queenbeeheadTexture, headtrailX[i] - xmod, headtrailY[i] - ymod, 750, 750, 0, 1, headtrailrotation[i], 1.0F, 1.0F,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(QBEETRAILMAX - i)/(float)QBEETRAILMAX));
	}

	Sprite_Transform_Draw(queenbeeheadTexture, headX - xmod, headY - ymod, 750, 750, 0, 1, headrotation);

	for (int i = 0; i < visortrailsize; i++)
	{
		Sprite_Transform_Draw(queenbeevisorTexture, visortrailX[i] - xmod, visortrailY[i] - ymod, 405, 330, visorframe, 9, visortrailrotation[i], 1.0F, 1.0F,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(QBEETRAILMAX - i)/(float)QBEETRAILMAX));
	}

	if (visorstatus == 1 || visorstatus == 2)
	{
		Sprite_Transform_Draw(queenbeevisorTexture, visorX - xmod, visorY - ymod, 405, 330, visorframe, 9, visorrotation);
	}
	if ((visorstatus == 0 || visorstatus == 2) && status != 2)
	{
		Sprite_Transform_Draw(queenbeeeyesTexture, headX + 206.0 - xmod, headY + 220.0 - ymod, 315, 215, eyesframe, 11);
	}

	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < eyetrailsize[j]; i++)
		{
			Sprite_Transform_Draw(eyeTexture, eyetrailX[j][i] - xmod, eyetrailY[j][i] - ymod, 100, 100, 0, 1, eyetrailrotation[j][i], 1.0F, 1.0F,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(QBEETRAILMAX - i)/(float)QBEETRAILMAX));
		}
	}

	for (int i = 0; i < 10; i++)
	{
		if (eyesactive[i])
		{
			Sprite_Transform_Draw(eyeTexture, eyesX[i] - xmod, eyesY[i] - ymod, 100, 100, 0, 1, eyesrotation[i]);
		}
	}
	
}

/*========================================================================


========================================================================*/

void QueenBee::gethit()
{
	if (shaking || visorshaking || status == 2)
	{
		return;
	}
	hp--;
	if (hp > 10)
	{
		visorshaking = true;
		shakeamount = 0;
		if (hp % 4 == 0 && visorframe < 8)
		{
			visorframe++;
		}
	}
	else if (hp == 10)
	{
		visortrailing = true;
		visorstatus = 2;
		queenbeecounter = 0;
		visorshaking = false;
		shakeamount = 0;
	}
	else if (hp < 10)
	{
		eyesframe++;
		shaking = true;
		shakeamount = 0;
		savedX = headX;

		eyesvelX[9-hp] = 5.0;
		eyesvelY[9-hp] = -20.0;
		eyesrotation[9-hp] = 0.0;
		eyesactive[9-hp] = true;
	}
	if (hp <= 0)
	{
		headtrailing = true;
		status = 2;
		shaking = false;
		visorshaking = false;
		shakeamount = 0;
		queenbeecounter = 0;
	}
}

/*========================================================================


========================================================================*/

RECT QueenBee::getHitbox()
{
	RECT hb;

	hb.left = headX + 168.0;
	hb.right = hb.left + 375.0;
	hb.top = headY + 125.0;
	hb.bottom = hb.top + 300.0;

	return hb;
}

/*========================================================================


========================================================================*/

int QueenBee::gethp()
{
	return hp;
}