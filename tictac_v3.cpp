//v3 intends to add a computer opponent. 
//computer will always block a win if possible or pick an option from the tree if no block

#include<iostream>
#include<vector>
#include<unordered_map>
#include<utility>
using std::vector;
using std::cout;
using std::cin;
using std::endl;


//tree generation
class TicTacNode {

public:
	//turn and board state
	char turnState = { 'x' };
	std::string boardState = { 'e','e','e','e','e','e','e','e','e' };
	std::pair<unsigned, unsigned> lastMove;

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

	void sortChildrenOld(){
		if(this->childNodes.size()==0) return;
		
		//sorts vector , [0] least x wins
		std::vector<TicTacNode*> vTemp;
		vTemp.push_back(this->childNodes[0]);
		for(size_t i = 1; i < this->childNodes.size(); i++){
			for(auto k= vTemp.begin(); k <= vTemp.end(); k++){
				if (k==vTemp.end()) {
					vTemp.push_back(childNodes[i]);
					break;
				}

				if (childNodes[i]->xScore() == (*k)->xScore()){
                   if (childNodes[i]->oScore() >= (*k)->oScore()){
					vTemp.insert(k, childNodes[i]);
                   }
                   else {
					vTemp.insert(k+1, childNodes[i]);
                   }
                   break;
				}				
                if (childNodes[i]->xScore() <= (*k)->xScore()){
					vTemp.insert(k, childNodes[i]);
					break;
				}}
		}

		childNodes=vTemp;
		return;
	}
    void sortChildren(){
        //this one sorts by O-score
		if(this->childNodes.size()==0) return;
		
		//sorts vector , [0] least x wins
		std::vector<TicTacNode*> vTemp;
		//vTemp.push_back(this->childNodes[0]);
		for(size_t i = 0; i < this->childNodes.size(); i++){
			for(auto k= vTemp.begin(); k <= vTemp.end(); k++){
				if (k==vTemp.end()) {
					vTemp.push_back(childNodes[i]);
					break;
				}
                double cOscore = childNodes[i]->oScore();
                double kOscore = (*k)->oScore();

                if (cOscore < kOscore){
                    continue;
                }

				else if (cOscore== kOscore){
                   if (childNodes[i]->xScore() <= (*k)->xScore()){
					vTemp.insert(k, childNodes[i]);
                   }
                   else {
					vTemp.insert(k+1, childNodes[i]);
                   }
                   break;
				}
                else if (cOscore > kOscore){
					vTemp.insert(k, childNodes[i]);
					break;
				}
            }
		}

		childNodes=vTemp;
		return;
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
    double xScore(){
        int sum;
        for( int i : winsXOD){
            sum+=i;
        } 
        return static_cast<double>( this->winsXOD[0] - this->winsXOD[1] )/sum;} 
    double oScore(){
        int sum;
        for( int i : winsXOD){
            sum+=i;
        } 
        return static_cast<double>( this->winsXOD[1] - this->winsXOD[0] )/sum;} 
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
				tempTacToe->lastMove={i/3, i%3};
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

	//sort children by lowest x wins
	Head->sortChildren();

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

//tree generation


void PlayGame();
void PrintBoard (const vector<vector<char>> &Board);
void SetCell(vector<vector<char>> &Board, char mark, unsigned &x, unsigned &y, int &MovesLeft);
bool CheckBoard(const vector<vector<char>> &Board, const int &MovesLeft);
std::pair<unsigned, unsigned> CheckBoard2(const vector<vector<char>> &Board, const int &MovesLeft, const char player);

int lastMove(TicTacNode* head, TicTacNode* child){
    std::string oldBoard = head->boardState;
    std::string newBoard = child->boardState;

    //for readability, return a number 1-9, or (i+1)
    for (unsigned i = 0; i < oldBoard.size(); i++){
        if(oldBoard[i]!=newBoard[i]){return (i+1);}
    }
    return 0;
}

void printNodeInfo(TicTacNode* ttNode){
    std::cout << "Next player: " << ttNode->turnState << '\n';
    for( auto T : ttNode->childNodes){
        std::cout << lastMove(ttNode, T) << " : " << "o Score: " << T->oScore() 
                  << "|| x Score: " << T->xScore() << '\n';
    }
    return;
}
std::pair<unsigned, unsigned> treeMove(const vector<vector<char>> &Board, const char player){
    //picks best move for O based on tree wins. 
    //convert board to string
    std::string sBoard={""};
    for( auto v : Board){
        for(char c : v){
            if (!isdigit(c)){
                sBoard+= tolower(c);
            }
            else{
                sBoard+='e';
            }
        }
    }

    char notPlayer;
    switch (player)
    {
    case 'x':   
        notPlayer= 'o';
        break;
    case 'o':   
        notPlayer= 'x';
        break;
    }
    
    auto tempBoard = nodeMap[sBoard];
    printNodeInfo(tempBoard);
    double maxScore = tempBoard->childNodes[0]->oScore();
    int maxIndex = 0;
    for(size_t i = 1; i < tempBoard->childNodes.size();i++){
        double oscore= tempBoard->childNodes[i]->oScore();
        if (oscore > maxScore){
            maxScore = oscore;
            maxIndex=i;
        }
    }
    //debug thing
    std::cout << "Move for " << notPlayer << " picked from tree.\n";
    std::string nBoard = tempBoard->childNodes[maxIndex]->boardState;
    for (size_t i = 0; i < 9; i++)
    {
        if (sBoard[i]!=nBoard[i]) return {(i/3),(i%3)};
    }
    

    return tempBoard->childNodes[0]->lastMove;

}

void GetCoord (unsigned &x, unsigned &y);
bool PlayAgain();


int main(){
    //generate tree
    std::cout << "generating tree. . . . \n";
    auto Root = BuildTicTacTree();
    std::cout << "done! \n";

    do{
        PlayGame();
    }while(PlayAgain());
    return 0;
}




//functions

bool PlayAgain() {
    char c{'a'};

    while(true){
    cout << "Would you like to play again?(Y/N):";
    cin.ignore();
    cin >> c;

    if ((c=='Y')||(c=='y'))
        return true;
    else if ((c=='N')||(c=='n'))
        return false;
    else
        cout << "invalid input.\n";

    }
}

void PlayGame(){
     vector<vector<char>> Board{{'1','2','3'},{'4','5','6'},{'7','8','9'}};
    int MovesLeft{9};    
    unsigned x;
    unsigned y;
    bool win{false};

    cout << "Let's play tic tac toe! \n\n";
    PrintBoard(Board);
    do
    {
        cout << "Player X \n";
        cout << "where will you move?";        
        GetCoord(x, y);
        SetCell(Board, 'X', x, y, MovesLeft);
        PrintBoard(Board);
        win =CheckBoard(Board, MovesLeft);
        if(win) break;        
        
        auto oMove = CheckBoard2(Board, MovesLeft, 'x');
        y=oMove.first;
        x=oMove.second;
        std::cout << "Player O moves to... " << (y*3)+x+1 << std::endl;
        SetCell(Board, 'O', x, y, MovesLeft);
        PrintBoard(Board);
        win =CheckBoard(Board, MovesLeft);
        if(win) break;



    }while((!win)&&(MovesLeft>0));
    if (MovesLeft<=0)
    {
        cout <<"No more Moves! This game is a Draw!";        
    }   
    else if (win)
    {
        cout << "The winner is.... \nPlayer " << Board[y][x] << "!";        
    }
    else
    {
        cout << "yeah dude, idk what happened. uhhhhhhhhhh.....";        
    }    

    cout << "\n\n";
}

void SetCell(vector<vector<char>> &Board, char mark, unsigned &x, unsigned &y, int &MovesLeft){
    do{
        if (isdigit(Board[y][x])&&(x<=2)&&(y<=2))
        { 
         Board[y][x]=mark;
         MovesLeft--;
         return;
        }
        else
        {
         cout<< "Invalid move, please try again: ";
         GetCoord(x,y);            
        }
    }while(true);
}

void PrintBoard (const vector<vector<char>> &Board)
{
    cout << " _______\n";
    for(unsigned i{0}; i < Board.size(); ++i)
    {
        cout << "| ";
        for(unsigned j{0}; j < Board[i].size(); ++j)
        {
            cout << Board[i][j] << ' ';
        }
        cout << "| \n";
    }
    cout << " -------\n";

    return;
}

bool CheckBoard(const vector<vector<char>> &Board, const int &MovesLeft){    

    //horizontal
    for(unsigned i{0}; i < Board.size(); ++i)
    {        
        if ((Board[i][0]==Board[i][1])&&(Board[i][1])==(Board[i][2]))
            return true;
    }
    //vertical
    for(unsigned i{0}; i < Board.size(); ++i)
    {        
        if ((Board[0][i]==Board[1][i])&&(Board[1][i])==(Board[2][i]))
            return true;
    }

    // 2 diagonal       
        if ((Board[0][0]==Board[1][1])&&(Board[1][1])==(Board[2][2]))
            return true;
        if ((Board[0][2]==Board[1][1])&&(Board[1][1])==(Board[2][0]))
            return true;
    
    // out of moves
        if (MovesLeft<=0)
            return true;

    return false;
}

std::pair<unsigned, unsigned> CheckBoard2(const vector<vector<char>> &Board, const int &MovesLeft, const char Player){    

    // let's try just the tree again
        return treeMove(Board, Player);
    //checks for boards where x has a connect 2
    std::pair<unsigned, unsigned> move;
    bool mFlag = false;

    //use upppercase character in comparison
    char player = toupper(Player);

    //horizontal
    for(unsigned i{0}; i < Board.size(); ++i)
    {        
        if ((Board[i][0]==player)&&(Board[i][0]==Board[i][1])){
            move = {i, 2};
            mFlag=true;
            std::cout << "horizon line 1 \n";
        } 
        else if ((Board[i][0]==player)&&(Board[i][0])==(Board[i][2])) {
            move = {i, 1};
            mFlag=true;
            std::cout << "horizon line 2 \n";
        }
        else if ((Board[i][1]==player)&&(Board[i][1])==(Board[i][2])){
            move = {i, 0};
            mFlag=true;
            std::cout << "horizon line 3 \n";
        }
        if (mFlag){
            if (isdigit(Board[move.first][move.second])) {
            std::cout <<"successfully used flag" << std::endl;
            return move;
            }
        }
        // else continue checking
    }
    //vertical
    for(unsigned i{0}; i < 3; i++)
    {
        if ((Board[0][i]==player)&&(Board[0][i]==Board[1][i])) {
            mFlag=true;
            move = {2, i};
        }
        else if ((Board[0][i]==player)&&(Board[0][i]==Board[2][i])){
            mFlag=true;
            move = {1, i};
        }
        else if ((Board[1][i]==player)&&(Board[1][i]==Board[2][i])){
             move = {0, i};
             mFlag=true;
        }

        if (mFlag){
            if (isdigit(Board[move.first][move.second])) {
            std::cout <<"successfully used flag" << std::endl;
            return move;
            }
        }
        //else continue checking. 
    }

    // 2 diagonal       
        //top left
        if ((Board[0][0] ==player)&&(Board[0][0]==Board[1][1])) {
            move = {2,2};
            mFlag=true;
        }
        else if ((Board[0][0] ==player)&&(Board[0][0]==Board[2][2])) {
            move = {1,1};
            mFlag=true;
        }
        else if ((Board[1][1] ==player)&&(Board[1][1]==Board[2][2])){ 
            move = {0,0};
            mFlag=true;
        }

        if (mFlag){
            if (isdigit(Board[move.first][move.second])) {
            std::cout <<"successfully used flag" << std::endl;
            return move;
            }
        }

        //bottom left
        mFlag = true;
        if ((Board[2][0] ==player)&&(Board[2][0]==Board[1][1])) move = {0,2};
        else if ((Board[2][0] ==player)&&(Board[2][0]==Board[0][2])) move = {1,1};
        else if ((Board[1][1] ==player)&&(Board[1][1]==Board[0][2])) move = {2,0};
        else mFlag=false;

        if (mFlag){
            if (isdigit(Board[move.first][move.second])) {
            std::cout <<"successfully used flag" << std::endl;
            return move;
            }
        }


    // check if valid, otherwise move using tree. 
    return treeMove(Board, Player);
}


void GetCoord (unsigned &x, unsigned &y)
{   
    int move;
    cin >> move;
     do{
         if ((move >=1)&&(move <=9))
        {
            y=(move-1)/3;
            x=(move-1)%3;
            return;
        }    
        else
        {
         cout<< "Invalid move, please try again: ";
         cin >> move;            
        }
    }while(true);
}