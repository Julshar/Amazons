#pragma once
#include "Board.h"
#include "Possible_Moves.h"

struct Node
{
    Node * parent;
    std::vector <Node> children;
    Board gameState;

    int proof;
    int disproof;

    bool isExpanded;
    bool isAND;
    bool isOR;

    Node ();
    Node (Board board); //Create root
    Node (Node * parent, Board board);
};

int min (int a, int b);
int max (int a, int b);
void generateChildren (Node * node);