#include "Board.h"

Board::Board () : whitePlays(true)
{
}

Board::Board (int height, int width, int whiteCount, int blackCount, bool whitePlays) : height(height), width(width), 
board(height, std::vector<int>(width, 0)), white(1, whiteCount), black(2, blackCount), whitePlays(whitePlays)
{   
}

Board::Board (const Board & original) : height(original.height), width(original.width), board(original.board),
white(original.white), black(original.black), whitePlays(original.whitePlays)
{
}

void Board::clearBoard () //Fill the entire board with 0
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

void Board::printBoard () //Prints current state of the board into console
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
    std::cout << "\n";

    std::cout << "  " << (char)201; //Upper border line
    for (int i = 0; i < width; i++)
    {
        std::cout << (char)205 << (char)205 << (char)205;
    }
    std::cout << (char)187 << "\n";

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
        std::cout << (char)186 << " " << i << "\n";
    }

    std::cout << "  " << (char)200; //Bottom border line
    for (int i = 0; i < width; i++)
    {
        std::cout << (char)205 << (char)205 << (char)205;
    }
    std::cout << (char)188 << "\n";

    std::cout << "    "; //Botttom numeration
    for (int i = 0; i < width; i++)
    {
        std::cout << i << "  ";
    }
    std::cout << "\n";
}

void Board::spawnAmazons () //Place amazons on the board, according to their positions described by Amazons::pos
{
    for (int i = 0; i < white.count; i++)
        board[white.pos[i].height][white.pos[i].width] = white.fieldValue;

    for (int i = 0; i < black.count; i++)
        board[black.pos[i].height][black.pos[i].width] = black.fieldValue;
}

void Board::makeMove (Field from, Field to, Field arrowTo)
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

bool Board::checkDefeat (Amazons amazons)
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

void Board::generateRandomBoard ()
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
    int cap = (rand() % 2) + 7; //Randomize chance for arrow field - ...8) + 1 is default
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

int Board::boardValue () //+ favors white, - favors black
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

bool Board::checkBlocks (Amazons amazonColor, int blockLimit)
{
    int preCounter = 0;
    int postCounter = 0;

    Amazons * color;

    if (amazonColor.fieldValue == 1)
    {
        color = &white;
    }
    else
    {
        color = &black;
    }

    for (int i = 0; i < color->count; i++)
    {
        if (color->blocked[i] == true)
        {
            preCounter++;
        }

        int availableMoves = 0;
        color -> blocked[i] = true;
        Board tmpBoard = * this;
        bool canMove = true;
        std::vector <Field> freeFields;
        std::vector <Field> nextFree (1, color -> pos[i]);

        while (canMove)
        {
            freeFields = nextFree;
            nextFree.clear();
            for (int fld = 0; fld < freeFields.size(); fld++)
            {
                for (int x = -1; x < 2; x++)
                {
                    for (int y = -1; y < 2; y++)
                    {
                        if (x == 0 && y == 0)
                            continue;
                        
                        int tmpX = freeFields[fld].height+x;
                        int tmpY = freeFields[fld].width+y;

                        while (tmpX > -1 && tmpX < height && tmpY > -1 && tmpY < width)
                        {
                            if (tmpBoard.board[tmpX][tmpY] == 0)
                            {
                                availableMoves++;
                                if (availableMoves > blockLimit)
                                {
                                    color->blocked[i] = false;
                                    break;
                                }
                                tmpBoard.board[tmpX][tmpY] = color -> fieldValue; //When field is empty, have to seek from it further
                                Field tmpField;
                                tmpField.height = tmpX;
                                tmpField.width = tmpY;
                                nextFree.push_back(tmpField);
                            }
                            else
                            {
                                break;
                            }            
                            tmpX += x;
                            tmpY += y;
                        }
                        if (color->blocked[i] == false)
                            break;
                    }
                    if (color->blocked[i] == false)
                        break;
                }
                if (color->blocked[i] == false)
                        break;
            }
            if (color->blocked[i] == false)
                break;
            if (freeFields.size() == 0)
                canMove = false;
        }
        if (color -> blocked[i] == true)
            postCounter++;
    }
    return (postCounter > preCounter) ? true : false;
}

int getDepth (int moveCount)
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

int getAlphaBetaDepth (int moveCount)
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