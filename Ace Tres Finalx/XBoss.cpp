#include "MyDirectX.h"
#include "Reyk.h"
#include "XBoss.h"

extern Reyk* players[];
bool detectCollision(RECT r1, RECT r2);

extern LPDIRECT3DTEXTURE9 lightningTexture;

extern CSound *crack;
extern CSound *metalfist;
extern CSound *shatterglass;
extern CSound *fireball;
extern CSound *lightning;
extern CSound *explosion;

/*========================================================================


========================================================================*/

XBoss::XBoss()
{
	xbossTexture = LoadTexture("xboss.png");
	glassTexture = LoadTexture("glass.png");
	for (int i = 0; i < 13; i++)
	{
		switch(i)
		{
		case 0:
			crackTextures[i] = LoadTexture("cracks1.png");
			break;
		case 1:
			crackTextures[i] = LoadTexture("cracks2.png");
			break;
		case 2:
			crackTextures[i] = LoadTexture("cracks3.png");
			break;
		case 3:
			crackTextures[i] = LoadTexture("cracks4.png");
			break;
		case 4:
			crackTextures[i] = LoadTexture("cracks5.png");
			break;
		case 5:
			crackTextures[i] = LoadTexture("cracks6.png");
			break;
		case 6:
			crackTextures[i] = LoadTexture("cracks7.png");
			break;
		case 7:
			crackTextures[i] = LoadTexture("cracks8.png");
			break;
		case 8:
			crackTextures[i] = LoadTexture("cracks9.png");
			break;
		case 9:
			crackTextures[i] = LoadTexture("cracks10.png");
			break;
		case 10:
			crackTextures[i] = LoadTexture("cracks11.png");
			break;
		case 11:
			crackTextures[i] = LoadTexture("cracks12.png");
			break;
		case 12:
			crackTextures[i] = LoadTexture("cracks13.png");
			break;
		}
	}
	shardTexture = LoadTexture("shard.png");
	lefthandTexture = LoadTexture("xbosshandleft.png");
	righthandTexture = LoadTexture("xbosshandright.png");
	fireTexture = LoadTexture("purplefire.png");
	leftTexture = LoadTexture("xbossleft.png");
	rightTexture = LoadTexture("xbossright.png");
	tinyTexture = LoadTexture("xbosstiny.png");

	xbosscounter = 0;
	attackcounter = 0;
	status = 1;
	shakeamount = 0;
	hp = 25;
	shaking = false;

	lightningframe = 0;
	delay = 10;

	xbossX = 2470.0;
	xbossY -11200.0;
	savedX = xbossX;
	Ybase = -11200.0;
	xbossvelX = 4.0;

	glassX = 2500.0;

	lefthandstatus = 0;
	lefthandattackstatus = 0;
	lefthandshakeamount = 0;
	lefthandrotation = 0.0;

	lefthandX = 2300.0;
	lefthandY = -10800.0;

	righthandstatus = 0;
	righthandattackstatus = 0;
	righthandshakeamount = 0;	
	righthandrotation = 0.0;

	righthandX = 2940.0;
	righthandY = -10800.0;

	crackframe = 0;

	leftrange = 1950.0;
	rightrange = 3000.0;

	for (int i = 0; i < 7; i++)
	{
		fireballsX[i] = 0.0;
		fireballsY[i] = 0.0;
		fireballsvelX[i] = 0.0;
		fireballsvelY[i] = 0.0;
		fireballsactive[i] = false;
		fireballsrotation[i] = 0.0;
	}

	fireballframe = 0;
	numfireballs = 7;

	halvesvelY = 0.0;
	halvesrotation = 0.0;

	headtrailing = false;
	headtrailsize = 0;
	lefttrailsize = 0;
	righttrailsize = 0;

	for (int i = 0; i < XBOSSTRAILMAX; i++)
	{
		headtrailX[i] = 0.0;
		headtrailY[i] = 0.0;
	
		lefttrailX[i] = 0.0;
		lefttrailY[i] = 0.0;
		lefttrailrotation[i] = 0.0;
	
		righttrailX[i] = 0.0;
		righttrailY[i] = 0.0;
		righttrailrotation[i] = 0.0;

		for (int j = 0; j < 7; j++)
		{
			fireballtrailsize[j] = 0;
			fireballtrailX[j][i] = 0.0;
			fireballtrailY[j][i] = 0.0;
		}
	}
}

/*========================================================================


========================================================================*/

