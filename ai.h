#ifndef AI_H
#define AI_H

#include <QtCore/QPoint>
#include "player.h"
#include "extra_t.h"

class AI : public Player
{
	Q_OBJECT
	
public:
	AI(PlayerT pt, QObject *battleship);	// constructor
	
	void reset();
	
public slots:
	void slotPlaceFleet();
	void slotFire();
	void slotFireFeedback(int row, int col, ConditionT condi);

signals:
	void sigShotAtHuman(int row, int col);


private:
	int attempts;
	int stepsBack;
	bool targetAquired;
	QPoint lastHit;
	QPoint firstHit;
	ShipAlignmentT shipAlignment;

	void resetStrategy();
	QPoint destroyShipFire();
	QPoint randomFire();
};

#endif
