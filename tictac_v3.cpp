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

	void sortChildren(){
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

				if (childNodes[i]->winsXOD[0] <= (*k)->winsXOD[0]){
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
std::pair<unsigned, unsigned> CheckBoard2(const vector<vector<char>> &Board, const int &MovesLeft);
std::pair<unsigned, unsigned> treeMove(const vector<vector<char>> &Board){
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
    
    auto tempBoard = nodeMap[sBoard];

    auto nBoard = tempBoard->childNodes[0]->boardState;

    for (size_t i = 0; i < 9; i++)
    {
        if (sBoard[i]!=nBoard[i]) return {(i/3),(i%3)};
    }
    

    //return tempBoard->childNodes[0]->lastMove;

}

void GetCoord (unsigned &x, unsigned &y);
bool PlayAgain();


int main(){
    //generate tree
    std::cout << "generating tree. . . . \n";
    BuildTicTacTree();
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
        win =CheckBoard(Board, MovesLeft);
        if(win) break;        
        
        auto oMove = treeMove(Board);
        y=oMove.first;
        x=oMove.second;
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

std::pair<unsigned, unsigned> CheckBoard2(const vector<vector<char>> &Board, const int &MovesLeft){    

    //checks for boards where x has a connect 2
    //might work better if i just use the tree instead? 
    return treeMove(Board);

    //horizontal
    for(unsigned i{0}; i < Board.size(); ++i)
    {        
        if ((Board[i][0]=='x')&&(Board[i][0]==Board[i][1])) return {i, 2};
        else if ((Board[i][0]=='x')&&(Board[i][0])==(Board[i][2])) return {i, 1};
        else if ((Board[i][1]=='x')&&(Board[i][1])==(Board[i][2])) return {i, 0};
    }
    //vertical
    for(unsigned i{0}; i < Board.size(); ++i)
    {
        if ((Board[0][i]=='x')&&(Board[0][i]==Board[1][i])) return {2, i};
        else if ((Board[0][i]=='x')&&(Board[0][i]==Board[2][i])) return {1, i};
        else if ((Board[1][i]=='x')&&(Board[1][i]==Board[2][i])) return {0, i};
    }

    // 2 diagonal       
        //top left
        if ((Board[0][0] == 'x')&&(Board[0][0]==Board[1][1])) return {2,2};
        else if ((Board[0][0] == 'x')&&(Board[0][0]==Board[2][2])) return {1,1};
        else if ((Board[1][1] == 'x')&&(Board[1][1]==Board[2][2])) return {0,0};

        //bottom left
        if ((Board[2][0] == 'x')&&(Board[2][0]==Board[1][1])) return {0,2};
        else if ((Board[2][0] == 'x')&&(Board[2][0]==Board[0][2])) return {1,1};
        else if ((Board[1][1] == 'x')&&(Board[1][1]==Board[0][2])) return {2,0};



        
    
   

    // no set of two, pick from tree ? 
    return treeMove(Board);
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