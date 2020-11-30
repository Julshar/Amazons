#include "algorithms/IterativeDeepening.h"
/*

Zostały wykonane dwie wersje algorytmu ustalającego głębokość poszukiwań w zależności od dostępnego czasu.
Obydwie wersje opierają się na algorytmie MINIMAX z odcięciami alfa-beta.

PIERWSZY z nich sprawdza czy nie przekroczył wyznaczonego limitu czasowego po przeszukaniu drzewa gry do danej głębokości.
Dzięki temu szybkość działania algorytmu pozostaje prawie nienaruszona, jednakże występuje tu pewien problem.

Przy dużych ilościach dostępnych ruchów (liczba dostępnych pierwszych ruchów na planszy 10x10 to 2176), czas potrzebny na
przeszukanie drzewa gry wraz ze wzrostem głębokości zwiększa się o bardzo duże wartości. 
Skutkuje to tym, że sztywno ustalony limit czasowy sprawdzany po przeszukaniu drzewa może zostać mocno przekroczony przy
sprawdzaniu do kolejnej głębokości. 
Przykładowo limit czasowy ustalony jest na 1 sekundę.
W początkowej fazie gry, algorytm przeszukujący na głębokości 2, jest w stanie skończyć działanie poniżej 1 sekundy.
Rozpocznie wtedy przeszukiwanie drzewa do głębokości 3, które może już trwać nawet więcej niż 200 sekund.

DRUGA wersja algorytmu sprawdza limit czasowy podczas działania zmodyfikowanego algorytmu MINIMAX.
Kiedy czas zostaje przekroczony, działanie algorytmu zostaje przerwane.
Dzięki temu przy ustaleniu limitu czasowego na 3 sekundy, ruch zostaje wykonany zawsze w nieco ponad 3 sekundy.
Minusem tego rozwiązania, jest spowolnienie działania algorytmu, ponieważ poza sprawdzaniem ruchów musi też regularnie
sprawdzać czy limit czasowy nie został przekroczony.

*/

int alphaBetaIterativeDeepeing (Board & board, int depth, int alpha, int beta, bool maximizing, Field (&bestMove)[3])
{
    if (board.whitePlays)
    {
        if (depth == 0 || board.checkDefeat(board.white))
            return board.boardValue();
    }
    else
    {
        if (depth == 0 || board.checkDefeat(board.black))
            return board.boardValue();
    }

    int chosenMove [3];
    bool branchCut = false;

    if (maximizing)
    {
        int maxValue = std::numeric_limits<int>::min();
        Possible_Moves playerMoves (board);
        playerMoves.getMoves();
        for (int a = 0; a < playerMoves.from.size(); a++)
        {
            for (int b = 0; b < playerMoves.to[a].size(); b++)
            {
                for (int c = 0; c < playerMoves.arrowTo[a][b].size(); c++)
                {
                    Board tmpBoard = board;
                    tmpBoard.makeMove(playerMoves.from[a], playerMoves.to[a][b], playerMoves.arrowTo[a][b][c]);
                    int value = alphaBetaIterativeDeepeing(tmpBoard, depth -1, alpha, beta, false, bestMove);
                    if (value > maxValue)
                    {
                        chosenMove[0] = a;
                        chosenMove[1] = b;
                        chosenMove[2] = c;
                        maxValue = value;
                    }
                    if (alpha < value)
                        alpha = value;
                    if (beta <= alpha)
                    {
                        branchCut = true;
                        break;
                    }
                }
                if (branchCut)
                    break;
            }
            if (branchCut)
                break;
        }                                    
        board.makeMove(playerMoves.from[chosenMove[0]], playerMoves.to[chosenMove[0]][chosenMove[1]], playerMoves.arrowTo[chosenMove[0]][chosenMove[1]][chosenMove[2]]);
        bestMove[0] = playerMoves.from[chosenMove[0]];
        bestMove[1] = playerMoves.to[chosenMove[0]][chosenMove[1]];
        bestMove[2] = playerMoves.arrowTo[chosenMove[0]][chosenMove[1]][chosenMove[2]];
        return maxValue;
    }
    else
    {
        int minValue = std::numeric_limits<int>::max();
        Possible_Moves playerMoves (board);
        playerMoves.getMoves();
        for (int a = 0; a < playerMoves.from.size(); a++)
        {
            for (int b = 0; b < playerMoves.to[a].size(); b++)
            {
                for (int c = 0; c < playerMoves.arrowTo[a][b].size(); c++)
                {
                    Board tmpBoard = board;
                    tmpBoard.makeMove(playerMoves.from[a], playerMoves.to[a][b], playerMoves.arrowTo[a][b][c]);
                    int value = alphaBetaIterativeDeepeing(tmpBoard, depth -1, alpha, beta, true, bestMove);
                    if (value < minValue)
                    {
                        chosenMove[0] = a;
                        chosenMove[1] = b;
                        chosenMove[2] = c;
                        minValue = value;
                    }
                    if (beta > value)
                        beta = value;
                    if (beta <= alpha)
                    {
                        branchCut = true;
                        break;
                    }
                }
                if (branchCut)
                    break;
            }
            if (branchCut)
                break;
        }
        board.makeMove(playerMoves.from[chosenMove[0]], playerMoves.to[chosenMove[0]][chosenMove[1]], playerMoves.arrowTo[chosenMove[0]][chosenMove[1]][chosenMove[2]]);
        bestMove[0] = playerMoves.from[chosenMove[0]];
        bestMove[1] = playerMoves.to[chosenMove[0]][chosenMove[1]];
        bestMove[2] = playerMoves.arrowTo[chosenMove[0]][chosenMove[1]][chosenMove[2]];
        return minValue;
    }    
}

void iterativeDeepening (Board & board, int maxDepth, double timeLimit) //timeLimit is measured in seconds
{
    Field bestMove [3];
    auto start = std::chrono::high_resolution_clock::now(); //Get current time

    for (int i = 1; i <= maxDepth; i++)
    {
        Board tmpBoard = board;

        alphaBetaIterativeDeepeing(tmpBoard, i, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), board.whitePlays, bestMove);

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        if (elapsed.count() > timeLimit || i == maxDepth)
        {
            std::cout << "Elapsed time: " << elapsed.count() << " s\n";
            std::cout << "Searching depth: " << i - 1 << "\n";
            break;
        }
    }
    board.makeMove(bestMove[0], bestMove[1], bestMove[2]);
}