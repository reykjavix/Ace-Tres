// YOU NEED TO COPY THIS FILE/LINK



#include <stdlib.h>
#include "fmod.h"
#include "fmod_errors.h"
#include <iostream>
#include <fstream>

using namespace std;



#ifndef _SONGPLAYER_H_
#define _SONGPLAYER_H_


/// \brief	You must call this function before calling any other SongPlayer function
extern FSOUND_STREAM * LoadAndPlay(char *p_FileName, int p_Volume, int p_Loop);

extern bool UpdateSong(FSOUND_STREAM * & SongData, char *p_FileName, int p_Volume, int p_Loop);


extern void SongPlayerClose(FSOUND_STREAM * & SongData);

extern void SwitchSong(FSOUND_STREAM * & SongData, char *p_FileName);

//Assumptions:  Song files are in directory, text file is in directory which has the names of the song files to be played, in order, one file name per line.  A song that is to be looped infinitely should have a line containing only '.' (a period) following that file name.
extern char * parseTextFile(char * textFileName, int fileNumber); 

#endif
