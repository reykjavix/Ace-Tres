/*
    Ace Tres
*/

#include "SongPlayer.h"
#include "fmod.h"
#include "fmod_errors.h"
#include "MyDirectX.h"
#include "Reyk.h"
#include "PlatformHandler.h"
#include "EnemyHandler.h"
#include "LyricsHandler.h"

#include "ApeBoss.h"
#include "Croctopus.h"
#include "QueenBee.h"
#include "XBoss.h"

#include "Credits.h"

int gamestatus = 1;
int currentlevel = 1;
int clientIndex = 0;

const string APPTITLE = "Ace Tres";
const int SCREENW = 1280;
const int SCREENH = 1024;

//font variables
LPD3DXFONT font;
LPD3DXFONT debugfont;
LPD3DXFONT debugfont2;

//timing variables
bool paused = false;
DWORD refresh = 0;
DWORD screentime = 0;
double screenfps = 0.0;
double screencount = 0.0;
DWORD coretime = 0;
double corefps = 0.0;
double corecount = 0.0;
DWORD currenttime, newtime, songtime;

//background scrolling variables
const int BUFFERW = SCREENW * 2;
const int BUFFERH = SCREENH;
double scrollx=0;
double scrolly=0;
LPDIRECT3DSURFACE9 background = NULL;
LPDIRECT3DSURFACE9 background2 = NULL;
LPDIRECT3DSURFACE9 background2b = NULL;
LPDIRECT3DSURFACE9 background2c = NULL;
LPDIRECT3DSURFACE9 background2d = NULL;
LPDIRECT3DSURFACE9 background2e = NULL;
LPDIRECT3DSURFACE9 background3 = NULL;
const double virtual_level_size = BUFFERW * 5;
double virtual_scrollx = 0;

int waterfallanimcount = 0;
int waterfallframe = 0;

//sound effects
CSound *axe=NULL;
CSound *jumping=NULL;
CSound *fireball=NULL;

CSound *apehit=NULL;
CSound *apecatch=NULL;
CSound *apethrow=NULL;

CSound *apebosshit=NULL;
CSound *apebossfinalhit=NULL;
CSound *handslide=NULL;
CSound *snotrocket=NULL;
CSound *apefist=NULL;

CSound *decapedehit=NULL;

CSound *chameleonbarf=NULL;

CSound *crochit=NULL;
CSound *crocopen=NULL;
CSound *crocattack=NULL;
CSound *crunch=NULL;

CSound *explosion=NULL;
CSound *drill=NULL;
CSound *lasercharge=NULL;
CSound *lightning=NULL;

CSound *beehit=NULL;
CSound *beefinal=NULL;
CSound *crack=NULL;
CSound *metalfist=NULL;
CSound *shatterglass=NULL;

//controller vibration
int vibrating = 0;
int vibration = 100;

LPDIRECT3DTEXTURE9 hpbarTexture = NULL;
LPDIRECT3DTEXTURE9 energybarTexture = NULL;
LPDIRECT3DTEXTURE9 apehealthbarTexture = NULL;
LPDIRECT3DTEXTURE9 borderTexture = NULL;
LPDIRECT3DTEXTURE9 loadingTexture = NULL;

bool detectCollision(RECT r1, RECT r2);
void getSlope(RECT r, bool goingright);
void FindMinimum(float* myArray, int length, int* index);
LPDIRECT3DTEXTURE9 GetPlayerTexture(int index);

Reyk* players[MAX_CLIENTS];
ApeBoss *aboss;
Croctopus *croc;
QueenBee *qbee;
XBoss *xboss;

PlatformHandler *ph;
EnemyHandler *eh;
LyricsHandler *lh;

ofstream debugfile;

