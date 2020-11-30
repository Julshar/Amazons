#pragma once
#include "../Node.h"
#include <limits>
#include <ctime>
#include <chrono>

void evaluate (Node * node);
bool resourcesAvailable (double timeElapsed);
void setProofAndDisproofNumbers (Node * node);
Node * selectMostProvidingNode (Node * node);
Node * selectFinalMove (Node * root);
void expandNode (Node * node);
Node * updateAncestors (Node * node, Node * root);
void PNS (Board & board);