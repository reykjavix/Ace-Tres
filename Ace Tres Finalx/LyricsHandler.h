#include <d3d9.h>
#include <d3dx9.h>

class LyricsHandler
{
	string * currentlyrics;
	DWORD * currentlyricstime;
	D3DXCOLOR currentcolor;
	int currentline;
	bool drawing;

	float lyricsR;
	float lyricsG;
	float lyricsB;
	int lyricsstate;

public:
	LyricsHandler();
	void draw();
	void update();
	int getcurrentline();
	int getcurrentlyricstime();
};