#pragma once
//TODO: implement difficulties using a random chance to make an unmapped move
#include "TicTacNode.h"
#include <random>
class TicTacGame
{
    public:
    //data members
    TicTacNode* root;
    char player; // play as X, O, or (B)oth -no computer
    bool earlyDraw = true;
	bool isDebug = false;

    //constructor

    TicTacGame(TicTacNode* ttNode, char Player): root{ttNode}, player{Player}{}
    //no default constructor. 

    //member functions
    void PrintBoard (){
        std::string Board = this->root->boardState;
        auto XOD = this->root->winsXOD;
        std::cout << " -------\n";
        std::cout << "| " << Board[0] << " " << Board[1] << " " << Board[2] << " | \t" << "Board score for X: " << this->root->boardScore('x') << std::endl;
		std::cout << "| " << Board[3] << " " << Board[4] << " " << Board[5] << " | \t" << "Board score for O: " << this->root->boardScore('o') << std::endl;
        std::cout << "| " << Board[6] << " " << Board[7] << " " << Board[8] << " | \t" << "wins XOD: " << XOD[0] << " : " << XOD[1] << " : " << XOD[2] << '\n';
        std::cout << " -------\n";

        return;
    }
};
char userAlphaInput() {
    std::string input;
    int userInt = -1;
    while (true) {
        std::getline(std::cin, input);
        std::cin.clear();
        if (std::isalpha(input[0])) {
            return input[0];
        }
        else {
            std::cout << "oops, not a letter. try again. \n";
        }
    }
}

int userIntInput() {
    std::string input;
    int userInt = -1;
    while (true) {
        std::getline(std::cin, input);
        std::cin.clear();
        try {
            userInt = std::stoi(input);
            break;
        }
        catch (std::exception e) {
            std::cout << "oops, not an integer. try again.\n";
            continue;
            //std::exit(9);
        }
    }
    return userInt;
}

bool selectEarlyDraw() { 
    char c; 
    while(true){
		std::cout << "Would you like to use early draw?(Y/N):";
		c = std::tolower(userAlphaInput());

		switch (c)
		{
		case 'y':
			return true;
			break;
		
		case 'n':
			return false;
			break;
		default:
			std::cout << "invalid input. Try again!\n";
			break;
		}
    } 
}

