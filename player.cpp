#include "player.h"
#include <iostream>

// constructor
Player::Player(PlayerT pt, QObject *battleship) : QObject(battleship)
{
	ptype = pt;
	fleet = new Fleet(ptype, battleship);
}

int Player::placeShip(int row, int col, int ali)
{
	if (checkBoxes(row, col, ali) == 0) {
		if (setBoxes(row, col, ali) == 0) {
			fleet->status++;
			if (fleet->status == 5) {
				mode = wait;
				field[row][col]->setPreview(none); //disable preview for the last ship (boat):
				emit sigFleetComplete();	//TO: battleship.slotPlayerReady()
			}
			if (ptype == human)
				emit sigPlaySound("splish.wav");
			return 0;
		}
	}
	return 1;
}

// destructor
Player::~Player()
{
	delete fleet;

}



void Player::slotPlaceFleet()
{
	mode = setup;
}

int Player::checkBoxes(int row, int col, int ali)
{
	if (ali == (int)h && (col+4-fleet->status) < 10)	{	//Ship horizontal and inside field
		int c=col;
		for (c = col; c < (col+5-fleet->status); c++) { //check if other ships are in the way
			if (field[row][c]->getCondition() != ocean)
				return 1;
		}
		return 0;
	}
	if (ali == (int)v && (row+4-fleet->status) < 10)	{	//Ship vertical and inside field
		int r=row;
		for (r = row; r < (row+5-fleet->status); r++) { // check if other ships are in the way
			if (field[r][col]->getCondition() != ocean)
				return 1;
		}
		return 0;
	}
	return 1;
}

int Player::setBoxes(int row, int col, int ali)
{
	int c=col, r=row, i=0;
	if (ali == (int)h)	{	//Ship horizontal and inside field
		for (c = col-1; c < (col+6-fleet->status); c++) { // set boxes on the field
			if (c >= col && c < (col+5-fleet->status)) { //set boxes in this interval to ship
				field[row][c]->setCondition(ship);
				if (row+1 < 10)
					field[row+1][c]->setCondition(restricted);
				if (row-1 >= 0)
					field[row-1][c]->setCondition(restricted);
				field[row][c]->associatedShip = fleet->ship[fleet->status];
				fleet->ship[fleet->status]->box[i] = field[row][c];
				i++;
			} else if (c >= 0 && c < 10) {
				field[row][c]->setCondition(restricted);
				if (row+1 < 10)
					field[row+1][c]->setCondition(restricted);
				if (row-1 >= 0)
					field[row-1][c]->setCondition(restricted);
			}
		}
		return 0;
	}
	i=0;
	if (ali == (int)v)	{	//Ship vertical and inside field
		for (r = row-1; r < (row+6-fleet->status); r++) { // set boxes on the field
			if (r >= row && r < (row+5-fleet->status)) { //set boxes in this interval to ship
				field[r][col]->setCondition(ship);
				if (col+1 < 10)
					field[r][col+1]->setCondition(restricted);
				if (col-1 >= 0)
					field[r][col-1]->setCondition(restricted);
				field[r][col]->associatedShip = fleet->ship[fleet->status];
				fleet->ship[fleet->status]->box[i] = field[r][col];
				i++;
			} else if (r >= 0 && r < 10) {
				field[r][col]->setCondition(restricted);
				if (col+1 < 10)
					field[r][col+1]->setCondition(restricted);
				if (col-1 >= 0)
					field[r][col-1]->setCondition(restricted);
			}
		}
		return 0;
	}
	return 1;
}

void Player::slotPlaceShip(int row, int col, int ali)
{
	placeShip(row, col, ali);
}

void Player::slotShotAt(int row, int col)	// Player gets shot
{
	ConditionT condi;
	if (row>=0 && row<10 && col>=0 && col<10) {
		//std::cout << "Auf Player " << (int)ptype << " geschossen: row: " << row << " , col: " << col << " condition: " << (int)field[row][col]->getCondition() << std::endl; // for debug
		condi = field[row][col]->onFire();
		emit sigFireFeedback(row, col, condi);
		if (condi == hit || condi == countersunk || condi == error) {
			if (condi == countersunk) {
				fleet->status--;
				//if (ptype == enemy) {
					emit sigPlaySound("explosion1.wav");
					emit sigPlayDelayedSound("underWater.wav", 600);
				//}
			} else if (condi == hit/* && ptype == enemy*/) {
				emit sigPlaySound("explosion2.wav");
			}
			if (fleet->status == 0) {
				mode = wait;
				emit sigGameOver(ptype); //ptype has lost the game
			} else {	// shoot again
				if (ptype == enemy) {
					emit sigDone(human);
				} else if (condi == error){ //ptype == human && condi == error  -> request new shot imediately
					emit sigEnemyTryAgain();
				} else {
					emit sigDone(enemy);
				}
			}
		} else if (condi == nohit){		// enemy may shoot now
			//if (ptype == enemy)
				emit sigPlaySound("splash.wav");
			emit sigDone(ptype);
		}
	} else
		emit sigPrint("ERROR!!! out of bounds");	// This can not happen ;-)
	return;
}

void Player::reset()
{
	int row, col;

	// Reset BoxButtons
	for (row=0; row<10; row++) {
		for (col=0; col<10; col++) {
			field[row][col]->setCondition(ocean);
			field[row][col]->associatedShip=NULL;
			field[row][col]->updateColor();
		}
	}

	fleet->reset();
}