int holdbutton[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

int gamebegintime, rand2, rand3, rand4;
int* randarr;

LPDIRECT3DTEXTURE9 alphabetTexture;

// Platform Textures
LPDIRECT3DTEXTURE9 platformTexture;
LPDIRECT3DTEXTURE9 rampTexture;
LPDIRECT3DTEXTURE9 squareTexture;
LPDIRECT3DTEXTURE9 blueTexture;
LPDIRECT3DTEXTURE9 honeytop1Texture;
LPDIRECT3DTEXTURE9 honeytop2Texture;
LPDIRECT3DTEXTURE9 honey1Texture;
LPDIRECT3DTEXTURE9 honey2Texture;
LPDIRECT3DTEXTURE9 conveyerrightTexture;
LPDIRECT3DTEXTURE9 conveyerrightendleftTexture;
LPDIRECT3DTEXTURE9 conveyerrightendrightTexture;
LPDIRECT3DTEXTURE9 conveyerleftTexture;
LPDIRECT3DTEXTURE9 conveyerleftendleftTexture;
LPDIRECT3DTEXTURE9 conveyerleftendrightTexture;

// Enemy Textures
LPDIRECT3DTEXTURE9 chameleonTexture;
LPDIRECT3DTEXTURE9 tongueTexture;
LPDIRECT3DTEXTURE9 decapedeTexture;
LPDIRECT3DTEXTURE9 decapedeRTexture;
LPDIRECT3DTEXTURE9 decapedeheadTexture;
LPDIRECT3DTEXTURE9 decapedeheadRTexture;
LPDIRECT3DTEXTURE9 decapedeeyeTexture;
LPDIRECT3DTEXTURE9 decapedeeyeRTexture;
LPDIRECT3DTEXTURE9 drillbeeTexture;
LPDIRECT3DTEXTURE9 lightningbeeTexture;
LPDIRECT3DTEXTURE9 lightningTexture;
LPDIRECT3DTEXTURE9 projectileapeTexture;
LPDIRECT3DTEXTURE9 projectileapeheadTexture;
LPDIRECT3DTEXTURE9 projectileTexture;
LPDIRECT3DTEXTURE9 smallapeTexture;
LPDIRECT3DTEXTURE9 smallapeheadTexture;
LPDIRECT3DTEXTURE9 tankbeeTexture;
LPDIRECT3DTEXTURE9 tankbeeheadTexture;

// Player textures
LPDIRECT3DTEXTURE9 reykTexture1;
LPDIRECT3DTEXTURE9 reykTexture2;
LPDIRECT3DTEXTURE9 reykTexture3;
LPDIRECT3DTEXTURE9 reykTexture4;

MODEL *platMesh=NULL;
MODEL *projectile0=NULL;
MODEL *projectile1=NULL;
MODEL *honeycombMesh;
MODEL *asteriskFruit;
MODEL *ringBerry;
MODEL *XFruit;
MODEL *OFruit;
MODEL *spikedAcorn;


int fileN = 1;
char * playlist = "Playlist.txt"; 
FSOUND_STREAM * Song;
char * fname = parseTextFile(playlist, fileN);
bool flag = true;

Credits *creds;

void loadBoss1();
void loadLevel2();
void loadBoss2();
void loadLevel3();
void loadBoss3();
void loadLevel4();
void loadBoss4();

int enemiesmin = 0;
int enemiesmax = 19;
int powerupsmin = 92;
int powerupsmax = 103;
int platformsmin = 0;
int platformsmax = 41;

bool deletintime = false;

/*========================================================================


========================================================================*/

bool Game_Init(HWND window)
{
	//MoveWindow(window, 200, 200, 960, 768, true);
	fileN++;
	//debugfile << (fname);
	//Song = LoadAndPlay(fname,100,0); //name, volume, loop (always 100 and 0)
	//if(!IS_SERVER)
		Song = LoadAndPlay("load.mp3",100,0); 

	debugfile.open("debug.txt");

	RECT temp;
	temp.left = 0;
	temp.right = 200;
	temp.top = 0;
	temp.bottom = 100;
	getSlope(temp, true);

    Direct3D_Init(window, SCREENW, SCREENH, false);
    DirectInput_Init(window);
    DirectSound_Init(window);

	//MoveWindow(window, 400, 100, 640, 512, true);

	LPDIRECT3DSURFACE9 loadingsurface = NULL;
    loadingsurface = LoadSurface("loading4.png");

	 //get pointer to the back buffer
    d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, 
        &backbuffer);

	RECT dr = { 0, 0, 1280, 1024};
	RECT sr = { 0, 0, 1280, 1024};

    d3ddev->StretchRect(loadingsurface, &sr, backbuffer, 
        &dr, D3DTEXF_NONE);
	d3ddev->BeginScene();
	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
	loadingsurface->Release();

	srand(GetTickCount());

    //create a font
    font = MakeFont("Arial Bold", 24);
    debugfont = MakeFont("Verdana Italic", 20);
	debugfont2 = MakeFont("Tahoma Bold", 36);
	/*
    //load audio files
    axe = LoadSound("axe.wav");
    jumping = LoadSound("jumping.wav");
    fireball = LoadSound("fireball.wav");

	apehit = LoadSound("apehit.wav");
	apecatch = LoadSound("apecatch.wav");
	apethrow = LoadSound("apethrow.wav");

	apebosshit = LoadSound("apebosshit.wav");
	apebossfinalhit = LoadSound("apebossfinalhit.wav");
	handslide = LoadSound("handslide.wav");
	snotrocket = LoadSound("snotrocket.wav");
	apefist = LoadSound("apebosshit.wav");

	decapedehit = LoadSound("decapedehit.wav");

	chameleonbarf = LoadSound("chameleonbarf.wav");

	crochit = LoadSound("crochit.wav");
	crocopen = LoadSound("crocopen.wav");
	crocattack = LoadSound("crocattack.wav");
	crunch = LoadSound("crunch.wav");

	explosion = LoadSound("explosion.wav");
	drill = LoadSound("drill.wav");
	lasercharge = LoadSound("lasercharge.wav");
	lightning = LoadSound("lightning.wav");

	beehit = LoadSound("beehit.wav");
	beefinal = LoadSound("beefinal.wav");
	crack = LoadSound("crack.wav");

	metalfist = LoadSound("metalfist.wav");
	shatterglass = LoadSound("shatterglass.wav");
	*/
	//load background
    LPDIRECT3DSURFACE9 image = NULL;
    image = LoadSurface("treebg4.png");
    if (!image) return false;

    //create background
    HRESULT result = 
    d3ddev->CreateOffscreenPlainSurface(
       5992,
        800,
        D3DFMT_X8R8G8B8,
        D3DPOOL_DEFAULT,
        &background,
        NULL);
    if (result != D3D_OK) return false;

    //copy image to upper left corner of background
    RECT source_rect = {0, 0, 5992, 800 };
    RECT dest_ul = { 0, 0, 5992, 800 };

    d3ddev->StretchRect(
        image, 
        &source_rect, 
        background, 
        &dest_ul, 
        D3DTEXF_NONE);

    //remove image
    image->Release();

	alphabetTexture= LoadTexture("alphabet.png");
    if (!alphabetTexture) {
        MessageBox(window, "Error loading sprite", "Error", 0);
        return false;
    }

	hpbarTexture= LoadTexture("hpbar.png");
    if (!hpbarTexture) {
        MessageBox(window, "Error loading sprite", "Error", 0);
        return false;
    }

	energybarTexture= LoadTexture("energybar.png");
    if (!energybarTexture) {
        MessageBox(window, "Error loading sprite", "Error", 0);
        return false;
    }

	apehealthbarTexture= LoadTexture("apehealthbar.png");
    if (!apehealthbarTexture) {
        MessageBox(window, "Error loading sprite", "Error", 0);
        return false;
    }

	platformTexture = LoadTexture("block.png");
	rampTexture = LoadTexture("ramp.png");
	squareTexture = LoadTexture("square.png");
	/*blueTexture = LoadTexture("blueblock.png");
	honeytop1Texture = LoadTexture("honeytop1.png");
	honeytop2Texture = LoadTexture("honeytop2.png");
	honey1Texture = LoadTexture("honey1.png");
	honey2Texture = LoadTexture("honey2.png");
	conveyerrightTexture = LoadTexture("conveyerright.png");
	conveyerrightendleftTexture = LoadTexture("conveyerrightendleft.png");
	conveyerrightendrightTexture = LoadTexture("conveyerrightendright.png");
	conveyerleftTexture = LoadTexture("conveyerleft.png");
	conveyerleftendleftTexture = LoadTexture("conveyerleftendleft.png");
	conveyerleftendrightTexture = LoadTexture("conveyerleftendright.png");
	*/
	/*chameleonTexture = LoadTexture("chameleon.png");
	tongueTexture = LoadTexture("tongue.png");
	decapedeTexture = LoadTexture("decapede.png");
	decapedeRTexture = LoadTexture("decapedeR.png");
	decapedeheadTexture = LoadTexture("decapedehead.png");
	decapedeheadRTexture = LoadTexture("decapedeheadR");
	decapedeeyeTexture = LoadTexture("decapedeeye.png");
	decapedeeyeRTexture = LoadTexture("decapedeeyeR.png");
	drillbeeTexture = LoadTexture("drillbee.png");
	lightningbeeTexture = LoadTexture("lightningbee.png");
	lightningTexture = LoadTexture("lightning.png");*/
	projectileapeTexture = LoadTexture("projectileape.png");
	projectileapeheadTexture = LoadTexture("projectileapehead.png");
	projectileTexture = LoadTexture("shit2.png");
	smallapeTexture = LoadTexture("smallape.png");
	smallapeheadTexture = LoadTexture("smallapehead.png");
	//tankbeeTexture = LoadTexture("tankbee.png");
	//tankbeeheadTexture = LoadTexture("tankbeehead.png");

	reykTexture1 = LoadTexture("reyknew.png");
	/*reykTexture2 = LoadTexture("reyknew2.png");
	reykTexture3 = LoadTexture("reyknew3.png");
	reykTexture4 = LoadTexture("reyknew4.png");
	*/
	d3ddev->SetRenderState(D3DRS_ZENABLE, true);

	platMesh = LoadModel("cube.x");
    if (platMesh == NULL)
    {
        MessageBox(window, "Error loading mesh", APPTITLE.c_str(), MB_OK);
        return 0;
    }

	projectile0 = LoadModel("spikedball.x");
    if (platMesh == NULL)
    {
        MessageBox(window, "Error loading mesh", APPTITLE.c_str(), MB_OK);
        return 0;
    }

	projectile1 = LoadModel("ringberry.x");
    if (platMesh == NULL)
    {
        MessageBox(window, "Error loading mesh", APPTITLE.c_str(), MB_OK);
        return 0;
    }
	
	honeycombMesh = LoadModel("honeycomb.x");
    if (honeycombMesh == NULL)
    {
        MessageBox(window, "Error loading mesh", APPTITLE.c_str(), MB_OK);
        return 0;
    }
	
	XFruit = LoadModel("Xfruit.x");
    if (XFruit == NULL)
    {
        MessageBox(window, "Error loading mesh", APPTITLE.c_str(), MB_OK);
        return 0;
    }

	asteriskFruit = LoadModel("asteriskfruit.x");
    if (asteriskFruit == NULL)
    {
        MessageBox(window, "Error loading mesh", APPTITLE.c_str(), MB_OK);
        return 0;
    }

	ringBerry = LoadModel("ringberry.x");
    if (ringBerry == NULL)
    {
        MessageBox(window, "Error loading mesh", APPTITLE.c_str(), MB_OK);
        return 0;
    }

	OFruit = LoadModel("OFruit.x");
    if (OFruit == NULL)
    {
        MessageBox(window, "Error loading mesh", APPTITLE.c_str(), MB_OK);
        return 0;
    }

	spikedAcorn = LoadModel("SpikedAcorn.x");
    if (spikedAcorn == NULL)
    {
        MessageBox(window, "Error loading mesh", APPTITLE.c_str(), MB_OK);
        return 0;
    }

	ph = new PlatformHandler();
	eh = new EnemyHandler();
	lh = new LyricsHandler();

	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		players[i] = new Reyk(i);
		players[i]->setTexture(GetPlayerTexture(i));
	}

	//aboss = new ApeBoss();
	//croc = new Croctopus();
	//qbee = new QueenBee();
	//xboss = new XBoss();
	
	//creds = new Credits();

	if (currentlevel == 2)
	{
		loadLevel2();
	}
	else if (currentlevel == 3)
	{
		loadLevel3();
	}

	gamebegintime = GetTickCount();
	songtime = 0;
    return true;
}

