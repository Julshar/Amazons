#include "algorithms/IterativeDeepeningRigidTimeLimit.h"

int alphaBetaIterativeDeepeingTimeLimit (Board & board, int depth, int alpha, int beta, bool maximizing, Field (&bestMove)[3], double timeLimit, std::chrono::time_point<std::chrono::high_resolution_clock> start, bool & outOfTime)
{
    if (outOfTime)
    {
        return 0;
    }

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
    bool branchCut = false;

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
                    auto finish = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed = finish - start;
                    if (elapsed.count() > timeLimit)
                    {
                        outOfTime = true; //Out of time
                        return 0;
                    }

                    Board tmpBoard = board;
                    tmpBoard.makeMove(playerMoves.from[a], playerMoves.to[a][b], playerMoves.arrowTo[a][b][c]);
                    int value = alphaBetaIterativeDeepeingTimeLimit(tmpBoard, depth - 1, alpha, beta, board.whitePlays, bestMove, timeLimit, start, outOfTime);
                    if (value > maxValue)
                    {
                        chosenMove[0] = a;
                        chosenMove[1] = b;
                        chosenMove[2] = c;
                        maxValue = value;
                    }
                    if (alpha < value)
                        alpha = value;
                    if (beta <= alpha)
                    {
                        branchCut = true;
                        break;
                    }
                }
                if (branchCut)
                    break;
            }
            if (branchCut)
                break;
        }                                    
        board.makeMove(playerMoves.from[chosenMove[0]], playerMoves.to[chosenMove[0]][chosenMove[1]], playerMoves.arrowTo[chosenMove[0]][chosenMove[1]][chosenMove[2]]);
        bestMove[0] = playerMoves.from[chosenMove[0]];
        bestMove[1] = playerMoves.to[chosenMove[0]][chosenMove[1]];
        bestMove[2] = playerMoves.arrowTo[chosenMove[0]][chosenMove[1]][chosenMove[2]];
        return maxValue;
    }
    else
    {
        int minValue = std::numeric_limits<int>::max();
        Possible_Moves playerMoves (board);
        playerMoves.getMoves();
        for (int a = 0; a < playerMoves.from.size(); a++)
        {
            for (int b = 0; b < playerMoves.to[a].size(); b++)
            {
                for (int c = 0; c < playerMoves.arrowTo[a][b].size(); c++)
                {
                    auto finish = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed = finish - start;
                    if (elapsed.count() > timeLimit)
                    {
                        outOfTime = true; //Out of time
                        return 0;
                    }

                    Board tmpBoard = board;
                    tmpBoard.makeMove(playerMoves.from[a], playerMoves.to[a][b], playerMoves.arrowTo[a][b][c]);
                    int value = alphaBetaIterativeDeepeingTimeLimit(tmpBoard, depth - 1, alpha, beta, board.whitePlays, bestMove, timeLimit, start, outOfTime);
                    if (value < minValue)
                    {
                        chosenMove[0] = a;
                        chosenMove[1] = b;
                        chosenMove[2] = c;
                        minValue = value;
                    }
                    if (beta > value)
                        beta = value;
                    if (beta <= alpha)
                    {
                        branchCut = true;
                        break;
                    }
                }
                if (branchCut)
                    break;
            }
            if (branchCut)
                break;
        }
        board.makeMove(playerMoves.from[chosenMove[0]], playerMoves.to[chosenMove[0]][chosenMove[1]], playerMoves.arrowTo[chosenMove[0]][chosenMove[1]][chosenMove[2]]);
        bestMove[0] = playerMoves.from[chosenMove[0]];
        bestMove[1] = playerMoves.to[chosenMove[0]][chosenMove[1]];
        bestMove[2] = playerMoves.arrowTo[chosenMove[0]][chosenMove[1]][chosenMove[2]];
        return minValue;
    }    
}

void iterativeDeepeningRelentlessTimeLimit (Board & board, int maxDepth, double timeLimit) //timeLimit is measured in seconds
{
    bool outOfTime = false; //Determine if the algorithm ran out of time. If it did, do not take its "bestMove" under consideration
    Field bestMove [3];
    Field moveToDo [3];
    auto start = std::chrono::high_resolution_clock::now(); //Get current time

    for (int i = 1; i <= maxDepth; i++)
    {
        Board tmpBoard = board;

        alphaBetaIterativeDeepeingTimeLimit(tmpBoard, i, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), board.whitePlays, bestMove, timeLimit, start, outOfTime);

        if (!outOfTime || i == 1)
        {
            moveToDo[0] = bestMove[0];
            moveToDo[1] = bestMove[1];
            moveToDo[2] = bestMove[2];
        }

        if (outOfTime || i == maxDepth)
        {
            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
            std::cout << "Elapsed time: " << elapsed.count() << " s\n";
            std::cout << "Searching depth: " << i - 1 << "\n";
            break;
        }
    }
    board.makeMove(moveToDo[0], moveToDo[1], moveToDo[2]);
}