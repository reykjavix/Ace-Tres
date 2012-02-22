#include <d3d9.h>
#include <d3dx9.h>
#include "particlesystem.h"

const int APEBOSSTRAILMAX = 34;

class ApeBoss
{
	LPDIRECT3DTEXTURE9 apefaceTexture ;
	LPDIRECT3DTEXTURE9 apehandleftTexture ;
	LPDIRECT3DTEXTURE9 apehandrightTexture ;
	LPDIRECT3DTEXTURE9 apenoseTexture ;
	LPDIRECT3DTEXTURE9 boogerTexture ;
	LPDIRECT3DTEXTURE9 apebodyTexture;

	CParticleSystem *g_pParticleSystem[3];

	int apefaceframe;
	int apefaceshake;
	int apehp;
	int apefacestatus;
	float apefaceX;
	float apefaceY;
	float apefacevel;
	int apeframecount;
	int apecount;
	int apecountmax;
	float apedurationmod;
	float apespeedmod;
	float apefacevelX;
	float apefacevelY;
	float apefacerotation;
	float apehandleftrotation;
	float apehandrightrotation;

	float apebodyX;
	float apebodyY;

	int apehandleftframe;
	int apehandleftstatus;
	int apehandleftshake;
	int apehandleftnumstomps;
	int apehandleftwhichattack;
	float apehandleftsavedX;

	float apehandleftX;
	float apehandleftY;
	float apehandleftvelX;
	float apehandleftvelY;

	int apehandrightframe;
	int apehandrightstatus;
	int apehandrightshake;
	int apehandrightnumstomps;
	int apehandrightwhichattack;
	float apehandrightsavedX;

	int apenosestatus;
	int apenoseshake;
	float apenoseX;
	float apenoseY;
	float apenosevelX;
	float apenosevelY;
	float apenoserotation;
	int apenosebleedtimer;

	float apehandrightX;
	float apehandrightY;
	float apehandrightvelX;
	float apehandrightvelY;

	int numboogers;
	int boogerstatus[5];
	float boogerX[5];
	float boogerY[5];
	float boogervelX[5];
	float boogervelY[5];
	float boogerrotation[5];

	float aperisingoffset;
	boolean aperising;

	float apefallingoffset;
	boolean apefalling;

	bool apenosebleeding;

	bool lefthandtrailing;
	int lefthandtrailsize;
	float lefthandtrailX[APEBOSSTRAILMAX];
	float lefthandtrailY[APEBOSSTRAILMAX];
	int lefthandtrailframe[APEBOSSTRAILMAX];

	bool righthandtrailing;
	int righthandtrailsize;
	float righthandtrailX[APEBOSSTRAILMAX];
	float righthandtrailY[APEBOSSTRAILMAX];
	int righthandtrailframe[APEBOSSTRAILMAX];

	bool nosetrailing;
	int nosetrailsize;
	float nosetrailX[APEBOSSTRAILMAX];
	float nosetrailY[APEBOSSTRAILMAX];
	float nosetrailrotation[APEBOSSTRAILMAX];

	bool headtrailing;
	int headtrailsize;
	float headtrailX[APEBOSSTRAILMAX];
	float headtrailY[APEBOSSTRAILMAX];
	float headtrailrotation[APEBOSSTRAILMAX];

	int boogertrailsize;
	float boogertrailX[5][APEBOSSTRAILMAX];
	float boogertrailY[5][APEBOSSTRAILMAX];

public:
	ApeBoss();
	~ApeBoss();
	void updateApe(int rand1, int rand2, int rand3);
	void apeAttack(int rand1, int rand2, int rand3);
	void apeAttack2(int rand);
	void updateLeftHand();
	void updateRightHand();
	RECT lefthandGetHitbox();
	RECT righthandGetHitbox();
	void animateApe(int &frame, int startframe, int endframe, int direction, int &starttime, int delay);
	void shootBoogers(int b);
	void updateBoogers();
	void shootSmallApe();
	RECT apenoseGetHitbox();
	void apenoseHit();
	void apefaceHit();
	RECT apefaceGetHitbox();
	int getstatus();
	int getlefthandstatus();
	int gethp();
	float getX();
	float getY();

	void ApeFace_Sprite_Transform_Draw();
	void ApeBody_Sprite_Transform_Draw();
	void ApeNose_Sprite_Transform_Draw();
	void ApeHandLeft_Sprite_Transform_Draw();
	void ApeHandRight_Sprite_Transform_Draw();
	void Boogers_Sprite_Transform_Draw();

	void drawParticle();

	void startBossBattle();

	D3DXVECTOR3 particlePosition[3];
};