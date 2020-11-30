#include "algorithms/pns.h"
#include "algorithms/IterativeDeepeningRigidTimeLimit.h"

/*

Seeks for win condition, and blocking. It will choose to block if the number of blocked enemy Amazons is higher than before making move.
Recognizes if Amazon is blocked, based on how many empty fields can it access in any amout of moves (The path is not blocked by anything)

If PNS will run out of time so it doesn't find anything, minimax with iterative deepening will be called, with a time cap of 0.3 seconds,
since it will generate a much better solution most of the time.

*/

void evaluate (Node * node) // DETERMINE IF NODE IS WINNING, LOSING OR (most likely) UNKNOWN
{
    bool lose = false;
    bool win = false;
    int blockLimit = 2;

    if ((node -> gameState.whitePlays && node -> isOR) || (!(node -> gameState.whitePlays) && node -> isAND)) //if true, white is player
    {  
        if (node -> gameState.checkBlocks(node -> gameState.white, blockLimit) || node -> gameState.checkDefeat(node -> gameState.white))
        {
            lose = true;
        }
        if (node -> gameState.checkBlocks(node -> gameState.black, blockLimit) || node -> gameState.checkDefeat(node -> gameState.black))
        {
            win = true;
        }   
    }
    else //Black is player
    {
        if (node -> gameState.checkBlocks(node -> gameState.black, blockLimit) || node -> gameState.checkDefeat(node -> gameState.black))
        {
            lose = true;
        }
        if (node -> gameState.checkBlocks(node -> gameState.white, blockLimit) || node -> gameState.checkDefeat(node -> gameState.white))
        {
            win = true;
        }    
    }

    if (win)
    {
        node -> proof = 0;
        node -> disproof = std::numeric_limits<int>::max();
    }
    else if (lose)
    {
        node -> proof = std::numeric_limits<int>::max();
        node -> disproof = 0;
    }
    else
    {
        node -> proof = 1;
        node -> disproof = 1;
    }
}

bool resourcesAvailable (double timeElapsed)
{
    if (timeElapsed > 2.0)
    {
        return false;
    }
    return true;
}

void setProofAndDisproofNumbers (Node * node)
{
    if (node -> isExpanded)
    {
        bool maxValue = false;
        if (node -> isAND)
        {
            node -> proof = 0;
            node -> disproof = std::numeric_limits<int>::max();
            for (int i = 0; i < node -> children.size(); i++)
            {
                if (!maxValue)
                {
                    if (node->children[i].proof == std::numeric_limits<int>::max())
                    {
                        maxValue = true;
                        node -> proof = node->children[i].proof;
                    }
                    else
                    {
                        node->proof += node->children[i].proof;
                    }                    
                }
                node->disproof = min(node->disproof, node->children[i].disproof);
            }
        }
        else if (node->isOR)
        {
            node->proof = std::numeric_limits<int>::max();
            node->disproof = 0;
            for (int i = 0; i < node->children.size(); i++)
            {
                if (!maxValue)
                {
                    if (node->children[i].disproof == std::numeric_limits<int>::max())
                    {
                        maxValue = true;
                        node -> disproof = node->children[i].disproof;
                    }
                    else
                    {
                        node->disproof += node->children[i].disproof;
                    }                    
                }
                node->proof = min(node->proof, node->children[i].proof);
            }
        }
    }
    else
    {
        evaluate (node);    
    }   
}

Node * selectMostProvidingNode (Node * node)
{
    while (node -> isExpanded)
    {
        int value = std::numeric_limits<int>::max();
        Node * best;
        if (node->isAND)
        {
            for (int i = 0; i < node->children.size(); i++)
            {
                if (value > node->children[i].disproof)
                {
                    best = & (node -> children[i]);
                    value = node->children[i].disproof;
                }
            }
        }
        else if (node->isOR)
        {
            for (int i = 0; i < node->children.size(); i++)
            {
                if (value > node->children[i].proof)
                {
                    best = & (node->children[i]);
                    value = node->children[i].proof;
                }
            }
        }
        node = best;
    }
    return node;
}

Node * selectFinalMove (Node * root)
{
    int value = std::numeric_limits<int>::max();
    Node * best;

    for (int i = 0; i < root->children.size(); i++)
    {
        if (value >= root->children[i].proof)
        {
            best = & (root -> children[i]);
            value = root->children[i].proof;
        }
    }
    return best;
}

void expandNode (Node * node)
{
    generateChildren (node);
    for (int i = 0; i < node -> children.size(); i++)
    {
        evaluate (&(node -> children[i]));
        setProofAndDisproofNumbers (&(node -> children[i]));
        if (node -> isAND)
        {
            if (node -> children[i].disproof == 0)
                break;
        }
        else if (node -> isOR)
        {
            if (node -> children[i].proof == 0)
                break;
        }
    }
    node -> isExpanded = true;
}

Node * updateAncestors (Node * node, Node * root)
{
    while (node != root)
    {
        int oldProof = node -> proof;
        int oldDisproof = node -> disproof;
        setProofAndDisproofNumbers (node);
        if (node -> proof == oldProof && node -> disproof == oldDisproof)
        {
            return node;
        }
        node = node -> parent;
    }
    setProofAndDisproofNumbers (root);
    return root;
}

void PNS (Board & board)
{
    auto start = std::chrono::high_resolution_clock::now(); //TIME

    Node rootVal (board);
    Node * root = & rootVal;

    evaluate (root);
    setProofAndDisproofNumbers (root);
    Node * current = root;

    if (root -> proof == 0 || root -> disproof == 0) //Handle case when root is leaf-node, so while loop is omitted
    {
        generateChildren (root);
    }

    auto finish = std::chrono::high_resolution_clock::now(); //TIME
    std::chrono::duration<double> elapsed = finish - start;

    while (root -> proof != 0 && root -> disproof != 0 && resourcesAvailable(elapsed.count()))
    {
        Node * mostProviding = selectMostProvidingNode (current);
        expandNode (mostProviding);
        current = updateAncestors (mostProviding, root);

        finish = std::chrono::high_resolution_clock::now(); //TIME
        elapsed = finish - start;
    }

    std::cout << root -> proof << " proof - disproof " << root -> disproof << "\n"; //DISPLAY
    std::cout << "Time elapsed: " << elapsed.count() << " s" << "\n";

    if (elapsed.count() > 2.0)
    {
        iterativeDeepeningRelentlessTimeLimit (board, 3, 0.3);
    }
    else
    {
        board = selectFinalMove(root) -> gameState;
    }
}