#include "fleet.h"

// constructor
Fleet::Fleet(PlayerT pt, QObject *parent) : QObject(parent)
{
	status = 0;
	ptype = pt;
	battleship = parent;
	int i;
	for (i=0;i<5;i++) {
		ship[i] = NULL;
	}

}



int Fleet::createShip()
{
	switch (status) {
		case 0: ship[0] = new Ship(ptype, carrier, battleship); break;
		case 1: ship[1] = new Ship(ptype, destroyer, battleship); break;
		case 2: ship[2] = new Ship(ptype, cruiser, battleship); break;
		case 3: ship[3] = new Ship(ptype, submarine, battleship); break;
		case 4: ship[4] = new Ship(ptype, boat, battleship); break;
		default: return 1;
	}
	return 0;
}

void Fleet::reset()
{
	int i;
	
	// Delete Ships
	for (i=0;i<5;i++) {
		if (ship[i] != NULL) {
			delete (ship[i]);
			ship[i] = NULL;
		}
	}
	
	// Reset Fleet Status Counter
	status=0;
}
