#include "MyDirectX.h"
#include "EnemyHandler.h"
#include "PlatformHandler.h"
#include "Reyk.h"

const int NUMENEMIES = 40 + 44 + 7 + 1 + 11 + 18 + 32; //44;

extern MODEL* projectile0;
extern MODEL* asteriskFruit;
extern MODEL* ringBerry;
extern MODEL* XFruit;
extern MODEL* OFruit;
extern MODEL* spikedAcorn;

extern int gamestatus;

extern int enemiesmin;
extern int enemiesmax;

extern Reyk *players[];

float enemiesX[NUMENEMIES] = {
	
	// Level 1
	900.0, 1200.0, 2000.0, 2500.0, 3500.0,
	3600.0, 0.0, 5300.0, 5700.0, 6000.0,
	7000.0, 7300.0, 7300.0, 8200.0, 8400.0,
	8600.0, 9550.0, 10250.0, 9700.0, 9800.0,

	// Level 2
	1500.0, 4000.0, 2800.0, 3200.0, 3200.0,
	3840.0, 4160.0, 3850.0, 4460.0, 3180.0,
	2100.0, 2890.0, 2080.0, 2900.0, 4400.0,
	3740.0, 3740.0, 4400.0, 1700.0, 3800.0,

	// Level 3
	2750.0, 3560.0, 650.0, 530.0, 650.0,
	-1270.0, -430.0, -970.0, -480.0, 1170.0, 
	1730.0, 3030.0, 2930.0, 4430.0, 4880.0,
	5330.0, 6060.0, 5130.0, 5630.0, 3030.0,
	3280.0, 1530.0, 2450.0, 1200.0, 3200.0,
	-1000.0, 2000.0, -1000.0, -1500.0, -1500.0,
	-600.0, 1000.0, 1330.0, 0.0, 4500.0,
	1500.0, 3000.0, 1700.0, 3300.0, 6200.0,
	4900.0, 6200.0, 4200.0, 5200.0,

	// Queen Bee Boss
	3900.0, 6000.0, 3800.0, 3000.0, 7000.0,
	6000.0, 3900.0,

	// Ape Boss's Small Ape
	-1000.0,
	
	// Level 1 powerups
	1000.0, 1600.0, 2100.0, 3200.0, 5600.0,
	6100.0, 6900.0, 7300.0, 8950.0, 9950.0,
	10400.0,

	// Level 2 powerups
	12200.0, 12700.0, 13200.0, 14700.0, 15200.0,
	15650.0, 16300.0, 16500.0, 15100.0, 15200.0,
	15000.0, 15900.0, 16400.0, 17050.0, 16600.0,
	16000.0, 15200.0, 14400.0,

	// Level 3 powerups
	11800.0, 11200.0, 10200.0, 9650.0, 8900.0,
	9100.0, 8800.0, 8500.0, 8200.0, 7900.0,
	7600.0, 6500.0, 6500.0, 6500.0, 5900.0,
	5100.0, 4900.0, 4950.0, 5600.0, 6100.0,
	6700.0, 8150.0, 8600.0, 10100.0, 10700.0,
	11200.0, 11200.0, 11300.0, 10700.0, 7700.0,
	7200.0, 6700.0

};

	

