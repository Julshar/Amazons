#pragma once
#include "../Board.h"
#include "../Possible_Moves.h"
#include <limits>

int negamax (Board & board, int depth, int sign); //white plays