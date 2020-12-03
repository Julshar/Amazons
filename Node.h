#pragma once
#include "Board.h"
#include "Possible_Moves.h"

struct Node
{
    Node * parent;
    std::vector <Node> children;
    Board gameState;

    //pns
    int proof;
    int disproof;
    bool isAND;
    bool isOR;
    bool isExpanded;

    //utc
    int totalSim;
    int wonSim;
    bool fullyExpanded;
    int nextChildIndex;

    Node ();
    Node (Board board); //Create root
    Node (Node * parent, Board board);
    Node (Board board, bool utc); //Create utcRoot
    Node (Node * parent, Board board, bool utc);
};

int min (int a, int b);
int max (int a, int b);
void generateChildren (Node * node);
void generateChildrenUTC (Node * node);
bool timeLeft (double timeElapsed, double timeLimit);
void makeRandomMove (Node * node);
