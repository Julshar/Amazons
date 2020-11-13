#include <iostream>
#include <vector> 
#include <stdlib.h>
#include <ctime>
#include <limits>
#include <chrono>
#include <string>

//using namespace std;

struct Field //Basic datatype - contains field coordinates to locate it on the board
{
    int height;
    int width;
};

struct Amazons //If white initialize with {1, count}, if black then with {2, count}
{
    int fieldValue; //value on the board - 1 for white - 2 for black
    int count; //number of amazons
    std::vector<Field> pos; //actual amazon positions on current board

    Amazons ()
    {
    }

    Amazons (int fieldValue, int count) : fieldValue(fieldValue), count(count), pos(count)
    {
    }

    void setClassic () //Classic amazon setup (4 amazons for each player)
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
    }
};

struct Board //Initialize with {height, width}
{
    int height;
    int width;
    std::vector<std::vector<int>> board;
    bool whitePlays;

    Amazons white;
    Amazons black;

    Board () : whitePlays(true)
    {
    }

    Board (int height, int width, int whiteCount, int blackCount, bool whitePlays) : height(height), width(width), 
    board(height, std::vector<int>(width, 0)), white(1, whiteCount), black(2, blackCount), whitePlays(whitePlays)
    {   
    }

    Board (const Board & original) : height(original.height), width(original.width), board(original.board),
    white(original.white), black(original.black), whitePlays(original.whitePlays)
    {
    }

    void clearBoard () //Fill the entire board with 0
    {
        for (int i = 0; i < height; i++)
        {
            for (int k = 0; k < width; k++)
            {
                board [i][k] = 0;
            }
        }
        whitePlays = true;
    }

    void printBoard () //Prints current state of the board into console
    {
        int empty = 45; //ascii values
	    int white = 87; //ascii values
	    int black = 66; //ascii values
	    int arrow = 254; //ascii values

        std::cout << "    "; //Upper numeration
        for (int i = 0; i < width; i++)
        {
            std::cout << i << "  ";
        }
        std::cout << std::endl;

        std::cout << "  " << (char)201; //Upper border line
        for (int i = 0; i < width; i++)
        {
            std::cout << (char)205 << (char)205 << (char)205;
        }
        std::cout << (char)187 << std::endl;

        for (int i = 0; i < height; i++) //Board
        {
            std::cout << i << " " << (char)186;
            for (int k = 0; k < width; k++)
            {
                if (board [i][k] == 0)
                    std::cout << " " << (char)empty << " ";
                else if (board [i][k] == 3)
                    std::cout << " " << (char)arrow << " ";
                else if (board [i][k] == 1)
                    std::cout << " " << (char)white << " ";
                else if (board [i][k] == 2)
                    std::cout << " " << (char)black << " ";
                else
                    std::cout << " " << board[i][k] << " ";
            }
            std::cout << (char)186 << " " << i << std::endl;
        }

        std::cout << "  " << (char)200; //Bottom border line
        for (int i = 0; i < width; i++)
        {
            std::cout << (char)205 << (char)205 << (char)205;
        }
        std::cout << (char)188 << std::endl;

        std::cout << "    "; //Botttom numeration
        for (int i = 0; i < width; i++)
        {
            std::cout << i << "  ";
        }
        std::cout << std::endl;
    }

    void spawnAmazons () //Place amazons on the board, according to their positions described by Amazons::pos
    {
        for (int i = 0; i < white.count; i++)
            board[white.pos[i].height][white.pos[i].width] = white.fieldValue;

        for (int i = 0; i < black.count; i++)
            board[black.pos[i].height][black.pos[i].width] = black.fieldValue;
    }

    void makeMove (Field from, Field to, Field arrowTo)
    {
        if (board[from.height][from.width] == 1) //Check if its white
        {
            for (int i = 0; i < white.count; i++)
            {
                if (white.pos[i].height == from.height && white.pos[i].width == from.width)
                    white.pos[i] = to;
            }
        }
        else //If not, its black
        {
            for (int i = 0; i < black.count; i++)
            {
                if (black.pos[i].height == from.height && black.pos[i].width == from.width)
                    black.pos[i] = to;
            }
        }
        board[to.height][to.width] = board[from.height][from.width];
        board[from.height][from.width] = 0;
        board[arrowTo.height][arrowTo.width] = 3;
        whitePlays = !whitePlays;
    }

