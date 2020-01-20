//time spent: 20 hrs to finish
#include<iostream>
#include<chrono>
#include<thread>
#include"board.h"
#include"ship.h"

int main()
{
    board player;
    board comp;
    int shotCounter=0;
    char setUpSkip='R';
    std::cout<<"\nWelcome to Battleship! Prepare to face your doom!\n";
    std::cout<<"\nWould you like to set your ships manually, or randomly (M/R)?: ";
    std::cin>>setUpSkip;
    if(toupper(setUpSkip)=='M')
    {
        std::cout<<"\nInstructions: To place your ships input the coordinate for the upper-left-most ship slot.\n\n";
        player.setup();
    }
    std::cout<<"\n**************************************\n";
    std::cout<<"The battle has begun; prepare to fire!";
    std::cout<<"\n**************************************\n\n";
    while(player.totalHitCount<17&&comp.totalHitCount<17)//17 hits for all ships
    {
        // std::cout<<"comp: "<<comp.radius<<"  "<<shotCounter<<"  "<<comp.hits[2]<<comp.hits[3];
        // std::cout<<"  "<<comp.trackingMode<<"  "<<player.totalHitCount;
        // comp.shotPrint();
        // comp.shipPrint();
        // std::cout<<"player:";
        player.shotPrint();
        player.shipPrint();
        player.playerTurn(comp);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));//added pauses for better gameplay
        comp.compTurn(player);
        //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        shotCounter++;
    }
        player.shotPrint();
        player.shipPrint();
        std::cout<<"Computers ships: ";
        comp.shipPrint();
//closing text
    std::cout<<"\n***************************************************\n";
    if(player.totalHitCount<17||(player.totalHitCount>=17&&comp.totalHitCount>=17))
    {//in the rare case player and computer finish eachother in b2b moves, player wins
        std::cout<<"Congratulations! You've won! Thanks for playing!";
    }
    else
    {
        std::cout<<"*blub blub blub* You've lost. Try again next time.";
        std::cout<<"\nThanks for playing!";
    }
    std::cout<<"\n***************************************************\n";
    std::cout<<"Total shots: "<<shotCounter<<"\n";
}