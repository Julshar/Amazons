#pragma once
#include "../Node.h"
#include <ctime>
#include <chrono>
#include <iostream>

int estimateNumberOfSimulations (int numberOfMoves);
void makeRandomMove (Node * node);
void monteCarloMove (Board & board);