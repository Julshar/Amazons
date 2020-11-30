#pragma once
#include "../Board.h"
#include "../Possible_Moves.h"
#include <limits>
#include <ctime>
#include <chrono>

int alphaBetaIterativeDeepeingTimeLimit (Board & board, int depth, int alpha, int beta, bool maximizing, Field (&bestMove)[3], double timeLimit, std::chrono::time_point<std::chrono::high_resolution_clock> start, bool & outOfTime);
void iterativeDeepeningRelentlessTimeLimit (Board & board, int maxDepth, double timeLimit);