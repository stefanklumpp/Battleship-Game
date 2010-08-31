#include "battleship.h"
#include <iostream>
#include <time.h>
#include <QMouseEvent>
#include <QtCore/QDir>
#include <QSound>
#include <QtCore/QTimer>

// Constructor
Battleship::Battleship(QWidget *parent) : QMainWindow(parent)
{
	QApplication::setStyle("plastique");
	setupUi(this);
	gridLayoutHuman->sizePolicy().setHeightForWidth(true);
	gridLayoutEnemy->sizePolicy().setHeightForWidth(true);
	labelStatus = new QLabel(QString::fromUtf8("Press Ön to start the Game"), statusbar);
	statusbar->setSizeGripEnabled(true);
	statusbar->addWidget(labelStatus, 3);
	imageDialog = new ImageDialog(this);
	alignment = h;	// set default alignment
	enemyPlayer = new AI(enemy, this);		// Create players and their fields
	setupField(enemyPlayer);
	humanPlayer = new Human(human, this);
	setupField(humanPlayer);

	round = 0;		// for first round

	gridLayoutHuman->installEventFilter(this);	// EventFilter for right mouse button

	gridLayoutEnemy->setCursor(QCursor(Qt::ForbiddenCursor));
	gridLayoutHuman->setCursor(QCursor(Qt::ForbiddenCursor));


	// Buttons:
	connect(pushButtonEwo, SIGNAL(toggled(bool)), this, SLOT(slotSetEwoMode(bool)));
	connect(pushButtonStartGame, SIGNAL(clicked()), this, SLOT(slotStartGame()));

	// Place fleet & ships
	connect(this, SIGNAL(sigHumanPlaceShip(int, int, int)), humanPlayer, SLOT(slotPlaceShip(int, int, int)));
	connect(this, SIGNAL(sigPlaceFleet()), humanPlayer, SLOT(slotPlaceFleet()));
	connect(this, SIGNAL(sigPlaceFleet()), enemyPlayer, SLOT(slotPlaceFleet()));
	connect(humanPlayer, SIGNAL(sigFleetComplete()), this, SLOT(slotPlayerReady()));
	connect(enemyPlayer, SIGNAL(sigFleetComplete()), this, SLOT(slotPlayerReady()));

	// players get shot
	connect(this, SIGNAL(sigShotAtEnemy(int, int)), enemyPlayer, SLOT(slotShotAt(int, int)));
	connect(enemyPlayer, SIGNAL(sigShotAtHuman(int, int)), humanPlayer, SLOT(slotShotAt(int, int)));

	// toggle player turn
	connect(enemyPlayer, SIGNAL(sigDone(PlayerT)), this, SLOT(slotPlayerTurn(PlayerT)));
	connect(humanPlayer, SIGNAL(sigDone(PlayerT)), this, SLOT(slotPlayerTurn(PlayerT)));

	// AI shall shoot
	connect(this, SIGNAL(sigRequestShotFromEnemy()), enemyPlayer, SLOT(slotFire()));
	connect(humanPlayer, SIGNAL(sigEnemyTryAgain()), this, SLOT(slotRequestShotNow()));

	//Feedback of shots for AI
	connect(humanPlayer, SIGNAL(sigFireFeedback(int, int, ConditionT)), enemyPlayer, SLOT(slotFireFeedback(int, int, ConditionT)));

	// Game Over
	connect(enemyPlayer, SIGNAL(sigGameOver(PlayerT)), this, SLOT(slotGameOver(PlayerT)));
	connect(humanPlayer, SIGNAL(sigGameOver(PlayerT)), this, SLOT(slotGameOver(PlayerT)));

	// console output
	connect(humanPlayer, SIGNAL(sigPrint(QString)), this, SLOT(slotPrintToConsole(QString)));
	connect(enemyPlayer, SIGNAL(sigPrint(QString)), this, SLOT(slotPrintToConsole(QString)));

	//sound output
	connect(humanPlayer, SIGNAL(sigPlaySound(QString)), this, SLOT(slotPlaySound(QString)));
	connect(enemyPlayer, SIGNAL(sigPlaySound(QString)), this, SLOT(slotPlaySound(QString)));
	connect(humanPlayer, SIGNAL(sigPlayDelayedSound(QString, int)), this, SLOT(slotPlayDelayedSound(QString, int)));
	connect(enemyPlayer, SIGNAL(sigPlayDelayedSound(QString, int)), this, SLOT(slotPlayDelayedSound(QString, int)));

	// help menu -> display info
	infoDialog = new InfoDialog(this);
	connect(actionManual, SIGNAL(triggered()), infoDialog, SLOT(slotLoadTutorial()));
	connect(actionAbout, SIGNAL(triggered()), infoDialog, SLOT(slotLoadAbout()));
	connect(actionLicense, SIGNAL(triggered()), infoDialog, SLOT(slotLoadLicense()));

	//options menu
	connect(actionSound_on, SIGNAL(triggered()), this, SLOT(slotSoundOn()));

	if(QSound::isAvailable()) {
		soundAvailable = true;
		soundOn = true;
		slotPrintToConsole("Sound is availiable\n");
	} else {
		soundAvailable = false;
		soundOn = false;
		slotPrintToConsole("Sound is not availiable\n");
	}
}