float enemiesY[NUMENEMIES] = {
	
	// Level 1
	670.0, 670.0, 230.0, 30.0, 670.0,
	670.0, 0.0, 300.0, 250.0, 100.0,
	400.0, 100.0, 200.0, 600.0, 750.0,
	150.0, -50.0, 350.0, 700.0, 700.0,

	// Level 2
	1070.0, 1070.0, 1240.0, 1240.0, 840.0,
	540.0, 40.0, -560.0, -1060.0, -2160.0,
	-1370.0, -1970.0, -2570.0, -3170.0, -3570.0,
	-4170.0, -4770.0, -4770.0, -5220.0, -5220.0,

	// Level 3
	1230.0, 530.0, -570.0, 230.0, 1030.0,
	-270.0, -270.0, -970.0, -1570.0, -1670.0, 
	-1370.0, -1470.0, -2170.0, -1770.0, -1770.0,
	-1770.0, -1770.0, -2670.0, -4270.0, -4570.0,
	-3970.0, -3770.0, 1030.0, -750.0, -750.0,
	330.0, 730.0, 1130.0, -750.0, -1800.0,
	-2300.0, -2300.0, -2270.0, -1500.0, -1500.0,
	-1500.0, -1500.0, -2300.0, -2300.0, -2300.0,
	-2800.0, -3600.0, -4500.0, -4500.0,

	// Queen Bee Boss
	-10165.0, -10165.0, -10500.0, -11000.0, -11600.0,
	-10800.0, -11300.0,

	// Ape Boss's Small Ape
	-1000.0,
	
	// Level 1 powerups
	550.0, 550.0, 550.0, 550.0, 550.0, 
	350.0, 600.0, 400.0, 700.0, 500.0, 
	300.0,

	// Level 2 powerups
	600.0, 600.0, 600.0, 600.0, 1200.0,
	800.0, 200.0, -1200.0, -1100.0, -1800.0,
	-3000.0, -3300.0, -3300.0, -3800.0, -4500.0,
	-5100.0, -5100.0, -5100.0,

	// Level 3 powerups
	-5600.0, -5600.0, -5600.0, -5600.0, -6200.0,
	-7100.0, -7100.0, -7100.0, -7100.0, -7100.0, 
	-7100.0, -5700.0, -6600.0, -7100.0, -6900.0,
	-7100.0, -8200.0, -8800.0, -8800.0, -8800.0,
	-8800.0, -8900.0, -8900.0, -8600.0, -8600.0,
	-8600.0, -10400.0, -11100.0, -11100.0, -11000.0,
	-11000.0, -11000.0
};

	

int enemytypes[NUMENEMIES] = {

	// Level 1
	1, 0, 0, 0, 0,
	0, 0, 1, 1, 0,
	0, 0, 1, 0, 0,
	1, 1, 1, 0, 0,

	// Level 2
	3, 3, 0, 0, 1,
	1, 1, 1, 1, 1,
	2, 2, 2, 2, 2,
	2, 2, 2, 3, 3,

	// Level 3
	5, 5, 5, 5, 5,
	5, 5, 5, 5, 5,
	5, 5, 5, 5, 5,
	5, 5, 5, 5, 5,
	5, 5, 4, 6, 6,
	4, 4, 4, 6, 6,
	6, 6, 5, 4, 4,
	4, 4, 6, 6, 6,
	4, 4, 6, 6,

	// Queen Bee Boss
	5, 5, 4, 4, 4,
	6, 6,

	// Ape Boss's Small Ape
	0,
	
	// Level 1 powerups
	10, 10, 10, 10, 10, 
	10, 10, 10, 10, 10, 
	10,

	// Level 2 powerups
	10, 10, 10, 10, 10, 
	10, 10, 10, 10, 10, 
	10, 10, 10, 10, 10, 
	10, 10, 10,

	// Level 3 powerups
	10, 10, 10, 10, 10, 
	10, 10, 10, 10, 10, 
	10, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 
	10, 10, 10, 10, 10, 
	10, 10, 10, 10, 10,
	10, 10

}; 

	

int enemydirections[NUMENEMIES] = {

	// Level 1
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	// Level 2
	0, 1, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 1, 0, 1, 1,
	0, 0, 1, 0, 1,

	// Level 3
	0, 1, 1, 0, 1,
	0, 1, 0, 1, 0,
	1, 0, 0, 0, 1,
	0, 1, 0, 0, 0,
	1, 0, 0, 0, 1,
	0, 1, 0, 0, 1,
	0, 1, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 0, 1,

	// Queen Bee Boss
	0, 1, 0, 0, 1,
	1, 0,

	// Ape Boss's Small Ape
	0, 

	// Level 1 powerups
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0,

	// Level 2 powerups
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0,

	// Level 3 powerups
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0,
	0, 0

};

