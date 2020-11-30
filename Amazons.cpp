#include "Amazons.h"

int fieldValue; //value on the board - 1 for white - 2 for black
int count; //number of amazons
std::vector<Field> pos; //actual amazon positions on current board
std::vector<bool> blocked; //flag for PNS

Amazons::Amazons ()
{
}

Amazons::Amazons (int fieldValue, int count) : fieldValue(fieldValue), count(count), pos(count), blocked(count, false)
{
}

void Amazons::setClassic () //Classic amazon setup (4 amazons for each player)
{
    if (fieldValue == 1)
    {
        pos[0].height = 6;
        pos[0].width = 0;
        pos[1].height = 6;
        pos[1].width = 9;
        pos[2].height = 9;
        pos[2].width = 3;
        pos[3].height = 9;
        pos[3].width = 6;
    }
    else if (fieldValue == 2)
    {
        pos[0].height = 0;
        pos[0].width = 3;
        pos[1].height = 0;
        pos[1].width = 6;
        pos[2].height = 3;
        pos[2].width = 0;
        pos[3].height = 3;
        pos[3].width = 9;
    }
    for (int i = 0; i < count; i++)
        blocked[i] = false;
}