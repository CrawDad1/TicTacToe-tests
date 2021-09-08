#include<iostream>
#include<vector>
using std::vector;
using std::cout;
using std::cin;
using std::endl;

void PlayGame();
void PrintBoard (const vector<vector<char>> &Board);
void SetCell(vector<vector<char>> &Board, char mark, unsigned &x, unsigned &y, int &MovesLeft);
bool CheckBoard(const vector<vector<char>> &Board, const int &MovesLeft);
void GetCoord (unsigned &x, unsigned &y);
bool PlayAgain();

int main(){

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

        
        cout << "Player O \n";
        cout << "where will you move?";        
        GetCoord(x, y);
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