float enemyleftranges[NUMENEMIES] = {

	// Level 1
	0.0, 0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0, 0.0,

	// Level 2
	1500.0, 1500.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 1700.0, 1700.0,

	// Level 3
	2850.0, 3340.0, 430.0, 430.0, 430.0,
	-1370.0, -1370.0, -1070.0, -480.0, 930.0, 
	1730.0, 3030.0, 2930.0, 4430.0, 4430.0,
	4430.0, 4430.0, 5130.0, 5630.0, 2930.0,
	3030.0, 1530.0, 2350.0, 1100.0, 1100.0,
	-1100.0, -1100.0, -1100.0, -1600.0, -1600.0,
	-700.0, -700.0, 1230.0, -100.0, -100.0,
	-100.0, -100.0, 1600.0, 3200.0, 5200.0,
	4800.0, 4800.0, 4100.0, 4100.0,

	// Queen Bee Boss
	4000.0, 4000.0, 3600.0, 3600.0, 3600.0,
	3600.0, 3600.0,

	// Ape Boss's Small Ape
	0.0,
	
	// Level 1 powerups
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0,

	// Level 2 powerups
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0,

	// Level 3 powerups
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0, 0.0, 0.0,
	0.0, 0.0
};



float enemyrightranges[NUMENEMIES] = {

	// Level 1
	0.0, 0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0, 0.0,

	// Level 2
	3800.0, 3800.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 3800.0, 3800.0,

	// Level 3
	3460.0, 3660.0, 780.0, 780.0, 780.0,
	-330.0, -330.0, -740.0, -120.0, 1270.0, 
	2580.0, 3380.0, 3280.0, 6160.0, 6160.0,
	6160.0, 6160.0, 5480.0, 5970.0, 3270.0,
	3380.0, 1880.0, 5000.0, 3500.0, 3500.0,
	2100.0, 2100.0, 2100.0, 1100.0, -600.0,
	1100.0, 1100.0, 1570.0, 4600.0, 4600.0,
	4600.0, 4600.0, 2800.0, 4200.0, 6300.0,
	6300.0, 6300.0, 5300.0, 5300.0,

	// Queen Bee Boss
	6000.0, 6000.0, 6200.0, 6200.0, 6200.0,
	6200.0, 6200.0,

	// Ape Boss's Small Ape
	0.0,

	// Level 1 powerups
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0,

	// Level 2 powerups
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0,

	// Level 3 powerups
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0, 0.0, 0.0, 
	0.0, 0.0, 0.0, 0.0, 0.0,
	0.0, 0.0
	
};

	

int enemyPowerupTypes[NUMENEMIES] = {
	// dummy values
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 
	
	// Level 1 powerups
	3, 5, 3, 5, 6,
	4, 3, 3, 6, 4,
	2,

	// Level 2 powerups
	3, 4, 2, 4, 3,
	6, 3, 5, 3, 5,
	3, 5, 6, 6, 4,
	3, 3, 2,

	// Level 3 powerups
	2, 5, 5, 4, 3,
	3, 5, 3, 5, 4,
	2, 3, 5, 4, 2,
	6, 5, 6, 4, 2,
	5, 2, 3, 3, 5,
	6, 2, 5, 3, 5,
	4, 2
};

	

Enemy *enemyarray[NUMENEMIES];

bool detectCollision(RECT r1, RECT r2);

extern Reyk* players[];

extern int currentlevel;

/*========================================================================


========================================================================*/