LPDIRECT3DTEXTURE9 GetPlayerTexture(int index)
{
	switch(index){
	case 0:
		return reykTexture1;
	case 1:
		return reykTexture2;
	case 2:
		return reykTexture3;
	case 3:
		return reykTexture4;
	default:
		return NULL;
	}
}

/*========================================================================


========================================================================*/

void Game_End()
{
    //background->Release();
    font->Release();
    debugfont->Release();
	debugfont2->Release();
	/*
    delete fireball;
    delete jumping;
    delete axe;

	delete apehit;
	delete apecatch;
	delete apethrow;

	delete apebosshit;
	delete apebossfinalhit;
	delete handslide;
	delete snotrocket;
	delete apefist;
	*/
	for(int i = 0; i < MAX_CLIENTS; i++)
		delete players[i];
	/*
	if (aboss)
		delete aboss;
	if (croc)
		delete croc;
	if (qbee)
		delete qbee;
	if (xboss)
		delete xboss;
	if (creds)
		delete creds;
		*/
	ph->end();

    DirectSound_Shutdown();
    DirectInput_Shutdown();
    Direct3D_Shutdown();

	debugfile.close();
}

/*========================================================================


========================================================================*/

template <class T>
std::string static ToString(const T & t, int places = 2)
{
    ostringstream oss;
    oss.precision(places);
    oss.setf(ios_base::fixed);
    oss << t; 
    return oss.str();
}


