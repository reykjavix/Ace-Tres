#include "particlesystem.h"
#include "Powerup.h"
#include "Reyk.h"

const int ENEMYTRAILMAX = 34;

class Enemy
{
	int type;
public:
	Enemy(){return;};
	virtual int getstatus() { return 0; };
	virtual void move(float, float) { return; };
	virtual void draw() { return; };
	virtual void update() { return; };
	virtual void gethit(Reyk*) { return; };
	virtual void tonguegethit() { return; };
	virtual void animate() { return; };
	virtual void getShot() { return; };
	virtual RECT getHitbox() { RECT a = {0, 0, 0, 0}; return a; };
	virtual RECT getfeetpoint() { RECT a = {0, 0, 0, 0}; return a; };
	virtual RECT getTongueTipHitbox() { RECT a = {0, 0, 0, 0}; return a; };
	virtual int gettype() { return type; };
	virtual void settype(int t) { type = t; };
	virtual void draw3D() { return; };
	virtual void drawParticle() { return; };
	virtual void drawPowerup() { return; };
	virtual RECT getPowerupHitbox() { RECT rct = {0, 0, 0, 0}; return rct; };
	virtual float getPowerupDepth() { return 0; };
	virtual MODEL* getPowerupModel() { return NULL; };
	virtual int getPowerupStatus() { return 0; };
	virtual int getPowerupType() { return 0; };
	virtual void eatPowerup() { return; };
	virtual void sethp(int h) { hp = h; };
	virtual void setstatus(int s) { status = s; };

	float powerupX;
	float powerupY;
	float powerupvelY;

	int status;
	int hp;
	int droprate;

	D3DCOLOR drawcolor;
	D3DXVECTOR3 particlePosition;
};

class SmallApe: public Enemy
{
	LPDIRECT3DTEXTURE9 smallapeTexture;
	LPDIRECT3DTEXTURE9 smallapeheadTexture;
	CParticleSystem *g_pParticleSystem;
	int smallapeframe;
	int smallapecounter;
	int smallapecountermax;
	float smallapeX;
	float smallapeY;
	float smallapevelX;
	float smallapevelY;
	float smallaperotation;
	float smallapescale;

	int smallapeheadframe;
	float smallapeheadX;
	float smallapeheadY;
	float smallapeheadvelX;
	float smallapeheadvelY;
	float smallapeheadrotation;
	bool smallapebleeding;

	Powerup *smallapepowerup;

	bool trailing;
	int trailsize;
	float trailX[ENEMYTRAILMAX];
	float trailY[ENEMYTRAILMAX];
	float trailrotation[ENEMYTRAILMAX];
	float trailscale[ENEMYTRAILMAX];
	int trailframe[ENEMYTRAILMAX];

	bool headtrailing;
	int headtrailsize;
	float headtrailX[ENEMYTRAILMAX];
	float headtrailY[ENEMYTRAILMAX];
	float headtrailrotation[ENEMYTRAILMAX];

public:
	SmallApe();
	SmallApe(float, float);
	void update();
	void smallapeAttack(Reyk*);
	void smallapeDie(Reyk*);
	void getShot();
	RECT getHitbox();
	RECT apeGetFeetHitbox();

	int getstatus();
	void setstatus(int);
	int getX();
	int getY();
	int getframe();
	float getrotation();
	float getscale();
	int getcounter();
	void move(float, float);

	RECT getfeetpoint();

	void draw();
	void drawParticle();

	void drawPowerup();
	RECT getPowerupHitbox();
	float getPowerupDepth();
	MODEL* getPowerupModel();
	int getPowerupStatus();
	int getPowerupType();
	void eatPowerup();

	void gethit(Reyk* player);

	int getSWX();
	int getSWY();
};

class ProjectileApe: public Enemy
{
	LPDIRECT3DTEXTURE9 projectileapeTexture;
	LPDIRECT3DTEXTURE9 projectileapeheadTexture;
	LPDIRECT3DTEXTURE9 projectileTexture;
	CParticleSystem *g_pParticleSystem;

	int direction;
	int projectileapeframe;
	int projectileapestatus;
	int projectileapecounter;
	int projectileapecountermax;
	float projectileapeX;
	float projectileapeY;
	float projectileapevelX;
	float projectileapevelY;

	int projectileapeheadframe;
	float projectileapeheadX;
	float projectileapeheadY;
	float projectileapeheadvelX;
	float projectileapeheadvelY;
	float projectileapeheadrotation;
	bool projectileapebleeding;

	int startframe;
	int starttime;
	int endframe;
	int delay;

	float projectileX;
	float projectileY;
	float projectilevelX;
	float projectilevelY;
	float projectilerotation;

