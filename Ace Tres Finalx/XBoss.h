#include <d3d9.h>
#include <d3dx9.h>

const int XBOSSTRAILMAX = 34;

class XBoss
{
	LPDIRECT3DTEXTURE9 xbossTexture;
	LPDIRECT3DTEXTURE9 glassTexture;
	LPDIRECT3DTEXTURE9 crackTextures[13];
	LPDIRECT3DTEXTURE9 shardTexture;
	LPDIRECT3DTEXTURE9 lefthandTexture;
	LPDIRECT3DTEXTURE9 righthandTexture;
	LPDIRECT3DTEXTURE9 fireTexture;
	LPDIRECT3DTEXTURE9 leftTexture;
	LPDIRECT3DTEXTURE9 rightTexture;
	LPDIRECT3DTEXTURE9 tinyTexture;

	int xbosscounter;
	int attackcounter;
	int status;
	int shakeamount;
	int hp;
	bool shaking;

	int lightningframe;
	int starttime;
	int delay;

	float xbossX;
	float xbossY;
	float savedX;
	float Ybase;
	float xbossvelX;

	float glassX;

	int lefthandstatus;
	int lefthandattackstatus;
	int lefthandshakeamount;

	float lefthandX;
	float lefthandY;
	float savedlefthandX;
	float lefthandvelY;
	float lefthandrotation;

	int righthandstatus;
	int righthandattackstatus;
	int righthandshakeamount;
	float righthandvelY;
	float righthandrotation;

	float righthandX;
	float righthandY;
	float savedrighthandX;

	int crackframe;

	static const int NUMSHARDS = 100;

	float shardsX[NUMSHARDS];
	float shardsY[NUMSHARDS];
	float shardsvelX[NUMSHARDS];
	float shardsvelY[NUMSHARDS];
	float shardsrotation[NUMSHARDS];

	float leftrange;
	float rightrange;

	float fireballsX[7];
	float fireballsY[7];
	float fireballsvelX[7];
	float fireballsvelY[7];
	float fireballsrotation[7];
	bool fireballsactive[7];

	int fireballframe;
	int numfireballs;

	float halvesvelY;
	float halvesrotation;

	float leftX;
	float leftY;
	float rightX;
	float rightY;

	float tinyX;
	float tinyY;
	float tinysavedY;

	bool headtrailing;
	int headtrailsize;
	float headtrailX[XBOSSTRAILMAX];
	float headtrailY[XBOSSTRAILMAX];

	int lefttrailsize;
	float lefttrailX[XBOSSTRAILMAX];
	float lefttrailY[XBOSSTRAILMAX];
	float lefttrailrotation[XBOSSTRAILMAX];

	int righttrailsize;
	float righttrailX[XBOSSTRAILMAX];
	float righttrailY[XBOSSTRAILMAX];
	float righttrailrotation[XBOSSTRAILMAX];

	int fireballtrailsize[7];
	float fireballtrailX[7][XBOSSTRAILMAX];
	float fireballtrailY[7][XBOSSTRAILMAX];

public:

	XBoss();
	~XBoss();
	void update();
	void updateHands();
	void animate();
	void draw();
	void gethit(int rand670, int rand2, int rand100);
	void barfFire();
	RECT getHitbox();
	int getstatus();
	void setstatus(int);

	RECT getleftfistHitbox();
	RECT getleftlightning1Hitbox();
	RECT getleftlightning2Hitbox();
	RECT getleftlightning3Hitbox();

	RECT getrightfistHitbox();
	RECT getrightlightning1Hitbox();
	RECT getrightlightning2Hitbox();
	RECT getrightlightning3Hitbox();

	RECT getfireballHitbox(int);

	RECT getAttackHitbox();
	RECT getFaceHitbox();
	RECT getTinyHitbox();

	int gethp();
};