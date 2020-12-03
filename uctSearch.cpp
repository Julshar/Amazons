#include "algorithms/uctSearch.h"

Node * bestChild (Node * node)
{
    /*
    Each node has wonSim value that is total simulations won by the player that makes move in that node
    It means, that children of any node have wonSim value for opponent
    So in order to search for the best node among children, following rules were implemented :

    - Searches for lowest child value
    - There is substraction instead of adding in the formula
    */
    double c = 1;
    double value = (double)std::numeric_limits<int>::max();
    Node * bestChild = NULL;
    for (int i = 0; i < node -> children.size(); i++)
    {
        double childValue = (double)node -> children[i].wonSim / (double)node -> children[i].totalSim - c * sqrt(log((double)node -> totalSim) / (double)node -> children[i].totalSim);
        if (childValue < value)
        {
            value = childValue;
            bestChild = &(node -> children[i]);
        }
    }
    return bestChild;
}

Node * selectNewRandomChild (Node * node)
{
    generateChildrenUTC (&(node -> children[node -> nextChildIndex]));
    (node -> nextChildIndex)++;
    if (node -> nextChildIndex == node -> children.size())
    {
        node -> fullyExpanded = true;
    }
    return &(node -> children[node -> nextChildIndex - 1]);
}

Node * selectCurrentNode (Node * node)
{
    while (!((node -> gameState.checkDefeat(node -> gameState.white)) || (node -> gameState.checkDefeat(node -> gameState.black))))
    {
        if (!(node -> fullyExpanded))
        {
            //std::cout << "GOING TO RETURN selectNewRandomChild\n";
            return selectNewRandomChild (node);
        }
        else
        {
            node = bestChild (node);
        }
    }
    return node;
}

int runRandomSimulation (Node node) //Returns 1 (win) whenever the player who made first move won
{
    bool isLeaf = false;
    bool whiteIsPlayer = node.gameState.whitePlays;

    if (node.gameState.checkDefeat(node.gameState.white))
    {
        if (!whiteIsPlayer)
        {
            return 1;
        }
        isLeaf = true;
    }
    else if (node.gameState.checkDefeat(node.gameState.black))
    {
        if (whiteIsPlayer)
        {
            return 1;
        }
        isLeaf = true;
    }

    while (!isLeaf)
    {
        makeRandomMove(&node); //MIGHT BE POINTER ERROR
        if (node.gameState.whitePlays) //White is playing now, so black made last move
        {
            if (node.gameState.checkDefeat(node.gameState.white)) //If true, white lost
            {
                isLeaf = true;
                if (!whiteIsPlayer)
                {
                    return 1;
                }
            }
        }
        else
        {
            if (node.gameState.checkDefeat(node.gameState.black)) //If true, black lost
            {
                isLeaf = true;
                if (whiteIsPlayer)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void updateTree (Node * node, int result) //Result is already adjusted by runRandomSimulation (runs for player that makes move in node)
{
    while (node != NULL)
    {
        (node -> totalSim)++;
        node -> wonSim += result;
        node = node -> parent;
        result = (result == 1) ? 0 : 1;
    }
}

/*
Tries to simulate each child of root one time at first 
If it fails due to time limit and too many possibilities it makes random move

The longer the game goes, the less possibilities so uctSearch becomes more accurate
*/
void uctSearch (Board & board, double timeLimit)
{
    auto start = std::chrono::high_resolution_clock::now(); //TIME

    Node rootVal (board, true);
    Node * root = &rootVal;
    Node * current = root;
    generateChildrenUTC (root);

    auto finish = std::chrono::high_resolution_clock::now(); //TIME
    std::chrono::duration<double> elapsed = finish - start;
    int iterations = 0;

    while (timeLeft (elapsed.count(), timeLimit))
    {
        current = selectCurrentNode(current);
        int result = runRandomSimulation (*current);
        updateTree (current, result);
        current = root;

        iterations++;

        finish = std::chrono::high_resolution_clock::now(); //TIME
        elapsed = finish - start;
    }
    std::cout << root -> nextChildIndex << " children of root searched out of total " << root -> children.size() << "\n";
    std::cout << "Total iteration count: " << iterations << "\n";
    Node * bestChild = NULL;
    if (root -> nextChildIndex < root -> children.size())
    {
        makeRandomMove(root);
        bestChild = root;
        std::cout << "Move is random\n";
    }
    else
    {
        std::cout << "Root iterations: " << root -> totalSim << ", Root wins: " << root -> wonSim << "\n";
        double value = (double)std::numeric_limits<int>::max();
        for (int i = 0; i < root -> children.size(); i++)
        {
            //std::cout << i << " Child iterations: " << root -> children[i].totalSim << ", Child wins: " << root -> children[i].wonSim << " --- "; //Uncomment for details
            double childValue = (double)root -> children[i].wonSim / (double)root -> children[i].totalSim;
            if (childValue < value)
            {
                value = childValue;
                bestChild = &(root -> children[i]);
                //std::cout << "\nNew best child index - " << i << "\n"; //Uncomment for details
            }
        }
    }
    board = bestChild -> gameState;
}