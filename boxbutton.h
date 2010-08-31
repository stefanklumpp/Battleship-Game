#ifndef BOX_BUTTON_H
#define BOX_BUTTON_H

#include <QtGui/QPushButton>
#include <QtCore/QEvent>
#include "extra_t.h"
#include "ship.h"
class Battleship;

class BoxButton : public QPushButton {
   Q_OBJECT

public:
	int position[2];			 //[0]=row  [1]=col
	Ship *associatedShip;	//to which ship it belongs
	bool ewoMode;

	BoxButton(PlayerT p, Battleship *battleship, QWidget *parent=NULL);	// constructor
	
	ConditionT onFire();
	ConditionT getCondition();
	ConditionT setCondition(ConditionT c);
	PreviewT setPreview(PreviewT p);
	PreviewT getPreview();
	void updateColor();

protected:
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);

private:
	ConditionT condition;
	PreviewT preview;
	PlayerT playert;			//to which player it belongs
		   
public slots:
	void slotOnClick();

signals:
	void sigRelayClick(PlayerT pt, int row, int col); // TO: Battleship.slotClickOnField
	void sigPreview(int row, int col); //To: human.slotPreviewShip(row, col);
	void sigUnPreview(int row, int col); //To: human.slotUnPreviewShip(row, col);
};

#endif