    bool checkDefeat (Amazons amazons)
    {
        for (int i = 0; i < amazons.count; i++)
        {
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    if (x == 0 && y == 0)
                        continue;

                    int tmpX = amazons.pos[i].height + x;
                    int tmpY = amazons.pos[i].width + y;

                    if (tmpX > -1 && tmpX < height && tmpY > -1 && tmpY < width)
                    {
                        if (board[tmpX][tmpY] == 0)
                            return false;
                    }
                }
            }
        }
        return true;
    }

    void generateRandomBoard ()
    {
        clearBoard();
        bool unique;

        for (int i = 0; i < white.count; i++) //Randomize white
        {
            unique = false;
            while(!unique)
            {
                int tmpX = rand() % height;
                int tmpY = rand() % width;

                if(board[tmpX][tmpY] == 0)
                {
                    board[tmpX][tmpY] = white.fieldValue;
                    white.pos[i].height = tmpX;
                    white.pos[i].width = tmpY;
                    unique = true;
                }
            }
        }
        for (int i = 0; i < black.count; i++) //Randomize black
        {
            unique = false;
            while(!unique)
            {
                int tmpX = rand() % height;
                int tmpY = rand() % width;

                if(board[tmpX][tmpY] == 0)
                {
                    board[tmpX][tmpY] = black.fieldValue;
                    black.pos[i].height = tmpX;
                    black.pos[i].width = tmpY;
                    unique = true;
                }
            }
        }
        int cap = (rand() % 8) + 1; //Randomize chance for arrow field - ...8) + 1 is default
        for (int x = 0; x < height; x++) //Randomize empty board fields
        {
            for (int y = 0; y < width; y++)
            {
                if (board[x][y] == 0)
                {
                    if(rand() % 11 < cap)
                    {
                        board[x][y] = 3;
                    }
                }
            }
        }
        whitePlays = true; 
    }

    int boardValue () //+ favors white, - favors black
    {
        bool movesPossible = true;
        int whiteMarker = 24;
        int blackMarker = 48;
        int neutralMarker = 4;
        std::vector<std::vector<Field>> whiteFields (1, std::vector<Field>());
        std::vector<std::vector<Field>> blackFields (1, std::vector<Field>());
        whiteFields[0] = white.pos;
        blackFields[0] = black.pos;
        int fieldsIndex = 0;
        int tmpX;
        int tmpY;
        Field tmpField;

        while (movesPossible)
        {
            for (int i = 0; i < whiteFields[fieldsIndex].size(); i++)
            {
                for (int x = -1; x < 2; x++)
                {
                    for (int y = -1; y < 2; y++)
                    {
                        if (x == 0 && y == 0)
                            continue;
                        tmpX = whiteFields[fieldsIndex][i].height + x;
                        tmpY = whiteFields[fieldsIndex][i].width + y;

                        while (tmpX > -1 && tmpX < height && tmpY > -1 && tmpY < width)
                        {
                            if (board[tmpX][tmpY] == 0 || board[tmpX][tmpY] == whiteMarker)
                            {
                                board[tmpX][tmpY] = whiteMarker;
                            }
                            else
                            {
                                break;
                            }                    
                            tmpX += x;
                            tmpY += y;
                        }
                    }
                }
            }
            for (int i = 0; i < blackFields[fieldsIndex].size(); i++)
            {
                for (int x = -1; x < 2; x++)
                {
                    for (int y = -1; y < 2; y++)
                    {
                        if (x == 0 && y == 0)
                            continue;
                        
                        tmpX = blackFields[fieldsIndex][i].height + x;
                        tmpY = blackFields[fieldsIndex][i].width + y;

                        while (tmpX > -1 && tmpX < height && tmpY > -1 && tmpY < width)
                        {
                            if (board[tmpX][tmpY] == 0 || board[tmpX][tmpY] == blackMarker)
                            {
                                board[tmpX][tmpY] = blackMarker;
                            }
                            else if (board[tmpX][tmpY] == whiteMarker || board[tmpX][tmpY] == neutralMarker)
                            {
                                board[tmpX][tmpY] = neutralMarker;
                            }
                            else
                            {
                                break;
                            }
                            tmpX += x;
                            tmpY += y;
                        }
                    }
                }
            }
            whiteFields.push_back(std::vector<Field>());
            blackFields.push_back(std::vector<Field>());
            fieldsIndex++;

            for (int x = 0; x < height; x++)
            {
                for (int y = 0; y < width; y++)
                {
                    if (board[x][y] == whiteMarker)
                    {
                        tmpField.height = x;
                        tmpField.width = y;
                        whiteFields[fieldsIndex].push_back(tmpField);
                    }
                    if (board[x][y] == blackMarker)
                    {
                        tmpField.height = x;
                        tmpField.width = y;
                        blackFields[fieldsIndex].push_back(tmpField);
                    }
                }
            }
            if (blackFields[fieldsIndex].size() == 0 && whiteFields[fieldsIndex].size() == 0)
            {
                movesPossible = false;
                break;
            }
            whiteMarker++;
            blackMarker++;
            neutralMarker++;
        }

        int whites = 0;
        int blacks = 0;
        for (int i = 1; i < whiteFields.size(); i++)
        {
            whites += whiteFields[i].size();
            blacks += blackFields[i].size();
        }  

        if (whites == 0)
            return std::numeric_limits<int>::min() + 1; //+1 is to make sure that minimax will make move (it has minValue set without +1)

        else if (blacks == 0)
            return std::numeric_limits<int>::max() - 1; //same for -1
                
        return whites - blacks;
    }
};

