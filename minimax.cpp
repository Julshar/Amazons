#include "algorithms/minimax.h"

int minimax (Board & board, int depth, bool maximizing)
{
    if (board.whitePlays)
    {
        if (depth == 0 || board.checkDefeat(board.white))
            return board.boardValue();
    }
    else
    {
        if (depth == 0 || board.checkDefeat(board.black))
            return board.boardValue();
    }

    int chosenMove [3];

    if (maximizing)
    {
        int maxValue = std::numeric_limits<int>::min();
        Possible_Moves playerMoves (board);
        playerMoves.getMoves();
        for (int a = 0; a < playerMoves.from.size(); a++)
        {
            for (int b = 0; b < playerMoves.to[a].size(); b++)
            {
                for (int c = 0; c < playerMoves.arrowTo[a][b].size(); c++)
                {
                    Board tmpBoard = board;
                    tmpBoard.makeMove(playerMoves.from[a], playerMoves.to[a][b], playerMoves.arrowTo[a][b][c]);
                    int value = minimax(tmpBoard, depth - 1, false);
                    if (value > maxValue)
                    {
                        chosenMove[0] = a;
                        chosenMove[1] = b;
                        chosenMove[2] = c;
                        maxValue = value;
                    }
                }
            }
        }
        board.makeMove(playerMoves.from[chosenMove[0]], playerMoves.to[chosenMove[0]][chosenMove[1]], playerMoves.arrowTo[chosenMove[0]][chosenMove[1]][chosenMove[2]]);
        return maxValue;
    }
    else
    {
        int minValue = std::numeric_limits<int>::max();
        Possible_Moves enemyMoves (board);
        enemyMoves.getMoves();
        for (int a = 0; a < enemyMoves.from.size(); a++)
        {
            for (int b = 0; b < enemyMoves.to[a].size(); b++)
            {
                for (int c = 0; c < enemyMoves.arrowTo[a][b].size(); c++)
                {
                    Board tmpBoard = board;
                    tmpBoard.makeMove(enemyMoves.from[a], enemyMoves.to[a][b], enemyMoves.arrowTo[a][b][c]);
                    int value = minimax(tmpBoard, depth - 1, true);
                    if (value < minValue)
                    {
                        chosenMove[0] = a;
                        chosenMove[1] = b;
                        chosenMove[2] = c;
                        minValue = value;
                    }
                }
            }
        }
        board.makeMove(enemyMoves.from[chosenMove[0]], enemyMoves.to[chosenMove[0]][chosenMove[1]], enemyMoves.arrowTo[chosenMove[0]][chosenMove[1]][chosenMove[2]]);
        return minValue;
    }    
}