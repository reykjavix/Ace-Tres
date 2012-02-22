#include "MyDirectX.h"

#ifndef MYMESH_
#define MYMESH_

class MyMesh
{
public:
	MyMesh();
	MyMesh(MODEL *);
	~MyMesh();
	void setMesh(MODEL *);
	void setPosition(float, float, float);
	void setPosition(D3DXVECTOR3);
	D3DXVECTOR3 getPosition();
	void setRotation(float, float, float);
	void setRotation(D3DXVECTOR3);
	D3DXVECTOR3 getRotation();
	void Draw();
	MODEL* getModel();

private:
	float m_xPos, m_yPos, m_zPos, m_fPitch, m_fYaw, m_fRoll;
	MODEL * m_mesh;
};

#endif