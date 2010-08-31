#include "human.h"
#include "battleship.h"
#include <iostream>

Human::Human(PlayerT pt, Battleship *battleship) : Player(pt, (QObject*)battleship)
{
	mode = wait;
	bship = battleship;
}

void Human::slotPlaceShip(int row, int col, int ali)
{
	fleet->createShip();
	if(!placeShip(row, col, ali)) {	//print messages if ship placement was possible
		switch (fleet->status) {
			case 1: emit sigPrint("Destroyer [4 boxes]\n"); break;
			case 2: emit sigPrint("Cruiser [3 boxes]\n"); break;
			case 3: emit sigPrint("Submarine [2 boxes]\n"); break;
			case 4: emit sigPrint("Boat [1 box]\n"); break;
			default: break;
		} 
	}
	return;
}

void Human::slotPreviewShip(int row, int col)
{
	//std::cout << "slotPreviewShip aufgerufen" << std::endl; // for debug
	if (mode == setup) {
		int i;
		if (bship->alignment == h) {
			if (checkBoxes(row, col, 0) == 0) {
				for (i = col; i < (col+5-fleet->status); i++) {
					field[row][i]->setPreview(ok);
				}
			} else {	
				for (i = col; (i < (col+5-fleet->status)) && (i < 10); i++) {
					field[row][i]->setPreview(moep);
				}
			}
		} else {
			if (checkBoxes(row, col, 1) == 0) {
				for (i = row; i < (row+5-fleet->status); i++) {
					field[i][col]->setPreview(ok);
				}
			} else {
				for (i = row; (i < (row+5-fleet->status)) && (i < 10); i++) {
					field[i][col]->setPreview(moep);
				}
			}
		}
	}
	return;
}

void Human::slotUnPreviewShip(int row, int col)
{
	//std::cout << "slotUnPreviewShip aufgerufen" << std::endl; // for debug
	if (mode == setup) {
		int i;
		if (bship->alignment == h) {
			for (i = col; (i < (col+6-fleet->status)) && (i < 10); i++) {
				field[row][i]->setPreview(none);
			}
		} else {
			for (i = row; (i < (row+6-fleet->status)) && (i < 10); i++) {
				field[i][col]->setPreview(none);
			}
		}
	}
	return;
}