XBoss::~XBoss()
{
	xbossTexture->Release();
	glassTexture->Release();
	for (int i = 0; i < 13; i++)
	{
		crackTextures[i]->Release();
	}
	shardTexture->Release();
	lefthandTexture->Release();
	righthandTexture->Release();
	fireTexture->Release();
	leftTexture->Release();
	rightTexture->Release();
	tinyTexture->Release();
}

/*========================================================================


========================================================================*/

void XBoss::update()
{
	xbosscounter++;

	switch(status)
	{
	case 0:
		break;
	case 1:
		if (lefthandstatus == 0 && righthandstatus == 0)
		{
			attackcounter++;
		}
		if (attackcounter>= 200)
		{
			lefthandstatus = 1;
			righthandstatus = 1;
			attackcounter = 0;
		}
		xbossY = (float)(Ybase + 20.0 * sin((double)xbosscounter/25.0));
		if (shaking)
		{
			glassX = (xbossX) + 50.0 * (1.0 - (double)shakeamount/30.0) * sin((double)xbosscounter);
			shakeamount++;
			if (shakeamount >= 30)
			{
				shaking = false;
				shakeamount = 0;
			}
		}
		else
		{
			glassX = xbossX;
		}
		break;
	case 2:
		for (int i = 0; i < NUMSHARDS; i++)
		{
			shardsX[i] += shardsvelX[i];
			shardsY[i] += shardsvelY[i];
			shardsvelY[i] += 1.5;
			if (shardsvelX[i] > 0.0)
			{
				shardsrotation[i] += 0.25;
			}
			else
			{
				shardsrotation[i] -= 0.25;
			}
		}
		lefthandX -= 2.0;
		lefthandY += lefthandvelY;
		lefthandvelY += 1.2;
		lefthandrotation -= 0.2;
		righthandX += 2.0;
		righthandY += righthandvelY;
		righthandvelY += 1.2;
		righthandrotation += 0.2;
		if (xbossY < -11200.0)
		{
			xbossY += 5.0;
			if (xbossY > -11200.0)
			{
				xbossY = -11200.0;
			}
		}
		else if (xbossY > -11200.0)
		{
			xbossY -= 5.0;
			if (xbossY < -11200.0)
			{
				xbossY = -11200.0;
			}
		}
		if (xbosscounter >= 100)
		{
			status = 3;
			delay = 25;
			shaking = false;
			attackcounter = 0;
		}
		break;
	case 3:
		if(IS_SERVER)
		{
			for(int i = 0; i < MAX_CLIENTS; i++)
				if (detectCollision(getAttackHitbox(), players[i]->getHitbox()))
				{
					if (xbossX + 310.0 > players[i]->getCenterX())
					{
						players[i]->Reyk_GetHit(1);
					}
					else
					{
						players[i]->Reyk_GetHit(0);
					}
				}
		}
		attackcounter++;
		if (attackcounter >= 150)
		{
			barfFire();
			attackcounter = 0;
		}

		xbossY = (float)(Ybase + 150.0 * sin((double)xbosscounter/25.0));
		savedX += xbossvelX;

		if (xbossvelX > 0.0)
		{
			if (xbossX >= rightrange)
			{
				xbossvelX *= -1.0;
			}
		}
		else if (xbossvelX < 0.0)
		{
			if (xbossX <= leftrange)
			{
				xbossvelX *= -1.0;
			}
		}

		if (shaking)
		{
			xbossX = (savedX) + 40.0 * (1.0 - (double)shakeamount/30.0) * sin((double)xbosscounter);
			shakeamount++;
			if (shakeamount >= 30)
			{
				shaking = false;
				shakeamount = 0;
			}
		}
		else
		{
			xbossX = savedX;
		}
		if(IS_SERVER)
			for (int i = 0; i < numfireballs; i++)
			{
				if (fireballsactive[i])
				{
					for(int j = 0; j < MAX_CLIENTS; j++)
						if (detectCollision(getfireballHitbox(i), players[j]->getHitbox()))
						{
							if (fireballsX[i] + 75.0 > players[j]->getCenterX())
							{
								players[j]->Reyk_GetHit(1);
							}
							else
							{
								players[j]->Reyk_GetHit(0);
							}
						}

					fireballsX[i] += fireballsvelX[i];
					fireballsY[i] += fireballsvelY[i];

					if (fireballsX[i] >= 4000.0 || fireballsX[i] <= 1500.0
						|| fireballsY[i] >= -9000.0)
					{
						fireballsactive[i] = false;
					}
				}
			}
		break;
	case 4:
		if (savedX > 2470.0)
		{
			savedX -= 2.0;
			if (savedX < 2470.0)
			{
				savedX = 2470.0;
			}
		}
		else if (savedX < 2470.0)
		{
			savedX += 2.0;
			if (savedX > 2470.0)
			{
				savedX = 2470.0;
			}
		}

		xbossX = savedX + (20.0 * sin((double)xbosscounter*5.0));
		
		if (xbossY < -10800.0)
		{
			xbossY += 2.0;
			if (xbossY > -10800.0)
			{
				xbossY = -10800.0;
			}
		}

		if (savedX == 2470.0 && xbossY == -10800.0)
		{
			status = 5;
			attackcounter = 0;
			leftX = xbossX + 22.0;
			leftY = xbossY + 107.0;
			rightX = xbossX + 332.0;
			rightY = xbossY + 107.0;
		}
		break;
	case 5:
		attackcounter++;

		if (attackcounter >= 50)
		{
			status = 6;
			attackcounter = 0;
		}
		break;
	case 6:
		attackcounter++;

		leftY -= 1.0;
		rightY += 1.0;

		if (attackcounter >= 50)
		{
			status = 7;
			halvesvelY = -30.0;
			tinyX = xbossX + 22.0 + 252.0;
			tinyY = xbossY + 107.0 + 150.0;
			tinysavedY = tinyY;
		}

		break;
	case 7:
		leftX -= 8.0;
		leftY += halvesvelY;
		rightX += 8.0;
		rightY += halvesvelY;

		halvesvelY += 1.5;

		halvesrotation += 0.35;

		tinyY = (float)(tinysavedY + 20.0 * sin((double)xbosscounter/25.0));
	}

	if (status < 2)
	{
		updateHands();
	}

	// HEAD

	if (!headtrailing)
	{
		if (headtrailsize > 0)
			headtrailsize--;
	}
	else
	{
		if (headtrailsize < XBOSSTRAILMAX)
			headtrailsize++;
	}

	for (int i = XBOSSTRAILMAX-1; i > 0; i--)
	{
		headtrailX[i] = headtrailX[i-1];
		headtrailY[i] = headtrailY[i-1];
	}

	headtrailX[0] = xbossX;
	headtrailY[0] = xbossY;

	// LEFT

	if (lefttrailsize < XBOSSTRAILMAX)
		lefttrailsize++;

	for (int i = XBOSSTRAILMAX-1; i > 0; i--)
	{
		lefttrailX[i] = lefttrailX[i-1];
		lefttrailY[i] = lefttrailY[i-1];
		lefttrailrotation[i] = lefttrailrotation[i-1];
	}
	
	if (status < 5)
	{
		lefttrailrotation[0] = lefthandrotation;
		lefttrailX[0] = lefthandX;
		lefttrailY[0] = lefthandY;
	}
	else
	{
		lefttrailrotation[0] = -1.0 * halvesrotation;
		lefttrailX[0] = leftX;
		lefttrailY[0] = leftY;
	}

	// RIGHT

	if (righttrailsize < XBOSSTRAILMAX)
		righttrailsize++;

	for (int i = XBOSSTRAILMAX-1; i > 0; i--)
	{
		righttrailX[i] = righttrailX[i-1];
		righttrailY[i] = righttrailY[i-1];
		righttrailrotation[i] = righttrailrotation[i-1];
	}
	
	if (status < 5)
	{
		righttrailrotation[0] = righthandrotation;
		righttrailX[0] = righthandX;
		righttrailY[0] = righthandY;
	}
	else
	{
		righttrailrotation[0] = halvesrotation;
		righttrailX[0] = rightX;
		righttrailY[0] = rightY;
	}

	// FIREBALLS

	for (int j = 0; j < 7; j++)
	{
		if (fireballsactive[j] == true && fireballtrailsize[j] < XBOSSTRAILMAX)
			fireballtrailsize[j]++;
	}

	for (int i = XBOSSTRAILMAX-1; i > 0; i--)
	{
		for (int j = 0; j < 7; j++)
		{
			fireballtrailX[j][i] = fireballtrailX[j][i-1];
			fireballtrailY[j][i] = fireballtrailY[j][i-1];
		}
	}

	for (int j = 0; j < 7; j++)
	{
		fireballtrailX[j][0] = fireballsX[j];
		fireballtrailY[j][0] = fireballsY[j];
	}
}

