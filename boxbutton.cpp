#include "boxbutton.h"
#include "battleship.h"
#include <iostream>


// constructor
BoxButton::BoxButton(PlayerT p, Battleship *battleship, QWidget *parent)  : QPushButton(parent)
{
	playert = p;
	condition = ocean;
	preview = none;
	ewoMode = false;
	associatedShip = NULL;
	updateColor();
	connect(this, SIGNAL(clicked()), this, SLOT(slotOnClick()));
	connect(this, SIGNAL(sigRelayClick(PlayerT, int, int)), battleship, SLOT(slotClickOnField(PlayerT, int, int)));
	if (playert == human) {
		//std::cout << "versuche connects" << std::endl; // for debug
		if (!connect(this, SIGNAL(sigPreview(int, int)), battleship->humanPlayer, SLOT(slotPreviewShip(int, int))))
			std::cout << "sigPreview connect fehlgeschlagen" << std::endl; // for debug
		if (!connect(this, SIGNAL(sigUnPreview(int, int)), battleship->humanPlayer, SLOT(slotUnPreviewShip(int, int))))
			std::cout << "sigUnPreview connect fehlgeschlagen" << std::endl; // for debug
	}
}

ConditionT BoxButton::onFire() {
	switch (condition) {
		case ocean: condition = nohit; break;
		case restricted: condition = nohit; break;
		case ship: associatedShip->onFire(this); break;
		default: return error;
	}
	updateColor();
	return condition;
}

ConditionT BoxButton::getCondition()
{
	return condition;
}

ConditionT BoxButton::setCondition(ConditionT c)
{
	condition = c;
	updateColor();
	return condition;
}

PreviewT BoxButton::setPreview(PreviewT p)
{
	preview = p;
	updateColor();
	return preview;
}

PreviewT BoxButton::getPreview()
{
	return preview;
}

void BoxButton::slotOnClick()
{
	emit sigRelayClick(playert, position[0], position[1]);	//TO: battleship.slotClickOnField(pt, row, col)
	return;
}

void BoxButton::updateColor()
{
	//std::cout << "boxButton: updateColor()" << std::endl; // for debug
	if (preview != none) {
		//std::cout << "boxButton: updateColor() preview!=none" << std::endl; // for debug
		if (preview == ok)
			setPalette(QPalette(Qt::green));
		else
			setPalette(QPalette(Qt::red));
	} else {
		switch (condition) {
			case ocean: setPalette(QPalette(Qt::blue)); break;
			case restricted: setPalette(QPalette(Qt::blue)); break;
			case hit: setPalette(QPalette(Qt::yellow)); break;
			case nohit: setPalette(QPalette(Qt::darkBlue)); break;
			case countersunk: setPalette(QPalette(Qt::red)); break;
			case ship:
				if (playert == human)
					setPalette(QPalette(Qt::green));
				else if (playert == enemy && ewoMode == true)
					setPalette(QPalette(Qt::cyan));
				else
					setPalette(QPalette(Qt::blue));
				break;
			default: setPalette(QPalette(Qt::white)); break;
		}
	}
	//repaint();	//force repaint immediately
}

void BoxButton::enterEvent(QEvent *)
{
	//std::cout << "sigPreview emitiert" << std::endl; // for debug
	if (playert == human)
		emit sigPreview(position[0], position[1]); //To: human.slotPreviewShip(row, col);
	return;
}

void BoxButton::leaveEvent(QEvent *)
{
	//std::cout << "sigUnPreview emitiert" << std::endl; // for debug
	if (playert == human)
		emit sigUnPreview(position[0], position[1]); //To: human.slotUnPreviewShip(row, col);
	return;
}
