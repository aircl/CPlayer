#include "CPlayer.h"



CPlayer::CPlayer() {
	if ( !BASS_Init ( -1, 44100, NULL, NULL, NULL ) ) {
		cout << "BASS ERROR: " << BASS_ErrorGetCode() << endl;
	}
	m_volume = 0.2f;
	BASS_SetVolume( m_volume );
	m_playing = false;
	m_currentSongID = 0;
	m_currentAudioFile = "";
	m_pAudioStream = 0;
}



CPlayer::~CPlayer() {
	if ( m_pAudioStream ) {
		BASS_ChannelPlay( m_pAudioStream, false );
	}
	unloadAudioFile();
}


bool CPlayer::loadAudioFile( string filePath ) {
	m_currentAudioFile = filePath;
	if ( m_pAudioStream = BASS_StreamCreateFile( FALSE, filePath.c_str(), 0, 0, 0 ) )
		return true;
	return false;
}


void CPlayer::unloadAudioFile() {
	if ( m_pAudioStream ) {
		BASS_ChannelStop( m_pAudioStream );
		m_playing = false;
		BASS_StreamFree( m_pAudioStream );
		m_pAudioStream = 0;
	}
}



void CPlayer::play() {
	if ( isPlayListEmpty() ) {
		cout << "# Playlist is empty" << endl;
	} else {
		if ( m_pAudioStream && !m_playing ) {
			m_playing = true;
			BASS_CHANNELINFO m_AudioInfo;
			BASS_ChannelPlay( m_pAudioStream, false );
			BASS_ChannelGetInfo( m_pAudioStream, &m_AudioInfo );
			double audioLengthInSeconds, audioLengthInMinutes;
			audioLengthInSeconds = (BASS_ChannelBytes2Seconds(m_pAudioStream, BASS_ChannelGetLength(m_pAudioStream, BASS_POS_BYTE)))/60;
			audioLengthInSeconds = modf (audioLengthInSeconds, &audioLengthInMinutes);
			audioLengthInSeconds *= 60;
			cout << "# Now Playing: " << m_AudioInfo.filename << " - " << (int)audioLengthInMinutes << ":" << (int)audioLengthInSeconds << " @ " <<  m_AudioInfo.freq << " Hz" << endl;
		} else if ( !m_pAudioStream ) {
			m_currentSongID = 0;
			if ( loadAudioFile( m_Playlist.at( m_currentSongID ).second ) ) {
				play();
			} else {
				cout << "# Error playing file: " << m_Playlist.at( m_currentSongID ).second << " - removing from playlist." << endl;
				removeFromPlayList( m_currentSongID );
			}
		} 
	}
}


void CPlayer::stop() {
	if ( m_pAudioStream ) {
		BASS_ChannelStop( m_pAudioStream );
		m_playing = false;
	}
}


void CPlayer::playNext() {
	unloadAudioFile();
	if ( isPlayListEmpty() ) {
		cout << "# Playlist is empty" << endl;
	} else {
		if ( m_Playlist.size() > (m_currentSongID + 1) ) {
			if ( loadAudioFile( m_Playlist.at( m_currentSongID + 1 ).second ) ) {
				cout << "# Playing next song ID: " << (m_currentSongID + 1) << endl;
				m_currentSongID = m_Playlist.at( m_currentSongID + 1 ).first;
				play();
			} else {
				cout << "# Error playing file: " << m_Playlist.at( m_currentSongID + 1 ).second << " - removing from playlist." << endl;
				removeFromPlayList( m_currentSongID + 1 );
			}
		} else if ( loadAudioFile( m_Playlist.at( 0 ).second ) ) {
			cout << "# Playing next song ID: " << 0 << endl;
			m_currentSongID = m_Playlist.at( 0 ).first;
			play();
		} else {
			cout << "Error playing next song." << endl;
		}
	}
}

void CPlayer::playPrevious() {
	unloadAudioFile();
	if ( isPlayListEmpty() ) {
		cout << "# Playlist is empty" << endl;
	} else {
		if ( (m_currentSongID - 1) > -1 ) {
			if ( loadAudioFile( m_Playlist.at( m_currentSongID - 1 ).second ) ) {
				cout << "# Playing next song ID: " << (m_currentSongID - 1) << endl;
				m_currentSongID = m_Playlist.at( m_currentSongID - 1 ).first;
				play();
			} else {
				cout << "# Error playing file: " << m_Playlist.at( m_currentSongID - 1 ).second << " - removing from playlist." << endl;
				removeFromPlayList( m_currentSongID - 1 );
			}
		} else if ( loadAudioFile( m_Playlist.at( m_Playlist.size()-1 ).second ) ) {
			cout << "# Playing next song ID: " << m_Playlist.size()-1 << endl;
			m_currentSongID = m_Playlist.at( m_Playlist.size()-1 ).first;
			play();
		} else {
			cout << "Error playing next song." << endl;
		}
	}
}


void CPlayer::volumeUp() {
	if ( m_volume < 1.0f ) {
		m_volume += 0.02f;
		BASS_SetVolume( m_volume );
		cout << "# Volume: " << (int)(m_volume*100) << "%" << endl;
	} else {
		m_volume = 1.0f;
		BASS_SetVolume( m_volume );
		cout << "# Volume: " << (int)(m_volume*100) << "%" << endl;
	}
}

void CPlayer::volumeDown() {
	if ( m_volume > 0.1f ) {
		m_volume -= 0.02f;
		BASS_SetVolume( m_volume );
		cout << "# Volume: " << (int)(m_volume*100) << "%" << endl;
	} else {
		m_volume = 0.0f;
		BASS_SetVolume( m_volume );
		cout << "# Volume: " << (int)(m_volume*100) << "%" << endl;
	}
}



// Playlist control
void CPlayer::addToPlayList( string file ) {
	m_Playlist.push_back( make_pair( m_Playlist.size(), file ) );
	cout << "# Added " << file << " to playlist" << endl;
}


void CPlayer::removeFromPlayList( int songId ) {
	if ( m_Playlist.size() > songId && songId > -1 ) {
		unloadAudioFile();
		swap( m_Playlist.at(songId), m_Playlist.back() );
		m_Playlist.pop_back();
		cout << "# Removed song ID: " << songId << endl;
	} else {
		cout << "# Playlist is empty" << endl;
	}
}

void CPlayer::showPlayList( ) {
	for ( int i = 0; i < m_Playlist.size(); i++ ) {
		cout << i << ": " << m_Playlist.at(i).second << endl;
	}	
	if ( isPlayListEmpty() ) {
		cout << "# Playlist is empty" << endl;
	}
}

bool CPlayer::isPlayListEmpty() {
	if ( m_Playlist.size() == 0 ) {
		return true;
	}
	return false;
}