/*========================================================================


========================================================================*/

void XBoss::updateHands()
{
	/*========================================================================
				UPDATE LEFT
	========================================================================*/
	if (IS_SERVER && status < 2)
	{
		for(int i = 0; i < MAX_CLIENTS; i++)
			if (detectCollision(getleftfistHitbox(), players[i]->getHitbox()))
			{
				if (lefthandX + 185.0 > players[i]->getCenterX())
				{
					players[i]->Reyk_GetHit(1);
				}
				else
				{
					players[i]->Reyk_GetHit(0);
				}
			}
	}

	int leftshakemax = 15;

	switch(lefthandstatus)
	{
	case 0:
		lefthandY = -10800.0 + (30 * sin((double)xbosscounter/15.0));
		break;
	case 1:
		lefthandY -= 6.0;
		if (lefthandY <= -11100.0)
		{
			lefthandY = -11100.0;
			lefthandstatus = 2;
			lefthandshakeamount = 0;
			savedlefthandX = lefthandX;
		}
		break;
	case 2:
		lefthandX = (float)(2300.0 - 50.0 * (1.0 - ((double)lefthandshakeamount/30.0)) * sin((double)xbosscounter));
		lefthandshakeamount++;
		if (lefthandshakeamount >= 30)
		{
			lefthandX = savedlefthandX;
			lefthandshakeamount = 0;
			lefthandstatus = 3;
			if(!IS_SERVER) PlaySound(metalfist);
		}
		break;
	case 3:
		lefthandY += 100.0;
		if (lefthandY >= -10150.0)
		{
			if(!IS_SERVER) PlaySound(lightning);
			lefthandY = -10150.0;
			lefthandstatus = 4;
			lefthandshakeamount = 0;
			lefthandattackstatus = 0;
		}
		break;
	case 4:
		if(IS_SERVER)
			for(int i = 0; i < MAX_CLIENTS; i++)
				if (detectCollision(getleftlightning1Hitbox(), players[i]->getHitbox()))
				{
					if (lefthandX + 185.0 > players[i]->getCenterX())
					{
						players[i]->Reyk_GetHit(1);
					}
					else
					{
						players[i]->Reyk_GetHit(0);
					}
				}
				else if ( (lefthandattackstatus == 0 || lefthandattackstatus == 2) && detectCollision(getleftlightning2Hitbox(), players[i]->getHitbox()))
				{
					if (lefthandX + 185.0 > players[i]->getCenterX())
					{
						players[i]->Reyk_GetHit(1);
					}
					else
					{
						players[i]->Reyk_GetHit(0);
					}
				}
				else if ( lefthandattackstatus == 0 && detectCollision(getleftlightning3Hitbox(), players[i]->getHitbox()))
				{
					if (lefthandX + 185.0 > players[i]->getCenterX())
					{
						players[i]->Reyk_GetHit(1);
					}
					else
					{
						players[i]->Reyk_GetHit(0);
					}
				}

		if (lefthandattackstatus == 1)
		{
			leftshakemax = 60;
		}
		else if (lefthandattackstatus == 0)
		{
			leftshakemax = 15;
		}
		else
		{
			leftshakemax = 10;
		}

		lefthandX = (float)(savedlefthandX - 50.0 * (1.0 - ((double)lefthandshakeamount/(float)leftshakemax)) * sin((double)xbosscounter));
		lefthandshakeamount++;
		if (lefthandshakeamount >= leftshakemax)
		{
			lefthandX = savedlefthandX;
			lefthandshakeamount = 0;
			if (lefthandattackstatus == 0)
			{
				lefthandstatus = 5;
			}
			else if (lefthandattackstatus == 1)
			{
				lefthandstatus = 6;
				if(!IS_SERVER) PlaySound(metalfist);
			}
			else
			{
				lefthandstatus = 7;
			}
		}
		break;
	case 5:
		lefthandX -= 80.0;
		if (lefthandX <= 1850.0)
		{
			if(!IS_SERVER) PlaySound(lightning);
			lefthandX = 1850.0;
			savedlefthandX = 1850.0;
			lefthandstatus = 4;
			lefthandshakeamount = 0;
			lefthandattackstatus = 1;
		}
		break;
	case 6:
		lefthandX += 80.0;
		if (lefthandX >= 2450.0)
		{
			if(!IS_SERVER) PlaySound(lightning);
			lefthandX = 2450.0;
			savedlefthandX = 2450.0;
			lefthandstatus = 4;
			lefthandshakeamount = 0;
			lefthandattackstatus = 2;
		}
		break;
	case 7:
		if (lefthandX < 2300.0)
		{
			lefthandX += 30.0;
			if (lefthandX > 2300.0)
			{
				lefthandX = 2300.0;
			}
		}
		else if (lefthandX > 2300.0)
		{
			lefthandX -= 30.0;
			if (lefthandX < 2300.0)
			{
				lefthandX = 2300.0;
			}
		}

		if (lefthandY > -10800.0)
		{
			lefthandY -= 20.0;
			if (lefthandY < -10800.0)
			{
				lefthandY = -10800.0;
			}
		}

		if (lefthandX == 2300.0 && lefthandY == -10800.0)
		{
			lefthandstatus = 0;
		}
		break;
	}

	/*========================================================================
				UPDATE RIGHT
	========================================================================*/

	if (IS_SERVER && status < 2)
	{
		for(int i = 0; i < MAX_CLIENTS; i++)
			if ( detectCollision(getrightfistHitbox(), players[i]->getHitbox()))
			{
				if (righthandX + 185.0 > players[i]->getCenterX())
				{
					players[i]->Reyk_GetHit(1);
				}
				else
				{
					players[i]->Reyk_GetHit(0);
				}
			}
	}

	int rightshakemax = 15;

	switch(righthandstatus)
	{
	case 0:
		righthandY = -10800.0 + (30 * sin((double)xbosscounter/15.0));
		break;
	case 1:
		righthandY -= 6.0;
		if (righthandY <= -11100.0)
		{
			righthandY = -11100.0;
			righthandstatus = 2;
			righthandshakeamount = 0;
			savedrighthandX = righthandX;
		}
		break;
	case 2:
		righthandX = (float)(savedrighthandX + 50.0 * (1.0 - ((double)righthandshakeamount/30.0)) * sin((double)xbosscounter));
		righthandshakeamount++;
		if (righthandshakeamount >= 30)
		{
			righthandX = savedrighthandX;
			righthandshakeamount = 0;
			righthandstatus = 3;
		}
		break;
	case 3:
		righthandY += 100.0;
		if (righthandY >= -10150.0)
		{
			righthandY = -10150.0;
			righthandstatus = 4;
			righthandshakeamount = 0;
			righthandattackstatus = 0;
		}
		break;
	case 4:
		if(IS_SERVER)
			for(int i = 0; i < MAX_CLIENTS; i++)
				if ( detectCollision(getrightlightning1Hitbox(), players[i]->getHitbox()))
				{
					if (righthandX + 185.0 > players[i]->getCenterX())
					{
						players[i]->Reyk_GetHit(1);
					}
					else
					{
						players[i]->Reyk_GetHit(0);
					}
				}
				else if ( (righthandattackstatus == 0 || righthandattackstatus == 2) && detectCollision(getrightlightning2Hitbox(), players[i]->getHitbox()))
				{
					if (righthandX + 185.0 > players[i]->getCenterX())
					{
						players[i]->Reyk_GetHit(1);
					}
					else
					{
						players[i]->Reyk_GetHit(0);
					}
				}
				else if ( righthandattackstatus == 0 && detectCollision(getrightlightning3Hitbox(), players[i]->getHitbox()))
				{
					if (righthandX + 185.0 > players[i]->getCenterX())
					{
						players[i]->Reyk_GetHit(1);
					}
					else
					{
						players[i]->Reyk_GetHit(0);
					}
				}

		if (righthandattackstatus == 1)
		{
			rightshakemax = 60;
		}
		else if (righthandattackstatus == 0)
		{
			rightshakemax = 15;
		}
		else
		{
			rightshakemax = 10;
		}

		righthandX = (float)(savedrighthandX + 50.0 * (1.0 - ((double)righthandshakeamount/(float)rightshakemax)) * sin((double)xbosscounter));
		righthandshakeamount++;
		if (righthandshakeamount >= rightshakemax)
		{
			righthandX = savedrighthandX;
			righthandshakeamount = 0;
			if (righthandattackstatus == 0)
			{
				righthandstatus = 6;
			}
			else if (righthandattackstatus == 1)
			{
				righthandstatus = 5;
			}
			else
			{
				righthandstatus = 7;
			}
		}
		break;
	case 5:
		righthandX -= 80.0;
		if (righthandX <= 2790.0)
		{
			righthandX = 2790.0;
			savedrighthandX = 2790.0;
			righthandstatus = 4;
			righthandshakeamount = 0;
			righthandattackstatus = 2;
		}
		break;
	case 6:
		righthandX += 80.0;
		if (righthandX >= 3390.0)
		{
			righthandX = 3390.0;
			savedrighthandX = 3390.0;
			righthandstatus = 4;
			righthandshakeamount = 0;
			righthandattackstatus = 1;
		}
		break;
	case 7:
		if (righthandX < 2940.0)
		{
			righthandX += 30.0;
			if (righthandX > 2940.0)
			{
				righthandX = 2940.0;
			}
		}
		else if (righthandX > 2940.0)
		{
			righthandX -= 30.0;
			if (righthandX < 2940.0)
			{
				righthandX = 2940.0;
			}
		}

		if (righthandY > -10800.0)
		{
			righthandY -= 20.0;
			if (righthandY < -10800.0)
			{
				righthandY = -10800.0;
			}
		}

		if (righthandX == 2940.0 && righthandY == -10800.0)
		{
			righthandstatus = 0;
		}
		break;
	}
}

