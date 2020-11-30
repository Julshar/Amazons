#pragma once
#include <vector>
#include "Field.h"

struct Amazons
{
    int fieldValue; //value on the board - 1 for white - 2 for black
    int count; //number of amazons
    std::vector<Field> pos; //actual amazon positions on current board
    std::vector<bool> blocked; //flag for PNS

    Amazons ();
    Amazons (int fieldValue, int count);
    void setClassic ();
};