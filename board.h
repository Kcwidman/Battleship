#ifndef BOARD_H
#define BOARD_H
#include"ship.h"

//each player has 5 ships with following slot sizes {2,3,3,4,5}
class board
{
    public:
    char** shotGrid;//. empty,0 miss,X hit
    char** shipGrid;//. emtpty,o safe,! hit
    ship** ships;
    int totalHitCount;//total hits that own ships have taken
//computer turn variables
    int radius;
    bool trackingMode;//used for the computer turn
    int hits[20];//used for computer turn
    //hits ordering. 0-hits since activated, 1-misses in a row, ordered pairs of hits (x,y)

    board();
    ~board();
    void setup();//takes user io to place ships
        bool shipCheck();//ensures user defined ships placement remains on the board, true=good input
    void playerTurn(board &comp);//called every turn of the round
        bool shotInputCheck(int shotX,int shotY);//ensures shots remain on the board and there are no repeats,true=good input
        bool hitCheck(board &comp,int shotX,int shotY);//true=hit
        int sinkCheck(board &comp,int shotX,int shotY);//0 for still alive, size for sunk
    void compTurn(board &player);//computer AI
        bool clumpCheck(int shotX,int shotY,int &shotRepeatCount,board &player);//checks for repeates and ensures shots are spread out, true=shot is good
        bool hitCheckComp(board &player,int shotX,int shotY);//true=hit
        int sinkCheckComp(board &player,int shotX,int shotY);//0 for still alive, size for sunk
        char trackingDirection();
    void shotPrint();//prints the shotGrid
    void shipPrint();//prints the shipGrid
};
#endif