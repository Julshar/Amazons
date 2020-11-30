#pragma once
#include "../Board.h"
#include "../Possible_Moves.h"
#include <limits>

int alphaBeta (Board & board, int depth, int alpha, int beta, bool maximizing);