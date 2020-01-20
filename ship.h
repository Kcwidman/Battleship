#ifndef SHIP_H
#define SHIP_H

class ship
{
    public:
    ship(int m_size);
    int size;//vales from 2-5
    bool orien;//0-hor, 1-vert
    bool* slots;//0-safe, 1-hit, reads top-down or left-right
    int xLoc;//values from 0-9
    int yLoc;//locations given my upper left most slot
    int shipHitCount;//values from 0-size, if shipHitCount==size then ship is sunk
};
#endif