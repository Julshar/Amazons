#pragma once
#include "../Board.h"
#include "../Possible_Moves.h"
#include <limits>

int minimax (Board & board, int depth, bool maximizing);