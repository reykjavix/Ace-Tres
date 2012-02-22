#include <d3d9.h>
#include <d3dx9.h>
#include "Enemy.h"
#include "Reyk.h"

class EnemyHandler
{
public:
	EnemyHandler();
	void drawEnemies();
	void drawParticles();
	void draw3D();
	void moveEnemies(float, float);
	void updateEnemies(int rand2, int rand800, int rand20);
	void checkEnemyAxeCollision(RECT, RECT, Reyk*);
	void checkEnemyFireballCollision(RECT, Reyk*);
	void shootApe();
	int getstatus(int);
	bool checkLickCollision(RECT, Reyk*);
	void enemyGetHit(int index, Reyk*);
	void enemyTongueGetHit(int index);
	void drawPowerups();
	void setplayercaught(int index, bool playercaught, int whichplayer);
	Enemy* getEnemy(int index);
	void setLightningBeeShit(int index, int shakeamount, int status, float savedX, int savedframecount);
	void killAllEnemies();
};