/*========================================================================


========================================================================*/

void XBoss::barfFire()
{
	if(!IS_SERVER) PlaySound(fireball);

	for (int i = 0; i < numfireballs; i++)
	{
		fireballsX[i] = xbossX + 265.0;
		fireballsY[i] = xbossY + 275.0;
		fireballsactive[i] = true;
	}

	fireballsvelX[0] = -10.0;
	fireballsvelY[0] = 5.0;
	fireballsrotation[0] = 0.80;

	fireballsvelX[1] = -9.0;
	fireballsvelY[1] = 8.0;
	fireballsrotation[1] = 0.65;

	fireballsvelX[2] = -5.0;
	fireballsvelY[2] = 12.0;
	fireballsrotation[2] = 0.35;

	fireballsvelX[3] = 0.0;
	fireballsvelY[3] = 15.0;
	fireballsrotation[3] = 0.0;

	fireballsvelX[4] = 5.0;
	fireballsvelY[4] = 12.0;
	fireballsrotation[4] = -0.35;

	fireballsvelX[5] = 9.0;
	fireballsvelY[5] = 8.0;
	fireballsrotation[5] = -0.65;

	fireballsvelX[6] = 10.0;
	fireballsvelY[6] = 5.0;
	fireballsrotation[6] = -0.80;
}

