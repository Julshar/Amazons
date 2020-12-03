#include "algorithms/monteCarlo.h"

int estimateNumberOfSimulations (int numberOfMoves)
{
    if (numberOfMoves < 15)
        return 640;
    else if (numberOfMoves < 25)
        return 320;
    else if (numberOfMoves < 40)
        return 160;
    else if (numberOfMoves < 55)
        return 80;
    else if (numberOfMoves < 100)
        return 40;
    else if (numberOfMoves < 150)
        return 20;
    else if (numberOfMoves < 200)
        return 10;
    else if (numberOfMoves < 300)
        return 5;
    else if (numberOfMoves < 700)
        return 1;
    else
        return 0;
}

void monteCarloMove (Board & board)
{
    auto start = std::chrono::high_resolution_clock::now();
    Node rootVal (board);
    Node * root = & rootVal;
    Node * bestChild = NULL;
    generateChildren (root);
    int highestWinCount = -1;
    bool whiteIsPlayer = root -> gameState.whitePlays;

    int numberOfSimulations = estimateNumberOfSimulations (root -> children.size());
    
    if (numberOfSimulations == 0)
    {
        makeRandomMove (root);
        bestChild = root;
    }
    else
    {
        for (int a = 0; a < root -> children.size(); a++)
        {
            if (whiteIsPlayer) //Handle exception - children of root are game states after making move by player. When the opponent has no moves - makeRandomMove(child) fails due to no available move
            {
                if (root -> children[a].gameState.checkDefeat(root -> children[a].gameState.black))
                {
                    bestChild = &(root -> children[a]);
                    continue;
                }
            }
            else
            {
                if (root -> children[a].gameState.checkDefeat(root -> children[a].gameState.white))
                {
                    bestChild = &(root -> children[a]);
                    continue;
                }
            }

            int winCount = 0;
            for (int b = 0; b < numberOfSimulations; b++)
            {
                Node tmpNode = root -> children[a];
                Node * child = &tmpNode;
                bool isLeaf = false;

                while (!isLeaf)
                {
                    makeRandomMove(child);
                    if (child -> gameState.whitePlays) //White is playing now, so black made last move
                    {
                        if (child -> gameState.checkDefeat(child -> gameState.white)) //If true, white lost
                        {
                            isLeaf = true;
                            if (!whiteIsPlayer)
                            {
                                winCount++;
                            }
                        }
                    }
                    else
                    {
                        if (child -> gameState.checkDefeat(child -> gameState.black)) //If true, black lost
                        {
                            isLeaf = true;
                            if (whiteIsPlayer)
                            {
                                winCount++;
                            }
                        }
                    }
                }
            }
            if (winCount > highestWinCount)
            {
                highestWinCount = winCount;
                bestChild = &(root -> children[a]);
            }
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    std::cout << "Available moves = " << root -> children.size() << "\nEstimated simulation count = " << numberOfSimulations << "\n";
    std::cout << "Time elapsed: " << elapsed.count() << " s\n";
    std::cout << "highest Win Count = " << highestWinCount << "\n";

    board = bestChild -> gameState;
}