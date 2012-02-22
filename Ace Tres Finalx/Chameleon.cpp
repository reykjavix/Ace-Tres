#include "MyDirectX.h"
#include "Reyk.h"
#include "Enemy.h"

extern Reyk* players[];
bool detectCollision(RECT r1, RECT r2);

const int OPENMOUTH[7] = {0, 1, 2, 3, 4, 5, 6};
const int MOVEMOUTH[4] = {7, 6, 5, 6};
const int CLOSEMOUTH[2] = {7, 8};
const int BARF[12] = {1, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2, 1};

extern LPDIRECT3DTEXTURE9 chameleonTexture;
extern LPDIRECT3DTEXTURE9 tongueTexture;

extern CSound *chameleonbarf;

/*========================================================================


========================================================================*/

Chameleon::Chameleon()
{
	//chameleonTexture = LoadTexture("chameleon.png");
	//tongueTexture = LoadTexture("tongue.png");

	status = 0;
	tonguespeed = 0.0;
	chameleoncounter = 0;
	chameleoncountermax = 100;
	chameleonshake = 0;
	playercaught = false;
	chameleonframecount = 0;

	chameleonX = -50.0;
	chameleonY = 500.0;
	tongueX = -1280.0;
	tongueY = 650.0;
	savedchameleonX = 0.0;
	direction = 0;

	frame = 0;
	startframe = 0;
	endframe = 0;
	delay = 20;	

	whichplayer = -1;
}

/*========================================================================


========================================================================*/

Chameleon::Chameleon(float x, float y, int d)
{
	chameleonTexture = LoadTexture("chameleon.png");
	tongueTexture = LoadTexture("tongue.png");

	status = 0;
	tonguespeed = 0.0;
	chameleoncounter = 0;
	chameleoncountermax = 100;
	chameleonshake = 0;
	playercaught = false;
	chameleonframecount = 0;

	chameleonX = x;
	chameleonY = y;
	tongueX = -1100.0;
	tongueY = chameleonY + 200.0;
	savedchameleonX = 0.0;
	direction = d;

	frame = 0;
	startframe = 0;
	endframe = 0;
	delay = 30;	

	whichplayer = -1;
}

/*========================================================================


========================================================================*/

void Chameleon::animate()
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
			if (status == 1)
			{
				status = 2;
				if (direction == 0)
				{
					tonguespeed = 50.0;
					tongueX = chameleonX - 1100.0;
				}
				else
				{
					tonguespeed = -50.0;
					tongueX = chameleonX + 520.0;
				}
			}
			else if (status == 3)
			{
				status = 4;
				chameleonshake = 0;
				savedchameleonX = chameleonX;
			}
			else if (status == 5)
			{
				status = 0;
			}
		}
		if (frame < startframe) frame = endframe;
	}
}

/*========================================================================


========================================================================*/

void Chameleon::update(int i)
{
	int numPlayersOutOfBounds = 0;
	for(int j = 0; j < MAX_CLIENTS; j++)
	{
		if ((abs((int)(chameleonX - players[j]->getCenterX())) > 1280.0
			|| abs((int)(chameleonY - players[j]->getCenterY())) > 2048.0)
			&& status == 0)
		{
			numPlayersOutOfBounds++;
		}
	}

	if(numPlayersOutOfBounds == MAX_CLIENTS)
		return;

	chameleonframecount++;
	switch(status)
	{
	case 0:
		frame = 0;
		endframe = 0;
		chameleoncounter++;
		if (chameleoncounter > chameleoncountermax)
		{
			chameleoncounter = 0;
			status = 1;
			endframe = 6;
			frame = 0;
		}
		break;
	case 1:
		delay = 30;
		endframe = 6;
		break;
	case 2:
		delay = 55;
		endframe = 3;

		tongueX += tonguespeed;

		if (direction == 0)
		{
			tonguespeed -= 1.1;
		}
		else
		{
			tonguespeed += 1.1;
		}

		if (tongueX <= chameleonX - 1100.0 && direction == 0)
		{
			tonguespeed = 0.0;
			status = 3;
			frame = 0;
			tongueX = chameleonX - 1100.0;
		}
		else if (tongueX >= chameleonX + 520.0 && direction == 1)
		{
			tonguespeed = 0.0;
			status = 3;
			frame = 0;
			tongueX = chameleonX + 520.0;
		}

		if(IS_SERVER)
			for(int j = 0; j < MAX_CLIENTS; j++)
				if (detectCollision(players[j]->getHitbox(), getTongueHitbox()) && players[j]->getStatus() != 8 && players[j]->getStatus() != 12 && !players[j]->getinvincible())
				{
					players[j]->setStatus(8);
					
					playercaught = true;
					whichplayer = j;

					players[j]->setMapY(tongueY - 20.0);
				}

		if (playercaught)
		{
			players[whichplayer]->setSendPacket(false);
			if (players[whichplayer]->getMapX() >= chameleonX && direction == 0)
			{
				players[whichplayer]->setMapX(players[whichplayer]->getMapX() + tonguespeed);
			}
			else if (players[whichplayer]->getMapX() <= chameleonX + 80.0 && direction == 1)
			{
				players[whichplayer]->setMapX(players[whichplayer]->getMapX() + tonguespeed);
			}
			players[whichplayer]->setSendPacket(true);
		}
		break;
	case 3:
		delay = 40;
		endframe = 1;
		break;
	case 4:
		frame = 0;
		chameleonX = (float)(savedchameleonX + 20.0 * (1.0 - ((double)chameleonshake/15.0)) * sin((double)chameleonframecount));
		chameleonshake++;

		if (chameleonshake >= 15)
		{
			frame = 0;
			status = 0;
			chameleonshake = 0;
			chameleonX = savedchameleonX;
			if (playercaught)
			{
				if(!IS_SERVER) PlaySound(chameleonbarf);
				
				players[whichplayer]->setSendPacket(false);
				players[whichplayer]->setMapX(chameleonX + 50.0);
				frame = 0;
				status = 5;
				if (direction == 0)
				{
					players[whichplayer]->Reyk_GetHit(0);
				}
				else
				{
					players[whichplayer]->Reyk_GetHit(1);
				}
				playercaught = false;
				whichplayer = -1;
				players[whichplayer]->setSendPacket(true);
			}
		}
		break;
	case 5:
		delay = 15;
		endframe = 11;
		break;
	}
}