char selectPlayer(){
    char player; //using string to prevent cin issues
    while(true){
        std::cout << "Select Player ((X), (O), or (B)oth) : ";
        player =std::tolower(userAlphaInput());
        switch (player)
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
    return player;
}

bool checkMove(TicTacGame& currGame, int& inputInt) {

        if ((inputInt > 0) && (inputInt <= 9)) {
            //good input
            inputInt -= 1; //convert to 0-index. user inputs 1-index
            if (isdigit(currGame.root->boardState[inputInt])) {
                //valid move
                //check if defined
                if (currGame.root->childNodes[inputInt] == nullptr) {
                    //generate node if undefined
					auto boardCopy = currGame.root->boardState;
					boardCopy[inputInt] = currGame.root->turnState;
                    auto temptactoe = new TicTacNode(currGame.root->turnState, boardCopy);
                    BuildTicTacTree(temptactoe, true);
                    currGame.root->childNodes[inputInt] = temptactoe; 
                }
                return true;
            }
        }
        // else 
		//bad input
		std::cout << "That input seems invalid. Let's try again. \n";
		return false; 
}
 
bool checkWinner(TicTacGame& currGame, char& lastPlayer) { 
   auto XOD = currGame.root->winsXOD;
   int wins = XOD[0] + XOD[1];
	if ((currGame.root->childNodes.size()==0)|| ((wins==0)&&(currGame.earlyDraw))){
		//is leaf node, game finishes here
	   std::cout << "Game Over!! \n"; 
	   if(wins == 0){
		   if ((XOD[2] >= 1)&&(currGame.earlyDraw)) {
				std::cout << "This game is an early Draw! \n"; 
		   }
			else std::cout << "This game was a Draw! \n";
	   }
	   else {
			std::cout << "Player " << lastPlayer << " is the winner! \n";
	   }
	   return true;
	} 
	return false;
}

bool humanMove(TicTacGame& currGame){
    
    bool xFlag = false;
    std::string input{""};
    int inputInt = 100; //init to arbitrary invalid input
    std::cout << "\n\n" << "*********************************************\n";
    while (!xFlag) {
        std::cout << "It's now player " << currGame.root->turnState << "'s turn. \n";
        std::cout << "Where would you like to move? : ";
        inputInt = userIntInput();
        xFlag = checkMove(currGame, inputInt);
    }
        //input accepted
        char lastPlayer = currGame.root->turnState; // used when printing a winner
        currGame.root=currGame.root->childNodes[inputInt];
        currGame.PrintBoard();
    std::cout << "*********************************************\n";

        //check for win
        return checkWinner(currGame, lastPlayer);
}

bool debugHumanMove(TicTacGame& currGame){
	
	bool xFlag = false;
	std::string input{""};
	int inputInt = 100; //init to arbitrary invalid input
	while (!xFlag) {
		std::cout << "It's now player " << currGame.root->turnState << "'s turn. \n";
		std::cout << "what move would you like to project? : ";
		std::cin.clear();
		std::cin >> input;
		std::cin.ignore();
		try {
			inputInt = std::stoi(input);
		}
		catch (std::exception e) {
			std::cout << "int conversion issues dawg.";
			continue;
		}
		if ((inputInt > 0) && (inputInt <= 9)) {
			//good input
			inputInt -= 1; //convert to 0-index. user inputs 1-index
			if (isdigit(currGame.root->boardState[inputInt])) {
				//valid move
				auto tempGame = currGame;
				tempGame.root=currGame.root->childNodes[inputInt];
				tempGame.PrintBoard();
				std::cout << " would you like to commit this move? (c to commit): ";
				std::string toCommit;
				std::getline(std::cin, toCommit);
				std::cin.ignore();
				if (tolower(toCommit[0]) == 'c') {
				xFlag = true;
				}
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
    std::cout << "\n\n" << "*********************************************\n";
    std::cout << "Computer player " << cpuPlayer << " has moved to : " << mIndex << std::endl;
    currGame.PrintBoard();
    std::cout << "*********************************************\n";

    //check for win
    return checkWinner(currGame, cpuPlayer);
}


void firstMove(TicTacGame& currGame){
    std::random_device rd;
    std::srand(rd());
    int move = (std::rand()%9+1);
    currGame.root=currGame.root->childNodes[move-1];
    currGame.PrintBoard();
    std::cout << "X has moved to: " << move << '\n';
    return;
}

void PlayGame(TicTacNode* ttNode){
    TicTacGame currGame{ttNode, selectPlayer()};
    currGame.earlyDraw = selectEarlyDraw();
    bool winFlag = false;
    std::cout << " \n\n\n ******************************************* \n \n \n";
    std::cout << "Let's go! Tic-Tac-Toe!\n\n";
    //turn order from switch/case based on game.player
    switch (currGame.player)
    {
    case 'x':
        currGame.PrintBoard();
        // loop repeatedly makes a human move and human move until one of the moves returns true
        while (!winFlag) {
            if (currGame.isDebug) {
                if (debugHumanMove(currGame)) {
                    winFlag = true;
                }
                else if (cpuMove(currGame)) {
                    winFlag = true;
                } 
            }
            else {
				if (humanMove(currGame)) {
					winFlag = true;
				}
				else if (cpuMove(currGame)) {
					winFlag = true;
				} 
            } 
        }
        break;

    case 'o':
        firstMove(currGame);
        while (!winFlag) {
            if (currGame.isDebug) {
                if (debugHumanMove(currGame)) {
                    winFlag = true;
                }
                else if (cpuMove(currGame)) {
                    winFlag = true;
                } 
            }
            else {
				if (humanMove(currGame)) {
					winFlag = true;
				}
				else if (cpuMove(currGame, false)) {
					winFlag = true;
				} 
            } 
        }
        break;
    
    case 'b':
        currGame.PrintBoard();
        // loop repeatedly makes a human move and human move until one of the moves returns true
        while (!winFlag){
            if (currGame.isDebug) {
				if(debugHumanMove(currGame)){
					winFlag=true;
				} 
            }
            else {
                if (humanMove(currGame)) {
                    winFlag = true;
                }
                else if (humanMove(currGame)) {
                    winFlag = true;
                }
            }
        }
        break;

    default:
        break;
    }

}

bool PlayAgain(){
    char c; 
    while(true){
		std::cout << "Would you like to play again?(Y/N):";
		c = std::tolower(userAlphaInput());

		switch (c)
		{
		case 'y':
			return true;
			break;
		
		case 'n':
			return false;
			break;
		default:
			std::cout << "invalid input. Try again!\n";
			break;
		}
    }
}

