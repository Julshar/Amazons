#pragma once
#include "../Node.h"
#include <ctime>
#include <chrono>
#include <iostream>

int estimateNumberOfSimulations (int numberOfMoves);
void monteCarloMove (Board & board);