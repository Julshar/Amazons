#include "Node.h"

Node::Node ()
{
}

Node::Node (Board board) : parent(NULL), gameState(board), isOR(true), isAND(false), isExpanded(false) //Create root
{
}

Node::Node (Node * parent, Board board) : parent(parent), gameState(board), isExpanded(false)
{
    if (parent -> isAND)
    {
        this -> isOR = true;
        this -> isAND = false;
    }
    else if (parent -> isOR)
    {
        this -> isOR = false;
        this -> isAND = true;
    }
}

int min (int a, int b)
{
    return (a < b) ? a : b;
}

int max (int a, int b)
{
    return (a > b) ? a : b;
}

void generateChildren (Node * node)
{
    Possible_Moves moves (node -> gameState);
    moves.getMoves();
    for (int a = 0; a < moves.from.size(); a++)
    {
        for (int b = 0; b < moves.to[a].size(); b++)
        {
            for (int c = 0; c < moves.arrowTo[a][b].size(); c++)
            {
                Board newGameState = node -> gameState;
                newGameState.makeMove(moves.from[a], moves.to[a][b], moves.arrowTo[a][b][c]);
                Node child (node, newGameState);
                node -> children.push_back(child);
            }
        }
    }
}