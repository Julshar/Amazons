#include "algorithms/negamax.h"

int negamax (Board & board, int depth, int sign) //white plays
{
    if (board.whitePlays)
    {
        if (depth == 0 || board.checkDefeat(board.white))
            return sign * board.boardValue();
    }
    else
    {
        if (depth == 0 || board.checkDefeat(board.black))
            return sign * board.boardValue();
    }

    int chosenMove [3];
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
                int value = -1 * negamax(tmpBoard, depth - 1, -1 * sign);
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