#include "MyDirectX.h"
#include "Reyk.h"
#include "PlatformHandler.h"
#include "Enemy.h"

PowerupHolder::PowerupHolder(float x, float y, int t)
{
	p = new Powerup(x, y, t);
}


void PowerupHolder::drawPowerup()
{
	p->draw();
}

RECT PowerupHolder::getPowerupHitbox()
{
	return p->getHitbox();
}

float PowerupHolder::getPowerupDepth()
{
	return p->getDepth();
}

MODEL* PowerupHolder::getPowerupModel()
{
	return p->getModel();
}

int PowerupHolder::getPowerupStatus()
{
	return p->getStatus();
}

int PowerupHolder::getPowerupType()
{
	return p->getType();
}

void PowerupHolder::eatPowerup()
{
	p->setStatus(2);
}