#include "TicTacNode.h"
	//static variables
	std::unordered_map<std::string, TicTacNode*> TicTacNode::nodeMap;

    // Member functions
	bool TicTacNode::CheckForcedMoveHelper(std::vector<int> cells, std::string& boardCopy){
		int x = 0;
		int o = 0;
		int e = 0;

		for(auto i : cells){
			switch (boardCopy[i])
			{
			case 'x':
				x++;
				break;
			case 'o':
				o++;
				break;
			
			default:
				e =i;
				break;
			}	
		}
		if ((x + o) >= 3) return false;
		else if ((x==2) || (o==2)){
			//force a move
			boardCopy[e] = 'f';
			return true;
		}
		return false;
	}
	bool TicTacNode::CheckForcedMove(std::string& boardCopy){
		//checks this.boardstate for two-in-a-row
		//if true, places an F on the board and instructs builder to find F
		int moves = 0;

		//horizontal
		if (CheckForcedMoveHelper({0,1,2}, boardCopy)) moves++;
		if (CheckForcedMoveHelper({3,4,5}, boardCopy)) moves++;
		if (CheckForcedMoveHelper({6,7,8}, boardCopy)) moves++;

		//vertical
		if (CheckForcedMoveHelper({0,3,6}, boardCopy)) moves++;
		if (CheckForcedMoveHelper({1,4,7}, boardCopy)) moves++;
		if (CheckForcedMoveHelper({2,5,8}, boardCopy)) moves++;

		//diagonal
		if (CheckForcedMoveHelper({0,4,8}, boardCopy)) moves++;
		if (CheckForcedMoveHelper({6,4,2}, boardCopy)) moves++;

		//none
		if (moves >= 1) return true;
		else return false;
	}

	//todo: finish this VVV
	//bool TicTacNode::BoardCheckHelper(std::vector<int> cells, std::string& boardCopy) {
	//	int x = 0;
	//	int o = 0;
	//	int e = 0;

	//	for (auto i : cells) {
	//		switch (boardCopy[i])
	//		{
	//		case 'x':
	//			x++;
	//			break;
	//		case 'o':
	//			o++;
	//			break;

	//		default:
	//			e = i;
	//			break;
	//		}
	//	}
	//	if (x == 3) {
	//		// x wins
	//	}
	//	else if (o == 3) {
	//		//o wins
	//	}
	//	else if ((x + o) >= 3) return false;
	//	else if ((x == 2) || (o == 2)) {
	//		//force a move
	//		boardCopy[e] = 'f';
	//		return true;
	//	}
	//	return false;
	//}

	//char TicTacNode::BoardCheck(/*checks this.boardstate*/) {
	//	//TODO: try to implement using the helper function, and the newest move
	//}
	char TicTacNode::BoardCheck(/*checks this.boardstate*/) { //old
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
		if (childNodes.size() == 0) return; //don't run on leaf node
		for(auto child : childNodes){
			if (child == nullptr) { continue; }
			for(size_t i = 0; i<winsXOD.size(); i++){
				winsXOD[i]+=child->winsXOD[i];
			}
		}
	}
	
	double TicTacNode::boardScore(char winner){
	/* Old scoring
		winner = std::tolower(winner);
		size_t i = 0;
		int diff = 0;
		auto XOD = this->winsXOD;
		if(winner=='x'){diff = XOD[0]-XOD[1];}
		if(winner=='o'){diff = XOD[1]-XOD[0];}
		if(diff == 0){return diff;} //prevents dividing zero issues
				else {return (static_cast<double>(diff)/(XOD[0]+XOD[1]+XOD[2]));}*/ 
		winner = std::tolower(winner);
		auto XOD = this->winsXOD;
		int Sum = XOD[0] + XOD[1] + XOD[2];
		switch (winner)
		{
		case 'x':
			return static_cast<double>(XOD[0]) / Sum;
			break;
		case 'o':
			return static_cast<double>(XOD[1]) / Sum;
			break;
		case 'd':
			return static_cast<double>(XOD[2]) / Sum;
			break;
		default:
			break;
		}
		//edge case
		return 999;
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
		return;

	case 'o':
		Head->oWins();
		return;

		//Draw is determined when attempting to move

	default:
		break;
	}


	auto boardCopy = Head->boardState;

	bool moveFlag = false;
	//this line allows for easier traversal
	Head->childNodes = std::vector<TicTacNode*>(9, nullptr); 

	//check for forced move and generate list of valid moves 
	bool isForced = Head->CheckForcedMove(boardCopy);
	std::vector<size_t> moveList;
	if (isForced) {
		size_t index = 0;
		for (auto c : boardCopy) {
			if (c == 'f') moveList.push_back(index);
			index++;
		} 
	}
	else {
		for (size_t i = 0; i < boardCopy.size(); i++) {
			if (std::isdigit(boardCopy[i])) moveList.push_back(i);
		}
	}

	//make move and generate from new board
	for (auto i : moveList) {
		boardCopy = Head->boardState; //reset any changes
		moveFlag = true;
		boardCopy.at(i) = Head->turnState; //make move
		TicTacNode* tempTacToe = nullptr; //initialize pointer
		if (TicTacNode::nodeMap.find(boardCopy) == TicTacNode::nodeMap.end()) {
			//node not in map
			tempTacToe = (new TicTacNode(Head->turnState, boardCopy));
			//add new node to map
			TicTacNode::nodeMap[boardCopy] = tempTacToe;
			BuildTicTacTree(tempTacToe, mapped);
		}
		else {
			//node found in map
			//extract node from map
			tempTacToe = TicTacNode::nodeMap[boardCopy];
		}
		Head->childNodes[i] = tempTacToe;
	} 
	//all children created. bring win sums up to head
	Head->addWins();
	//draw if no moves available
	if (!moveFlag) {
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



