#include "Board.h"
#include "Possible_Moves.h"
#include "Amazons.h"
#include "Node.h"

#include "algorithms/minimax.h"
#include "algorithms/negamax.h"
#include "algorithms/alphaBeta.h"
#include "algorithms/IterativeDeepening.h"
#include "algorithms/IterativeDeepeningRigidTimeLimit.h"
#include "algorithms/pns.h"
#include "algorithms/monteCarlo.h"

#include <iostream>
// #include <vector> 
// #include <stdlib.h>
#include <ctime>
// #include <limits>
#include <chrono>
// #include <string>

int main ()
{
    srand((unsigned) time(0)); //Seed random number generator

    Board board (10, 10, 4, 4, true);

    board.clearBoard();

    board.white.setClassic();
    board.black.setClassic();

    board.spawnAmazons();

    bool playing = true;

    //MINIMAX PRESET
    int depth = 1;

    //ALPHA BETA POURING
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    //ITERATIVE DEEPEING PRESET
    int maxDepth = 10;
    double timeLimit = 0.3;
    double relentlessTimeLimit = 3.0;

    //PNS PRESET
    int blockLimit = 2;

    //board.generateRandomBoard();      //Uncomment to start from a random generated board
    //board.checkBlocks(board.white, blockLimit);
    //board.checkBlocks(board.black, blockLimit);       //For PNS


    board.printBoard();

    while(playing)
    {
        // Possible_Moves moves(board);        //Uncomment if using MINIMAX, NEGAMAX or ALPHABETA
        // moves.getMoves();
        // int moveCount = moves.getMoveCount();

        // depth = getDepth(moveCount);      //Smaller depths for slower algorithms (MINIMAX, NEGAMAX)
        // //depth = getAlphaBetaDepth(moveCount);       //Larger depths for faster algorithms (alphaBeta)
        // std::cout << "CURRENT DEPTH: " << depth << "\n";

        //std::cout << moveCount << "\n";      //Display currently available moves

        //#################################################################################
        //#################################---WHITE---#####################################
        //#################################################################################

        std::cout << "\nWHITE MAKES MOVE...\n";

        //std::cout << minimax(board, depth, true);         //WHITE PLAYER ALGORITHMS
        //std::cout << negamax(board, depth, 1);
        //alphaBeta(board, depth, alpha, beta, true);
        iterativeDeepening (board, maxDepth, timeLimit);
        //iterativeDeepeningRelentlessTimeLimit (board, maxDepth, relentlessTimeLimit);
        //manualInputMove (board);
        //PNS (board);
        //monteCarloMove (board);

        std::cout << "\n";

        board.printBoard();

        if(board.checkDefeat(board.black))
        {
            std::cout << "BLACK LOST\n";
            break;
        }

        //#################################################################################
        //#################################---BLACK---#####################################
        //#################################################################################

        std::cout << "\nBLACK MAKES MOVE...\n";

        //std::cout << minimax(board, depth, false);        //BLACK PLAYER ALGORITHMS
        //std::cout << negamax(board, depth, -1);
        //alphaBeta(board, depth, alpha, beta, false);
        //iterativeDeepening (board, maxDepth, timeLimit);
        iterativeDeepeningRelentlessTimeLimit (board, maxDepth, relentlessTimeLimit);
        //manualInputMove (board);
        //PNS (board);
        //monteCarloMove (board);

        std::cout << "\n";

        board.printBoard();

        if(board.checkDefeat(board.white))
        {
            std::cout << "WHITE LOST\n";
            break;
        }
    }
    return 0;
}