/*========================================================================


========================================================================*/

const double PI = 3.1415926535;
const double PI_under_180 = 180.0f / PI;
const double PI_over_180 = PI / 180.0f;

double toRadians(double degrees)
{
    return degrees * PI_over_180;
}

double toDegrees(double radians)
{
    return radians * PI_under_180;
}

/*========================================================================


========================================================================*/

void Vibrate(int contnum, int amount, int length)
{
    vibrating = 1;
    vibration = length;
    XInput_Vibrate(contnum, amount);
}

/*========================================================================


========================================================================*/
void Game_Run(HWND window)
{
    if (!d3ddev) return;
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
        D3DCOLOR_XRGB(0,0,100), 1.0f, 0);

    //get current ticks
    currenttime = timeGetTime();

    //calculate core frame rate
    corecount += 1.0;
    if (currenttime > coretime + 1000) 
    {
        corefps = corecount;
        corecount = 0.0;
        coretime = currenttime;
    }


    //run update on normal processes (like input) at 60 hz

    if (currenttime > refresh + 12)
    {
		if(flag ) {
			SwitchSong(Song, "level1.mp3");
			flag = false;
		}

		songtime = currenttime - gamebegintime;
		lh->update();

        refresh = currenttime;

        DirectInput_Update();

		ph->animatePlatforms();

		for(int i = 0; i < MAX_CLIENTS; i++)
		{
			players[i]->setoldmapY();
			players[i]->setOldX();
			players[i]->setOldY();
		}

		if (gamestatus == 5)
		{
			xboss->animate();
		}

        if (!paused)
        {
			if (deletintime)
			{
				if (currentlevel == 2 && gamestatus == 1)
				{
					delete aboss;
					background->Release();
				}
				else if (currentlevel == 3 && gamestatus == 1)
				{

					delete croc;
					background2b->Release();
					background2c->Release();
					background2d->Release();
					background2e->Release();
				}
				else if (currentlevel == 3 && gamestatus == 5)
				{
					delete qbee;
				}
				deletintime = false;
			}

				if (controllers[0].sThumbLX < -5000 || Key_Down(DIK_LEFT))
				{
					if(DEBUG) players[clientIndex]->setpressingleft(true);	
				}
				else
				{
					if(DEBUG) players[clientIndex]->setpressingleft(false);
				}

				if (controllers[0].sThumbLX > 5000 || Key_Down(DIK_RIGHT)) 
				{
					if(DEBUG) players[clientIndex]->setpressingright(true);
				}
				else
				{
					if(DEBUG) players[clientIndex]->setpressingright(false);
				}

				int p1status = players[clientIndex]->getStatus();

				if (p1status == 0 || p1status == 1 || p1status == 3)
				{
					if (controllers[0].sThumbLX < -5000 || Key_Down(DIK_LEFT))
					{
						if (p1status == 0)
						{
							if(DEBUG) players[clientIndex]->setStatus(1);
						}
						if(DEBUG) players[clientIndex]->setDirection(1);
						if(DEBUG) players[clientIndex]->Reyk_Move();	
					}
					else if (controllers[0].sThumbLX > 5000 || Key_Down(DIK_RIGHT)) 
					{
						if (p1status == 0)
						{
							if(DEBUG) players[clientIndex]->setStatus(1);
						}
						if(DEBUG) players[clientIndex]->setDirection(0);
						if(DEBUG) players[clientIndex]->Reyk_Move();
					}
					else if (p1status != 3)
					{
						if(DEBUG) players[clientIndex]->setStatus(0);	
					}
				}
				if (p1status == 5 || p1status == 9)
				{
					if(DEBUG) players[clientIndex]->Reyk_Move();
				}

				if(DEBUG) players[clientIndex]->Reyk_Update();

				if ((controllers[0].wButtons & XINPUT_GAMEPAD_X || Key_Down(DIK_Z)))
				{
					
					if (holdbutton[4] == 0
						&& p1status != 2 && p1status != 4 
						&& p1status != 6 && p1status != 7)
					{
						if (p1status == 3)
						{
							if ((controllers[0].sThumbLX < -5000 || Key_Down(DIK_LEFT)) || (controllers[0].sThumbLX > 5000 || Key_Down(DIK_RIGHT)))
							{
								if(DEBUG) players[clientIndex]->setfallingforward(true);
							}
						}
						if(DEBUG) players[clientIndex]->Reyk_Attack();
						holdbutton[4]++;
					}
					/*
					if (holdbutton[4] == 0)
					{
						debugfile << songtime << "," << endl;
						holdbutton[4]++;
					}
					*/
					else
					{
						holdbutton[4]++;
					}
				}
				else
				{
					holdbutton[4] = 0;
				}

				if ((controllers[0].wButtons & XINPUT_GAMEPAD_A || Key_Down(DIK_X)))
				{
					if (holdbutton[5] == 0)
						/*&& p1status != 3 && p1status != 4
						&& p1status != 6 && p1status != 7
						&& p1status != 10 && p1status != 11)*/
					{
						if(DEBUG) players[clientIndex]->Reyk_Jump();
						holdbutton[5]++;
					}
					else
					{
						holdbutton[5]++;
					}
				}
				else
				{
					holdbutton[5] = 0;
				}

				if ((controllers[0].wButtons & XINPUT_GAMEPAD_Y || Key_Down(DIK_A)))
				{
					if (holdbutton[6] == 0
						&& p1status != 2 && p1status != 4
						&& p1status != 6 && p1status != 7)
					{
						if (p1status == 3)
						{
							if ((controllers[0].sThumbLX < -5000 || Key_Down(DIK_LEFT)) || (controllers[0].sThumbLX > 5000 || Key_Down(DIK_RIGHT)))
							{
								if(DEBUG) players[clientIndex]->setfallingforward(true);
							}
						}
						if(DEBUG) players[clientIndex]->Reyk_Lick();
						holdbutton[6]++;
					}
					else
					{
						holdbutton[6]++;
					}
				}
				else
				{
					holdbutton[6] = 0;
				}

				if ((controllers[0].wButtons & XINPUT_GAMEPAD_B || Key_Down(DIK_S)))
				{
					if (holdbutton[7] == 0
						&& p1status != 2 && p1status != 4
						&& p1status != 6 && p1status != 7
						&& p1status != 9 && p1status != 10
						&& p1status != 5)
					{
						if (p1status == 3)
						{
							if ((controllers[0].sThumbLX < -5000 || Key_Down(DIK_LEFT)) || (controllers[0].sThumbLX > 5000 || Key_Down(DIK_RIGHT)))
							{
								if(DEBUG) players[clientIndex]->setfallingforward(true);
							}
						}
						if(DEBUG) players[clientIndex]->Reyk_BarfFireball();
						holdbutton[7]++;
					}
					else
					{
						holdbutton[7]++;
					}
				}
				else
				{
					holdbutton[7] = 0;
				}

			//} //end if(!IS_SERVER) block AKA client input code
			
			for(int i = 0; i < MAX_CLIENTS; i++)
			{
				players[i]->setNewX();
				players[i]->setNewY();
				ph->checkAllPlatformsCollision(players[i]);
			}

			//ph->checkAllPlatformsCollision();

			if(IS_SERVER)
			{ 
				ph->movePlatforms();
				if (gamestatus == 0)
				{
					rand2 = rand()%2;
					rand3 = rand()%3;
					rand4 = rand()%4;
					aboss->updateApe(rand2, rand3, rand4);
					aboss->updateBoogers();
					aboss->updateLeftHand();
					aboss->updateRightHand();	
				}

				if (gamestatus == 0 || gamestatus == 1 || gamestatus == 3 || gamestatus == 4)
				{
					//ph->checkAllPlatformsCollision();
				}
			
				//ph->movePlatforms(player1->getXchange(), player1->getYchange());
				

				//croc->move(player1->getXchange(), 0.0);
				if (gamestatus == 3)
				{
					croc->animate();

					randarr = croc->randomizeAttack();
					rand4 = rand()%4;
					rand3 = rand()%3;
					croc->update(rand4, randarr, rand3);
				}

				if (gamestatus == 4)
				{
					qbee->update();
				}

				if (gamestatus == 5)
				{
					xboss->update();
				}

				//eh->moveEnemies(player1->getXchange(), 0.0);
				rand2 = rand()%6;
				rand3 = rand()%800;
				rand4 = rand()%20;
				eh->updateEnemies(rand2, rand3, rand4);
			} // end server update code

		} // end if (!paused)
		else
		{
			creds->animate();
			creds->update();
		}

        //update controller vibration 
        if (vibrating > 0)
        {
            vibrating++;
            if (vibrating > vibration)
            {
                XInput_Vibrate(0, 0);
                vibrating = 0;
            }
        } 

        //calculate screen frame rate
        screencount += 1.0;
        if (currenttime > screentime + 1000) 
        {
            screenfps = screencount;
            screencount = 0.0;
            screentime = currenttime;
        }

        if (Key_Down(DIK_P) && IS_SERVER) {
			/*paused = !paused;
			
			bsOut.Write((RakNet::MessageID)ID_GAME_PAUSED);
			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 1, peer->GetMyGUID(), true);
			bsOut.Reset();*/
		}

        //if (KEY_DOWN(VK_ESCAPE)) gameover = true;
        //if (controllers[0].wButtons & XINPUT_GAMEPAD_BACK)
            //gameover = true;
    }// end if (currenttime > refresh + 14)
	//scrollx = 0;

	if (currentlevel == 1)
	{
		if (scrollx >= 1650 && scrollx <= 2100)
		{
			scrolly = 225 - (abs(scrollx - 1650)/450) * 225;
		}
		else if (scrollx >= 2100)
		{
			scrolly = 0;
		}
		else if (scrollx <= 1650)
		{
			scrolly = 225;
		}
	}
	else if (currentlevel == 2)
	{
		scrolly = 0 + 575;
	}
	else if (currentlevel == 3)
	{
		scrolly = 0;
	}
	
	if (!(gamestatus == 3 && (players[clientIndex]->getlockedleft() || players[clientIndex]->getlockedright()))
		&& !(gamestatus == 4 && (players[clientIndex]->getlockedleft() || players[clientIndex]->getlockedright())))
	{
		scrollx = ((players[clientIndex]->getMapX() - 415.0)/11984.0)*5992.0;
	}

	if (scrollx < 0)
	{
		scrollx = 0;
	}

	if (gamestatus == 0)
	{
		scrollx = 5350.0;
	}

	if (currentlevel == 2 || currentlevel == 3 || currentlevel == 4)
	{
		scrollx = 0;
		scrolly = 0;
	}

	if (currentlevel == 1)
	{
		//draw scrolling background
		RECT source_rect = {
			(long)scrollx, 
			(long)scrolly, 
			(long)scrollx+ 640/*SCREENW*/, 
			(long)scrolly+512/*SCREENH*/ 
		};
		RECT dest_rect = { 0, 0, 1280, 1024};
		d3ddev->StretchRect(background, &source_rect, backbuffer, 
			&dest_rect, D3DTEXF_NONE);
	}
	else if (currentlevel == 2)
	{
		waterfallanimcount++;

		if (waterfallanimcount > 150)
		{
			waterfallanimcount = 0;
			waterfallframe++;
			if (waterfallframe > 4)
			{
				waterfallframe = 0;
			}
		}
		//draw scrolling background
		RECT source_rect = {
			(long)scrollx, 
			(long)scrolly, 
			(long)scrollx + 1280/*SCREENW*/, 
			(long)scrolly + 1024/*SCREENH*/ 
		};
		RECT dest_rect = { 0, 0, 1280, 1024};

		switch (waterfallframe)
		{
		case 0:
			d3ddev->StretchRect(background2, &source_rect, backbuffer, 
				&dest_rect, D3DTEXF_NONE);
			break;
		case 1:
			d3ddev->StretchRect(background2b, &source_rect, backbuffer, 
				&dest_rect, D3DTEXF_NONE);
			break;
		case 2:
			d3ddev->StretchRect(background2c, &source_rect, backbuffer, 
				&dest_rect, D3DTEXF_NONE);
			break;
		case 3:
			d3ddev->StretchRect(background2d, &source_rect, backbuffer, 
				&dest_rect, D3DTEXF_NONE);
			break;
		case 4:
			d3ddev->StretchRect(background2e, &source_rect, backbuffer, 
				&dest_rect, D3DTEXF_NONE);
			break;
		}
	}
	else if (currentlevel == 3 || currentlevel == 4)
	{
		//draw scrolling background
		RECT source_rect = {
			(long)scrollx, 
			(long)scrolly, 
			(long)scrollx + 1280/*SCREENW*/, 
			(long)scrolly + 1024/*SCREENH*/ 
		};
		RECT dest_rect = { 0, 0, 1280, 1024};
		d3ddev->StretchRect(background2, &source_rect, backbuffer, 
			&dest_rect, D3DTEXF_NONE);
	}

	//========================================================================================================================
	//	DRAWIN TIME
	//========================================================================================================================

    //begin rendering
    if (d3ddev->BeginScene())
    {
		if (gamestatus == 0)
		{
			spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
			d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

			aboss->ApeBody_Sprite_Transform_Draw();

			spriteobj->End();
		}

		if (gamestatus == 4)
		{
			spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
			d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
			
			qbee->draw();
			
			spriteobj->End();
		}

		if (gamestatus != 10)
		{
			spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

			SetCamera(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

			d3ddev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			d3ddev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
		
			ph->drawPlatforms3D();

			spriteobj->End();

			d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
		}
		else
		{
			d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
		}
        spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		if (gamestatus != 10)
		{

			if (gamestatus == 3)
			{
				D3DCOLOR chealthcolor = D3DCOLOR_XRGB( (int)((1.0 - ((float)croc->gethp() / 210.0F)) * 255), (int)(((float)croc->gethp() / 210.0F) * 255), 0) ;
				Sprite_Transform_Draw(apehealthbarTexture, 0, 10, 1280, 106, 0, 1, 0.0F, ((float)(croc->gethp()) / 210.0F) , 1.0F, chealthcolor);
				croc->draw();
			}

			if (gamestatus == 4)
			{
				D3DCOLOR qhealthcolor = D3DCOLOR_XRGB( (int)((1.0 - ((float)qbee->gethp() / 40.0F)) * 255), (int)(((float)qbee->gethp() / 40.0F) * 255), 0) ;
				Sprite_Transform_Draw(apehealthbarTexture, 0, 10, 1280, 106, 0, 1, 0.0F, ((float)(qbee->gethp()) / 40.0F) , 1.0F, qhealthcolor);
			}

			if (gamestatus == 5)
			{
				D3DCOLOR qhealthcolor = D3DCOLOR_XRGB( (int)((1.0 - ((float)xboss->gethp() / 25.0F)) * 255), (int)(((float)xboss->gethp() / 25.0F) * 255), 0) ;
				Sprite_Transform_Draw(apehealthbarTexture, 0, 10, 1280, 106, 0, 1, 0.0F, ((float)(xboss->gethp()) / 25.0F) , 1.0F, qhealthcolor);
				xboss->draw();
			}

			if (gamestatus == 0)
			{

				D3DCOLOR ahealthcolor = D3DCOLOR_XRGB( (int)((1.0 - ((float)aboss->gethp() / 50.0F)) * 255), (int)(((float)aboss->gethp() / 50.0F) * 255), 0);
				Sprite_Transform_Draw(apehealthbarTexture, 0, 10, 1280, 106, 0, 1, 0.0F, ((float)(aboss->gethp()) / 50.0F) , 1.0F, ahealthcolor);
				aboss->ApeFace_Sprite_Transform_Draw();

				//ph->drawPlatforms();

				aboss->ApeNose_Sprite_Transform_Draw();

				if (aboss->getstatus() < 6)
				{
					for(int i = 0; i < MAX_CLIENTS; i++)
					{
						players[i]->Reyk_Animate();
						players[i]->Reyk_Transform_Draw();
					}

					aboss->ApeHandLeft_Sprite_Transform_Draw();
					aboss->ApeHandRight_Sprite_Transform_Draw();
				}
				else
				{
					aboss->ApeHandLeft_Sprite_Transform_Draw();
					for(int i = 0; i < MAX_CLIENTS; i++)
					{
						players[i]->Reyk_Animate();
						players[i]->Reyk_Transform_Draw();
					}
					aboss->ApeHandRight_Sprite_Transform_Draw();
				}

				aboss->Boogers_Sprite_Transform_Draw();
			}
			else
			{
				ph->drawPlatforms();
			}
			if (gamestatus == 1 || gamestatus == 0 || gamestatus == 3 || gamestatus == 4 || gamestatus == 5)
			{
				eh->drawEnemies();
				for(int i = 0; i < MAX_CLIENTS; i++)
					if (players[i]->getStatus() != 8)
					{
						players[i]->Reyk_Animate();
						players[i]->Reyk_Transform_Draw();
					}
			}

			if (gamestatus == 3)
			{
				croc->drawcrocs();
			}

			/*for (int i = 0; i < player1->gethp(); i++)
			{
				Sprite_Transform_Draw(hpbarTexture, 15 + (30*i), 970, 30, 50);
			}*/

			int y = 70;
			D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 255, 0);
			D3DCOLOR debugcolor = D3DCOLOR_ARGB(255, 0, 255, 0);

			//draw debug messages
		/*	FontPrint(debugfont, 0, y, "", debugcolor);
        
			FontPrint(debugfont, 0, y-40, 
				"Core FPS = " + ToString(corefps) 
				+ " (" + ToString(1000.0 / corefps) + " ms)", 
				debugcolor);
				*/
			FontPrint(debugfont2, 1120, y-72, 
			   ToString(screenfps), 
				debugcolor);
			
			FontPrint(debugfont2, 1120, y-42, 
			   ToString(songtime), 
				debugcolor);
			}
			else
			{
				creds->draw();
			}
        spriteobj->End();

		
		/* second pass, render 3D elements*/
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		SetCamera(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

		d3ddev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		d3ddev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

		if (gamestatus != 10)
		{
			if (currentlevel != 4)
			{
				eh->drawParticles();

				if (gamestatus == 0)
				{
					aboss->drawParticle();
				}
				if (gamestatus == 3)
				{
					croc->drawParticle();
				}
			}
			d3ddev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );

			if (currentlevel != 4)
			{
				eh->draw3D();
				eh->drawPowerups();
				for(int i = 0; i < MAX_CLIENTS; i++)
					players[i]->Reyk_Draw3D();
			}
		}
		else
		{
			/* second pass, render 3D elements*/
			spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

			SetCamera(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

			d3ddev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			d3ddev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

			creds->drawFire();
		}
		
		spriteobj->End();

        spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
		d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

		if (gamestatus != 10)
		{
			for (int i = 0; i < players[clientIndex]->gethp(); i++)
			{
				D3DCOLOR rhealthcolor = D3DCOLOR_XRGB( (int)((1.0 - ((float)players[clientIndex]->gethp() / 16.0F)) * 255), (int)(((float)players[clientIndex]->gethp() / 16.0F) * 255), 0);
				Sprite_Transform_Draw(hpbarTexture, 15 + (30*i), 10, 30, 50, 0, 1, 0.0, 1.0, 1.0, rhealthcolor);
			}

			D3DCOLOR energycolor = D3DCOLOR_XRGB( (int)((1.0 - ((float)players[clientIndex]->getenergy() / 100.0F)) * 255), (int)(((float)players[clientIndex]->getenergy() / 100.0F) * 255), 0);
			Sprite_Transform_Draw(energybarTexture, 15, 70, 480, 40, 0, 1, 0.0, ((float)(players[clientIndex]->getenergy()) / 100.0F), 1.0F, energycolor);
		}
		songtime = currenttime - gamebegintime;
		lh->draw();

		spriteobj->End();
        d3ddev->EndScene();
        d3ddev->Present(NULL, NULL, NULL, NULL);
    } // end rendering
}

