#pragma once
#include "Amazons.h"
#include <iostream>
#include <limits>

struct Board
{
    int height;
    int width;
    std::vector<std::vector<int>> board;
    bool whitePlays;
    Amazons white;
    Amazons black;

    Board ();
    Board (int height, int width, int whiteCount, int blackCount, bool whitePlays);
    Board (const Board & original);
    void clearBoard (); //Fill the entire board with 0
    void printBoard (); //Prints current state of the board into console
    void spawnAmazons (); //Place amazons on the board, according to their positions described by Amazons::pos
    void makeMove (Field from, Field to, Field arrowTo);
    bool checkDefeat (Amazons amazons);
    void generateRandomBoard ();
    int boardValue (); //+ favors white, - favors black
    bool checkBlocks (Amazons amazonColor, int blockLimit);
};

int getDepth (int moveCount); //Return depth of the search depending on available move count
int getAlphaBetaDepth (int moveCount); //Return depth of the search depending on available move count (for alphaBeta search)