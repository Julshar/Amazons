#pragma once
#include <vector>
#include "Board.h"

struct Possible_Moves //from[amazonFrom], to[amazonFrom][amazonTo], arrowTo[amazonFrom][amazonTo][arrowTo]
{
    std::vector<Field> from; //Positions of amazons
    std::vector<std::vector<Field>> to; //Positions of amazons after making move
    std::vector<std::vector<std::vector<Field>>> arrowTo; //Possitions of arrows
    Board board;

    Possible_Moves ();
    Possible_Moves (Board board);
    void getMoves ();
    int getMoveCount ();
};