struct Possible_Moves //from[amazonFrom], to[amazonFrom][amazonTo], arrowTo[amazonFrom][amazonTo][arrowTo]
{
    std::vector<Field> from; //Positions of amazons
    std::vector<std::vector<Field>> to; //Positions of amazons after making move
    std::vector<std::vector<std::vector<Field>>> arrowTo; //Possitions of arrows
    Board board;

    Possible_Moves ()
    {        
    }

    Possible_Moves (Board board)
    {
        this -> board = board;
        int amazonCount;
        if (board.whitePlays)
        {
            amazonCount = board.white.count;
        }
        else
        {
            amazonCount = board.black.count; 
        }

        from.resize(amazonCount);
        for (int i = 0; i < amazonCount; i++)
        {
            to.push_back(std::vector<Field>(0));
            arrowTo.push_back(std::vector<std::vector<Field>>(0, std::vector<Field>(0)));
        }        
    }

    void getMoves ()
    {
        if (board.whitePlays)
        {
            from = board.white.pos;
        }
        else
        {
            from = board.black.pos;
        }        

        Field tmpTo;
        Field tmpArrowTo;

        int moveIndex; //Index of amazon move (second dimension)

        for (int i = 0; i < from.size(); i++) //Iterate through all amazons
        {
            board.board[from[i].height][from[i].width] = 0; //Make possible to shoot through/to currently occupied field
            moveIndex = 0; //Refresh counter (when switching amazon)

            for (int x = -1; x < 2; x++) //Iterate height modifier
            {
                for (int y = -1; y < 2; y++) //Iterate width modifier
                {
                    if(x == 0 && y == 0) //Handle no change exception
                        continue;
                    
                    tmpTo.height = from[i].height + x; //Get nearest field coordinates
                    tmpTo.width = from[i].width + y; //Get nearest field coordinates

                    while (tmpTo.height > -1 && tmpTo.height < board.height && tmpTo.width > -1 && tmpTo.width < board.width) //Iterate till out of bounds
                    {
                        if (board.board[tmpTo.height][tmpTo.width] == 0) //Check if field is empty
                        {          
                            to[i].push_back(tmpTo); //Add field as available for amazon to move to
                            arrowTo[i].push_back(std::vector<Field>()); //Expand second dimension of arrowTo vector size

                            for (int xx = -1; xx < 2; xx++) //Iterate height modifier for arrow
                            {
                                for (int yy = -1; yy < 2; yy++) //Iterate width modifier for arrow
                                {
                                    if(xx == 0 && yy == 0) //Handle no change exception
                                        continue;

                                    tmpArrowTo.height = tmpTo.height + xx; //Get nearest arrow field coordinates
                                    tmpArrowTo.width = tmpTo.width + yy; //Get nearest arrow field coordinates

                                    while (tmpArrowTo.height > -1 && tmpArrowTo.height < board.height && tmpArrowTo.width > -1 && tmpArrowTo.width < board.width) //Iterate till out of bounds
                                    {
                                        if (board.board[tmpArrowTo.height][tmpArrowTo.width] == 0) //Check if field is empty
                                        {
                                            arrowTo[i][moveIndex].push_back(tmpArrowTo); //Add field as available for amazon to shoot

                                            tmpArrowTo.height += xx; //Move to the next field in current direction (arrow shooting)
                                            tmpArrowTo.width += yy; //Move to the next field in current direction (arrow shooting)
                                        }
                                        else //If there is an obstacle at the examined field then change direction
                                        {
                                            break;
                                        }                                        
                                    }
                                }
                            }
                            tmpTo.height += x; //Move to the next field in current direction
                            tmpTo.width += y; //Move to the next field in current direction

                            moveIndex++; //Increment move index
                        }
                        else //If there is an obstacle at the examined field then change direction
                        {
                            break;
                        }                        
                    }
                }
            }
            if (board.whitePlays)
            {
                board.board[from[i].height][from[i].width] = 1; //Make possible to shoot through/to currently occupied field (change back)
            }
            else
            {
                board.board[from[i].height][from[i].width] = 2; //Make possible to shoot through/to currently occupied field (change back)
            }           
        }
    }

