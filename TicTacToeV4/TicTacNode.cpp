#include "TicTacNode.h"
	//static variables
	std::vector<TicTacNode*> TicTacNode::vXwins={};
	std::vector<TicTacNode*> TicTacNode::vOwins={};
	std::vector<TicTacNode*> TicTacNode::vDwins={};
	std::unordered_map<std::string, TicTacNode*> TicTacNode::nodeMap;

    // Member functions

	char TicTacNode::BoardCheck(/*checks this.boardstate*/) {
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
    void TicTacNode::xWins(){
		winsXOD[0]+=1;
	}
    void TicTacNode::oWins(){
		winsXOD[1]+=1;
	}
    void TicTacNode::dWins(){
		winsXOD[2]+=1;
	}
	void TicTacNode::addWins(){
		//adds the calculated wins from all childnodes to current node
		for(auto child : childNodes){
			if (child == nullptr) { continue; }
			for(size_t i = 0; i<winsXOD.size(); i++){
				winsXOD[i]+=child->winsXOD[i];
			}
		}
	}
	
	double TicTacNode::boardScore(char winner){
		winner=std::tolower(winner);
		size_t i = 0;
		int diff = 0;
		auto XOD = this->winsXOD;
		if(winner=='x'){diff = XOD[0]-XOD[1];}
		if(winner=='o'){diff = XOD[1]-XOD[0];}
		if(diff == 0){return diff;} //prevents dividing zero issues
				else {return (static_cast<double>(diff)/(XOD[0]+XOD[1]+XOD[2]));}
	}

	TicTacNode::TicTacNode(const char& turn, std::string board) : boardState{ board } { 
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

//Non-member functions



void BuildTicTacTree(TicTacNode* Head, bool mapped) {
	//break, check board and return winner. 
	switch (Head->BoardCheck())
	{
	case 'n':
		//do nothing, continue playing
		break;

	case 'x':
		Head->xWins();
		TicTacNode::vXwins.push_back(Head);
		return;

	case 'o':
		Head->oWins();
		TicTacNode::vOwins.push_back(Head);
		return;

		//Draw is determined when attempting to move

	default:
		break;
	}


	auto boardCopy = Head->boardState;

	bool moveFlag = false;
	//this line allows for easier traversal
	Head->childNodes = std::vector<TicTacNode*>(9, nullptr); 
	for (size_t i = 0; i < boardCopy.size(); i++) {
		boardCopy = Head->boardState; //reset any changes
		if(!mapped){
			if (isdigit(boardCopy.at(i))) { //valid move to empty space
				moveFlag = true;
				boardCopy.at(i) = Head->turnState; //make move

				TicTacNode* tempTacToe = nullptr;			
				tempTacToe = (new TicTacNode(Head->turnState, boardCopy));
				BuildTicTacTree(tempTacToe, mapped);
				Head->childNodes[i]=tempTacToe;
			}
		}
		else{
			if (isdigit(boardCopy.at(i))) { //valid move to empty space
				moveFlag = true;
				boardCopy.at(i) = Head->turnState; //make move
				TicTacNode* tempTacToe = nullptr;			
				if(TicTacNode::nodeMap.find(boardCopy)==TicTacNode::nodeMap.end()){
					//node not in map
					tempTacToe = (new TicTacNode(Head->turnState, boardCopy));
					//add new node to map
					TicTacNode::nodeMap[boardCopy]=tempTacToe;
					BuildTicTacTree(tempTacToe, mapped);
				}
				else{
					//node found in map
					//extract node from map
					tempTacToe = TicTacNode::nodeMap[boardCopy];
				}
				Head->childNodes[i]=tempTacToe;
			}

		}
		//else: occupied, check next space. 
	}

	Head->addWins();
	//draw if no moves available
	if (!moveFlag) {
		TicTacNode::vDwins.push_back(Head);
		Head->dWins();
		Head->childNodes={}; //resets to zero for leaf-detection later
		}

	return;
}

TicTacNode* BuildTicTacTree(bool mapped) {
	//wrap for tree builder
	TicTacNode* HeadNode = new TicTacNode; //build default empty node

	BuildTicTacTree(HeadNode, mapped);
	return HeadNode;
}