EnemyHandler::EnemyHandler()
{
	float xmod = 0.0;
	float ymod = 0.0;
	for (int i = 0; i < NUMENEMIES; i++)
	{
		if (i < 20)
		{
			xmod = 0.0;
			ymod = 0.0;
		}
		else if (i >= 20 && i <= 39)
		{
			xmod = 12300.0;
		}
		else if (i <= 83)
		{
			xmod = 12300.0 - 6500.0;
			ymod = -6700.0;
		}
		else
		{
			xmod = 0.0;
			ymod = 0.0;
		}
		if (enemytypes[i] == 0)
		{
			enemyarray[i] = new SmallApe(enemiesX[i] + xmod, enemiesY[i] + ymod);
			if (i == 91)
			{
				(*enemyarray[i]).setstatus(0);
			}
		}
		else if (enemytypes[i] == 1)
		{
			enemyarray[i] = new ProjectileApe(enemiesX[i] + xmod, enemiesY[i] + ymod);
		}
		else if (enemytypes[i] == 2)
		{
			enemyarray[i] = new Chameleon(enemiesX[i] + xmod, enemiesY[i] + ymod, enemydirections[i]);
		}
		else if (enemytypes[i] == 3)
		{
			enemyarray[i] = new Decapede(enemiesX[i] + xmod, enemiesY[i] + ymod, enemyleftranges[i] + xmod, enemyrightranges[i] + xmod, enemydirections[i]);
		}
		else if (enemytypes[i] == 4)
		{
			bool bossbee = false;
			if (i >= 84)
			{
				bossbee = true;
			}
			enemyarray[i] = new DrillBee(enemiesX[i] + xmod, enemiesY[i] + ymod, enemyleftranges[i] + xmod, enemyrightranges[i] + xmod, enemydirections[i], bossbee);
		}
		else if (enemytypes[i] == 5)
		{
			bool bossbee = false;
			if (i >= 84)
			{
				bossbee = true;
			}
			enemyarray[i] = new TankBee(enemiesX[i] + xmod, enemiesY[i] + ymod, enemyleftranges[i] + xmod, enemyrightranges[i] + xmod, enemydirections[i], bossbee);
		}
		else if (enemytypes[i] == 6)
		{
			bool bossbee = false;
			if (i >= 84)
			{
				bossbee = true;
			}
			enemyarray[i] = new LightningBee(enemiesX[i] + xmod, enemiesY[i] + ymod, enemyleftranges[i] + xmod, enemyrightranges[i] + xmod, enemydirections[i], bossbee);
		}
		else if (enemytypes[i] == 10)
		{
			enemyarray[i] = new PowerupHolder(enemiesX[i] + xmod, enemiesY[i] + ymod, enemyPowerupTypes[i]);
		}
		(*enemyarray[i]).settype(enemytypes[i]);
	}
}

/*========================================================================


========================================================================*/

void EnemyHandler::drawEnemies()
{
	if (gamestatus == 5)
	{
		return;
	}
	for (int i = enemiesmin; i < enemiesmax; i++)
	{
		(*enemyarray[i]).draw();
	}
}

void EnemyHandler::drawParticles()
{
	if (gamestatus == 5)
	{
		return;
	}
	for (int i = enemiesmin; i < enemiesmax; i++)
		(*enemyarray[i]).drawParticle();
}

void EnemyHandler::draw3D()
{
	if (gamestatus == 5)
	{
		return;
	}
	for (int i = enemiesmin; i < enemiesmax; i++)
		(*enemyarray[i]).draw3D();
}

void EnemyHandler::drawPowerups()
{
	for (int i = enemiesmin; i < enemiesmax; i++)
		(*enemyarray[i]).drawPowerup();
}

/*========================================================================


========================================================================*/

void EnemyHandler::updateEnemies(int rand2, int rand800, int rand20)
{
	if (gamestatus == 5)
	{
		return;
	}
	for (int i = enemiesmin; i < enemiesmax; i++)
	{
		(*enemyarray[i]).animate();

		if((*enemyarray[i]).gettype() == PROJECTILEAPE)
			(*(ProjectileApe*)enemyarray[i]).update( rand2 );
		else if((*enemyarray[i]).gettype() == DRILLBEE)
			(*(DrillBee*)(enemyarray[i])).update(rand2, rand800, rand20);
		else if((*enemyarray[i]).gettype() == CHAMELEON)
			(*(Chameleon*)(enemyarray[i])).update(i);
		else if((*enemyarray[i]).gettype() == LIGHTNINGBEE)
			(*(LightningBee*)(enemyarray[i])).update(i);
		else
			(*enemyarray[i]).update();
	}
}


/*========================================================================


========================================================================*/

void EnemyHandler::moveEnemies(float x, float y)
{
	for (int i = enemiesmin; i < enemiesmax; i++)
	{
		(*enemyarray[i]).move(x, y);
	}
}

/*========================================================================


========================================================================*/