	int projectileframe;

	int projectilestatus;

	Powerup *projectileapepowerup;

	bool trailing;
	int trailsize;
	float trailX[ENEMYTRAILMAX];
	float trailY[ENEMYTRAILMAX];
	int trailframe[ENEMYTRAILMAX];

	bool headtrailing;
	int headtrailsize;
	float headtrailX[ENEMYTRAILMAX];
	float headtrailY[ENEMYTRAILMAX];
	float headtrailrotation[ENEMYTRAILMAX];

public:
	ProjectileApe();
	ProjectileApe(float, float);
	void animate();
	void update(int rand2);
	void draw();
	void draw3D();
	void drawParticle();
	void move(float, float);
	void throwProjectile(Reyk*);
	void projectileapeDie(Reyk*);
	int getstatus();
	int getprojectilestatus();
	void reset();
	RECT getHitbox();
	RECT projectileapeGetFeetHitbox();
	RECT projectileGetHitbox();
	LPDIRECT3DTEXTURE9* getTexture();
	int getProjectileFrame();
	RECT getfeetpoint();

	void drawPowerup();
	RECT getPowerupHitbox();
	float getPowerupDepth();
	MODEL* getPowerupModel();
	int getPowerupStatus();
	int getPowerupType();
	void eatPowerup();

	void gethit(Reyk* player);
};

class Chameleon: public Enemy
{
	LPDIRECT3DTEXTURE9 chameleonTexture;
	LPDIRECT3DTEXTURE9 tongueTexture;

	int status;
	float tonguespeed;
	int chameleoncounter;
	int chameleoncountermax;
	int chameleonshake;
	bool playercaught;
	int chameleonframecount;
	int direction;

	float chameleonX;
	float chameleonY;
	float tongueX;
	float tongueY;
	float savedchameleonX;

	int frame;
	int startframe;
	int starttime;
	int endframe;
	int delay;

	int whichplayer;

public:
	Chameleon();
	Chameleon(float, float, int);
	void animate();
	void update(int enemyIndex);
	void draw();
	void move(float, float);
	void gethit(Reyk*);
	void tonguegethit();
	int getstatus();
	RECT getTongueHitbox();	
	RECT getHitbox();
	RECT getTongueTipHitbox();
	void setplayercaught(bool b, int which);
};

class Decapede: public Enemy
{
	LPDIRECT3DTEXTURE9 decapedeTexture;
	LPDIRECT3DTEXTURE9 decapedeRTexture;
	LPDIRECT3DTEXTURE9 decapedeheadTexture;
	LPDIRECT3DTEXTURE9 decapedeheadRTexture;
	LPDIRECT3DTEXTURE9 decapedeeyeTexture;
	LPDIRECT3DTEXTURE9 decapedeeyeRTexture;

	CParticleSystem *g_pParticleSystem[5];

	int maxhp;
	int status;
	float movespeed;
	int direction;
	int headshake;
	bool headshaking;
	int decapedeframecount;

	float decapedeX;
	float decapedeY;
	float decapedeheadX;
	float decapedeheadY;
	float decapedeheadvelX;
	float decapedeheadvelY;
	float decapedeheadrotation;
	float decaeyeX[4];
	float decaeyeY[4];
	float decaeyevelX[4];
	float decaeyevelY[4];
	bool decaeyeactive[4];
	float decaeyerotation[4];
	
	int frame;
	int headframe;
	int startframe;
	int starttime;
	int endframe;
	int delay;

	float leftrange;
	float rightrange;

	Powerup *decapedepowerup;
	D3DXVECTOR3 particlePositions[5];

	bool headtrailing;
	int headtrailsize;
	float headtrailX[ENEMYTRAILMAX];
	float headtrailY[ENEMYTRAILMAX];
	float headtrailrotation[ENEMYTRAILMAX];

	int eyetrailsize[7];
	float eyetrailX[7][ENEMYTRAILMAX];
	float eyetrailY[7][ENEMYTRAILMAX];
	float eyetrailrotation[7][ENEMYTRAILMAX];

public:
	Decapede();
	Decapede(float, float, float, float, int);
	void animate();
	void update();
	void draw();
	void drawParticle();
	void move(float, float);
	int getstatus();
	RECT getHitbox();
	RECT getbodyHitbox();

	void drawPowerup();
	RECT getPowerupHitbox();
	float getPowerupDepth();
	MODEL* getPowerupModel();
	int getPowerupStatus();
	int getPowerupType();
	void eatPowerup();

	void gethit(Reyk* player);
};

class DrillBee: public Enemy
{
	LPDIRECT3DTEXTURE9 drillbeeTexture;
	CParticleSystem *g_pParticleSystem;
	