// Destructor
Battleship::~Battleship()
{
	int row, col;
	for (row=0; row<10; row++) {
		for (col=0; col<10; col++) {
			delete humanPlayer->field[row][col];		// deleting the dynamically created BoxButtons
		}
	}

	for (row=0; row<10; row++) {
		for (col=0; col<10; col++) {
			delete enemyPlayer->field[row][col];		// deleting the dynamically created BoxButtons
		}
	}

	delete enemyPlayer;
	delete humanPlayer;

}


void Battleship::setupField(Player *player)
{
	// Create BoxButtons, assign them to the right "gridLayout" and save its coordinates
	int col, row;
	QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	sizePolicy.setHeightForWidth(true);
	sizePolicy.setHorizontalStretch(1);
	sizePolicy.setVerticalStretch(1);
	for (row=0; row<10; row++) {
		for (col=0; col<10; col++) {
			player->field[row][col] = new BoxButton(player->ptype, this);
			player->field[row][col]->setSizePolicy(sizePolicy);
			player->field[row][col]->setFocusPolicy(Qt::NoFocus);
			player->field[row][col]->setMinimumSize(20,20);
			if (player->ptype == human) {
				gridLayout3->addWidget(player->field[row][col], row, col);
			}
			else {
				gridLayout2->addWidget(player->field[row][col], row, col);
			}
			player->field[row][col]->position[0] = row;
			player->field[row][col]->position[1] = col;
		}
	}
	return;
}

void Battleship::slotClickOnField(PlayerT pt, int row, int col)
{
	// Place Ship
	if (humanPlayer->mode == setup && pt == human)	{ // pt=PlayerType means: from which field the clickedEvent has been called
		//std::cout << "Versuch Schiff zu setzen" << std::endl;	// debug output
		emit sigHumanPlaceShip(row, col, (int) alignment);	//TO: player.slotPlaceShip
	}
	// Fire on enemy field
	if (humanPlayer->mode == shot && enemyPlayer->mode == shot && pt == enemy && turn == human) {
		blockBoxSignals(true);
		turn = enemy;
		//std::cout << "click: sigShotAtEnemy wird ausgeloest (row: " << row << ", col: " << col << std::endl;  //debug
		emit sigShotAtEnemy(row, col);	//TO: enemyPlayer.slotShotAt(row, col)
	}
	return;
}

void Battleship::toggleAlignment()
{
	if (alignment == h) {
		alignment = v;
		slotPlaySound("clonk.wav");
		labelStatus->setText(QString::fromUtf8("Click on your field to place ship vertically"));
		statusbar->showMessage("Vertical ship alignment", 500);
	} else {
		alignment = h;
		slotPlaySound("clonk.wav");
		labelStatus->setText(QString::fromUtf8("Click on your field to place ship horizontally"));
		statusbar->showMessage("Horizontal ship alignment", 500);
	}

	return;
}

void Battleship::slotSetEwoMode(bool checked)
{
	int col, row;
	if (checked) {
		for (row=0; row<10; row++) {
			for (col=0; col<10; col++) {
				enemyPlayer->field[row][col]->ewoMode=true;
				enemyPlayer->field[row][col]->updateColor();
			}
		}
		statusbar->showMessage("EWO-Mode activated", 1000);
		slotPlaySound("easy.wav");
	} else {
		for (row=0; row<10; row++) {
			for (col=0; col<10; col++) {
				enemyPlayer->field[row][col]->ewoMode=false;
				enemyPlayer->field[row][col]->updateColor();
		}
		statusbar->showMessage("EWO-Mode deactivated", 1000);
	}
}
return;

}