void EnemyHandler::checkEnemyAxeCollision(RECT r1, RECT r2, Reyk* player1)
{
	for (int i = enemiesmin; i < enemiesmax; i++)
	{
		RECT a = (*enemyarray[i]).getHitbox();
		if (detectCollision(a, r1) && 
			((*enemyarray[i]).getstatus() == 1 
			|| (*enemyarray[i]).getstatus() == 2
			|| (*enemyarray[i]).gettype() == LIGHTNINGBEE))
		{
			(*enemyarray[i]).gethit(player1);
		}
		else if (detectCollision(a, r2) && 
			((*enemyarray[i]).getstatus() == 1 
			|| (*enemyarray[i]).getstatus() == 2 
			|| (*enemyarray[i]).gettype() == LIGHTNINGBEE))
		{
			(*enemyarray[i]).gethit(player1);
		}
		if ((*enemyarray[i]).gettype() == CHAMELEON) 
		{
			RECT t = (*enemyarray[i]).getTongueTipHitbox(); 
			if (detectCollision(t, r1))
			{
				(*enemyarray[i]).tonguegethit();
			}
			else if (detectCollision(t, r2))
			{
				(*enemyarray[i]).tonguegethit();
			}
		}
	}
}

/*========================================================================


========================================================================*/

void EnemyHandler::checkEnemyFireballCollision(RECT r1, Reyk* player1)
{
	for (int i = enemiesmin; i < enemiesmax; i++)
	{
		RECT a = (*enemyarray[i]).getHitbox();
		if (detectCollision(a, r1) && ((*enemyarray[i]).getstatus() == 1 || (*enemyarray[i]).getstatus() == 2))
		{
			(*enemyarray[i]).gethit(player1);
		}

		if ((*enemyarray[i]).gettype() == CHAMELEON)
		{
			RECT t = (*enemyarray[i]).getTongueTipHitbox(); 
			if (detectCollision(t, r1))
			{
				(*enemyarray[i]).tonguegethit();
			}
		}
	}
}

/*========================================================================


========================================================================*/

void EnemyHandler::shootApe()
{
	(*enemyarray[91]).getShot();
}

/*========================================================================


========================================================================*/

int EnemyHandler::getstatus(int index)
{
	return (*enemyarray[index]).getstatus();
}

/*========================================================================


========================================================================*/

bool EnemyHandler::checkLickCollision(RECT r, Reyk* player1)
{
	for (int i = enemiesmin; i < enemiesmax; i++)
	{
		if (enemytypes[i] == 1)
		{
			ProjectileApe *pape = (ProjectileApe *)(enemyarray[i]);
			if (pape->getprojectilestatus() != 0)
			{
				if (detectCollision(pape->projectileGetHitbox(), r))
				{
					pape->reset();

					switch (pape->getProjectileFrame())
					{
					case 0:
						player1->setLick(projectile0, 80.0f, pape->getProjectileFrame());
						break;
					case 1:
						player1->setLick(ringBerry, 80.0f, pape->getProjectileFrame());
						break;
					case 2:
						player1->setLick(asteriskFruit, 80.0f, pape->getProjectileFrame());
						break;
					case 3:
						player1->setLick(OFruit, 80.0f, pape->getProjectileFrame());
						break;
					case 4:
						player1->setLick(XFruit, 80.0f, pape->getProjectileFrame());
						break;
					case 5:
						player1->setLick(spikedAcorn, 80.0f, pape->getProjectileFrame());
						break;
					}
					
					return true;
				}
			}
		}
		Enemy *e = enemyarray[i];
		if (e->getPowerupStatus() == 1 && detectCollision(e->getPowerupHitbox(), r))
		{
			e->eatPowerup();
			player1->setLick(e->getPowerupModel(), e->getPowerupDepth(), e->getPowerupType());
		}
	}
	return false;
}

void EnemyHandler::enemyGetHit(int index, Reyk* player1){
	(*enemyarray[index]).gethit(player1);
}

void EnemyHandler::enemyTongueGetHit(int index){
	(*enemyarray[index]).tonguegethit();
}

void EnemyHandler::setplayercaught(int index, bool playercaught, int whichplayer){
	(*(Chameleon*)enemyarray[index]).setplayercaught(playercaught, whichplayer);
}

Enemy* EnemyHandler::getEnemy(int index){
	return enemyarray[index];
}

void EnemyHandler::setLightningBeeShit(int index, int shakeamount, int status, float savedX, int savedframecount)
{
	(*(LightningBee*)enemyarray[index]).setSomeShit(shakeamount, status, savedX, savedframecount);
}

/*========================================================================


========================================================================*/

void EnemyHandler::killAllEnemies()
{
	for (int i = enemiesmin; i < enemiesmax; i++)
	{
		(*enemyarray[i]).sethp(0);
		(*enemyarray[i]).gethit(players[0]);
	}
}