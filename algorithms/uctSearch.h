#pragma once
#include "../Node.h"
#include <limits>
#include <ctime>
#include <chrono>
#include <math.h>

Node * bestChild (Node * node);
Node * selectNewRandomChild (Node * node);
Node * selectCurrentNode (Node * node);
int runRandomSimulation (Node * node);
void updateTree (Node * node, int result);
void uctSearch (Board & board, double timeLimit);