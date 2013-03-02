#include "CPlayer.h"



int main ( void ) {

	cout << "CPlayer v0.1 - 2013" << endl << endl;
	CPlayer * player = new CPlayer();
	string input;

	 do {
		cout << "> ";
		getline( cin, input );
		if ( input == "play" ) {
			player->play();
		} else if ( input == "stop" ) {
			player->stop();
		} else if ( (input.find("add ") != -1) && (input.length() > 8) ) {
			string song = input.substr( 4 );
			player->addToPlayList(song);
		} else if ( input == "remove" ) {
			player->removeFromPlayList(0);
		} else if ( input == "show" ) {
			player->showPlayList();
		} else if ( input == "next" ) {
			player->playNext();
		} else if ( input == "prev" ) {
			player->playPrevious();
		} else if ( input == "vu" ) {
			player->volumeUp();
		} else if ( input == "vd" ) {
			player->volumeDown();
		} else if ( input.empty() ) {
			continue;
		}
	} while ( (input != "quit") );
	
	delete player;
	return 0;
}