    int getMoveCount ()
    {
        int counter = 0;
        for (int i = 0; i < from.size(); i++)
        {
            for (int k = 0; k < to[i].size(); k++)
            {
                counter += arrowTo[i][k].size();
            }
        }
        return counter;
    }
};

//#################################################################################
//##############################----------------###################################
//##############################---ALGORITHMS---###################################
//##############################----------------###################################
//#################################################################################

int minimax (Board & board, int depth, bool maximizing)
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
                    int value = minimax(tmpBoard, depth - 1, false);
                    if (value > maxValue)
                    {
                        chosenMove[0] = a;
                        chosenMove[1] = b;
                        chosenMove[2] = c;
                        maxValue = value;
                    }
                }
            }
        }
        board.makeMove(playerMoves.from[chosenMove[0]], playerMoves.to[chosenMove[0]][chosenMove[1]], playerMoves.arrowTo[chosenMove[0]][chosenMove[1]][chosenMove[2]]);
        return maxValue;
    }
    else
    {
        int minValue = std::numeric_limits<int>::max();
        Possible_Moves enemyMoves (board);
        enemyMoves.getMoves();
        for (int a = 0; a < enemyMoves.from.size(); a++)
        {
            for (int b = 0; b < enemyMoves.to[a].size(); b++)
            {
                for (int c = 0; c < enemyMoves.arrowTo[a][b].size(); c++)
                {
                    Board tmpBoard = board;
                    tmpBoard.makeMove(enemyMoves.from[a], enemyMoves.to[a][b], enemyMoves.arrowTo[a][b][c]);
                    int value = minimax(tmpBoard, depth - 1, true);
                    if (value < minValue)
                    {
                        chosenMove[0] = a;
                        chosenMove[1] = b;
                        chosenMove[2] = c;
                        minValue = value;
                    }
                }
            }
        }
        board.makeMove(enemyMoves.from[chosenMove[0]], enemyMoves.to[chosenMove[0]][chosenMove[1]], enemyMoves.arrowTo[chosenMove[0]][chosenMove[1]][chosenMove[2]]);
        return minValue;
    }    
}

//#################################################################################
//#################################################################################
//#################################################################################

int negamax (Board & board, int depth, int sign) //white plays
{
    if (board.whitePlays)
    {
        if (depth == 0 || board.checkDefeat(board.white))
            return sign * board.boardValue();
    }
    else
    {
        if (depth == 0 || board.checkDefeat(board.black))
            return sign * board.boardValue();
    }

    int chosenMove [3];
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
                int value = -1 * negamax(tmpBoard, depth - 1, -1 * sign);
                if (value > maxValue)
                {
                    chosenMove[0] = a;
                    chosenMove[1] = b;
                    chosenMove[2] = c;
                    maxValue = value;
                }
            }
        }
    }
    board.makeMove(playerMoves.from[chosenMove[0]], playerMoves.to[chosenMove[0]][chosenMove[1]], playerMoves.arrowTo[chosenMove[0]][chosenMove[1]][chosenMove[2]]);
    return maxValue;  
}

//#################################################################################
//#################################################################################
//#################################################################################