/*========================================================================


========================================================================*/

bool detectCollision(RECT r1, RECT r2)
{
	return !(r1.left > r2.right || r1.right < r2.left || r1.top > r2.bottom || r1.bottom < r2.top);
}

/*========================================================================


========================================================================*/

void getSlope(RECT r, bool goingright)
{
	if (goingright)
	{
		long p1x = r.left;
		long p1y = r.bottom;
		long p2x = r.right;
		long p2y = r.top;

		double slope = ((double)p1y - (double)p2y) / ((double)p2x - (double)p1x);
		long intercept = p1y - (slope * (double)p2x);

		long x = rand() % (int)(p2x-p1x);
		long yint = slope * (double)x + (double)intercept;

		//debugfile << "Equation is y = " << slope << "x + " << intercept << endl;
		//debugfile << "X value is " << x << " which intercepts slope at y = " << yint << endl;
	}
}

/*========================================================================


========================================================================*/

void loadBoss1()
{
	aboss = new ApeBoss();
	enemiesmin = 90;
	enemiesmax = 92;
	platformsmin = 25;
	deletintime = true;
}

/*========================================================================


========================================================================*/

void loadLevel2()
{
	enemiesmin = 20;
	enemiesmax = 40;
	platformsmax = 367;

	//delete background;

	//load background
    LPDIRECT3DSURFACE9 image2 = NULL;
    image2 = LoadSurface("waterfall.png");

    //create background
    HRESULT result = 
    d3ddev->CreateOffscreenPlainSurface(
       1280,
        1024,
        D3DFMT_X8R8G8B8,
        D3DPOOL_DEFAULT,
        &background2,
        NULL);

    //copy image to upper left corner of background
    RECT source_rect2 = {0, 0, 1280, 1024 };
    RECT dest_ul2 = { 0, 0, 1280, 1024 };

    d3ddev->StretchRect(
        image2, 
        &source_rect2, 
        background2, 
        &dest_ul2, 
        D3DTEXF_NONE);

    //remove image
    image2->Release();

	//load background
    LPDIRECT3DSURFACE9 image2b = NULL;
    image2b = LoadSurface("waterfallb.png");

    //create background
    result = 
    d3ddev->CreateOffscreenPlainSurface(
       1280,
        1024,
        D3DFMT_X8R8G8B8,
        D3DPOOL_DEFAULT,
        &background2b,
        NULL);

    d3ddev->StretchRect(
        image2b, 
        &source_rect2, 
        background2b, 
        &dest_ul2, 
        D3DTEXF_NONE);

    //remove image
    image2b->Release();

	//load background
    LPDIRECT3DSURFACE9 image2c = NULL;
    image2c = LoadSurface("waterfallc.png");

    //create background
    result = 
    d3ddev->CreateOffscreenPlainSurface(
       1280,
        1024,
        D3DFMT_X8R8G8B8,
        D3DPOOL_DEFAULT,
        &background2c,
        NULL);

    d3ddev->StretchRect(
        image2c, 
        &source_rect2, 
        background2c, 
        &dest_ul2, 
        D3DTEXF_NONE);

    //remove image
    image2c->Release();

	//load background
    LPDIRECT3DSURFACE9 image2d = NULL;
    image2d = LoadSurface("waterfalld.png");

    //create background
    result = 
    d3ddev->CreateOffscreenPlainSurface(
       1280,
        1024,
        D3DFMT_X8R8G8B8,
        D3DPOOL_DEFAULT,
        &background2d,
        NULL);

    d3ddev->StretchRect(
        image2d, 
        &source_rect2, 
        background2d, 
        &dest_ul2, 
        D3DTEXF_NONE);

    //remove image
    image2d->Release();

	//load background
    LPDIRECT3DSURFACE9 image2e = NULL;
    image2e = LoadSurface("waterfalle.png");

    //create background
    result = 
    d3ddev->CreateOffscreenPlainSurface(
       1280,
        1024,
        D3DFMT_X8R8G8B8,
        D3DPOOL_DEFAULT,
        &background2e,
        NULL);

    d3ddev->StretchRect(
        image2e, 
        &source_rect2, 
        background2e, 
        &dest_ul2, 
        D3DTEXF_NONE);

    //remove image
    image2e->Release();

	chameleonTexture = LoadTexture("chameleon.png");
	tongueTexture = LoadTexture("tongue.png");
	decapedeTexture = LoadTexture("decapede.png");
	decapedeRTexture = LoadTexture("decapedeR.png");
	decapedeheadTexture = LoadTexture("decapedehead.png");
	decapedeheadRTexture = LoadTexture("decapedeheadR");
	decapedeeyeTexture = LoadTexture("decapedeeye.png");
	decapedeeyeRTexture = LoadTexture("decapedeeyeR.png");

	deletintime = true;
}