/*========================================================================


========================================================================*/

void XBoss::animate()
{
	if (!(lefthandstatus == 4) && !(righthandstatus == 4) && status < 3)
	{
		return;
	}
	if ((int)GetTickCount() > starttime + delay)
	{
		starttime = GetTickCount();

		lightningframe += 1;

		if (lightningframe > 4)
		{
			lightningframe = 0;
		}
	}
}

/*========================================================================


========================================================================*/

void XBoss::draw()
{
	if (status == 0)
	{
		return;
	}

	D3DCOLOR glasscolor = D3DCOLOR_ARGB(175, 255, 255, 255);

	Reyk* player1 = players[clientIndex];

	float xmod = player1->getXmod();
	float ymod = player1->getYmod();

	if (status == 3)
	{
		for (int j = 0; j < 7; j++)
		{
			for (int i = 0; i < fireballtrailsize[j]; i++)
			{
				Sprite_Transform_Draw(fireTexture, fireballtrailX[j][i] - xmod, fireballtrailY[j][i] - ymod, 150, 180, lightningframe, 5, fireballsrotation[j], 1.0F, 1.0F,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(XBOSSTRAILMAX - i)/(float)XBOSSTRAILMAX));
			}
		}

		for (int i = 0; i < numfireballs; i++)
		{
			if (fireballsactive[i])
			{
				Sprite_Transform_Draw(fireTexture, fireballsX[i] - xmod, fireballsY[i] - ymod, 150, 180, lightningframe, 5, fireballsrotation[i]);
			}
		}
	}

	if (status < 5)
	{
		for (int i = 0; i < headtrailsize; i++)
		{
			Sprite_Transform_Draw(xbossTexture, headtrailX[i] + 22.0 - xmod, headtrailY[i] + 107.0 - ymod, 620, 500, 0, 1, 0.0F, 1.0F, 1.0F,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(XBOSSTRAILMAX - i)/(float)XBOSSTRAILMAX));
		}

		Sprite_Transform_Draw(xbossTexture, xbossX + 22.0 - xmod, xbossY + 107.0 - ymod, 620, 500);
	}
	else
	{
		for (int i = 0; i < lefttrailsize; i++)
		{
			Sprite_Transform_Draw(leftTexture, lefttrailX[i] - xmod, lefttrailY[i] - ymod, 310, 500, 0, 1, lefttrailrotation[i], 1.0F, 1.0F,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(XBOSSTRAILMAX - i)/(float)XBOSSTRAILMAX));
		}
		Sprite_Transform_Draw(leftTexture, leftX - xmod, leftY - ymod, 310, 500, 0, 1, -1.0 * halvesrotation);

		for (int i = 0; i < righttrailsize; i++)
		{
			Sprite_Transform_Draw(rightTexture, righttrailX[i] - xmod, righttrailY[i] - ymod, 310, 500, 0, 1, righttrailrotation[i], 1.0F, 1.0F,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(XBOSSTRAILMAX - i)/(float)XBOSSTRAILMAX));
		}
		Sprite_Transform_Draw(rightTexture, rightX - xmod, rightY - ymod, 310, 500, 0, 1, halvesrotation);
	}

	if (status == 7)
	{
		Sprite_Transform_Draw(tinyTexture, tinyX - xmod, tinyY - ymod, 124, 100);
	}

	if (status == 1)
	{
		Sprite_Transform_Draw(glassTexture, glassX - xmod, xbossY - ymod, 670, 670, 0, 1, 0.0, 1.0, 1.0, glasscolor);
		Sprite_Transform_Draw(crackTextures[crackframe], glassX - xmod, xbossY - ymod, 670, 670);
	}
	else if (status == 2)
	{
		for (int i = 0; i < NUMSHARDS; i++)
		{
			Sprite_Transform_Draw(shardTexture, shardsX[i] - xmod, shardsY[i] - ymod, 100, 100, 0, 1, shardsrotation[i]);
		}
	}

	if (righthandstatus == 4 && righthandattackstatus == 0)
	{
		Sprite_Transform_Draw(lightningTexture, righthandX - xmod - 480, righthandY - ymod - 230, 250, 780, lightningframe, 5, 1.571);
	}
	if (lefthandstatus == 4 && lefthandattackstatus != 1)
	{
		Sprite_Transform_Draw(lightningTexture, lefthandX - xmod - 480, lefthandY - ymod - 230, 250, 780, lightningframe, 5, 1.571);
	}
	
	if (lefthandstatus == 4 && lefthandattackstatus == 0)
	{
		Sprite_Transform_Draw(lightningTexture, lefthandX - xmod + 600, lefthandY - ymod - 230, 250, 780, lightningframe, 5, -1.571);
	}
	if (righthandstatus == 4 && righthandattackstatus != 1)
	{
		Sprite_Transform_Draw(lightningTexture, righthandX - xmod + 600, righthandY - ymod - 230, 250, 780, lightningframe, 5, -1.571);
	}
	
	if (lefthandstatus == 4)
	{
		Sprite_Transform_Draw(lightningTexture, lefthandX - xmod + 50, lefthandY - ymod - 750, 250, 780, lightningframe, 5, 3.141);
	}	

	if (righthandstatus == 4)
	{
		Sprite_Transform_Draw(lightningTexture, righthandX - xmod + 50, righthandY - ymod - 750, 250, 780, lightningframe, 5, 3.141);	
	}

	if (status < 5)
	{
		for (int i = 0; i < lefttrailsize; i++)
		{
			Sprite_Transform_Draw(lefthandTexture, lefttrailX[i] - xmod, lefttrailY[i] - ymod, 370, 370, 0, 1, lefttrailrotation[i], 1.0F, 1.0F,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(XBOSSTRAILMAX - i)/(float)XBOSSTRAILMAX));
		}

		Sprite_Transform_Draw(lefthandTexture, lefthandX - xmod, lefthandY - ymod, 370, 370, 0, 1, lefthandrotation);

		for (int i = 0; i < righttrailsize; i++)
		{
			Sprite_Transform_Draw(righthandTexture, righttrailX[i] - xmod, righttrailY[i] - ymod, 370, 370, 0, 1, righttrailrotation[i], 1.0F, 1.0F,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(XBOSSTRAILMAX - i)/(float)XBOSSTRAILMAX));
		}

		Sprite_Transform_Draw(righthandTexture, righthandX - xmod, righthandY - ymod, 370, 370, 0, 1, righthandrotation);
	}
}

