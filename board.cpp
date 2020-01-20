#include"board.h"
#include"ship.h"
#include<iostream>
#include<string>
#include<chrono>
#include<random>

board::board()
{
//initializations and while loop if ships overlap
    totalHitCount=0;
    radius=2;//used for clumpCheck
    trackingMode=false;//used for computer turn
    for(int i=0;i<20;i++)
    {
        hits[i]=0;//initializes the hits array to 0
    }
    int slotCount=0;//total slot count, used to check for overlap
    while(slotCount!=17)
    {
//creates 5 ships with slots {2,3,3,4,5}
        ships=new ship*[5];
        int j=0;
        for(int i=0;i<5;i++)
        {
            if(i==1||i==3||i==4) j++;
            ships[i]=new ship(2+j);
        }
//creates empty grids
        shipGrid=new char*[10];
        shotGrid=new char*[10];
        for(int i=0;i<10;i++)
        {
            shipGrid[i]=new char[10];
            shotGrid[i]=new char[10];
            for(int j=0;j<10;j++)
            {
                shipGrid[i][j]='.';
                shotGrid[i][j]='.';
            }
        }
//places ships into the shipGrid
        for(int i=0;i<5;i++)//iterates through ships
        {
            for(int j=0;j<ships[i]->size;j++)
            {
                if(ships[i]->orien==false)//if hoizontal
                {
                    shipGrid[ships[i]->yLoc][ships[i]->xLoc+j]='o';
                }
                else//if vertical
                {
                    shipGrid[ships[i]->yLoc+j][ships[i]->xLoc]='o';
                }
            }
        }
//OVERLAP CHECK, if ships overlap then slot count<17 becuase slots are shared. Constructor repeats
        slotCount=0;
        for(int i=0;i<10;i++)
        {
            for(int j=0;j<10;j++)
            {
                if(shipGrid[i][j]=='o') slotCount++;
            }
        }
//if overlap check fails, free the heap before the loop resets        
        if(slotCount!=17)
        {
            for(int i=0;i<5;i++)
            {
                delete[]ships[i];
            }
            delete[]ships;
            for(int i=0;i<10;i++)
            {
                delete[]shotGrid[i];
                delete[]shipGrid[i];
            }
            delete[]shotGrid;
            delete[]shipGrid;            
        }
    }
}
board::~board()
{
    for(int i=0;i<5;i++)
    {
        delete[]ships[i];
    }
    delete[]ships;
    for(int i=0;i<10;i++)
    {
        delete[]shotGrid[i];
        delete[]shipGrid[i];
    }
    delete[]shotGrid;
    delete[]shipGrid;
}
void board::setup()
{
    int slotCount=0;//used for overlap chec
    do//loop ensures no ship overlap
    {
//re-initializes player shipGrid to '.' for easier visual reference
        for(int i=0;i<10;i++)
        {
            for(int j=0;j<10;j++)
            {
                shipGrid[i][j]='.';
            }
        }
//user IO
        std::string coordinate;
        char tempOrien;
        for(int i=0;i<5;i++)
        {
            do//trapped in loop if ship input is off the board
            {
                shipPrint();
                if(shipCheck()==false)
                {
                    std::cout<<"\nIt appears a ship has been placed off of the board. Please try again. \n";
                }
                std::cout<<"\nPlacing ship "<<i+1<<" of size "<<ships[i]->size<<", ";
                std::cout<<"Input a coordinate (letter/number): ";
                std::cin>>coordinate;
                ships[i]->yLoc=int(toupper(coordinate.at(0)))-65;//converts letter into int for yLoc
                coordinate.erase(0,1);//erases first index to turn string into only numbers for xLoc
                ships[i]->xLoc=stoi(coordinate)-1;

                std::cout<<"Input an orientation, horizontal or vertical (h/v): ";
                std::cin>>tempOrien;
                tempOrien=toupper(tempOrien);//makes it case non-sensitive
                if(tempOrien=='H') ships[i]->orien=false;
                if(tempOrien=='V') ships[i]->orien=true;
//updates shipGrid as ships are placed for visual referece
                if(shipCheck()==true)
                {   for(int j=0;j<ships[i]->size;j++)
                    {
                        if(ships[i]->orien==false)//if hoizontal
                        {
                            shipGrid[ships[i]->yLoc][ships[i]->xLoc+j]='o';
                        }
                        else//if vertical
                        {
                            shipGrid[ships[i]->yLoc+j][ships[i]->xLoc]='o';
                        }
                    }
                }
            }while(shipCheck()==false);
        }
        shipPrint();
//resets user ship placement if ships overlap
        slotCount=0;
        for(int i=0;i<10;i++)
        {
            for(int j=0;j<10;j++)
            {
                if(shipGrid[i][j]=='o') slotCount++;
            }
        }
        if(slotCount!=17)
        {
            std::cout<<"\n*****Two ships cannot overlap. Please start over.*****\n";
        }
    }while(slotCount!=17);
}
bool board::shipCheck()//ensures user defined ships placement remains on the board, true=good input
{
    for(int i=0;i<5;i++)
    {
        if(ships[i]->orien==false&&
        ships[i]->xLoc>10-ships[i]->size)//checks that ship doesn't overflow off the top of the board
        {
            return false;
        }
        if(ships[i]->orien==true&&
        ships[i]->yLoc>10-ships[i]->size)//checks that ship doesn't overflow off the bottom of the board
        {
            return false;
        }
        if(ships[i]->xLoc>9||ships[i]->xLoc<0||
        ships[i]->yLoc>9||ships[i]->yLoc<0)//ensures input remains within bounds of the board
        {
            return false;
        }
    }
    return true;
}
////////////////////////////////////////////////
void board::playerTurn(board &comp)
{
//Turn Input
    std::string coordinate="";
    int shotX=0;
    int shotY=0;
//******rapid input for testing
    // std::cout<<"Input: ";
    // std::cin>>coordinate;
    // if(coordinate=="x")
    // {
    //     shotX=0;
    //     shotY=0;
    //     while(shotInputCheck(shotX,shotY)==false)
    //     {
    //         unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //         std::minstd_rand0 generator (seed);
    //         shotX=generator()%10;
    //         shotY=generator()%10;
    //     }
    // }
    do//loop ensures valid shot input
    {
        do//ensures coordinate input doesn't crash program
        {
            std::cout<<"Input a coordinate (letter/number): ";
            std::cin>>coordinate;
            if(int(coordinate.at(0)<65)||(coordinate.length()>2&&coordinate.at(2)!='0'))//if frist index not a letter or string too long (allows for 10)
            {
                std::cout<<"Please double check your input.\n";
            }
        }while(int(coordinate.at(0)<65)||(coordinate.length()!=2&&coordinate.at(2)!='0'));
        shotY=int(toupper(coordinate.at(0)))-65;//converts letter to number for y
        coordinate.erase(0,1);//erases first index to turn string into only numbers for x
        shotX=stoi(coordinate)-1;

        if(shotInputCheck(shotX,shotY)==false)
        {
            std::cout<<"It appears this coordinate is not in range or has already been selected.\n";
            std::cout<<"Please try again.\n";
        }
    }while(shotInputCheck(shotX,shotY)==false);
//Turn Output
    if(hitCheck(comp,shotX,shotY)==true)
    {
        comp.totalHitCount++;
        std::cout<<"\n*****HIT!*****\n\n";
        int sinkTemp=sinkCheck(comp,shotX,shotY);//temp created as to not call function multiple times and inadvertantly increase counters
        if(sinkTemp>0)
        {
            std::cout<<"\n*****You have sunk your opponents ship of size "<<sinkTemp<<"!*****\n\n";
        }
        shotGrid[shotY][shotX]='X';//updates shot grid
        comp.shipGrid[shotY][shotX]='X';//update comp ship grid for debugging
    }
    else
    {
        std::cout<<"\n*****Miss*****\n\n";
        shotGrid[shotY][shotX]='0';//updates shot grid
    }
}
bool board::shotInputCheck(int shotX,int shotY)
{
    if(shotX>9||shotX<0||shotY>9||shotY<0)//checks to see if shot is within range
    {
        return false;
    }
    else if(shotGrid[shotY][shotX]=='.') return true;//good input
    else return false;//repeated shot
}
bool board::hitCheck(board &comp,int shotX,int shotY)
{
    if(comp.shipGrid[shotY][shotX]=='o') return true;
    else return false;
}
int board::sinkCheck(board &comp,int shotX,int shotY)//also increase hit counter
{
    for(int i=0;i<5;i++)//iterates through ships
    {
        for(int j=0;j<comp.ships[i]->size;j++)
        {
            if(comp.ships[i]->orien==false)//if hoizontal 
            {
                if(comp.ships[i]->yLoc==shotY&&//finds which ship was previously hit
                comp.ships[i]->xLoc+j==shotX)
                {
                    comp.ships[i]->shipHitCount++;
                    if(comp.ships[i]->shipHitCount==comp.ships[i]->size)//checks if ship is sunken
                    {
                        return(comp.ships[i]->size);
                    }
                }
            }
            else//if vertical extend along the y axis for length=size
            {
                if(comp.ships[i]->yLoc+j==shotY&&
                comp.ships[i]->xLoc==shotX)
                {
                    comp.ships[i]->shipHitCount++;
                    if(comp.ships[i]->shipHitCount==comp.ships[i]->size)
                    {
                        return(comp.ships[i]->size);
                    }
                }
            }
        }
    }
    return(0);   
}
////////////////////////////////////////////////
void board::shotPrint()
{
    std::cout<<"Shot Board:";
    std::cout<<"\n  1 2 3 4 5 6 7 8 9 10\n";
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(j==0)
            {
                std::cout<<static_cast<char>(i+65)<<" ";
            }
            std::cout<<shotGrid[i][j]<<" ";
        }
        std::cout<<"\n";
    }
}
void board::shipPrint()
{
    std::cout<<"Ship Board:";
    std::cout<<"\n  1 2 3 4 5 6 7 8 9 10\n";
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(j==0)
            {
                std::cout<<static_cast<char>(i+65)<<" ";//prints letters along y-axis
            }
            std::cout<<shipGrid[i][j]<<" ";
        }
        std::cout<<"\n";
    }
}
////////////////////////////////////////////////
void board::compTurn(board &player)
{
//RNG
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed);
//random shot selector
    int shotX=generator()%10;
    int shotY=generator()%10;
    int shotRepeatCount=0;
    while(clumpCheck(shotX,shotY,shotRepeatCount,player)==false&&trackingMode==false)
    {
        shotX=generator()%10;
        shotY=generator()%10;
        shotRepeatCount++;
    }
