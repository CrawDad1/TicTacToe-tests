#pragma once

#include "TicTacNode.h"
#include <random>
class TicTacGame
{
    public:
    //data members
    TicTacNode* root;
    char player; // play as X, O, or (B)oth -no computer

    //constructor

    TicTacGame(TicTacNode* ttNode, char Player): root{ttNode}, player{static_cast<char>(std::tolower(Player))}{}
    //no default constructor. 

    //member functions
    void PrintBoard (){
        std::string Board = this->root->boardState;
        std::cout << " -------\n";
        std::cout << "| " << Board[0] << " " << Board[1] << " " << Board[2] << " | \n";
        std::cout << "| " << Board[3] << " " << Board[4] << " " << Board[5] << " | \n";
        std::cout << "| " << Board[6] << " " << Board[7] << " " << Board[8] << " | \n";
        std::cout << " -------\n";

        return;
    }
};

char selectPlayer(){
    std::string player; //using string to prevent cin issues
    while(true){
        std::cin.clear();
        std::cout << "Select Player ((X), (O), or (B)oth) : ";
        std::cin >> player;
        switch (std::tolower(player[0]))
        {
        case 'x':
            return 'x';
            break;
        
        case 'o':
            return 'o';
            break;
        
        case 'b':
            return 'b';
            break;
        
        default:
            std::cout << "That input seems invalid. Let's try again. \n";
            break;
        }
    }
    return tolower(player[0]);
}

bool humanMove(TicTacGame& currGame){
    
    bool xFlag = false;
    std::string input{""};
    int inputInt = 100; //init to arbitrary invalid input
    while (!xFlag) {
        std::cout << "It's now player " << currGame.root->turnState << "'s turn. \n";
        std::cout << "Where would you like to move? : ";
        std::cin.clear();
        std::cin >> input;
        inputInt = std::stoi(input);
        if ((inputInt > 0) && (inputInt <= 9)) {
            //good input
            inputInt -= 1; //convert to 0-index. user inputs 1-index
            if (isdigit(currGame.root->boardState[inputInt])) {
                //valid move
                xFlag = true;
                continue;
            }
        }
        else {
            //bad input
            xFlag = false;
            std::cout << "That input seems invalid. Let's try again. \n";

        }
    }
        //input accepted
        char lastPlayer = currGame.root->turnState; // used when printing a winner
        currGame.root=currGame.root->childNodes[inputInt];
        currGame.PrintBoard();

        //check for win
        if (currGame.root->childNodes.size()==0){
            //is leaf node, game finishes here
           std::cout << "Game Over!! \n";
           if(currGame.root->winsXOD[2] >= 1){
                std::cout << "This game was a Draw! \n";
           }
           else {
                std::cout << "Player " << lastPlayer << " is the winner! \n";
           }
           return true;
        }

        return false;
}

bool cpuMove(TicTacGame& currGame, bool min = true){
    char cpuPlayer = currGame.root->turnState; 
    char hmnPlayer = currGame.player;
    double mScore;
    TicTacNode* mNode = nullptr;
    size_t mIndex = 0;
    size_t i = 0; //counts index of node, used in message

    if(min){
        mScore=100; //arbitrary, should be higher than any possible score (max magnitute of 1)
        for(auto child : currGame.root->childNodes){
            i+=1;
            if(child==nullptr){continue;}
            if(child->boardScore(hmnPlayer) < mScore){
                mScore = child->boardScore(hmnPlayer);
                mNode = child;
                mIndex=i;
            }
        }
    }
    else{ //max
        mScore=-100; //arbitrary high number
        for(auto child : currGame.root->childNodes){
            i+=1;
            if(child==nullptr){continue;}
            if(child->boardScore(cpuPlayer) > mScore){
                mScore = child->boardScore(cpuPlayer);
                mNode = child;
                mIndex=i;
            }
        }
    }

    //move selected
    currGame.root=mNode;
    currGame.PrintBoard();
    std::cout << "Computer player " << cpuPlayer << " has moved to : " << mIndex << std::endl;
    //check for win
    if (currGame.root->childNodes.size()==0){
        //is leaf node, game finishes here
        std::cout << "Game Over!! \n";
        if(currGame.root->winsXOD[2] >= 1){
            std::cout << "This game was a Draw! \n";
        }
        else {
            std::cout << "Player " << cpuPlayer << " is the winner! \n";
        }
        return true;
    }

    return false;
}


void firstMove(TicTacGame& currGame){
    std::random_device rd;
    std::srand(rd());
    int move = (std::rand()%9);
    currGame.root=currGame.root->childNodes[move];
    currGame.PrintBoard();
    std::cout << "X has moved to: " << move << '\n';
    return;
}

void PlayGame(TicTacNode* ttNode){
    TicTacGame currGame{ttNode, selectPlayer()};
    bool winFlag = false;
    std::cout << " \n\n\n ******************************************* \n \n \n";
    std::cout << "Let's go! Tic-Tac-Toe!\n\n";
    //turn order from switch/case based on game.player
    switch (currGame.player)
    {
    case 'x':
        currGame.PrintBoard();
        // loop repeatedly makes a human move and human move until one of the moves returns true
        while (!winFlag){
            if(humanMove(currGame)){
                winFlag=true;
            }
            else if(cpuMove(currGame)){
                winFlag=true;
            }
        }
        break;

    case 'o':
        firstMove(currGame);
        while (!winFlag){
            if(humanMove(currGame)){
                winFlag=true;
            }
            else if(cpuMove(currGame)){
                winFlag=true;
            }
        }
        break;
    
    case 'b':
        currGame.PrintBoard();
        // loop repeatedly makes a human move and human move until one of the moves returns true
        while (!winFlag){
            if(humanMove(currGame)){
                winFlag=true;
            }
            else if(humanMove(currGame)){
                winFlag=true;
            }
        }
        break;

    default:
        break;
    }

}
bool PlayAgain(){
    char c{'a'};

    while(true){
    std::cout << "Would you like to play again?(Y/N):";
    std::cin.ignore();
    std::cin >> c;

    if ((c=='Y')||(c=='y'))
        return true;
    else if ((c=='N')||(c=='n'))
        return false;
    else
        std::cout << "invalid input.\n";
    }
}