/*========================================================================


========================================================================*/

void XBoss::gethit(int rand670, int rand2, int rand100)
{
	if (shaking)
	{
		return;
	}
	hp--;

	if (hp <= 0)
	{
		status = 4;
		savedX = xbossX;
		lefttrailsize = 0;
		righttrailsize = 0;
		return;
	}

	if (status == 1)
	{
		crackframe++;
		if (crackframe >= 13)
		{
			for (int i = 0; i < NUMSHARDS; i++)
			{
				shardsX[i] = xbossX + (rand()%670);
				shardsY[i] = xbossY + (rand()%670);
				int negmod = 1;
				if (rand()%2 == 0)
				{
					negmod = -1;
				}
				shardsvelX[i] = negmod * 5.0 + (-20.0 * ((rand()%100)/100));
				shardsvelY[i] = -20.0 + (-300.0 * ((rand()%100)/100));
				shardsrotation[i] = 0.0;
			}
			lefthandrotation = 0.0;
			righthandrotation = 0.0;
			lefthandvelY = -30.0;
			righthandvelY = -30.0;
			status = 2;
			headtrailing = true;
			xbosscounter = 0;
		}
	}
	
	shaking = true;
	shakeamount = 0;
	return;
}

/*========================================================================


========================================================================*/

RECT XBoss::getHitbox()
{
	RECT hb;

	hb.left = xbossX;
	hb.right = hb.left + 670;
	hb.top = xbossY;
	hb.bottom = hb.top + 670;

	return hb;
}

