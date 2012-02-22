#include <d3d9.h>
#include <d3dx9.h>

const int QBEETRAILMAX = 34;

class QueenBee
{
	LPDIRECT3DTEXTURE9 queenbeebodyTexture;
	LPDIRECT3DTEXTURE9 queenbeeheadTexture;
	LPDIRECT3DTEXTURE9 queenbeevisorTexture;
	LPDIRECT3DTEXTURE9 queenbeeeyesTexture;
	LPDIRECT3DTEXTURE9 eyeTexture;

	int queenbeecounter;
	int status;
	int shakeamount;
	int hp;
	bool shaking;

	float bodyX;
	float bodyY;
	float headX;
	float headY;
	float headvelX;
	float headvelY;
	float headrotation;
	float rotationvel;
	float savedX;

	int visorframe;
	float visorX;
	float visorY;
	float visorvelX;
	float visorvelY;
	float visorrotation;
	int visorstatus;
	float savedvisorX;

	int eyesframe;

	bool visorshaking;

	float eyesX[10];
	float eyesY[10];
	float eyesvelX[10];
	float eyesvelY[10];
	float eyesrotation[10];
	bool eyesactive[10];

	bool visortrailing;
	int visortrailsize;
	float visortrailX[QBEETRAILMAX];
	float visortrailY[QBEETRAILMAX];
	float visortrailrotation[QBEETRAILMAX];

	bool headtrailing;
	int headtrailsize;
	float headtrailX[QBEETRAILMAX];
	float headtrailY[QBEETRAILMAX];
	float headtrailrotation[QBEETRAILMAX];

	int eyetrailsize[10];
	float eyetrailX[10][QBEETRAILMAX];
	float eyetrailY[10][QBEETRAILMAX];
	float eyetrailrotation[10][QBEETRAILMAX];

public:
	QueenBee();
	~QueenBee();
	void update();
	void draw();
	void gethit();
	RECT getHitbox();

	int gethp();
};