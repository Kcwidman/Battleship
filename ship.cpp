#include"ship.h"
#include<chrono>
#include<random>

ship::ship(int inputSize)
{
//RNG 
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed);

    size=inputSize;
    shipHitCount=0;
    if(generator()%2==0) orien=false;//randomly assigns orientation
    else orien=true;
//randomly assigning location but ensures ships remain on board
    if(orien==false)
    {
        xLoc=generator()%(11-size);
        yLoc=generator()%10;
    }
    else
    {
        xLoc=generator()%10;
        yLoc=generator()%(11-size);
    }
}