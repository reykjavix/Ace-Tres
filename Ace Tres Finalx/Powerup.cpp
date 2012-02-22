#include "Powerup.h"
#include "Enemy.h"
#include "Reyk.h"

extern Reyk* players[];

const double PI = 3.1415926535;

extern MODEL *asteriskFruit;
extern MODEL *ringBerry;
extern MODEL *XFruit;
extern MODEL *platMesh;
extern MODEL *OFruit;
extern MODEL *spikedAcorn;
extern Reyk* players[];


Powerup::Powerup(float x, float y, int t)
{
	X = x;
	Y = y;
	type = t;
	status = 1;
	depth = 0; /* arbitrary, set on draw */
	m_mesh = new MyMesh();
}

Powerup::Powerup(int t)
{
	X = 0;
	Y = 0;
	type = t;
	status = 0;
	depth = 0; /* arbitrary, set on draw */
	m_mesh = new MyMesh();
}

void Powerup::setPosition(float x, float y)
{
	X = x;
	Y = y;
}

void Powerup::draw()
{
	if (abs((int)(X - players[0]->getCenterX())) > 1280.0
		|| abs((int)(Y - players[0]->getCenterY())) > 1600.0
		|| status == 0)
	{
		return;
	}

	Reyk* player1 = players[clientIndex];

	D3DXVECTOR3 temp = m_mesh->getRotation();

	if (status != 1)
		return;

	switch (type)
	{
		case 2:
			m_mesh->setMesh(asteriskFruit);
			depth = 80.0f;
			break;
		case 3:
			m_mesh->setMesh(ringBerry);
			temp.x = PI/2;
			depth = 80.0f;
			break;
		case 4:
			m_mesh->setMesh(XFruit);
			depth = 80.0f;
			break;
		case 5:
			m_mesh->setMesh(OFruit);
			depth = 80.0f;
			break;
		case 6:
			m_mesh->setMesh(spikedAcorn);
			depth = 80.0f;
			break;
		default:
			m_mesh->setMesh(platMesh);
			depth = 20.0f;
			break;
	}

	temp.y += PI/256;
	if (temp.y > 2*PI) temp.y = 0;
	m_mesh->setRotation(temp);


	m_mesh->setPosition(screenToWorld(X - player1->getXmod(), Y - player1->getYmod(), depth));
	m_mesh->Draw();
}

RECT Powerup::getHitbox()
{
	RECT hitbox;
	
	hitbox.left = X - 50;
	hitbox.right = hitbox.left + 100.0;
	hitbox.top = Y - 50;
	hitbox.bottom = hitbox.top + 100.0;

	return hitbox;
}

int Powerup::getType()
{
	return type;
}

int Powerup::getStatus()
{
	return status;
}

float Powerup::getX()
{
	return X;
}

float Powerup::getY()
{
	return Y;
}

float Powerup::getDepth()
{
	return depth;
}

void Powerup::setType(int t)
{
	type = t;
}

void Powerup::setStatus(int s)
{
	status = s;
}

MODEL* Powerup::getModel()
{
	return m_mesh->getModel();
}