//tracking shot selector
    if(trackingMode==true)
    {
        do//loop ensures that if there is no valid shot in tracking direction, it will swap directions
        {
            if(hits[1]>0)//if there is a miss, reset to first hit location
            {
                hits[0]=1;
                hits[1]=0;
            }
            if(hits[0]==1)//until a second hit, look around first hit for empty spots and fire
            {
                
                hits[1]=0;//resets miss count to 0 until the second hit
                if(shotInputCheck(hits[2]-1,hits[3])==true)//if within range fire to left
                {
                    shotX=hits[2]-1;
                    shotY=hits[3];
                }
                else if(shotInputCheck(hits[2]+1,hits[3])==true)//fire right
                {
                    shotX=hits[2]+1;
                    shotY=hits[3];
                }
                else if(shotInputCheck(hits[2],hits[3]-1)==true)//fire up
                {
                    shotX=hits[2];
                    shotY=hits[3]-1;
                }
                else if(shotInputCheck(hits[2],hits[3]+1)==true)//fire down
                {
                    shotX=hits[2];
                    shotY=hits[3]+1;
                }
            }
            if(hits[0]>=2)//continue along tracked direction if 2 hits have been located
            {
                if(trackingDirection()=='R'&&shotInputCheck(hits[2]+hits[0],hits[3])==true)//if within range, fire right
                {
                    shotX=hits[2]+hits[0];//shotX+number of hits since activated
                    shotY=hits[3];
                }
                else if(trackingDirection()=='L'&&shotInputCheck(hits[2]-hits[0],hits[3])==true)//fire left
                {
                    shotX=hits[2]-hits[0];//shotX+number of hits since activated
                    shotY=hits[3];
                }
                else if(trackingDirection()=='D'&&shotInputCheck(hits[2],hits[3]+hits[0])==true)//fire down
                {
                    shotX=hits[2];//shotX+number of hits since activated
                    shotY=hits[3]+hits[0];
                }
                else if(trackingDirection()=='U'&&shotInputCheck(hits[2],hits[3]-hits[0])==true)//fire up
                {
                    shotX=hits[2];//shotX+number of hits since activated
                    shotY=hits[3]-hits[0];
                }
                else hits[1]++;//increase miss counter b/c no valid shot in tracking direction
            }
        }while(hits[1]>0);
    }
    if(trackingMode==false)
    {
        for(int i=0;i<20;i++)//when set to false, array resets
        {
            hits[i]=0;
        }
    }