/*========================================================================


========================================================================*/

RECT XBoss::getleftfistHitbox()
{
	RECT hb;

	hb.left = lefthandX + 44.0;
	hb.right = hb.left + 303.0;
	hb.top = lefthandY + 36.0;
	hb.bottom = hb.top + 315.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT XBoss::getleftlightning1Hitbox()
{
	RECT hb;

	hb.left = lefthandX + 31.0;
	hb.right = hb.left + 180.0;
	hb.top = lefthandY - 717.0;
	hb.bottom = hb.top + 750.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT XBoss::getleftlightning2Hitbox()
{
	RECT hb;

	hb.left = lefthandX - 702.0;
	hb.right = hb.left + 750.0;
	hb.top = lefthandY + 80.0;
	hb.bottom = hb.top + 160.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT XBoss::getleftlightning3Hitbox()
{
	RECT hb;

	hb.left = lefthandX + 320.0;
	hb.right = hb.left + 750.0;
	hb.top = lefthandY + 80.0;
	hb.bottom = hb.top + 160.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT XBoss::getrightfistHitbox()
{
	RECT hb;

	hb.left = righthandX + 44.0;
	hb.right = hb.left + 303.0;
	hb.top = righthandY + 36.0;
	hb.bottom = hb.top + 315.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT XBoss::getrightlightning1Hitbox()
{
	RECT hb;

	hb.left = righthandX + 100.0;
	hb.right = hb.left + 180.0;
	hb.top = righthandY - 717.0;
	hb.bottom = hb.top + 750.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT XBoss::getrightlightning2Hitbox()
{
	RECT hb;

	hb.left = righthandX - 702.0;
	hb.right = hb.left + 750.0;
	hb.top = righthandY + 80.0;
	hb.bottom = hb.top + 160.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT XBoss::getrightlightning3Hitbox()
{
	RECT hb;

	hb.left = righthandX + 320.0;
	hb.right = hb.left + 750.0;
	hb.top = righthandY + 80.0;
	hb.bottom = hb.top + 160.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT XBoss::getfireballHitbox(int f)
{
	RECT hb;

	hb.left = fireballsX[f] + 20.0;
	hb.right = hb.left + 130.0;
	hb.top = fireballsY[f] + 20.0;
	hb.bottom = hb.top + 160.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT XBoss::getAttackHitbox()
{
	RECT hb;

	hb.left = xbossX + 94.0;
	hb.right = hb.left + 440.0;
	hb.top = xbossY + 22.0;
	hb.bottom = hb.top + 465.0;

	return hb;
}

/*========================================================================


========================================================================*/

RECT XBoss::getFaceHitbox()
{
	RECT hb;

	hb.left = xbossX + 120.0;
	hb.right = hb.left + 387.0;
	hb.top = xbossY + 148.0;
	hb.bottom = hb.top + 205.0;

	return hb;
}

/*========================================================================


========================================================================*/

int XBoss::getstatus()
{
	return status;
}

/*========================================================================


========================================================================*/

void XBoss::setstatus(int s)
{
	status = s;
}

/*========================================================================


========================================================================*/

RECT XBoss::getTinyHitbox()
{
	RECT hb;

	hb.left = tinyX;
	hb.right = tinyX + 124.0;
	hb.top = tinyY;
	hb.bottom = tinyY + 100.0;

	return hb;
}

/*========================================================================


========================================================================*/

int XBoss::gethp()
{
	return hp;
}