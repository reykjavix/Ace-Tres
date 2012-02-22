//-----------------------------------------------------------------------------
//--SONGPLAYER CLASS-----------------------------------------------------------
//-----------------------------------------------------------------------------
// Current Methods: 
//    LoadAndPlay() - don't use/ignore looping code (looping logic is handled above this level)
//    UpdateSong()  - don't use/ignore looping code (looping logic is handled above this level)
//    SongPlayerClose() - for very end
//    parseTextFile()
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include "fmod.h"
#include "fmod_errors.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

#ifdef WIN32
	#include <windows.h>
	// automatically link to fmod library
	#pragma comment(lib,"fmod.lib")
#else
	#include <wincompat.h>
#endif

//FSOUND_SAMPLE *m_SampleData; /*pointer to sample, loaded memory */
int window = 350;  // if there are issues with looping, this value probably needs to decrease
int loop = 0;


//--LoadAndPlay() - for first song only
FSOUND_STREAM * LoadAndPlay(char *p_FileName, int p_Volume, int p_Loop)
{
	if( FSOUND_Init(44000,64,0) == FALSE )
	{
		std::cerr << "[ERROR] Could not initialize fmod\n";
		return 0;
	}

  if((p_FileName == NULL) || (p_Volume < 0)) { return(false); }

  //if(m_SongData != NULL) { FSOUND_Stream_Close(m_SongData); m_SongData = NULL; }
  FSOUND_STREAM * m_SongData = FSOUND_Stream_Open(p_FileName, FSOUND_LOOP_NORMAL, 0, 0);

  if(m_SongData == NULL) { return(false); }

  loop = p_Loop;

  FSOUND_Stream_SetLoopCount(m_SongData, p_Loop);  // currently no looping implemented
  FSOUND_SetVolumeAbsolute(0, p_Volume);
  FSOUND_Stream_Play(0, m_SongData);

  return(m_SongData);
}


// UpdateSong() will return true when the last song is done and the next one up has begun
// *Assumes UpdateSong() is called continuously (at least every ~350ms)
bool UpdateSong(FSOUND_STREAM * & SongData, char *p_FileName, int p_Volume, int p_Loop) {

	p_Loop = 0; // for now, don't use this method of looping inside UpdateSong(), it's done outside

	int currentt = FSOUND_Stream_GetTime(SongData);
	int lengthms = FSOUND_Stream_GetLengthMs(SongData); 
	int check = lengthms-window;// window of 350 ms - update calls must be more frequent than every 350ms (else increase window)
	
	// Checks current time of song - if song is done, load next song and play
	if (currentt >= check) {
			if (loop > 0) { // possible bug - if this is called frequently, the window variable probably needs to be decreased
				loop = loop -1;
			} else {
				SongData = FSOUND_Stream_Open(p_FileName, FSOUND_LOOP_NORMAL, 0, 0);
				if(SongData == NULL) { return(false); }
	 
				FSOUND_Stream_SetLoopCount(SongData, p_Loop);
				FSOUND_SetVolumeAbsolute(0, p_Volume);	
				FSOUND_Stream_Play(0, SongData);
				loop = p_Loop;
				return true;
			}
	}
	else return false;
}

void SwitchSong(FSOUND_STREAM * & SongData, char *p_FileName) {
	SongData = FSOUND_Stream_Open(p_FileName, FSOUND_LOOP_NORMAL, 0, 0);
				
	FSOUND_Stream_SetLoopCount(SongData, 0);
	FSOUND_SetVolumeAbsolute(0, 0);	
	FSOUND_Stream_Play(0, SongData);

}

void SongPlayerClose(FSOUND_STREAM * & SongData) {
		// Stop and close the mp3 file
	FSOUND_Stream_Stop( SongData );
	FSOUND_Stream_Close( SongData );
		// kill off fmod
	FSOUND_Close();
}


char * parseTextFile(char * textFileName, int fileNumber) {
	std::ifstream textFile;
	textFile.open(textFileName);
	char retFileName[255]; // 

	if (textFile.fail()){
		char * empty = "fail";
      return (empty); // cannot open file
	} 

	do {
		textFile.getline(retFileName, 255); // gets overwritten each time, delim defaults to new line

		fileNumber--;
	} while (textFile.is_open() && (fileNumber != 0) );
	textFile.close();
	// will return the file name on [int fileNumber] line of the text file
	return retFileName;

}