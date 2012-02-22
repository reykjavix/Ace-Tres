#include "MyDirectX.h"
#include "LyricsHandler.h"

extern LPDIRECT3DTEXTURE9 alphabetTexture;
extern DWORD songtime;

string lyrics1[10] = {	"",
						"NOW THIS IS A STORY",
						"ALL ABOUT HOW",
						"MY LIFE GOT FLIPPED",
						"TURNED UPSIDE DOWN",
						"AND I'D LIKE TO TAKE A MINUTE",
						"JUST SIT RIGHT THERE",
						"I'LL TELL YOU HOW I BECAME THE PRINCE",
						"OF A TOWN CALLED BEL-AIR",
						""};

DWORD lyricstime1[10] = {	11731,
						13084,
						14181,
						15403,
						16535,
						17748,
						18914,
						20167,
						21798,
						22000};


LyricsHandler::LyricsHandler()
{
	currentlyrics = lyrics1;
	currentlyricstime = lyricstime1;
	lyricsR = 255.0F;
	lyricsG = 255.0F;
	lyricsB = 255.0F;
	currentcolor = D3DXCOLOR(lyricsR, lyricsG, lyricsB, 1.0F);
	lyricsstate = 0;
	currentline = -1;
	drawing = false;
}

void LyricsHandler::update()
{
	if (!drawing)
	{
		if (songtime < currentlyricstime[0])
		{
			return;
		}
		else
		{
			drawing = true;
		}
	}

	if (songtime >= currentlyricstime[currentline] && currentline < 10)
	{
		currentline++;
	}

	switch(lyricsstate)
	{
	case 0:
		lyricsR -= 10.0;
		lyricsG += 10.0;
		if (lyricsR <= 0.0)
		{
			lyricsstate = 1;
		}
		break;
	case 1:
		lyricsG -= 10.0;
		lyricsB += 10.0;
		if (lyricsG <= 0.0)
		{
			lyricsstate = 2;
		}
		break;
	case 2:
		lyricsB -= 10.0;
		lyricsR += 10.0;
		if (lyricsB <= 0.0)
		{
			lyricsstate = 0;
		}
	}

	currentcolor = D3DXCOLOR(lyricsR/255.0F, lyricsG/255.0F, lyricsB/255.0F, 1.0F);
}

void LyricsHandler::draw()
{
	if (!drawing || currentline >= 10 || currentline < 0)
		return;

	int startX = 640 - (15 * currentlyrics[currentline].length());
	int drawframe = 0;

	for (int i = 0; i <  currentlyrics[currentline].length(); i++)
	{
		if ((int)currentlyrics[currentline][i] == 39)
			drawframe = 26;
		else if ((int)currentlyrics[currentline][i] == 34)
			drawframe = 27;
		else if ((int)currentlyrics[currentline][i] == 45)
			drawframe = 28;
		else if ((int)currentlyrics[currentline][i] == 33)
			drawframe = 29;
		else
			drawframe = (int)(currentlyrics[currentline][i]) - 65;

		Sprite_Transform_Draw(alphabetTexture, startX + (i * 30), 960, 30, 50, drawframe, 6, 0.0F, 1.0F, 1.0F, currentcolor);
	}
}

int LyricsHandler::getcurrentline()
{
	return currentline;
}

int LyricsHandler::getcurrentlyricstime()
{
	return currentlyricstime[currentline];
}