void Battleship::slotStartGame()
{
	if (round > 0) {
	/*** Reset ***/
		console->clear();
		slotPrintToConsole("New Round...\n");

		slotSetEwoMode(false);
		pushButtonEwo->setChecked(false);
		pushButtonEwo->setEnabled(false);

		blockBoxSignals(false);

		// Reset Players
		humanPlayer->reset();
		enemyPlayer->reset();
	}

	/*** Normal run ***/
	//if (checked)
		emit sigPlaceFleet();	//TO: both players: slotPlaceFleet()
	slotPlaySound("startRound.wav");
	alignment = h;	// set default alignment
	gridLayoutEnemy->setCursor(QCursor(Qt::ForbiddenCursor));
	gridLayoutHuman->setCursor(QCursor(Qt::PointingHandCursor));
	console->insertPlainText("Please place your fleet:\nRight click to toggle between horizontal and vertical ship alignment\nCarrier [5 boxes]\n");
	labelStatus->setText(QString::fromUtf8("Click on your field to place ship horizontally"));
	statusbar->showMessage("Game started...", 700);
	round++; // next Round

	return;
}

void Battleship::slotGameOver(PlayerT pt)
{
	blockBoxSignals(true);
	gridLayoutEnemy->setCursor(QCursor(Qt::ForbiddenCursor));
	gridLayoutHuman->setCursor(QCursor(Qt::ForbiddenCursor));
	if (pt == enemy) {
		slotPrintToConsole("You won this game!!!\n");
		imageDialog->slotYouWon();
		slotPlayDelayedSound("jingle.wav", 2700);
	} else {
		slotPrintToConsole("You lost this game!!!\n");
		imageDialog->slotYouLost();
		slotPlayDelayedSound("laugh.wav", 300);
		slotPlayDelayedSound("laugh.wav", 3500);
	}
}



void Battleship::slotPlayerReady()
{
	if (humanPlayer->mode == wait && enemyPlayer->mode == wait) {
		humanPlayer->mode = shot;
		enemyPlayer->mode = shot;
		turn = human; 	// humanPlayer's turn
		gridLayoutEnemy->setCursor(QCursor(Qt::CrossCursor));
		gridLayoutHuman->setCursor(QCursor(Qt::ForbiddenCursor));
		slotPrintToConsole("Let the battle begin....\n");
		labelStatus->setText(QString::fromUtf8("Click on the hostile field to shot at your enemy"));
		statusbar->showMessage("Let the battle begin...", 2000);
		pushButtonEwo->setEnabled(true);
	}
	return;
}


void Battleship::slotPlayerTurn(PlayerT pt)
{
	if (pt == human) {
		gridLayoutEnemy->setCursor(QCursor(Qt::CrossCursor));
		turn = human;
		blockBoxSignals(false);
		//std::cout << "box click Signale wieder freigegeben" << std::endl; //debug
	}
	else if (pt == enemy) {
		gridLayoutEnemy->setCursor(QCursor(Qt::ForbiddenCursor));
		turn = enemy;
		QTimer::singleShot(600, this, SLOT(slotRequestShotNow()));
	}
	return;
}

void Battleship::slotRequestShotNow()
{
	emit sigRequestShotFromEnemy();
}

void Battleship::slotPrintToConsole(QString text)
{
	console->insertPlainText(text);
	console->ensureCursorVisible();
	return;
}


void Battleship::blockBoxSignals(bool block)
{
	int row, col;
	for (row=0; row<10; row++) {
		for (col=0; col<10; col++) {
			enemyPlayer->field[row][col]->blockSignals(block);
		}
	}
}

bool Battleship::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonRelease && humanPlayer->mode == setup) {
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
		if (mouseEvent->button() == Qt::RightButton) {
			int row = mouseEvent->y()*10/gridLayoutHuman->height();
			int col = mouseEvent->x()*10/gridLayoutHuman->width();
			//std::cout << "right click: row " << row << ", col " << col << std::endl; //debug
			humanPlayer->slotUnPreviewShip(row, col);
			toggleAlignment();
			humanPlayer->slotPreviewShip(row, col);
		}
	}
	// standard event processing
	return QMainWindow::eventFilter(obj, event);
}

void Battleship::slotPlaySound(QString wavFile)
{
	if (soundOn) {
	        QString path = QDir::currentPath();
	        path.append("/sounds/");
                path.append(wavFile);
		//QSound soundFile(path);
		//soundFile.play();
		QSound::play(path);
	}
}

void Battleship::slotPlayDelayedSound(QString wavFile, int msecs)
{
	if (soundOn) {
                delayedSoundFile = wavFile;
                QTimer::singleShot(msecs, this, SLOT(slotPlayDelayedSoundNow()));
	}
}

void Battleship::slotPlayDelayedSoundNow()
{
        slotPlaySound(delayedSoundFile);
}

void Battleship::slotSoundOn()
{
	if (soundAvailable)
		soundOn = actionSound_on->isChecked();
	else
		soundOn = false;
}
