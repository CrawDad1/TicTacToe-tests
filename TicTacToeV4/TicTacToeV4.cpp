// TicTacToeV4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "TicTacNode.h"
#include "TicTacGame.h"

int main()
{
    std::cout << "Building tree.... \n";
    TicTacNode* Root = BuildTicTacTree(true);
    std::cout << "Tree complete! \n";

    std::cout << "\n\n**********************************************\n\n";

    do{
        PlayGame(Root);
    }while(PlayAgain());

    //exited game loop
    std::cout << "Thanks for playing! bye! ";
    return 0;
}