/*========================================================================


========================================================================*/

void Chameleon::draw()
{
	Reyk* player1 = players[clientIndex];

	if (abs((int)(chameleonX - player1->getCenterX())) > 1280
			|| abs((int)(chameleonY + 290 - player1->getCenterY())) > 1024)
		{
			return;
		}

	int curframe;
	int dirdiff = 9;
	if (direction == 1)
	{
		dirdiff = 0;
	}
	switch(status)
	{
	case 0:
		curframe = 0;
		frame = 0;
		break;
	case 1:
		curframe = OPENMOUTH[frame];
		break;
	case 2:
		curframe = MOVEMOUTH[frame];
		break;
	case 3:
		curframe = CLOSEMOUTH[frame];
		break;
	case 4:
		curframe = 0;
		break;
	case 5:
		curframe = BARF[frame];
		break;
	}

	curframe += dirdiff;

	//float xmod = player1->getMapX() - 415.0;
	//float ymod = player1->getMapY() - 415.0;

	float xmod = player1->getXmod();
	float ymod = player1->getYmod();

	int tongueframe = 0;
	if (direction == 1)
	{
		tongueframe = 1;
	}

	if (playercaught)
	{
		Sprite_Transform_Draw(tongueTexture, tongueX - xmod, tongueY - ymod, 1150, 290, tongueframe);
		players[whichplayer]->Reyk_Transform_Draw();
		Sprite_Transform_Draw(chameleonTexture, chameleonX - xmod, chameleonY - ymod, 580, 580, curframe, 9);
	}
	else
	{
		Sprite_Transform_Draw(tongueTexture, tongueX - xmod, tongueY - ymod, 1150, 290, tongueframe);
		Sprite_Transform_Draw(chameleonTexture, chameleonX - xmod, chameleonY - ymod, 580, 580, curframe, 9);
	}
}

/*========================================================================


========================================================================*/

void Chameleon::move(float x, float y)
{
	chameleonX += x;
	chameleonY += y;

	tongueX += x;
	tongueY += y;
}

/*========================================================================


========================================================================*/

void Chameleon::gethit(Reyk* player1)
{
	return;
}

/*========================================================================


========================================================================*/

void Chameleon::tonguegethit()
{
	if (direction == 0 && tonguespeed > 0.0)
	{
		tonguespeed *= -1.0;
	}
	else if (direction == 1 && tonguespeed < 0.0)
	{
		tonguespeed *= -1.0;
	}
}

/*========================================================================


========================================================================*/

int Chameleon::getstatus()
{
	return status;
}

/*========================================================================


========================================================================*/

RECT Chameleon::getTongueHitbox()
{
	RECT hb;

	if (direction == 0)
	{
		hb.left = tongueX;
	}
	else
	{
		hb.left = tongueX + 150.0;
	}
	hb.right = tongueX + 1000.0;
	hb.top = tongueY + 20.0;
	hb.bottom = tongueY + 270.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT Chameleon::getTongueTipHitbox()
{
	RECT hb;

	if (direction == 0)
	{
		hb.left = tongueX + 925.0;
	}
	else
	{
		hb.left = tongueX + 15.0;
	}
	hb.right = hb.left + 210.0;
	hb.top = tongueY + 38.0;
	hb.bottom = hb.top + 210.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT Chameleon::getHitbox()
{
	RECT hb;

	if (direction == 0)
	{
		hb.left = chameleonX + 63.0;
	}
	else
	{
		hb.left = chameleonX + 200.0;
	}
	hb.right = hb.left + 317.0;
	hb.top = chameleonY + 80.0;
	hb.bottom = hb.top + 338.0;

	return hb;
}

void Chameleon::setplayercaught(bool b, int which){
	playercaught = b;
	whichplayer = which;
}
