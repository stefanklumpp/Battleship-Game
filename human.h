#ifndef HUMAN_H
#define HUMAN_H

#include "player.h"
class Battleship;

class Human : public Player
{
	Q_OBJECT
	
public:
	Human(PlayerT pt, Battleship *battleship);	// constructor

private:
	Battleship *bship;
	
public slots:
	void slotPlaceShip(int row, int col, int ali);
	void slotPreviewShip(int row, int col);
	void slotUnPreviewShip(int row, int col);
};

#endif
