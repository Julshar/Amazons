#pragma once
#include "../Board.h"
#include "../Possible_Moves.h"
#include <limits>
#include <ctime>
#include <chrono>

int alphaBetaIterativeDeepeing (Board & board, int depth, int alpha, int beta, bool maximizing, Field (&bestMove)[3]);
void iterativeDeepening (Board & board, int maxDepth, double timeLimit);