int alphaBeta (Board & board, int depth, int alpha, int beta, bool maximizing)
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
                    int value = alphaBeta(tmpBoard, depth - 1, alpha, beta, false);
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
        return maxValue;
    }
    else
    {
        int minValue = std::numeric_limits<int>::max();
        Possible_Moves enemyMoves (board);
        enemyMoves.getMoves();
        for (int a = 0; a < enemyMoves.from.size(); a++)
        {
            for (int b = 0; b < enemyMoves.to[a].size(); b++)
            {
                for (int c = 0; c < enemyMoves.arrowTo[a][b].size(); c++)
                {
                    Board tmpBoard = board;
                    tmpBoard.makeMove(enemyMoves.from[a], enemyMoves.to[a][b], enemyMoves.arrowTo[a][b][c]);
                    int value = alphaBeta(tmpBoard, depth - 1, alpha, beta, true);
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
        board.makeMove(enemyMoves.from[chosenMove[0]], enemyMoves.to[chosenMove[0]][chosenMove[1]], enemyMoves.arrowTo[chosenMove[0]][chosenMove[1]][chosenMove[2]]);
        return minValue;
    }    
}

//#################################################################################
//#################################################################################
//#################################################################################

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
Dzięki temu przy ustaleniu limitu czasowego na 3 sekundy, ruch zostaje zazwyczaj wykonany w przeciągu 3-4 sekund.
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
                    int value = alphaBeta(tmpBoard, depth - 1, alpha, beta, false);
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
                    int value = alphaBeta(tmpBoard, depth - 1, alpha, beta, true);
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
            std::cout << "Searching depth: " << i << "\n";
            std::cout << "Elapsed time: " << elapsed.count() << " s\n";
            break;
        }
    }
    board.makeMove(bestMove[0], bestMove[1], bestMove[2]);
}

//#################################################################################
//#################################################################################
//#################################################################################

int alphaBetaIterativeDeepeingTimeLimit (Board & board, int depth, int alpha, int beta, bool maximizing, Field (&bestMove)[3], double timeLimit, std::chrono::time_point<std::chrono::high_resolution_clock> start, bool & outOfTime)
{
    if (outOfTime)
    {
        return 0;
    }

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
                    auto finish = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed = finish - start;
                    if (elapsed.count() > timeLimit)
                    {
                        outOfTime = true; //Out of time
                        return 0;
                    }

                    Board tmpBoard = board;
                    tmpBoard.makeMove(playerMoves.from[a], playerMoves.to[a][b], playerMoves.arrowTo[a][b][c]);
                    int value = alphaBeta(tmpBoard, depth - 1, alpha, beta, false);
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
                    auto finish = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed = finish - start;
                    if (elapsed.count() > timeLimit)
                    {
                        outOfTime = true; //Out of time
                        return 0;
                    }

                    Board tmpBoard = board;
                    tmpBoard.makeMove(playerMoves.from[a], playerMoves.to[a][b], playerMoves.arrowTo[a][b][c]);
                    int value = alphaBeta(tmpBoard, depth - 1, alpha, beta, true);
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

void iterativeDeepeningRelentlessTimeLimit (Board & board, int maxDepth, double timeLimit) //timeLimit is measured in seconds
{
    bool outOfTime = false; //Determine if the algorithm ran out of time. If it did, do not take its "bestMove" under consideration
    Field bestMove [3];
    Field moveToDo [3];
    auto start = std::chrono::high_resolution_clock::now(); //Get current time

    for (int i = 1; i <= maxDepth; i++)
    {
        Board tmpBoard = board;

        alphaBetaIterativeDeepeingTimeLimit(tmpBoard, i, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), board.whitePlays, bestMove, timeLimit, start, outOfTime);

        if (!outOfTime || i == 1)
        {
            moveToDo[0] = bestMove[0];
            moveToDo[1] = bestMove[1];
            moveToDo[2] = bestMove[2];
        }

        if (outOfTime || i == maxDepth)
        {
            std::cout << "Searching depth: " << i - 1 << "\n";

            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
            std::cout << "Elapsed time: " << elapsed.count() << " s\n";

            break;
        }
    }
    board.makeMove(moveToDo[0], moveToDo[1], moveToDo[2]);
}

//#################################################################################
//#################################################################################
//#################################################################################

