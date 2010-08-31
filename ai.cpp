#include "ai.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>

AI::AI(PlayerT pt, QObject *battleship) : Player(pt, battleship)
{
	mode = wait;
	srand(time(NULL));
	resetStrategy();
}

void AI::slotPlaceFleet()
{
	mode = setup;
	int moep=1;
	//random algorithm
	while (fleet->status < 5) {	//status will be increased if Player.placeShip() is successful
		if (fleet->createShip() == 0) {
			while (moep)
				moep = placeShip(rand()%10, rand()%10, rand()%2);
			moep = 1;
		}
	}
	mode = wait;
	emit sigFleetComplete();	//TO: battleship.slotPlayerReady()
}

void AI::reset()
{
	int row, col;
	
	resetStrategy();
	
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

void AI::slotFire()
{
	QPoint target;
	if (targetAquired)
		target = destroyShipFire();
	else
		target = randomFire();	
	emit sigShotAtHuman(target.y(), target.x());	//TO: humanPlayer.slotShotAt(row, col)
}

QPoint AI::randomFire()
{
	//random algorithm
	QPoint target(rand()%10, rand()%10);
	return target;
}

QPoint AI::destroyShipFire()
{
	QPoint target;
	if (shipAlignment == horizontal) {
		if (lastHit.x() < 9) {
			target.setX(lastHit.x()+1);
			target.setY(firstHit.y());
		} else
			stepsBack++;
		if (stepsBack > 0) {
			target.setX(firstHit.x()-stepsBack);
			target.setY(firstHit.y());
		}
	} else if (shipAlignment == vertical) {
		if (lastHit.y() < 9) {
			target.setX(firstHit.x());
			target.setY(lastHit.y()+1);
		} else
			stepsBack++;
		if (stepsBack > 0) {
			target.setX(firstHit.x());
			target.setY(firstHit.y()-stepsBack);
		}
	} else	{	//shipAlignment ist unknown
		switch (attempts) {
			case 0: if (firstHit.x() < 9) {
					target.setX(firstHit.x()+1);
					target.setY(firstHit.y());
					attempts++;
					break;
				}
				attempts++;
			case 1: if (firstHit.y() < 9) {
					target.setX(firstHit.x());
					target.setY(firstHit.y()+1);
					attempts++;
					break;
				}
				attempts++;
			case 2: if (firstHit.x() > 0) {
					target.setX(firstHit.x()-1);
					target.setY(firstHit.y());
					attempts++;
					break;
				}
				attempts++;
			case 3: if (firstHit.y() > 0) {
					target.setX(firstHit.x());
					target.setY(firstHit.y()-1);
					break;
				}
			default: break; //should be nothing to do
		}
	}
	return target;
}

void AI::slotFireFeedback(int row, int col, ConditionT condi)
{
	if (condi == hit) {
		if (targetAquired) {//already hit that ship
			if (firstHit == lastHit) {	//this is second hit
				if (firstHit.y() == row)
					shipAlignment = horizontal;
				else if (firstHit.x() == col)
					shipAlignment = vertical;
			}
		} else {	//first hit
			firstHit.setX(col);
			firstHit.setY(row);
			targetAquired = true;
		}
		lastHit.setX(col);
		lastHit.setY(row);
		attempts = 0;
	} else if (condi == countersunk) 
		resetStrategy();
	else if (targetAquired && condi == nohit || condi == error) {
		if (shipAlignment != unknown)
			stepsBack++;
	}
	
	return;
}

void AI::resetStrategy()
{
	attempts = 0;
	stepsBack = 0;
	shipAlignment = unknown;
	targetAquired = false;
	lastHit.setX(10);
	lastHit.setY(10);
	firstHit.setX(10);
	firstHit.setY(10);
}
