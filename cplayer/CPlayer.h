#ifndef __CPLAYER_H
#define __CPLAYER_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include "bass.h"
using namespace std;





class CPlayer 
{
public:
	CPlayer();
	~CPlayer();
	bool loadAudioFile( string );
	void unloadAudioFile();
	void play();
	void stop();
	void addToPlayList( string );
	void removeFromPlayList( int );
	void showPlayList();
	bool isPlayListEmpty();
	void playNext();
	void playPrevious();
	void volumeUp();
	void volumeDown();

private:
	vector < pair<int, string> > m_Playlist;
	string m_currentAudioFile;
	int m_currentSongID;
	HSTREAM m_pAudioStream;
	float m_volume;
	bool m_playing;
};




#endif