void getCoordinates (std::string move, Field (&coordinates)[3])
{
    for (int i = 0; i < move.length(); i++)
    {
        int cord = 0;
        if (move[i] == ' ')
        {
            continue;
        }
        else
        {
            cord = move[i] - 48;
            std::cout << cord << "\n";
        }
        switch (i)
        {
            case 0:
                coordinates[0].height = cord;
                break;
            case 1:
                coordinates[0].width = cord;
                break;
            case 3:
                coordinates[1].height = cord;
                break;
            case 4:
                coordinates[1].width = cord;
                break;
            case 6:
                coordinates[2].height = cord;
                break;
            case 7:
                coordinates[2].width = cord;
                break;
            default:
                break;
        }
    }
}

void manualInputMove (Board & board)
{
    std::cout << "\nType your chosen move in this format: xy xy xy\nx - height, y - width. First - amazon pos, Second - amazon dest, Third - arrow\n";
    std::string move;
    getline (std::cin, move);
    Field coordinates [3];
    getCoordinates (move, coordinates);
    board.makeMove (coordinates[0], coordinates[1], coordinates[2]);
}

//#################################################################################
//#################################################################################
//#################################################################################

int getDepth (int moveCount) //Return depth of the search depending on available move count
{
    if (moveCount > 200)
        return 1;
    else if (moveCount > 35)
        return 2;
    else if (moveCount > 12)
        return 3;
    else if (moveCount > 6)
        return 4;
    return 5;
}

int getAlphaBetaDepth (int moveCount) //Return depth of the search depending on available move count (for alphaBeta search)
{
    if (moveCount > 350)
        return 1;
    else if (moveCount > 100)
        return 2;
    else if (moveCount > 40)
        return 3;
    else if (moveCount > 25)
        return 4;
    else if (moveCount > 15)
        return 5;
    return 6;
}

//#################################################################################
//#################################----------######################################
//#################################---MAIN---######################################
//#################################----------######################################
//#################################################################################

int main ()
{
    srand((unsigned) time(0)); //Seed random number generator

    Board board (10, 10, 4, 4, true);

    board.clearBoard();

    board.white.setClassic();
    board.black.setClassic();

    board.spawnAmazons();

    bool playing = true;
    int depth = 1;

    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    //ITERATIVE DEEPEING PRESET
    int maxDepth = 10;
    double timeLimit = 0.3;
    double relentlessTimeLimit = 3.0;

    //board.generateRandomBoard(white, black);      //Uncomment to start from a random generated board

    board.printBoard();

    while(playing)
    {
        //#################################################################################
        //#################################---WHITE---#####################################
        //#################################################################################

        //Possible_Moves moves(board);        //Get number of currently available moves (To determine search depth)
        //moves.getMoves();
        //int moveCount = moves.getMoveCount();

        //depth = getDepth(moveCount);      //Smaller depths for slower algorithms
        //depth = getAlphaBetaDepth(moveCount);       //Larger depths for faster algorithms (alphaBeta)
        //std::cout << "CURRENT DEPTH: " << depth << std::endl;

        //std::cout << moveCount << std::endl;      //Display currently available moves

        std::cout << "\nWHITE MAKES MOVE...\n";

        //std::cout << minimax(board, depth, true);         //WHIE PLAYER ALGORITHMS
        //std::cout << negamax(board, depth, 1);
        //alphaBeta(board, depth, alpha, beta, true);
        //iterativeDeepening (board, maxDepth, timeLimit);
        iterativeDeepeningRelentlessTimeLimit (board, maxDepth, relentlessTimeLimit);
        //manualInputMove (board);

        std::cout << "\n";

        board.printBoard();

        if(board.checkDefeat(board.black))
        {
            std::cout << "BLACK LOST" << std::endl;
            break;
        }

        //#################################################################################
        //#################################---BLACK---#####################################
        //#################################################################################

        std::cout << "\nBLACK MAKES MOVE...\n";

        //std::cout << minimax(board, depth, false);        //BLACK PLAYER ALGORITHMS
        //std::cout << negamax(board, depth, -1);
        //alphaBeta(board, depth, alpha, beta, false);
        //iterativeDeepening (board, maxDepth, timeLimit);
        iterativeDeepeningRelentlessTimeLimit (board, maxDepth, relentlessTimeLimit);
        //manualInputMove (board);

        std::cout << "\n";

        board.printBoard();

        if(board.checkDefeat(board.white))
        {
            std::cout << "WHITE LOST" << std::endl;
            break;
        }
    }
}