//Devon Crawford
//10/17/2022
// Mapped version currently shows 958 finished games, move order ignored when using map

#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include<utility>




class TicTacNode {

public:
	//turn and board state
	char turnState = { 'x' };
	std::string boardState = { 'e','e','e','e','e','e','e','e','e' };

	//wins for branch
	std::vector<int> winsXOD = { 0,0,0 };


	//child nodes
	std::vector<TicTacNode*> childNodes;

	//**************public********************//

	char BoardCheck(/*checks this.boardstate*/) {
		char winner = 'n'; //x , o, or 'n'one

		//horizontal
		if ((boardState[0] != 'e') && (boardState[0] == boardState[1]) && (boardState[1] == boardState[2]))
		{
			winner = boardState[0];
			return winner;
		}
		else if ((boardState[3] != 'e') && (boardState[3] == boardState[4]) && (boardState[4] == boardState[5]))
		{
			winner = boardState[3];
			return winner;
		}
		else if ((boardState[6] != 'e') && (boardState[6] == boardState[7]) && (boardState[7] == boardState[8]))
		{
			winner = boardState[6];
			return winner;
		}


		//vertical
		if ((boardState[0] != 'e') && (boardState[0] == boardState[3]) && (boardState[3] == boardState[6]))
		{
			winner = boardState[0];
			return winner;
		}
		else if ((boardState[1] != 'e') && (boardState[1] == boardState[4]) && (boardState[4] == boardState[7]))
		{
			winner = boardState[1];
			return winner;
		}
		else if ((boardState[2] != 'e') && (boardState[2] == boardState[5]) && (boardState[5] == boardState[8]))
		{
			winner = boardState[2];
			return winner;
		}

		//diagonal
		if ((boardState[0] != 'e') && (boardState[0] == boardState[4]) && (boardState[4] == boardState[8]))
		{
			winner = boardState[0];
			return winner;
		}
		else if ((boardState[6] != 'e') && (boardState[6] == boardState[4]) && (boardState[4] == boardState[2]))
		{
			winner = boardState[6];
			return winner;
		}

		return winner; //if this line executes, winner = none
	}

	void xWins() {
		this->winsXOD[0] += 1;
	}

	void oWins() {
		this->winsXOD[1] += 1;
	}

	void dwins() {
		this->winsXOD[2] += 1;
	}

	void addWins(TicTacNode* child) {
		for (size_t i = 0; i < this->winsXOD.size(); i++) {
			this->winsXOD[i] += child->winsXOD[i];
		}
		return;
	}

	//constructors
	TicTacNode() = default;

	TicTacNode(const char& turn, std::string board) : boardState{ board } {
		switch (turn)
		{
		case 'x':
			this->turnState = 'o';
			break;

		case 'o':
			this->turnState = 'x';
			break;
		}
	}

};

//global variables, too lazeee
std::unordered_map<std::string, TicTacNode*> nodeMap;
std::unordered_map<std::string, TicTacNode*> leafMap;




/*MainFunctions***************************************************8*/



void BuildTicTacTree(TicTacNode* Head) {
	//break, check board and return winner. 
	switch (Head->BoardCheck())
	{
	case 'n':
		//do nothing, continue playing
		break;

	case 'x':
		Head->xWins();
		return;

	case 'o':
		Head->oWins();
		return;

		//Draw is determined when attempting to move

	default:
		break;
	}


	//TODO: Make move, generate Children

	auto boardCopy = Head->boardState;

	bool moveFlag = false;
	for (size_t i = 0; i < boardCopy.size(); i++) {
		boardCopy = Head->boardState; //reset any changes

		if (boardCopy.at(i) == 'e') { //valid move to empty space
			moveFlag = true;
			boardCopy.at(i) = Head->turnState; //make move
			TicTacNode* tempTacToe = nullptr;
			if (nodeMap.find(boardCopy) != nodeMap.end()) {
				//node found
				tempTacToe = nodeMap[boardCopy];
			}
			else {
				//node NOT found
				tempTacToe = (new TicTacNode(Head->turnState, boardCopy));
				BuildTicTacTree(tempTacToe);
				//add to leaf map if win
				if (tempTacToe->childNodes.size() == 0) {
					leafMap[tempTacToe->boardState] = tempTacToe;
				}

				nodeMap[boardCopy] = tempTacToe; //save to map                
			}

			Head->addWins(tempTacToe);
			Head->childNodes.push_back(tempTacToe);
		}
		//else: occupied, check next space. 
	}

	//draw if no moves available
	if (!moveFlag) Head->dwins();
	return;
}

TicTacNode* BuildTicTacTree() {
	//wrap for tree builder
	TicTacNode* HeadNode = new TicTacNode; //build default empty node

	BuildTicTacTree(HeadNode);
	return HeadNode;
}

int main() {
	std::cout << "Lets get started." << std::endl;

	BuildTicTacTree();

	std::cout << "Hey, i didn't break!! hahahahahaha" << std::endl;
	int wins = 0;
	for (auto game : leafMap) {
		if (game.second->childNodes.size() == 0) {
			if (game.second->BoardCheck() == 'x') {
				//print boards where x wins
				std::string board = game.first;

				std::cout << "\n***********************\n";
				for (size_t i = 0; i < board.size(); i += 3) {
					std::cout << board[i] << ' '
						<< board[i + 1] << ' ' << board[i + 2]
						<< std::endl;
				}
				wins++;
			}
		}
	}

	std::cout << "\n\nX won " << wins << " out of " << leafMap.size() << " possible games.";

	return 0;
}