	int frame;
	int endframe;
	int starttime;
	int delay;

	int status;
	int direction;
	int drillbeeframecount;
	int shakeamount;
	bool shaking;

	float velX;
	float velY;
	float drillbeeX;
	float drillbeeY;
	float savedX;
	float drillbeerotation;
	float rotationvel;
	float Ybase;

	int initialhp;
	bool bossbee;

	float leftrange;
	float rightrange;

	Powerup *drillbeepowerup;

	bool trailing;
	int trailsize;
	float trailX[ENEMYTRAILMAX];
	float trailY[ENEMYTRAILMAX];
	float trailrotation[ENEMYTRAILMAX];

public:
	DrillBee();
	DrillBee(float, float, float, float, int, bool);
	void animate();
	void update(int rand2, int rand800, int rand20);
	void draw();
	void drawParticle();
	int getstatus();
	RECT getHitbox();

	void drawPowerup();
	RECT getPowerupHitbox();
	float getPowerupDepth();
	MODEL* getPowerupModel();
	int getPowerupStatus();
	int getPowerupType();
	void eatPowerup();

	void gethit(Reyk* player);
};

class TankBee: public Enemy
{
	LPDIRECT3DTEXTURE9 tankbeeTexture;
	LPDIRECT3DTEXTURE9 tankbeeheadTexture;

	CParticleSystem *g_pParticleSystem;
	
	int frame;
	int headframe;
	int startframe;
	int endframe;
	int starttime;
	int delay;

	int status;
	int direction;
	int tankbeeframecount;
	int shakeamount;
	bool shaking;

	float velX;
	float velY;
	float tankbeeX;
	float tankbeeY;
	float savedX;
	float headX;
	float headY;
	float headvelX;
	float headvelY;
	float headrotation;
	float rotationvel;

	float initialX;
	float initialY;
	int initialdirection;
	int initialhp;

	float leftrange;
	float rightrange;

	bool bossbee;

	Powerup *tankbeepowerup;

	bool headtrailing;
	int headtrailsize;
	float headtrailX[ENEMYTRAILMAX];
	float headtrailY[ENEMYTRAILMAX];
	float headtrailrotation[ENEMYTRAILMAX];

public:
	TankBee();
	TankBee(float, float, float, float, int, bool b);
	void animate();
	void update();
	void draw();
	void drawParticle();
	int getstatus();
	RECT getHitbox();
	RECT getExplosionHitbox();

	void drawPowerup();
	RECT getPowerupHitbox();
	float getPowerupDepth();
	MODEL* getPowerupModel();
	int getPowerupStatus();
	int getPowerupType();
	void eatPowerup();

	void gethit(Reyk* player);
};

class LightningBee: public Enemy
{
	LPDIRECT3DTEXTURE9 lightningbeeTexture;
	LPDIRECT3DTEXTURE9 lightningTexture;
	CParticleSystem *g_pParticleSystem;
	
	int frame;
	int lightningframe;
	int endframe;
	int starttime;
	int delay;

	int status;
	int direction;
	int lightningbeeframecount;
	int shakeamount;
	bool shaking;

	float velX;
	float velY;
	float lightningbeeX;
	float lightningbeeY;
	float savedX;
	float lightningbeerotation;
	float rotationvel;
	float Ybase;
	float lightningX;
	float lightningY;

	float leftrange;
	float rightrange;

	int savedframecount;

	int initialhp;
	int initialdirection;
	float initialX;
	float initialY;
	float initialvelX;

	bool bossbee;

	Powerup *lightningbeepowerup;

	bool trailing;
	int trailsize;
	float trailX[ENEMYTRAILMAX];
	float trailY[ENEMYTRAILMAX];
	float trailrotation[ENEMYTRAILMAX];

public:
	LightningBee();
	LightningBee(float, float, float, float, int, bool);
	void animate();
	void update(int enemyIndex);
	void draw();
	void drawParticle();
	int getstatus();
	RECT getHitbox();
	RECT getLightningHitbox();

	void drawPowerup();
	RECT getPowerupHitbox();
	float getPowerupDepth();
	MODEL* getPowerupModel();
	int getPowerupStatus();
	int getPowerupType();
	void eatPowerup();
	void setSomeShit(int shakeamount, int status, float savedX, int savedframecount);

	void gethit(Reyk* player);
};

class PowerupHolder: public Enemy
{
	Powerup *p;

public:
	PowerupHolder(float, float, int);

	void drawPowerup();
	RECT getPowerupHitbox();
	float getPowerupDepth();
	MODEL* getPowerupModel();
	int getPowerupStatus();
	int getPowerupType();
	void eatPowerup();
};