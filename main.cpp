/********************************************************************************************************

Battleship
---------------

Battleship is a single and multiplayer strategy game.
It was developed in Spring 2006 by Stefan Klumpp and Felix Ruess as a
project thesis at the University of Applied Sciences Ulm (FH-Ulm)

This Software is Open Source and licensed under the GPL.

It is written in C++ with grant of Qt,
an open source, cross-platform graphical widget toolkit.

Battleship runs under UNIX, UNIX-like operating systems, Mac OS X and MS Windows.

*********************************************************************************************************/
#include <QApplication>
#include "battleship.h"

int main( int argc, char **argv )
{
    QApplication app(argc, argv);
    
    Battleship window;
    window.show();
    

    return app.exec();
}


