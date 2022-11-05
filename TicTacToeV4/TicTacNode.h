#pragma once

#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include<utility>

class TicTacNode
{
    public:

	static std::vector<TicTacNode*> vXwins;
	static std::vector<TicTacNode*> vOwins;
	static std::vector<TicTacNode*> vDwins;
	static std::unordered_map<std::string, TicTacNode*> nodeMap;

	char turnState = { 'x' };
	std::string boardState = { '1','2','3','4','5','6','7','8','9' };

	//wins for branch
	std::vector<int> winsXOD = { 0,0,0 };


	//child nodes
	std::vector<TicTacNode*> childNodes;


    // Member functions

    char BoardCheck(/*checks this.boardstate*/);
    void xWins();
    void oWins();
    void dWins();
	void addWins();
	double boardScore(char winner);
    TicTacNode()=default;
	TicTacNode(const char& turn, std::string board);
};

//Non-member functions


void BuildTicTacTree(TicTacNode* Head, bool mapped = false);
TicTacNode* BuildTicTacTree(bool mapped = false);
void PrintBoards(std::vector<TicTacNode*> vect, char input);

