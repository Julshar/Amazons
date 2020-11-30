#include "Possible_Moves.h"

Possible_Moves::Possible_Moves ()
{        
}

Possible_Moves::Possible_Moves (Board board)
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

void Possible_Moves::getMoves ()
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

int Possible_Moves::getMoveCount ()
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