/*========================================================================


========================================================================*/

void loadBoss2()
{
	platformsmin = 352;
	croc = new Croctopus();
	enemiesmin = 1000000;
	enemiesmax = 0;

	deletintime = true;
}

/*========================================================================


========================================================================*/

void loadLevel3()
{
	platformsmax = 1340;

	enemiesmin = 42;
	enemiesmax = 85;

	//load background
    LPDIRECT3DSURFACE9 image3 = NULL;
    image3 = LoadSurface("robobeehive.png");

    //copy image to upper left corner of background
    RECT source_rect3 = {0, 0, 1280, 1024 };
    RECT dest_ul3 = { 0, 0, 1280, 1024 };

    d3ddev->StretchRect(
        image3, 
        &source_rect3, 
        background2, 
        &dest_ul3, 
        D3DTEXF_NONE);

    //remove image
    image3->Release();

	drillbeeTexture = LoadTexture("drillbee.png");
	lightningbeeTexture = LoadTexture("lightningbee.png");
	lightningTexture = LoadTexture("lightning.png");
	tankbeeTexture = LoadTexture("tankbee.png");
	tankbeeheadTexture = LoadTexture("tankbeehead.png");

	honeytop1Texture = LoadTexture("honeytop1.png");
	honeytop2Texture = LoadTexture("honeytop2.png");
	honey1Texture = LoadTexture("honey1.png");
	honey2Texture = LoadTexture("honey2.png");
	conveyerrightTexture = LoadTexture("conveyerright.png");
	conveyerrightendleftTexture = LoadTexture("conveyerrightendleft.png");
	conveyerrightendrightTexture = LoadTexture("conveyerrightendright.png");
	conveyerleftTexture = LoadTexture("conveyerleft.png");
	conveyerleftendleftTexture = LoadTexture("conveyerleftendleft.png");
	conveyerleftendrightTexture = LoadTexture("conveyerleftendright.png");

	deletintime = true;
}

/*========================================================================


========================================================================*/

void loadBoss3()
{
	qbee = new QueenBee();
	platformsmin = 981;
	platformsmax = 1420;
	enemiesmin = 84;
	enemiesmax = 91;

	deletintime = true;
}

/*========================================================================


========================================================================*/

void loadBoss4()
{
	xboss = new XBoss();
	
	enemiesmin = 100;
	enemiesmax = 91;

	deletintime = true;
}

/*========================================================================


========================================================================*/

void loadCredits()
{
	delete xboss;

	platformsmin = 1000000;
	enemiesmin = 1000000;

	creds = new Credits();
	gamestatus = 10;
	currentlevel = 10;
	paused = true;
	SwitchSong(Song, "credits.mp3");
}
