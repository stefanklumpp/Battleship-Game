#ifndef EXTRA_T_H
#define EXTRA_T_H


enum ConditionT {ocean, nohit, hit, countersunk, ship, restricted, error};

enum PreviewT {none, ok, moep};

enum AlignmentT {h=0, v=1}; //v = vertical    h = horizontal

enum ShipAlignmentT {horizontal, vertical, unknown};

enum ShipT {boat=1, submarine=2, cruiser=3, destroyer=4, carrier=5};

enum PlayerT {human, enemy};

enum GameModeT {setup, shot, wait};


#endif
