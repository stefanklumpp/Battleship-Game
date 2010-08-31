#include "ship.h"
#include "boxbutton.h"

// constructor
Ship::Ship(PlayerT pt, ShipT st, QObject *parent) : QObject(parent)
{
	stype = st;
	if (pt == human)
		player = "Own";
	else
		player = "Enemy";
	status = (int) stype;
	alignment = (int)h;
	box = new BoxButton*[(int)stype];
	
	connect(this, SIGNAL(sigPrint(QString)), parent, SLOT(slotPrintToConsole(QString)));
}

// destructor
Ship::~Ship()
{
	delete box;
}


void Ship::onFire(BoxButton *hitBox)
{
	int i;
	QString  tmp(player);
	status--;	// decrease status if ship is hit
	if (status == 0) {	// ship is destroyed
		for (i=0; i<(int)stype; i++)
			box[i]->setCondition(countersunk);
		switch (stype) {
			case 5: emit sigPrint(tmp.append(" carrier has been destroyed\n")); break;
			case 4: emit sigPrint(tmp.append(" destroyer has been destroyed\n")); break;
			case 3: emit sigPrint(tmp.append(" cruiser has been destroyed\n")); break;
			case 2: emit sigPrint(tmp.append(" submarine has been destroyed\n")); break;
			case 1: emit sigPrint(tmp.append(" boat has been destroyed\n")); break;
			default: break;
		}
	}
	else
		hitBox->setCondition(hit);
}