//fires shot
    if(hitCheckComp(player,shotX,shotY)==true)
    {
        shotGrid[shotY][shotX]='X';//updates grids
        player.shipGrid[shotY][shotX]='X';
        hits[0]++;
        hits[2*hits[0]]=shotX;//even values
        hits[2*hits[0]+1]=shotY;//odd values
        trackingMode=true;
        std::cout<<"Opponent fires "<<static_cast<char>(shotY+65)<<shotX+1<<", ***Hit!***\n";
        if(sinkCheckComp(player,shotX,shotY)>0) trackingMode=false;//if tracking mode is activated and the ship is sunk, turn off
    }
    else 
    {
        shotGrid[shotY][shotX]='0';
        hits[1]++;
        std::cout<<"Opponent fires "<<static_cast<char>(shotY+65)<<shotX+1<<", ***Miss.***\n";
    }
}
char board::trackingDirection()
{
    if     (hits[4]-hits[2]==1) return 'R';//right
    else if(hits[4]-hits[2]==-1) return 'L';//left
    else if(hits[5]-hits[3]==1) return 'D';//down (y increases as the board moves down)
    else if(hits[5]-hits[3]==-1) return 'U';//right
    else return 'E';//E for error
}
int board::sinkCheckComp(board &player,int shotX,int shotY)
{
    for(int i=0;i<5;i++)//iterates through ships
    {
        for(int j=0;j<player.ships[i]->size;j++)
        {
            if(player.ships[i]->orien==false)//if hoizontal 
            {
                if(player.ships[i]->yLoc==shotY&&//finds which ship was previously hit
                player.ships[i]->xLoc+j==shotX)
                {
                    player.ships[i]->shipHitCount++;
                    player.totalHitCount++;
                    if(player.ships[i]->shipHitCount==player.ships[i]->size)//checks if ship is sunken
                    {
                        std::cout<<"\n*****Your opponent has sunk your ship of size "<<player.ships[i]->size<<"!*****\n\n";
                        return(player.ships[i]->size);
                    }
                }
            }
            else//if vertical extend along the y axis for length=size
            {
                if(player.ships[i]->yLoc+j==shotY&&
                player.ships[i]->xLoc==shotX)
                {
                    player.ships[i]->shipHitCount++;
                    player.totalHitCount++;
                    if(player.ships[i]->shipHitCount==player.ships[i]->size)
                    {
                        std::cout<<"\n*****Your opponent has sunk your ship of size "<<player.ships[i]->size<<"!*****\n\n";
                        return(player.ships[i]->size);
                    }
                }
            }
        }
    }
    return(0);   
}
bool board::hitCheckComp(board &player,int shotX,int shotY)//true=hit
{
    if(player.shipGrid[shotY][shotX]=='o') return true;
    else return false;
}
bool board::clumpCheck(int shotX,int shotY,int &shotRepeatCount,board &player)
{
    //the idea of this function is to look around the random shot called above and see
    //if any other shots have been in the vicinity. As the game progresses, the tolerance
    //deacreases to ensure that there is always a possible shot to be made.

    if(shotRepeatCount>100)
    {
        radius--;//if no other available shots, decrease radius
        shotRepeatCount=0;//in case it takes a few more tries with decreased radius it will only
                            //activate once
    }
    if(shotGrid[shotY][shotX]!='.') return false;//if repeat shot
    for(int i=(-1)*radius;i<radius+1;i++)
    {
            if((shotX+i)>=0 && (shotX+i)<=9 && shotGrid[shotY][shotX+i]!='.')
            {
                return false;
            }
            if((shotY+i)>=0 && (shotY+i)<=9 && shotGrid[shotY+i][shotX]!='.')
            {
                return false;
            }
    }
    if(radius<0||player.totalHitCount>=16)return true;//handles an edge case where algo leaves one safe ship slot on an even square
    if((shotY+shotX)%2==0) return false;//fires in a checker board pattern. Makes late game more efficient
    return true;
}