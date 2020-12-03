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

Node::Node (Board board, bool utc) : parent(NULL), gameState(board) //Create root UTC
{
    totalSim = 0;
    wonSim = 0;
    fullyExpanded = false;
    nextChildIndex = 0;
}

Node::Node (Node * parent, Board board, bool utc) : parent(parent), gameState(board)
{
    totalSim = 0;
    wonSim = 0;
    fullyExpanded = false;
    nextChildIndex = 0;
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

void generateChildrenUTC (Node * node)
{
    Possible_Moves moves (node -> gameState);
    moves.getMoves();
    if (moves.getMoveCount() == 0)
    {
        node -> fullyExpanded = true;
    }
    for (int a = 0; a < moves.from.size(); a++)
    {
        for (int b = 0; b < moves.to[a].size(); b++)
        {
            for (int c = 0; c < moves.arrowTo[a][b].size(); c++)
            {
                Board newGameState = node -> gameState;
                newGameState.makeMove(moves.from[a], moves.to[a][b], moves.arrowTo[a][b][c]);
                Node child (node, newGameState, true);
                node -> children.push_back(child);
            }
        }
    }
}

bool timeLeft (double timeElapsed, double timeLimit)
{
    if (timeElapsed > timeLimit)
    {
        return false;
    }
    return true;
}

void makeRandomMove (Node * node)
{
    Possible_Moves moves (node -> gameState);
    moves.getMoves();

    int r1 = (rand() % moves.from.size());
    bool isBlocked = true; //Chosen amazon might be blocked, but still has its instance in vector "from".
    while (isBlocked)
    {
        if (moves.to[r1].size() > 0)
        {
            isBlocked = false;
        }
        else
        {
            r1 = (rand() % moves.from.size());
        }
    }
    int r2 = (rand() % moves.to[r1].size());
    int r3 = (rand() % moves.arrowTo[r1][r2].size());
    node -> gameState.makeMove(moves.from[r1], moves.to[r1][r2], moves.arrowTo[r1][r2][r3]);
}
