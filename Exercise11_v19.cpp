#include <cstdlib>
#include <iostream>
#include "battleship.h"
#include <iomanip>
//#define debugging  // de-comment this to enable debugging cout statements.
//#define printGrid  // de-comment this to enable printing of grid each turn.
//#define searchDebugging // de-comment to enable printing of the SEARCH algorithm.
//#define printMyOcean
//#define placeDebugging
using namespace std;

// State of program:  Currently working to further optimize search algorithm.
// Bug in Destroyer ship placement.
// Need to rewrite fire function to call a separate tracking function and
// return coords to fire, instead of repeating the same code 5 times.

char intRowToChar(int rowDec)
{
    char row;

    row = rowDec + 'A';

    return row;
}

int charRowToInt(char rowChar)
{
    int row;

    row = rowChar - 'A';

    return row;
}

bool leftClear(int track[BS_GRID_ROWS][BS_GRID_COLS], int row, int col)
{
    bool isClear = false;
    if (col > 0)
    {
        if ((track[row][col - 1]) != 0)
            isClear = false;
        else
            isClear = true;
    }
    else
        isClear = false;
    return isClear;
}

bool rightClear(int track[BS_GRID_ROWS][BS_GRID_COLS], int row, int col)
{
    bool isClear = false;
    if (col < BS_GRID_COLS - 1)
    {
        if ((track[row][col + 1]) != 0)
            isClear = false;
        else
            isClear = true;
    }
    else
        isClear = false;
    return isClear;
}

bool topClear(int track[BS_GRID_ROWS][BS_GRID_COLS], int row, int col)
{
    bool isClear = false;
    if (row > 0)
    {
        if ((track[row - 1][col]) != 0)
            isClear = false;
        else
            isClear = true;
    }
    else
        isClear = false;
    return isClear;
}

bool botClear(int track[BS_GRID_ROWS][BS_GRID_COLS], int row, int col)
{
    bool isClear = false;
    if (row < BS_GRID_ROWS - 1)
    {
        if ((track[row + 1][col]) != 0)
            isClear = false;
        else
            isClear = true;
    }
    else
        isClear = false;
    return isClear;
}

int
scafidiI
(char row, int col)
{
    // Define your static data structures here such as your grid and any status
// variables you need.
    int returnCode = 0;
    int count1 = 0, rowStart = 0, colStart = 0, rowCalled, colCalled,
        searchCount=0;
    static int grid[BS_GRID_ROWS][BS_GRID_COLS];

    static int carrierHits = 0, battleshipHits = 0,
               cruiserHits = 0, submarineHits = 0, destroyerHits = 0;
    bool vert = false, occupied = false;

        colCalled = col - 1;
        rowCalled = row - 'A';

        // Starting new game
        if (row == '\0' && col == 0)
        {
            // need to initialize your grid, ship placements, etc
            int i = 0, j = 0;
            destroyerHits = 0;
            submarineHits = 0;
            cruiserHits = 0;
            battleshipHits = 0;
            carrierHits = 0;
            for (i = 0; i < BS_GRID_COLS; i++)
            {
                for (j = 0; j < BS_GRID_ROWS; j++)
                {
                    grid[i][j] = 0;
                }
            }


            // place Carrier ===============================================================================
            // Generate a start place and search to see if there is space.
            // decide horizontal or vertical.

            do
            {
                #ifdef placeDebugging
                cout << "Placing the Carrier." << endl;
                #endif // debugging
                occupied = false;
                count1 = 0;
                searchCount = 0;
                vert = rand() % 2;
                if (vert)
                {
                    rowStart = rand() % BS_CARRIER_SIZE;
                    colStart = rand() % BS_GRID_COLS;
                }
                else
                {
                    rowStart = rand() % BS_GRID_ROWS;
                    colStart = rand() % BS_CARRIER_SIZE;
                }
                while (count1 < BS_CARRIER_SIZE && !occupied)
                {
                    if (vert)
                    {
                        if (grid[rowStart + searchCount][colStart])
                        {
                            occupied = true;
                        }
                        else
                        {
                            searchCount++;
                        }
                    }
                    else
                    {
                        if (grid[rowStart][colStart + searchCount])
                            occupied = true;
                        else
                        {
                            searchCount++;
                        }
                    }
                    count1++;
                }
                // Now search around the ship to make sure it's not touching any other ship.
                searchCount = 0;
                for (int i = 0; i <= BS_CARRIER_SIZE && !occupied; i++)
                {
                    if (vert)
                    {
                        if(searchCount == 0 && rowStart != 0)
                        {
                            if (topClear(grid, rowStart, colStart))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (colStart != 0 && colStart != BS_GRID_COLS - 1)
                        {
                            if ((leftClear(grid, (rowStart + searchCount), colStart)) && (rightClear(grid, (rowStart + searchCount), colStart)))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (colStart == 0)
                        {
                            if ((rightClear(grid, (rowStart + searchCount), colStart)))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (colStart == BS_GRID_COLS - 1)
                        {
                            if ((leftClear(grid, (rowStart + searchCount), colStart)))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        if (searchCount == BS_CARRIER_SIZE && (rowStart + searchCount) != BS_GRID_ROWS - 1)
                        {
                            if (!(botClear(grid, (rowStart + searchCount), colStart)))
                            {
                                occupied = true;
                            }
                        }
                    }
                    else
                    {
                        if(searchCount == 0 && colStart != 0)
                        {
                            if (leftClear(grid, rowStart, colStart))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (rowStart != 0 && rowStart != BS_GRID_ROWS - 1)
                        {
                            if ((topClear(grid, rowStart, (colStart + searchCount))) && (botClear(grid, rowStart, (colStart + searchCount))))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (rowStart == 0)
                        {
                            if ((botClear(grid, rowStart, (colStart + searchCount))))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (rowStart == BS_GRID_ROWS - 1)
                        {
                            if ((topClear(grid, rowStart, (colStart + searchCount))))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        if (searchCount == BS_CARRIER_SIZE && (colStart + searchCount) != BS_GRID_COLS - 1)
                        {
                            if (!(rightClear(grid, rowStart, (colStart + searchCount))))
                            {
                                occupied = true;
                            }
                        }
                    }
                } // end of for loop
            } // end of do-while loop.
            while (occupied);
            count1 = 0;
            while(count1 < BS_CARRIER_SIZE)
            {
                if (vert)
                {
                    grid[rowStart + count1][colStart] = BS_CARRIER;
                    count1++;
                }
                else
                {
                    grid[rowStart][colStart + count1] = BS_CARRIER;
                    count1++;
                }

            }
            // Now place the Battleship.
            // ======================================================================================

            do
            {
                #ifdef placeDebugging
                cout << "Placing the battleship." << endl;
                #endif // debugging
                occupied = false;
                count1 = 0;
                searchCount = 0;
                vert = rand() % 2;
                if (vert)
                {
                    rowStart = rand() % BS_BATTLESHIP_SIZE;
                    colStart = rand() % BS_GRID_COLS;
                }
                else
                {
                    rowStart = rand() % BS_GRID_ROWS;
                    colStart = rand() % BS_BATTLESHIP_SIZE;
                }
                while (count1 < BS_BATTLESHIP_SIZE && !occupied)
                {
                    if (vert)
                    {
                        if (grid[rowStart + searchCount][colStart])
                        {
                            occupied = true;
                        }
                        else
                        {
                            searchCount++;
                        }
                    }
                    else
                    {
                        if (grid[rowStart][colStart + searchCount])
                            occupied = true;
                        else
                        {
                            searchCount++;
                        }
                    }
                    count1++;
                }
                // Now search around the ship to make sure it's not touching any other ship.
                searchCount = 0;
                for (int i = 0; i <= BS_BATTLESHIP_SIZE && !occupied; i++)
                {
                    if (vert)
                    {
                        if(searchCount == 0 && rowStart != 0)
                        {
                            if (topClear(grid, rowStart, colStart))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (colStart != 0 && colStart != BS_GRID_COLS - 1)
                        {
                            if ((leftClear(grid, (rowStart + searchCount), colStart)) && (rightClear(grid, (rowStart + searchCount), colStart)))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (colStart == 0)
                        {
                            if ((rightClear(grid, (rowStart + searchCount), colStart)))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (colStart == BS_GRID_COLS - 1)
                        {
                            if ((leftClear(grid, (rowStart + searchCount), colStart)))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        if (searchCount == BS_BATTLESHIP_SIZE && (rowStart + searchCount) != BS_GRID_ROWS - 1)
                        {
                            if (!(botClear(grid, (rowStart + searchCount), colStart)))
                            {
                                occupied = true;
                            }
                        }
                    }
                    else
                    {
                        if(searchCount == 0 && colStart != 0)
                        {
                            if (leftClear(grid, rowStart, colStart))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (rowStart != 0 && rowStart != BS_GRID_ROWS - 1)
                        {
                            if ((topClear(grid, rowStart, (colStart + searchCount))) && (botClear(grid, rowStart, (colStart + searchCount))))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (rowStart == 0)
                        {
                            if ((botClear(grid, rowStart, (colStart + searchCount))))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (rowStart == BS_GRID_ROWS - 1)
                        {
                            if ((topClear(grid, rowStart, (colStart + searchCount))))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        if (searchCount == BS_BATTLESHIP_SIZE && (colStart + searchCount) != BS_GRID_COLS - 1)
                        {
                            if (!(rightClear(grid, rowStart, (colStart + searchCount))))
                            {
                                occupied = true;
                            }
                        }
                    }
                } // end of for loop
            } // end of do-while loop.
            while (occupied);
            count1 = 0;
            while(count1 < BS_BATTLESHIP_SIZE)
            {

                if (vert)
                {
                    grid[rowStart + count1][colStart] = BS_BATTLESHIP;
                    count1++;
                }
                else
                {
                    grid[rowStart][colStart + count1] = BS_BATTLESHIP;
                    count1++;
                }

            }

            // Now place the cruiser
            //=================================================================================

            do
            {
                #ifdef placeDebugging
                cout << "Placing the cruiser." << endl;
                #endif // debugging
                occupied = false;
                count1 = 0;
                searchCount = 0;
                vert = rand() % 2;
                if (vert)
                {
                    rowStart = rand() % BS_CRUISER_SIZE;
                    colStart = rand() % BS_GRID_COLS;
                }
                else
                {
                    rowStart = rand() % BS_GRID_ROWS;
                    colStart = rand() % BS_CRUISER_SIZE;
                }
                while (count1 < BS_CRUISER_SIZE && !occupied)
                {
                    if (vert)
                    {
                        if (grid[rowStart + searchCount][colStart])
                        {
                            occupied = true;
                        }
                        else
                        {
                            searchCount++;
                        }
                    }
                    else
                    {
                        if (grid[rowStart][colStart + searchCount])
                            occupied = true;
                        else
                        {
                            searchCount++;
                        }
                    }
                    count1++;
                }
                // Now search around the ship to make sure it's not touching any other ship.
                searchCount = 0;
                for (int i = 0; i <= BS_CRUISER_SIZE && !occupied; i++)
                {
                    if (vert)
                    {
                        if(searchCount == 0 && rowStart != 0)
                        {
                            if (topClear(grid, rowStart, colStart))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (colStart != 0 && colStart != BS_GRID_COLS - 1)
                        {
                            if ((leftClear(grid, (rowStart + searchCount), colStart)) && (rightClear(grid, (rowStart + searchCount), colStart)))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (colStart == 0)
                        {
                            if ((rightClear(grid, (rowStart + searchCount), colStart)))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (colStart == BS_GRID_COLS - 1)
                        {
                            if ((leftClear(grid, (rowStart + searchCount), colStart)))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        if (searchCount == BS_CRUISER_SIZE && (rowStart + searchCount) != BS_GRID_ROWS - 1)
                        {
                            if (!(botClear(grid, (rowStart + searchCount), colStart)))
                            {
                                occupied = true;
                            }
                        }
                    }
                    else
                    {
                        if(searchCount == 0 && colStart != 0)
                        {
                            if (leftClear(grid, rowStart, colStart))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (rowStart != 0 && rowStart != BS_GRID_ROWS - 1)
                        {
                            if ((topClear(grid, rowStart, (colStart + searchCount))) && (botClear(grid, rowStart, (colStart + searchCount))))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (rowStart == 0)
                        {
                            if ((botClear(grid, rowStart, (colStart + searchCount))))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (rowStart == BS_GRID_ROWS - 1)
                        {
                            if ((topClear(grid, rowStart, (colStart + searchCount))))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        if (searchCount == BS_CRUISER_SIZE && (colStart + searchCount) != BS_GRID_COLS - 1)
                        {
                            if (!(rightClear(grid, rowStart, (colStart + searchCount))))
                            {
                                occupied = true;
                            }
                        }
                    }
                } // end of for loop
            } // end of do-while loop.
            while (occupied);
            count1 = 0;
            while(count1 < BS_CRUISER_SIZE)
            {

                if (vert)
                {
                    grid[rowStart + count1][colStart] = BS_CRUISER;
                    count1++;
                }
                else
                {
                    grid[rowStart][colStart + count1] = BS_CRUISER;
                    count1++;
                }

            }



           // Now place the submarine.
           //======================================================================================

           do
            {
                #ifdef placeDebugging
                cout << "Placing the submarine." << endl;
                #endif // debugging
                occupied = false;
                count1 = 0;
                searchCount = 0;
                vert = rand() % 2;
                if (vert)
                {
                    rowStart = rand() % BS_SUBMARINE_SIZE;
                    colStart = rand() % BS_GRID_COLS;
                }
                else
                {
                    rowStart = rand() % BS_GRID_ROWS;
                    colStart = rand() % BS_SUBMARINE_SIZE;
                }
                while (count1 < BS_SUBMARINE_SIZE && !occupied)
                {
                    if (vert)
                    {
                        if (grid[rowStart + searchCount][colStart])
                        {
                            occupied = true;
                        }
                        else
                        {
                            searchCount++;
                        }
                    }
                    else
                    {
                        if (grid[rowStart][colStart + searchCount])
                            occupied = true;
                        else
                        {
                            searchCount++;
                        }
                    }
                    count1++;
                }
                // Now search around the ship to make sure it's not touching any other ship.
                searchCount = 0;
                for (int i = 0; i <= BS_SUBMARINE_SIZE && !occupied; i++)
                {
                    if (vert)
                    {
                        if(searchCount == 0 && rowStart != 0)
                        {
                            if (topClear(grid, rowStart, colStart))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (colStart != 0 && colStart != BS_GRID_COLS - 1)
                        {
                            if ((leftClear(grid, (rowStart + searchCount), colStart)) && (rightClear(grid, (rowStart + searchCount), colStart)))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (colStart == 0)
                        {
                            if ((rightClear(grid, (rowStart + searchCount), colStart)))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (colStart == BS_GRID_COLS - 1)
                        {
                            if ((leftClear(grid, (rowStart + searchCount), colStart)))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        if (searchCount == BS_SUBMARINE_SIZE && (rowStart + searchCount) != BS_GRID_ROWS - 1)
                        {
                            if (!(botClear(grid, (rowStart + searchCount), colStart)))
                            {
                                occupied = true;
                            }
                        }
                    }
                    else
                    {
                        if(searchCount == 0 && colStart != 0)
                        {
                            if (leftClear(grid, rowStart, colStart))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (rowStart != 0 && rowStart != BS_GRID_ROWS - 1)
                        {
                            if ((topClear(grid, rowStart, (colStart + searchCount))) && (botClear(grid, rowStart, (colStart + searchCount))))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (rowStart == 0)
                        {
                            if ((botClear(grid, rowStart, (colStart + searchCount))))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        else if (rowStart == BS_GRID_ROWS - 1)
                        {
                            if ((topClear(grid, rowStart, (colStart + searchCount))))
                            {
                                searchCount++;
                            }
                            else
                            {
                                occupied = true;
                            }
                        }
                        if (searchCount == BS_SUBMARINE_SIZE && (colStart + searchCount) != BS_GRID_COLS - 1)
                        {
                            if (!(rightClear(grid, rowStart, (colStart + searchCount))))
                            {
                                occupied = true;
                            }
                        }
                    }
                } // end of for loop
            } // end of do-while loop.
            while (occupied);
            count1 = 0;

            while(count1 < BS_SUBMARINE_SIZE)
            {

                if (vert)
                {
                    grid[rowStart + count1][colStart] = BS_SUBMARINE;
                    count1++;
                }
                else
                {
                    grid[rowStart][colStart + count1] = BS_SUBMARINE;
                    count1++;
                }

            }

            // Now place the destroyer
            // ======================================================================================

            do
            {
                #ifdef placeDebugging
                cout << "Placing the Destroyer." << endl;
                #endif // debugging
                occupied = false;
                count1 = 0;
                searchCount = 0;
                #ifdef placeDebugging
                            cout << "SearchCount is " << searchCount << endl;
                            #endif // placeDebugging
                vert = rand() % 2;
                if (vert)
                {
                    rowStart = rand() % BS_DESTROYER_SIZE;
                    colStart = rand() % BS_GRID_COLS;
                }
                else
                {
                    rowStart = rand() % BS_GRID_ROWS;
                    colStart = rand() % BS_DESTROYER_SIZE;
                }
                #ifdef placeDebugging
                cout << "Attempting to place Destroyer at " << intRowToChar(rowStart) << (colStart + 1) << endl;
                cout << " Vert is " << boolalpha << vert << endl;
                cout << "Reading " << grid[rowStart][colStart] << " in this position." << endl;
                #endif // placeDebugging
                while (count1 < BS_DESTROYER_SIZE && !occupied)
                {
                    if (vert)
                    {
                        if (grid[rowStart + searchCount][colStart] != 0)
                        {
                            occupied = true;
                            #ifdef placeDebugging
                            cout << "Marked True ID1" << endl;
                            #endif // placeDebugging
                        }
                        else
                        {
                            searchCount++;
                            #ifdef placeDebugging
                            cout << "SearchCount is " << searchCount << endl;
                            #endif // placeDebugging
                        }
                    }
                    else
                    {
                        if (grid[rowStart][colStart + searchCount] != 0)
                        {
                            occupied = true;
                            #ifdef placeDebugging
                            cout << "Marked True ID2" << endl;
                            #endif // placeDebugging
                        }
                        else
                        {
                            searchCount++;
                            #ifdef placeDebugging
                            cout << "SearchCount is " << searchCount << endl;
                            #endif // placeDebugging
                        }
                    }
                    count1++;
                }/*
                // Now search around the ship to make sure it's not touching any other ship.
                searchCount = 0;
                for (int i = 0; i <= BS_DESTROYER_SIZE && !occupied; i++)
                {
                    if (vert)
                    {
                        if(searchCount == 0 && rowStart != 0)
                        {
                            if (topClear(grid, rowStart, colStart))
                            {
                                searchCount++;
                                #ifdef placeDebugging
                            cout << "SearchCount is " << searchCount << endl;
                            #endif // placeDebugging
                            }
                            else
                            {
                                #ifdef placeDebugging
                                cout << "Marked True ID3" << endl;
                                #endif // placeDebugging
                                occupied = true;
                            }
                        }
                        else if (colStart != 0 && colStart != BS_GRID_COLS - 1 && searchCount < BS_DESTROYER_SIZE)
                        {
                            if ((leftClear(grid, (rowStart + searchCount), colStart)) && (rightClear(grid, (rowStart + searchCount), colStart)))
                            {
                                searchCount++;
                                #ifdef placeDebugging
                            cout << "SearchCount is " << searchCount << endl;
                            #endif // placeDebugging
                            }
                            else
                            {
                                occupied = true;
                                #ifdef placeDebugging
                                cout << "Marked True ID4" << endl;
                                #endif // placeDebugging
                            }
                        }
                        else if (colStart == 0)
                        {
                            if ((rightClear(grid, (rowStart + searchCount), colStart)))
                            {
                                searchCount++;
                                #ifdef placeDebugging
                            cout << "SearchCount is " << searchCount << endl;
                            #endif // placeDebugging
                            }
                            else
                            {
                                occupied = true;
                                #ifdef placeDebugging
                                cout << "Marked True ID5" << endl;
                                #endif // placeDebugging
                            }
                        }
                        else if (colStart == BS_GRID_COLS - 1)
                        {
                            if ((leftClear(grid, (rowStart + searchCount), colStart)))
                            {
                                searchCount++;
                                #ifdef placeDebugging
                            cout << "SearchCount is " << searchCount << endl;
                            #endif // placeDebugging
                            }
                            else
                            {
                                occupied = true;
                                #ifdef placeDebugging
                                cout << "Marked True ID6" << endl;
                                #endif // placeDebugging
                            }
                        }
                        if (searchCount == BS_DESTROYER_SIZE && (rowStart + searchCount) != BS_GRID_ROWS - 1)
                        {
                            if (!(botClear(grid, (rowStart + searchCount), colStart)))
                            {
                                occupied = true;
                                #ifdef placeDebugging
                                cout << "Marked True ID7" << endl;
                                #endif // placeDebugging
                            }
                        }
                    }
                    else
                    {
                        if(searchCount == 0 && colStart != 0)
                        {
                            if (leftClear(grid, rowStart, colStart))
                            {
                                searchCount++;
                                #ifdef placeDebugging
                            cout << "SearchCount is " << searchCount << endl;
                            #endif // placeDebugging
                            }
                            else
                            {
                                occupied = true;
                                #ifdef placeDebugging
                                cout << "Marked True ID8" << endl;
                                #endif // placeDebugging
                            }
                        }
                        else if (rowStart != 0 && rowStart != BS_GRID_ROWS - 1)
                        {
                            if ((topClear(grid, rowStart, (colStart + searchCount))) && (botClear(grid, rowStart, (colStart + searchCount))))
                            {
                                searchCount++;
                                #ifdef placeDebugging
                            cout << "SearchCount is " << searchCount << endl;
                            #endif // placeDebugging
                            }
                            else
                            {
                                occupied = true;
                                #ifdef placeDebugging
                                cout << "Marked True ID9" << endl;
                                #endif // placeDebugging
                            }
                        }
                        else if (rowStart == 0)
                        {
                            if ((botClear(grid, rowStart, (colStart + searchCount))))
                            {
                                searchCount++;
                                #ifdef placeDebugging
                            cout << "SearchCount is " << searchCount << endl;
                            #endif // placeDebugging
                            }
                            else
                            {
                                occupied = true;
                                #ifdef placeDebugging
                                cout << "Marked True ID10" << endl;
                                #endif // placeDebugging
                            }
                        }
                        else if (rowStart == BS_GRID_ROWS - 1)
                        {
                            if ((topClear(grid, rowStart, (colStart + searchCount))))
                            {
                                searchCount++;
                                #ifdef placeDebugging
                            cout << "SearchCount is " << searchCount << endl;
                            #endif // placeDebugging
                            }
                            else
                            {
                                occupied = true;
                                #ifdef placeDebugging
                                cout << "Marked True ID11" << endl;
                                #endif // placeDebugging
                            }
                        }
                        if (searchCount == BS_DESTROYER_SIZE && (colStart + searchCount) != BS_GRID_COLS - 1)
                        {
                            if (!(rightClear(grid, rowStart, (colStart + searchCount))))
                            {
                                occupied = true;
                                #ifdef placeDebugging
                                cout << "Marked True ID12" << endl;
                                #endif // placeDebugging
                            }
                        }
                    }
                }
                #ifdef placeDebugging
                cout << "=================================" << endl << endl;
                cout << endl << "C: ";
                for (int i = 0; i < BS_GRID_ROWS; i++)
                {
                    cout << setw(2) << (i + 1);
                }
                for (int i = 0; i < BS_GRID_ROWS; i++)
                {
                    cout << endl << static_cast<char>('A' + i) << ": ";
                    for (int j = 0; j < BS_GRID_COLS; j++)
                    {
                        cout <<  setw(2) << grid[i][j];
                    }
                }
                cout << endl; // end of for loop
                #endif
                */
            } // end of do-while loop.
            while (occupied);
            count1 = 0;

            while(count1 < BS_DESTROYER_SIZE)
            {

                if (vert)
                {
                    grid[rowStart + count1][colStart] = BS_DESTROYER;
                    count1++;
                }
                else
                {
                    grid[rowStart][colStart + count1] = BS_DESTROYER;
                    count1++;
                }

            }
        }
        else if (toupper(row) < 'A' || toupper(row) > 'J' || col < 1 || col > BS_GRID_COLS)
        {
            return 0;
        }
        else
        {
            if (grid[rowCalled][colCalled] != 0)
            {
                returnCode |= grid[rowCalled][colCalled];
                if (grid[rowCalled][colCalled] == BS_CARRIER)
                {
                    carrierHits += 1;
                    returnCode |= BS_SHIP_HIT;
                    returnCode |= BS_CARRIER;
                    if (carrierHits >= BS_CARRIER_SIZE)
                        returnCode |= BS_SHIP_SANK;
                }
                else if (grid[rowCalled][colCalled] == BS_BATTLESHIP)
                {
                    battleshipHits += 1;
                    returnCode |= BS_SHIP_HIT;
                    returnCode |= BS_BATTLESHIP;
                    if (battleshipHits >= BS_BATTLESHIP_SIZE)
                        returnCode |= BS_SHIP_SANK;
                }
                else if (grid[rowCalled][colCalled] == BS_CRUISER)
                {
                    cruiserHits += 1;
                    returnCode |= BS_SHIP_HIT;
                    returnCode |= BS_CRUISER;
                    if (cruiserHits >= BS_CRUISER_SIZE)
                        returnCode |= BS_SHIP_SANK;
                }
                else if (grid[rowCalled][colCalled] == BS_SUBMARINE)
                {
                    submarineHits += 1;
                    returnCode |= BS_SHIP_HIT;
                    returnCode |= BS_SUBMARINE;
                    if (submarineHits >= BS_SUBMARINE_SIZE)
                        returnCode |= BS_SHIP_SANK;
                }
                else if (grid[rowCalled][colCalled] == BS_DESTROYER)
                {
                    destroyerHits += 1;
                    returnCode |= BS_SHIP_HIT;
                    returnCode |= BS_DESTROYER;
                    if (destroyerHits >= BS_DESTROYER_SIZE)
                        returnCode |= BS_SHIP_SANK;
                }


            // Check: if it is a hit, update the return code
            //  with the proper values
            }
            else
            {
                returnCode = 0;
            }


            // If open water, then leave the return code as 0
        }
        #ifdef printMyOcean
        cout << "=================================" << endl << endl;
        cout << endl << "C: ";
        for (int i = 0; i < BS_GRID_ROWS; i++)
        {
            cout << setw(2) << (i + 1);
        }
        for (int i = 0; i < BS_GRID_ROWS; i++)
        {
            cout << endl << static_cast<char>('A' + i) << ": ";
            for (int j = 0; j < BS_GRID_COLS; j++)
            {
                cout <<  setw(2) << grid[i][j];
            }
        }
        cout << endl;
        #endif // printOcean
return returnCode;
}


int
scafidiA
(int mode, int opponent)
{
/** THIS VERSION is currently working to optimize the Search algorithm.
/
/This algorithm searches for ships using two methods.
/ The first method I call "Search mode."  In this mode, the algorithm
/ calculates density of each row and column, and finds the most likely location
/ of a ship based on the number of previous attempts in that row or column.
/ It then calculates the average the median and fires on that spot.  The process
/ continues until it finds a ship.
/ Upon locating a ship, the algorithm enters "Destroy mode".
/ The algorithm searches in the spaces adjacent to the hit
/ and after the second hit, it determines if the ship is oriented vertical or horizontal.
/ Once the orientation is determined, it fires down a line until the ship is sunk.
/ The process repeats for every ship until all 5 are sunk.
**/

    // static variables are all declared here, along with the 2D array that will
    // keep track of where we have fired, and where ships are located.
    static bool hit = false, tracking1 = false, tracking2 = false,
                tracking3 = false, tracking4 = false, tracking5 = false, vert = false,
                carSank, batSank, cruSank, subSank, destSank, adjShipFnd = false, firstTurn = true;
    static int rowGen, column, trackAtmpt, origRow, origCol, ret, adjShipRow, adjShipCol, shipID,
               turn, numHits;
    bool pickNew = false;
    char row;
    int i = 0, j = 0;
    static int tracker[BS_GRID_ROWS][BS_GRID_COLS] = {{0}};

    switch(mode)
    {
        case BS_MODE_NEW_GAME:  // In a new game, the tracking array is reinitialized to all 0s.
            #ifdef placeDebugging
            cout << endl;
            cout << "><><><><><><><> *** NEW GAME *** NEW GAME *** NEW GAME *** -<><><><>" << endl;
            cout << "><><><><><><><> *** NEW GAME *** NEW GAME *** NEW GAME *** -<><><><>" << endl;
            cout << "><><><><><><><> *** NEW GAME *** NEW GAME *** NEW GAME *** -<><><><>" << endl;
            cout << "><><><><><><><> *** NEW GAME *** NEW GAME *** NEW GAME *** -<><><><>" << endl;
            cout << "><><><><><><><> *** NEW GAME *** NEW GAME *** NEW GAME *** -<><><><>" << endl;
            cout << "><><><><><><><> *** NEW GAME *** NEW GAME *** NEW GAME *** -<><><><>" << endl;
            cout << "><><><><><><><> *** NEW GAME *** NEW GAME *** NEW GAME *** -<><><><>" << endl;
            #endif // placeDebugging
            for (i = 0; i < BS_GRID_ROWS; i++)
            {
                for (j = 0; j < BS_GRID_COLS; j++)
                {
                    tracker[i][j] = 0;
                }
            }
            // Tracking variables are also reset to their appropriate values.
            firstTurn = true;
            hit = false;
            tracking1 = false;
            tracking2 = false;
            tracking3 = false;
            tracking4 = false;
            tracking5 = false;
            pickNew = false;
            vert = false;
            carSank = false;
            batSank = false;
            cruSank = false;
            subSank = false;
            destSank = false;
            adjShipCol = 0;
            adjShipRow = 0;
            adjShipFnd = false;
            numHits = 0;
            turn = 0;
            break;
        case BS_MODE_CONTINUE_GAME:
            turn++;
            #ifdef debugging
            cout << boolalpha;
            cout << "hit is now " << hit << endl;
            #endif // debugging
            // Enter "search" or "destroy" mode based on previous hit/miss.
            // The first turn picks a random location and fires.
            if (firstTurn)
            {
                #ifdef debugging
                cout << "Now in first turn." << endl;
                #endif // debugging
                hit = false;
                rowGen = (rand() % BS_GRID_ROWS);
                column = (rand() % BS_GRID_COLS);
                row = intRowToChar(rowGen);
                firstTurn = false;
                trackAtmpt = 0;
                ret = fire[opponent](row, (column + 1));
                #ifdef debugging
                cout << "Just fired on " << row << (column + 1) << "." << endl;
                #endif // debugging
                if (!ret)
                {
                    tracker[charRowToInt(row)][column] = -1;
                }
                else
                {
                    tracker[charRowToInt(row)][column] = (ret&BS_SHIP_MASK);
                    numHits++;
                    #ifdef debugging
                    cout << "SUCCESSFULLY ASSIGNED " << (ret&BS_SHIP_MASK) << " to grid on " << row << (column + 1) << endl;
                    cout << row << (column + 1) << " is now " << (tracker[charRowToInt(row)][column]) << endl;
                    cout << "(from first turn)" << endl;
                    #endif // debugging
                }
            }
            else if (tracking1)  // This branch reached when the Carrier is hit
            {
                trackAtmpt++;
                #ifdef debugging
                cout << "TRACKING ATTEMPT IS NOW "  << trackAtmpt << endl;
                cout << "Tracking the Carrier." << endl;
                cout << "=============================" << endl;
                #endif // debugging
                if (trackAtmpt == 1) // Checks if we're on the first attempt sets the original hit coordinates.
                {
                    origCol = column;
                    origRow = rowGen;
                    row = intRowToChar(rowGen);
                }
                if (hit && trackAtmpt == 1) // If it's the first attempt, check left, up, right, or down
                                            // and fire on first open coordinate.
                {
                    if (leftClear(tracker, origRow, origCol))  //checks left of first hit coordinate
                    {
                        row = intRowToChar(origRow);
                        column = origCol;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_CARRIER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CARRIER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 1Car ***" << endl;
                        #endif // debugging
                    }
                    else if (topClear(tracker, origRow, origCol))  //checks above first hit coordinate
                    {
                        row = intRowToChar(origRow) - 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_CARRIER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CARRIER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 2Car ***" << endl;
                        #endif // debugging
                    }
                    else if (rightClear(tracker, origRow, origCol))  // checks right of first hit coordinate
                    {
                        row = intRowToChar(origRow);
                        column = origCol + 2;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_CARRIER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CARRIER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 3Car ***" << endl;
                        #endif // debugging
                    }
                    else if (botClear(tracker, origRow, origCol))  // checks below first hit coordinate
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_CARRIER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CARRIER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 4Car ***" << endl;
                        #endif // debugging
                    }
                    if (!(ret))
                        hit = false;
                }
                else if (!hit && trackAtmpt == 2)  // Attempting third hit, if last shot was a miss.
                {
                    if (topClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) - 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_CARRIER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CARRIER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 5Car ***" << endl;
                        #endif // debugging
                    }
                    else if (rightClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow);
                        column = origCol + 2;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_CARRIER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CARRIER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 6Car ***" << endl;
                        #endif // debugging
                    }
                    else if (botClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_CARRIER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CARRIER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 7Car ***" << endl;
                        #endif // debugging
                    }
                    if (!(ret))
                        hit = false;
                }
                else if (!hit && trackAtmpt == 3) // Attempting 4th shot, if last shot was a miss.
                {
                    if (rightClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow);
                        column = origCol + 2;
                        ret = fire[opponent](row, (column));
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_CARRIER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CARRIER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 12Car ***" << endl;
                        #endif // debugging
                    }
                    else if (botClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_CARRIER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CARRIER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 13Car ***" << endl;
                        #endif // debugging
                    }
                    if (!(ret))
                        hit = false;
                }
                else if (!hit && trackAtmpt == 4)  // Attempting 5th shot, if last shot was a miss.
                                                   // This shot should be the last miss.
                {
                    if (botClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_CARRIER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CARRIER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 14Car ***" << endl;
                        #endif // debugging
                    }
                }
                else if (hit && trackAtmpt >= 2)  // All further shots once it's determined if the ship is vertical or not.
                                                  // all further shots are called from the searchRest function.
                {
                    column = origCol;
                    bool reverseSearch, exitSearch;
                    reverseSearch = false;
                    exitSearch = false;
                    if (vert)  // start looking UP rows.
                    {
                        #ifdef debugging
                        cout << "INSIDE UP SEARCH ID1" << endl;
                        #endif // debugging
                        i = origRow;
                        while ((i >= 0) && (i < (BS_GRID_ROWS)) && !(exitSearch) && !(reverseSearch)) // loop that searches left until
                                                                                            // it finds 0, then shoots, or reverses if
                                                                                            // it finds anything other than 0.
                        {
                            #ifdef debugging
                            cout << "Looping. " << i << endl;
                            cout << "I'm reading " << (tracker[i][column]) << " in this current position: "
                                 << (intRowToChar(i)) << (column + 1) << endl;
                            #endif // debugging
                            if ((tracker[i][column] == -1) || (tracker[i][column] != BS_CARRIER))
                            {
                                reverseSearch = true;
                            }
                            else if (i == 0 && tracker[i][column] != 0)
                            {
                                reverseSearch = true;
                            }

                            if (tracker[i][column] == 0)
                            {
                                row = intRowToChar(i);
                                column++;
                                ret = fire[opponent](row, column);
                                #ifdef debugging
                                cout << "Just fired on " << row << (column) << "." << endl;
                                #endif // debugging
                                exitSearch = true;
                                if ((ret&BS_SHIP_MASK) == BS_CARRIER)
                                {
                                    tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));
                                }
                                else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CARRIER)
                                {
                                    adjShipCol = column - 1;
                                    adjShipRow = charRowToInt(row);
                                    adjShipFnd = true;
                                    shipID = (ret&BS_SHIP_MASK);
                                    tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                    #ifdef debugging
                                    cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                    cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                    #endif // debugging
                                }
                                else
                                {
                                    tracker[charRowToInt(row)][column - 1] = -1;
                                }
                                #ifdef debugging
                                cout << "FIRE ID *** 15Car ***" << endl;
                                #endif // debugging
                            }
                            i--;
                        }
                        if (reverseSearch) // if a miss location is reached, or the edge is reached, then reverse and search down.
                                           // Fire on first open coordinate
                        {
                            #ifdef debugging
                            cout << "INSIDE REVERSE SEARCH ID1" << endl;
                            #endif // debugging
                            i = origRow;
                            while ((i >= origRow) && (i < (BS_GRID_ROWS)) && !(exitSearch))
                            {
                                #ifdef debugging
                                cout << "Looping. " << i << endl;
                                cout << "I'm reading " << (tracker[i][column]) << " in this current position: "
                                     << (intRowToChar(i)) << (column + 1) << endl;
                                #endif // debugging
                                if ((tracker[i][column]) == 0)
                                {
                                    row = intRowToChar(i);
                                    column++;
                                    ret = fire[opponent](row, column);
                                    #ifdef debugging
                                    cout << "Just fired on " << row << (column) << "." << endl;
                                    #endif // debugging

                                    exitSearch = true;
                                    if ((ret&BS_SHIP_MASK) == BS_CARRIER)
                                    {
                                        tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));

                                    }
                                    else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CARRIER)
                                    {
                                        adjShipCol = column - 1;
                                        adjShipRow = charRowToInt(row);
                                        adjShipFnd = true;
                                        shipID = (ret&BS_SHIP_MASK);
                                        tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                        #ifdef debugging
                                        cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                        cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                        #endif // debugging
                                    }
                                    else
                                    {
                                        tracker[charRowToInt(row)][column - 1] = -1;
                                    }
                                    #ifdef debugging
                                    cout << "FIRE ID *** 16Car ***" << endl;
                                    #endif // debugging
                                }
                                i++;
                            }
                        }
                    }
                    else // Or start looking LEFT columns.
                    {
                        row = intRowToChar(origRow);
                        #ifdef debugging
                        cout << "INSIDE LEFT SEARCH ID2" << endl;
                        #endif // debugging
                        i = origCol;
                        while ((i >= 0) && (i < (BS_GRID_COLS)) && !(reverseSearch) && !(exitSearch))
                        {
                            #ifdef debugging
                            cout << "Looping. " << i << endl;
                            cout << "I'm reading " << (tracker[charRowToInt(row)][i]) << " in this current position: "
                                 << row << (i + 1) << endl;
                            #endif // debugging
                            if ((tracker[charRowToInt(row)][i] == -1) || (tracker[charRowToInt(row)][i] != BS_CARRIER))
                            {
                                reverseSearch = true;
                            }
                            else if (i == 0 && tracker[charRowToInt(row)][i] != 0)
                            {
                                if ((tracker[charRowToInt(row)][i] != 0) || (tracker[charRowToInt(row)][i] == BS_CARRIER))
                                    reverseSearch = true;
                            }
                            if ((tracker[charRowToInt(row)][i]) == 0)
                            {
                                row = intRowToChar(origRow);
                                column = i + 1;
                                ret = fire[opponent](row, column);
                                #ifdef debugging
                                cout << "Just fired on " << row << (column) << "." << endl;
                                #endif // debugging
                                exitSearch = true;
                                if ((ret&BS_SHIP_MASK) == BS_CARRIER)
                                {
                                    tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));
                                }
                                else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != 1)
                                {
                                    adjShipCol = column - 1;
                                    adjShipRow = charRowToInt(row);
                                    adjShipFnd = true;
                                    shipID = (ret&BS_SHIP_MASK);
                                    tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                    #ifdef debugging
                                    cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                    cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                    #endif // debugging
                                }
                                else
                                {
                                    tracker[charRowToInt(row)][column - 1] = -1;
                                }
                                #ifdef debugging
                                cout << "FIRE ID *** 17Car ***" << endl;
                                #endif // debugging
                            }
                            i--;
                        }
                        if (reverseSearch) // if a miss location is reached, or the edge is reached, then reverse and search right.
                                           // Fire on first open coordinate.
                        {
                            row = intRowToChar(origRow);
                            #ifdef debugging
                            cout << "INSIDE REVERSE SEARCH ID2" << endl;
                            #endif // debugging
                            i = origCol;
                            while ((i >= column) && (i < (BS_GRID_COLS)) && !(exitSearch))
                            {
                                #ifdef debugging
                                cout << "Looping. " << i << endl;
                                #endif // debugging
                                if (tracker[charRowToInt(row)][i] == 0)
                                {
                                    #ifdef debugging
                                    cout << "I'm reading " << (tracker[charRowToInt(row)][i]) << " in this current position." << endl;
                                    #endif // debugging
                                    row = intRowToChar(origRow);
                                    column = i + 1;
                                    ret = fire[opponent](row, column);
                                    #ifdef debugging
                                    cout << "Just fired on " << row << (column) << "." << endl;
                                    #endif // debugging

                                    if ((ret&BS_SHIP_MASK) == BS_CARRIER)
                                    {
                                        exitSearch = true;
                                        tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));
                                    }
                                    else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CARRIER)
                                    {
                                        adjShipCol = column - 1;
                                        adjShipRow = charRowToInt(row);
                                        adjShipFnd = true;
                                        shipID = (ret&BS_SHIP_MASK);
                                        tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                        #ifdef debugging
                                        cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                        cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                        #endif // debugging
                                    }
                                    else
                                    {
                                        tracker[charRowToInt(row)][column - 1] = -1;
                                    }
                                    #ifdef debugging
                                    cout << "FIRE ID *** 18Car ***" << endl;
                                    #endif // debugging
                                }
                                i++;
                            }
                        } // end of Reverse search
                    } // end of "looking left" branch
                } // end of searching branch after vert/horizontal orientation has been determined
            } // end of tracking1 branch.
            else if (tracking2)  // the same basic process repeats for the Battleship
            {
                trackAtmpt++;
                #ifdef debugging
                cout << "TRACKING ATTEMPT IS NOW "  << trackAtmpt << endl;
                cout << "Tracking the Battleship." << endl;
                cout << "=============================" << endl;
                #endif // debugging
                if (trackAtmpt == 1) // Checks if we're on the first attempt sets the original hit coordinates.
                {
                    origCol = column;
                    origRow = rowGen;
                    row = intRowToChar(rowGen);
                }
                if (hit && trackAtmpt == 1) // If it's the first attempt, check left, up, right, or down
                                            // and fire on first open coordinate.
                {
                    if (leftClear(tracker, origRow, origCol))  //checks left of first hit coordinate
                    {
                        row = intRowToChar(origRow);
                        column = origCol;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_BATTLESHIP)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 1BS ***" << endl;
                        #endif // debugging
                    }
                    else if (topClear(tracker, origRow, origCol))  //checks above first hit coordinate
                    {
                        row = intRowToChar(origRow) - 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_BATTLESHIP)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 2BS ***" << endl;
                        #endif // debugging
                    }
                    else if (rightClear(tracker, origRow, origCol))  // checks right of first hit coordinate
                    {
                        row = intRowToChar(origRow);
                        column = origCol + 2;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_BATTLESHIP)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 3BS ***" << endl;
                        #endif // debugging
                    }
                    else if (botClear(tracker, origRow, origCol))  // checks below first hit coordinate
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_BATTLESHIP)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 4BS ***" << endl;
                        #endif // debugging
                    }
                    if (!(ret))
                        hit = false;
                }
                else if (!hit && trackAtmpt == 2)  // Attempting third hit, if last shot was a miss.
                {
                    if (topClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) - 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_BATTLESHIP)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 5BS ***" << endl;
                        #endif // debugging
                    }
                    else if (rightClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow);
                        column = origCol + 2;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_BATTLESHIP)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 6BS ***" << endl;
                        #endif // debugging
                    }
                    else if (botClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_BATTLESHIP)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 7BS ***" << endl;
                        #endif // debugging
                    }
                    if (!(ret))
                        hit = false;
                }
                else if (!hit && trackAtmpt == 3) // Attempting 4th shot, if last shot was a miss.
                {
                    if (rightClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow);
                        column = origCol + 2;
                        ret = fire[opponent](row, (column));
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_BATTLESHIP)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 12BS ***" << endl;
                        #endif // debugging
                    }
                    else if (botClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_BATTLESHIP)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 13BS ***" << endl;
                        #endif // debugging
                    }
                    if (!(ret))
                        hit = false;
                }
                else if (!hit && trackAtmpt == 4)  // Attempting 5th shot, if last shot was a miss.
                                                   // This shot should be the last miss.
                {
                    if (botClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_BATTLESHIP)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 14BS ***" << endl;
                        #endif // debugging
                    }
                }
                else if (hit && trackAtmpt >= 2)  // All further shots once it's determined if the ship is vertical or not.
                                                  // all further shots are called from the searchRest function.
                {
                    column = origCol;
                    bool reverseSearch, exitSearch;
                    reverseSearch = false;
                    exitSearch = false;
                    if (vert)  // start looking UP rows.
                    {
                        #ifdef debugging
                        cout << "INSIDE UP SEARCH ID1" << endl;
                        #endif // debugging
                        i = origRow;
                        while ((i >= 0) && (i < (BS_GRID_ROWS)) && !(exitSearch) && !(reverseSearch)) // loop that searches left until
                                                                                            // it finds 0, then shoots, or reverses if
                                                                                            // it finds anything other than 0.
                        {
                            #ifdef debugging
                            cout << "Looping. " << i << endl;
                            cout << "I'm reading " << (tracker[i][column]) << " in this current position: "
                                 << (intRowToChar(i)) << (column + 1) << endl;
                            #endif // debugging
                            if ((tracker[i][column] == -1) || (tracker[i][column] != BS_BATTLESHIP))
                            {
                                reverseSearch = true;
                            }
                            else if (i == 0 && tracker[i][column] != 0)
                            {
                                reverseSearch = true;
                            }
                            if (tracker[i][column] == 0)
                            {
                                row = intRowToChar(i);
                                column++;
                                ret = fire[opponent](row, column);
                                #ifdef debugging
                                cout << "Just fired on " << row << (column) << "." << endl;
                                #endif // debugging
                                exitSearch = true;
                                if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
                                {
                                    tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));
                                }
                                else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_BATTLESHIP)
                                {
                                    adjShipCol = column - 1;
                                    adjShipRow = charRowToInt(row);
                                    adjShipFnd = true;
                                    shipID = (ret&BS_SHIP_MASK);
                                    tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                    #ifdef debugging
                                    cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                    cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                    #endif // debugging
                                }
                                else
                                {
                                    tracker[charRowToInt(row)][column - 1] = -1;
                                }
                                #ifdef debugging
                                cout << "FIRE ID *** 15BS ***" << endl;
                                #endif // debugging
                            }
                            i--;
                        }
                        if (reverseSearch) // if a miss location is reached, or the edge is reached, then reverse and search down.
                                           // Fire on first open coordinate
                        {
                            #ifdef debugging
                            cout << "INSIDE REVERSE SEARCH ID1" << endl;
                            #endif // debugging
                            i = origRow;
                            while ((i >= origRow) && (i < (BS_GRID_ROWS)) && !(exitSearch))
                            {
                                #ifdef debugging
                                cout << "Looping. " << i << endl;
                                cout << "I'm reading " << (tracker[i][column]) << " in this current position: "
                                     << (intRowToChar(i)) << (column + 1) << endl;
                                #endif // debugging
                                if ((tracker[i][column]) == 0)
                                {
                                    row = intRowToChar(i);
                                    column++;
                                    ret = fire[opponent](row, column);
                                    #ifdef debugging
                                    cout << "Just fired on " << row << (column) << "." << endl;
                                    #endif // debugging

                                    exitSearch = true;
                                    if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
                                    {
                                        tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));

                                    }
                                    else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_BATTLESHIP)
                                    {
                                        adjShipCol = column - 1;
                                        adjShipRow = charRowToInt(row);
                                        adjShipFnd = true;
                                        shipID = (ret&BS_SHIP_MASK);
                                        tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                        #ifdef debugging
                                        cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                        cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                        #endif // debugging
                                    }
                                    else
                                    {
                                        tracker[charRowToInt(row)][column - 1] = -1;
                                    }
                                    #ifdef debugging
                                    cout << "FIRE ID *** 16BS ***" << endl;
                                    #endif // debugging
                                }
                                i++;
                            }
                        }
                    }
                    else // Or start looking LEFT columns.
                    {
                        row = intRowToChar(origRow);
                        #ifdef debugging
                        cout << "INSIDE LEFT SEARCH ID2" << endl;
                        #endif // debugging
                        i = origCol;
                        while ((i >= 0) && (i < (BS_GRID_COLS)) && !(reverseSearch) && !(exitSearch))
                        {
                            #ifdef debugging
                            cout << "Looping. " << i << endl;
                            cout << "I'm reading " << (tracker[charRowToInt(row)][i]) << " in this current position: "
                                 << row << (i + 1) << endl;
                            #endif // debugging
                            if ((tracker[charRowToInt(row)][i] == -1)  || (tracker[charRowToInt(row)][i] != BS_BATTLESHIP))
                            {
                                reverseSearch = true;
                            }
                            else if (i == 0 && tracker[charRowToInt(row)][i] != 0)
                            {
                                if (tracker[charRowToInt(row)][i] != 0 || tracker[charRowToInt(row)][i] == BS_BATTLESHIP)
                                    reverseSearch = true;
                            }
                            if ((tracker[charRowToInt(row)][i]) == 0)
                            {
                                row = intRowToChar(origRow);
                                column = i + 1;
                                ret = fire[opponent](row, column);
                                #ifdef debugging
                                cout << "Just fired on " << row << (column) << "." << endl;
                                #endif // debugging
                                exitSearch = true;
                                if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
                                {
                                    tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));
                                }
                                else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_BATTLESHIP)
                                {
                                    adjShipCol = column - 1;
                                    adjShipRow = charRowToInt(row);
                                    adjShipFnd = true;
                                    shipID = (ret&BS_SHIP_MASK);
                                    tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                    #ifdef debugging
                                    cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                    cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                    #endif // debugging
                                }
                                else
                                {
                                    tracker[charRowToInt(row)][column - 1] = -1;
                                }
                                #ifdef debugging
                                cout << "FIRE ID *** 17BS ***" << endl;
                                #endif // debugging
                            }
                            i--;
                        }
                        if (reverseSearch) // if a miss location is reached, or the edge is reached, then reverse and search right.
                                           // Fire on first open coordinate.
                        {
                            row = intRowToChar(origRow);
                            #ifdef debugging
                            cout << "INSIDE REVERSE SEARCH ID2" << endl;
                            #endif // debugging
                            i = origCol;
                            while ((i >= column) && (i < (BS_GRID_COLS)) && !(exitSearch))
                            {
                                #ifdef debugging
                                cout << "Looping. " << i << endl;
                                #endif // debugging
                                if (tracker[charRowToInt(row)][i] == 0)
                                {
                                    #ifdef debugging
                                    cout << "I'm reading " << (tracker[charRowToInt(row)][i]) << " in this current position." << endl;
                                    #endif // debugging
                                    row = intRowToChar(origRow);
                                    column = i + 1;
                                    ret = fire[opponent](row, column);
                                    exitSearch = true;
                                    #ifdef debugging
                                    cout << "Just fired on " << row << (column) << "." << endl;
                                    #endif // debugging

                                    if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
                                    {
                                        exitSearch = true;
                                        tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));
                                    }
                                    else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_BATTLESHIP)
                                    {
                                        adjShipCol = column - 1;
                                        adjShipRow = charRowToInt(row);
                                        adjShipFnd = true;
                                        shipID = (ret&BS_SHIP_MASK);
                                        tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                        #ifdef debugging
                                        cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                        cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                        #endif // debugging
                                    }
                                    else
                                    {
                                        tracker[charRowToInt(row)][column - 1] = -1;
                                    }
                                    #ifdef debugging
                                    cout << "FIRE ID *** 18BS ***" << endl;
                                    #endif // debugging
                                }
                                i++;
                            }
                        } // end of Reverse search
                    } // end of "looking left" branch
                } // end of searching branch after vert/horizontal orientation has been determined
            } // end of tracking2 branch.
            else if (tracking3) // the same basic process repeats for the Cruiser
            {
                trackAtmpt++;
                #ifdef debugging
                cout << "TRACKING ATTEMPT IS NOW "  << trackAtmpt << endl;
                cout << "Tracking the Cruiser." << endl;
                cout << "=============================" << endl;
                #endif // debugging
                if (trackAtmpt == 1) // Checks if we're on the first attempt sets the original hit coordinates.
                {
                    origCol = column;
                    origRow = rowGen;
                    row = intRowToChar(rowGen);
                }
                if (hit && trackAtmpt == 1) // If it's the first attempt, check left, up, right, or down
                                            // and fire on first open coordinate.
                {
                    if (leftClear(tracker, origRow, origCol))  //checks left of first hit coordinate
                    {
                        row = intRowToChar(origRow);
                        column = origCol;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_CRUISER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CRUISER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 1Cru ***" << endl;
                        #endif // debugging
                    }
                    else if (topClear(tracker, origRow, origCol))  //checks above first hit coordinate
                    {
                        row = intRowToChar(origRow) - 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_CRUISER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CRUISER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 2Cru ***" << endl;
                        #endif // debugging
                    }
                    else if (rightClear(tracker, origRow, origCol))  // checks right of first hit coordinate
                    {
                        row = intRowToChar(origRow);
                        column = origCol + 2;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_CRUISER)
                        {
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CRUISER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 3Cru ***" << endl;
                        #endif // debugging
                    }
                    else if (botClear(tracker, origRow, origCol))  // checks below first hit coordinate
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_CRUISER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CRUISER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 4Cru ***" << endl;
                        #endif // debugging
                    }
                    if (!(ret))
                        hit = false;
                }
                else if (!hit && trackAtmpt == 2)  // Attempting third hit, if last shot was a miss.
                {
                    if (topClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) - 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_CRUISER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CRUISER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 5Cru ***" << endl;
                        #endif // debugging
                    }
                    else if (rightClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow);
                        column = origCol + 2;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_CRUISER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CRUISER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 6Cru ***" << endl;
                        #endif // debugging
                    }
                    else if (botClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_CRUISER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CRUISER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 7Cru ***" << endl;
                        #endif // debugging
                    }
                    if (!(ret))
                        hit = false;
                }
                else if (!hit && trackAtmpt == 3) // Attempting 4th shot, if last shot was a miss.
                {
                    if (rightClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow);
                        column = origCol + 2;
                        ret = fire[opponent](row, (column));
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_CRUISER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CRUISER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 12Cru ***" << endl;
                        #endif // debugging
                    }
                    else if (botClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_CRUISER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CRUISER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 13Cru ***" << endl;
                        #endif // debugging
                    }
                    if (!(ret))
                        hit = false;
                }
                else if (!hit && trackAtmpt == 4)  // Attempting 5th shot, if last shot was a miss.
                                                   // This shot should be the last miss.
                {
                    if (botClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_CRUISER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CRUISER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 14Cru ***" << endl;
                        #endif // debugging
                    }
                }
                else if (hit && trackAtmpt >= 2)  // All further shots once it's determined if the ship is vertical or not.
                                                  // all further shots are called from the searchRest function.
                {
                    column = origCol;
                    bool reverseSearch, exitSearch;
                    reverseSearch = false;
                    exitSearch = false;
                    if (vert)  // start looking UP rows.
                    {
                        #ifdef debugging
                        cout << "INSIDE UP SEARCH ID1" << endl;
                        #endif // debugging
                        i = origRow;
                        while ((i >= 0) && (i < (BS_GRID_ROWS)) && !(exitSearch) && !(reverseSearch)) // loop that searches left until
                                                                                            // it finds 0, then shoots, or reverses if
                                                                                            // it finds anything other than 0.
                        {
                            #ifdef debugging
                            cout << "Looping. " << i << endl;
                            cout << "I'm reading " << (tracker[i][column]) << " in this current position: "
                                 << (intRowToChar(i)) << (column + 1) << endl;
                            #endif // debugging
                            if ((tracker[i][column] == -1) || (tracker[i][column] != BS_CRUISER))
                            {
                                reverseSearch = true;
                            }
                            else if (i == 0 && tracker[i][column] != 0)
                            {
                                reverseSearch = true;
                            }
                            if (tracker[i][column] == 0)
                            {
                                row = intRowToChar(i);
                                column++;
                                ret = fire[opponent](row, column);
                                #ifdef debugging
                                cout << "Just fired on " << row << (column) << "." << endl;
                                #endif // debugging
                                exitSearch = true;
                                if ((ret&BS_SHIP_MASK) == BS_CRUISER)
                                {
                                    tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));
                                }
                                else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CRUISER)
                                {
                                    adjShipCol = column - 1;
                                    adjShipRow = charRowToInt(row);
                                    adjShipFnd = true;
                                    shipID = (ret&BS_SHIP_MASK);
                                    tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                    #ifdef debugging
                                    cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                    cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                    #endif // debugging
                                }
                                else
                                {
                                    tracker[charRowToInt(row)][column - 1] = -1;
                                }
                                #ifdef debugging
                                cout << "FIRE ID *** 15Cru ***" << endl;
                                #endif // debugging
                            }
                            i--;
                        }
                        if (reverseSearch) // if a miss location is reached, or the edge is reached, then reverse and search down.
                                           // Fire on first open coordinate
                        {
                            #ifdef debugging
                            cout << "INSIDE REVERSE SEARCH ID1" << endl;
                            #endif // debugging
                            i = origRow;
                            while ((i >= origRow) && (i < (BS_GRID_ROWS)) && !(exitSearch))
                            {
                                #ifdef debugging
                                cout << "Looping. " << i << endl;
                                cout << "I'm reading " << (tracker[i][column]) << " in this current position: "
                                     << (intRowToChar(i)) << (column + 1) << endl;
                                #endif // debugging
                                if ((tracker[i][column]) == 0)
                                {
                                    row = intRowToChar(i);
                                    column++;
                                    ret = fire[opponent](row, column);
                                    #ifdef debugging
                                    cout << "Just fired on " << row << (column) << "." << endl;
                                    #endif // debugging

                                    exitSearch = true;
                                    if ((ret&BS_SHIP_MASK) == BS_CRUISER)
                                    {
                                        tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));

                                    }
                                    else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CRUISER)
                                    {
                                        adjShipCol = column - 1;
                                        adjShipRow = charRowToInt(row);
                                        adjShipFnd = true;
                                        shipID = (ret&BS_SHIP_MASK);
                                        tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                        #ifdef debugging
                                        cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                        cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                        #endif // debugging
                                    }
                                    else
                                    {
                                        tracker[charRowToInt(row)][column - 1] = -1;
                                    }
                                    #ifdef debugging
                                    cout << "FIRE ID *** 16Cru ***" << endl;
                                    #endif // debugging
                                }
                                i++;
                            }
                        }
                    }
                    else // Or start looking LEFT columns.
                    {
                        row = intRowToChar(origRow);
                        #ifdef debugging
                        cout << "INSIDE LEFT SEARCH ID2" << endl;
                        #endif // debugging
                        i = origCol;
                        while ((i >= 0) && (i < (BS_GRID_COLS)) && !(reverseSearch) && !(exitSearch))
                        {
                            #ifdef debugging
                            cout << "Looping. " << i << endl;
                            cout << "I'm reading " << (tracker[charRowToInt(row)][i]) << " in this current position: "
                                 << row << (i + 1) << endl;
                            #endif // debugging
                            if ((tracker[charRowToInt(row)][i] == -1) || (tracker[charRowToInt(row)][i] != BS_CRUISER))
                            {
                                reverseSearch = true;
                            }
                            else if (i == 0 && tracker[charRowToInt(row)][i] != 0)
                            {
                                if (tracker[charRowToInt(row)][i] != 0 || tracker[charRowToInt(row)][i] == BS_CRUISER)
                                    reverseSearch = true;
                            }
                            if ((tracker[charRowToInt(row)][i]) == 0)
                            {
                                row = intRowToChar(origRow);
                                column = i + 1;
                                ret = fire[opponent](row, column);
                                #ifdef debugging
                                cout << "Just fired on " << row << (column) << "." << endl;
                                #endif // debugging
                                exitSearch = true;
                                if ((ret&BS_SHIP_MASK) == BS_CRUISER)
                                {
                                    tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));
                                }
                                else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CRUISER)
                                {
                                    adjShipCol = column - 1;
                                    adjShipRow = charRowToInt(row);
                                    adjShipFnd = true;
                                    shipID = (ret&BS_SHIP_MASK);
                                    tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                    #ifdef debugging
                                    cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                    cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                    #endif // debugging
                                }
                                else
                                {
                                    tracker[charRowToInt(row)][column - 1] = -1;
                                }
                                #ifdef debugging
                                cout << "FIRE ID *** 17Cru ***" << endl;
                                #endif // debugging
                            }
                            i--;
                        }
                        if (reverseSearch) // if a miss location is reached, or the edge is reached, then reverse and search right.
                                           // Fire on first open coordinate.
                        {
                            row = intRowToChar(origRow);
                            #ifdef debugging
                            cout << "INSIDE REVERSE SEARCH ID2" << endl;
                            #endif // debugging
                            i = origCol;
                            while ((i >= column) && (i < (BS_GRID_COLS)) && !(exitSearch))
                            {
                                #ifdef debugging
                                cout << "Looping. " << i << endl;
                                #endif // debugging
                                if (tracker[charRowToInt(row)][i] == 0)
                                {
                                    #ifdef debugging
                                    cout << "I'm reading " << (tracker[charRowToInt(row)][i]) << " in this current position." << endl;
                                    #endif // debugging
                                    row = intRowToChar(origRow);
                                    column = i + 1;
                                    ret = fire[opponent](row, column);
                                    exitSearch = true;
                                    #ifdef debugging
                                    cout << "Just fired on " << row << (column) << "." << endl;
                                    #endif // debugging

                                    if ((ret&BS_SHIP_MASK) == BS_CRUISER)
                                    {
                                        exitSearch = true;
                                        tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));
                                    }
                                    else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_CRUISER)
                                    {
                                        adjShipCol = column - 1;
                                        adjShipRow = charRowToInt(row);
                                        adjShipFnd = true;
                                        shipID = (ret&BS_SHIP_MASK);
                                        tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                        #ifdef debugging
                                        cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                        cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                        #endif // debugging
                                    }
                                    else
                                    {
                                        tracker[charRowToInt(row)][column - 1] = -1;
                                    }
                                    #ifdef debugging
                                    cout << "FIRE ID *** 18Cru ***" << endl;
                                    #endif // debugging
                                }
                                i++;
                            }
                        } // end of Reverse search
                    } // end of "looking left" branch
                } // end of searching branch after vert/horizontal orientation has been determined
            } // end of tracking3 branch.
            else if (tracking4) // the same basic process repeats for the Destroyer
            {
                trackAtmpt++;
                #ifdef debugging
                cout << "TRACKING ATTEMPT IS NOW "  << trackAtmpt << endl;
                cout << "Tracking the Destroyer." << endl;
                cout << "=============================" << endl;
                #endif // debugging
                if (trackAtmpt == 1) // Checks if we're on the first attempt sets the original hit coordinates.
                {
                    origCol = column;
                    origRow = rowGen;
                    row = intRowToChar(rowGen);
                }
                if (hit && trackAtmpt == 1) // If it's the first attempt, check left, up, right, or down
                                            // and fire on first open coordinate.
                {
                    if (leftClear(tracker, origRow, origCol))  //checks left of first hit coordinate
                    {
                        row = intRowToChar(origRow);
                        column = origCol;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_DESTROYER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_DESTROYER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 1Dest ***" << endl;
                        #endif // debugging
                    }
                    else if (topClear(tracker, origRow, origCol))  //checks above first hit coordinate
                    {
                        row = intRowToChar(origRow) - 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_DESTROYER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_DESTROYER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 2Dest ***" << endl;
                        #endif // debugging
                    }
                    else if (rightClear(tracker, origRow, origCol))  // checks right of first hit coordinate
                    {
                        row = intRowToChar(origRow);
                        column = origCol + 2;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_DESTROYER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_DESTROYER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 3Dest ***" << endl;
                        #endif // debugging
                    }
                    else if (botClear(tracker, origRow, origCol))  // checks below first hit coordinate
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_DESTROYER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_DESTROYER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 4Dest ***" << endl;
                        #endif // debugging
                    }
                    if (!(ret))
                        hit = false;
                }
                else if (!hit && trackAtmpt == 2)  // Attempting third hit, if last shot was a miss.
                {
                    if (topClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) - 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_DESTROYER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_DESTROYER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 5Dest ***" << endl;
                        #endif // debugging
                    }
                    else if (rightClear(tracker, origRow, origCol))
                    {
                        numHits++;
                        row = intRowToChar(origRow);
                        column = origCol + 2;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_DESTROYER)
                        {
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_DESTROYER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 6Dest ***" << endl;
                        #endif // debugging
                    }
                    else if (botClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_DESTROYER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_DESTROYER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 7Dest ***" << endl;
                        #endif // debugging
                    }
                    if (!(ret))
                        hit = false;
                }
                else if (!hit && trackAtmpt == 3) // Attempting 4th shot, if last shot was a miss.
                {
                    if (rightClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow);
                        column = origCol + 2;
                        ret = fire[opponent](row, (column));
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_DESTROYER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_DESTROYER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 12Dest ***" << endl;
                        #endif // debugging
                    }
                    else if (botClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_DESTROYER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_DESTROYER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 13Dest ***" << endl;
                        #endif // debugging
                    }
                    if (!(ret))
                        hit = false;
                }
                else if (!hit && trackAtmpt == 4)  // Attempting 5th shot, if last shot was a miss.
                                                   // This shot should be the last miss.
                {
                    if (botClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_DESTROYER)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_DESTROYER)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 14Dest ***" << endl;
                        #endif // debugging
                    }
                }
            } // end of tracking4 branch.
            else if (tracking5) // the same basic process repeats for the Submarine
            {
                trackAtmpt++;
                #ifdef debugging
                cout << "TRACKING ATTEMPT IS NOW "  << trackAtmpt << endl;
                cout << "Tracking the Submarine." << endl;
                cout << "=============================" << endl;
                #endif // debugging
                if (trackAtmpt == 1) // Checks if we're on the first attempt sets the original hit coordinates.
                {
                    origCol = column;
                    origRow = rowGen;
                    row = intRowToChar(rowGen);
                }
                if (hit && trackAtmpt == 1) // If it's the first attempt, check left, up, right, or down
                                            // and fire on first open coordinate.
                {
                    if (leftClear(tracker, origRow, origCol))  //checks left of first hit coordinate
                    {
                        row = intRowToChar(origRow);
                        column = origCol;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_SUBMARINE)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 1Sub ***" << endl;
                        #endif // debugging
                    }
                    else if (topClear(tracker, origRow, origCol))  //checks above first hit coordinate
                    {
                        row = intRowToChar(origRow) - 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_SUBMARINE)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 2Sub ***" << endl;
                        #endif // debugging
                    }
                    else if (rightClear(tracker, origRow, origCol))  // checks right of first hit coordinate
                    {
                        row = intRowToChar(origRow);
                        column = origCol + 2;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_SUBMARINE)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 3Sub ***" << endl;
                        #endif // debugging
                    }
                    else if (botClear(tracker, origRow, origCol))  // checks below first hit coordinate
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_SUBMARINE)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 4Sub ***" << endl;
                        #endif // debugging
                    }
                    if (!(ret))
                        hit = false;
                }
                else if (!hit && trackAtmpt == 2)  // Attempting third hit, if last shot was a miss.
                {
                    if (topClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) - 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_SUBMARINE)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 5Sub ***" << endl;
                        #endif // debugging
                    }
                    else if (rightClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow);
                        column = origCol + 2;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_SUBMARINE)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 6Sub ***" << endl;
                        #endif // debugging
                    }
                    else if (botClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_SUBMARINE)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 7Sub ***" << endl;
                        #endif // debugging
                    }
                    if (!(ret))
                        hit = false;
                }
                else if (!hit && trackAtmpt == 3) // Attempting 4th shot, if last shot was a miss.
                {
                    if (rightClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow);
                        column = origCol + 2;
                        ret = fire[opponent](row, (column));
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = false;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_SUBMARINE)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 12Sub ***" << endl;
                        #endif // debugging
                    }
                    else if (botClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging

                        if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_SUBMARINE)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 13Sub ***" << endl;
                        #endif // debugging
                    }
                    if (!(ret))
                        hit = false;
                }
                else if (!hit && trackAtmpt == 4)  // Attempting 5th shot, if last shot was a miss.
                                                   // This shot should be the last miss.
                {
                    if (botClear(tracker, origRow, origCol))
                    {
                        row = intRowToChar(origRow) + 1;
                        column = origCol + 1;
                        ret = fire[opponent](row, column);
                        #ifdef debugging
                        cout << "Just fired on " << row << (column) << "." << endl;
                        #endif // debugging
                        if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
                        {
                            numHits++;
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            vert = true;
                        }
                        else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_SUBMARINE)
                        {
                            adjShipCol = column - 1;
                            adjShipRow = charRowToInt(row);
                            adjShipFnd = true;
                            shipID = (ret&BS_SHIP_MASK);
                            tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                            hit = false;
                            #ifdef debugging
                            cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                            cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                            #endif // debugging
                        }
                        else
                        {
                            tracker[charRowToInt(row)][column - 1] = -1;
                        }
                        #ifdef debugging
                        cout << "FIRE ID *** 14Sub ***" << endl;
                        #endif // debugging
                    }
                }
                else if (hit && trackAtmpt >= 2)  // All further shots once it's determined if the ship is vertical or not.
                                                  // all further shots are called from the searchRest function.
                {
                    column = origCol;
                    bool reverseSearch, exitSearch;
                    reverseSearch = false;
                    exitSearch = false;
                    if (vert)  // start looking UP rows.
                    {
                        #ifdef debugging
                        cout << "INSIDE UP SEARCH ID1" << endl;
                        #endif // debugging
                        i = origRow;
                        while ((i >= 0) && (i < (BS_GRID_ROWS)) && !(exitSearch) && !(reverseSearch)) // loop that searches left until
                                                                                            // it finds 0, then shoots, or reverses if
                                                                                            // it finds anything other than 0.
                        {
                            #ifdef debugging
                            cout << "Looping. " << i << endl;
                            cout << "I'm reading " << (tracker[i][column]) << " in this current position: "
                                 << (intRowToChar(i)) << (column + 1) << endl;
                            #endif // debugging
                            if ((tracker[i][column] == -1) || (tracker[i][column] != BS_SUBMARINE))
                            {
                                reverseSearch = true;
                            }
                            else if (i == 0 && tracker[i][column] != 0)
                            {
                                reverseSearch = true;
                            }
                            if (tracker[i][column] == 0)
                            {
                                row = intRowToChar(i);
                                column++;
                                ret = fire[opponent](row, column);
                                #ifdef debugging
                                cout << "Just fired on " << row << (column) << "." << endl;
                                #endif // debugging
                                exitSearch = true;
                                if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
                                {
                                    tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));
                                }
                                else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_SUBMARINE)
                                {
                                    adjShipCol = column - 1;
                                    adjShipRow = charRowToInt(row);
                                    adjShipFnd = true;
                                    shipID = (ret&BS_SHIP_MASK);
                                    tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                    #ifdef debugging
                                    cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                    cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                    #endif // debugging
                                }
                                else
                                {
                                    tracker[charRowToInt(row)][column - 1] = -1;
                                }
                                #ifdef debugging
                                cout << "FIRE ID *** 15Sub ***" << endl;
                                #endif // debugging
                            }
                            i--;
                        }
                        if (reverseSearch) // if a miss location is reached, or the edge is reached, then reverse and search down.
                                           // Fire on first open coordinate
                        {
                            #ifdef debugging
                            cout << "INSIDE REVERSE SEARCH ID1" << endl;
                            #endif // debugging
                            i = origRow;
                            while ((i >= origRow) && (i < (BS_GRID_ROWS)) && !(exitSearch))
                            {
                                #ifdef debugging
                                cout << "Looping. " << i << endl;
                                cout << "I'm reading " << (tracker[i][column]) << " in this current position: "
                                     << (intRowToChar(i)) << (column + 1) << endl;
                                #endif // debugging
                                if ((tracker[i][column]) == 0)
                                {
                                    row = intRowToChar(i);
                                    column++;
                                    ret = fire[opponent](row, column);
                                    #ifdef debugging
                                    cout << "Just fired on " << row << (column) << "." << endl;
                                    #endif // debugging

                                    exitSearch = true;
                                    if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
                                    {
                                        tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));

                                    }
                                    else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_SUBMARINE)
                                    {
                                        adjShipCol = column - 1;
                                        adjShipRow = charRowToInt(row);
                                        adjShipFnd = true;
                                        shipID = (ret&BS_SHIP_MASK);
                                        tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                        #ifdef debugging
                                        cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                        cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                        #endif // debugging
                                    }
                                    else
                                    {
                                        tracker[charRowToInt(row)][column - 1] = -1;
                                    }
                                    #ifdef debugging
                                    cout << "FIRE ID *** 16Sub ***" << endl;
                                    #endif // debugging
                                }
                                i++;
                            }
                        }
                    }
                    else // Or start looking LEFT columns.
                    {
                        row = intRowToChar(origRow);
                        #ifdef debugging
                        cout << "INSIDE LEFT SEARCH ID2" << endl;
                        #endif // debugging
                        i = origCol;
                        while ((i >= 0) && (i < (BS_GRID_COLS)) && !(reverseSearch) && !(exitSearch))
                        {
                            #ifdef debugging
                            cout << "Looping. " << i << endl;
                            cout << "I'm reading " << (tracker[charRowToInt(row)][i]) << " in this current position: "
                                 << row << (i + 1) << endl;
                            #endif // debugging
                            if ((tracker[charRowToInt(row)][i] == -1) || (tracker[charRowToInt(row)][i] != BS_SUBMARINE))
                            {
                                reverseSearch = true;
                            }
                            else if (i == 0 && tracker[charRowToInt(row)][i] != 0)
                            {
                                if (tracker[charRowToInt(row)][i] != 0 || tracker[charRowToInt(row)][i] == BS_SUBMARINE)
                                    reverseSearch = true;
                            }
                            if ((tracker[charRowToInt(row)][i]) == 0)
                            {
                                row = intRowToChar(origRow);
                                column = i + 1;
                                ret = fire[opponent](row, column);
                                #ifdef debugging
                                cout << "Just fired on " << row << (column) << "." << endl;
                                #endif // debugging
                                exitSearch = true;
                                if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
                                {
                                    tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));
                                }
                                else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_SUBMARINE)
                                {
                                    adjShipCol = column - 1;
                                    adjShipRow = charRowToInt(row);
                                    adjShipFnd = true;
                                    shipID = (ret&BS_SHIP_MASK);
                                    tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                    #ifdef debugging
                                    cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                    cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                    #endif // debugging
                                }
                                else
                                {
                                    tracker[charRowToInt(row)][column - 1] = -1;
                                }
                                #ifdef debugging
                                cout << "FIRE ID *** 17Sub ***" << endl;
                                #endif // debugging
                            }
                            i--;
                        }
                        if (reverseSearch) // if a miss location is reached, or the edge is reached, then reverse and search right.
                                           // Fire on first open coordinate.
                        {
                            row = intRowToChar(origRow);
                            #ifdef debugging
                            cout << "INSIDE REVERSE SEARCH ID2" << endl;
                            #endif // debugging
                            i = origCol;
                            while ((i >= column) && (i < (BS_GRID_COLS)) && !(exitSearch))
                            {
                                #ifdef debugging
                                cout << "Looping. " << i << endl;
                                #endif // debugging
                                if (tracker[charRowToInt(row)][i] == 0)
                                {
                                    #ifdef debugging
                                    cout << "I'm reading " << (tracker[charRowToInt(row)][i]) << " in this current position." << endl;
                                    #endif // debugging
                                    row = intRowToChar(origRow);
                                    column = i + 1;
                                    ret = fire[opponent](row, column);
                                    #ifdef debugging
                                    cout << "Just fired on " << row << (column) << "." << endl;
                                    #endif // debugging

                                    if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
                                    {
                                        exitSearch = true;
                                        tracker[charRowToInt(row)][column - 1] = ((ret&BS_SHIP_MASK));
                                    }
                                    else if ((ret&BS_SHIP_MASK) != 0 && (ret&BS_SHIP_MASK) != BS_SUBMARINE)
                                    {
                                        adjShipCol = column - 1;
                                        adjShipRow = charRowToInt(row);
                                        adjShipFnd = true;
                                        shipID = (ret&BS_SHIP_MASK);
                                        tracker[charRowToInt(row)][column - 1] = (ret&BS_SHIP_MASK);
                                        exitSearch = true;
                                        #ifdef debugging
                                        cout << boolalpha << "Found a ship is now " << adjShipFnd << endl;
                                        cout << "AdjShop coordinates set to " << (intRowToChar(adjShipRow)) << (adjShipCol + 1) << endl;
                                        #endif // debugging
                                    }
                                    else
                                    {
                                        tracker[charRowToInt(row)][column - 1] = -1;
                                    }
                                    #ifdef debugging
                                    cout << "FIRE ID *** 18Sub ***" << endl;
                                    #endif // debugging
                                }
                                i++;
                            }
                        } // end of Reverse search
                    } // end of "looking left" branch
                } // end of searching branch after vert/horizontal orientation has been determined
            } // end of tracking5 branch
            else // If we're not tracking a ship and the last shot was a miss, then we go into Search mode.
            {
                int density;
                double switchAlg;
                density = BS_GRID_ROWS * BS_GRID_COLS;
                switchAlg = .32 * density;
                int smallestShip;
                #ifdef searchDebugging
                cout << "Now searching." << endl;
                #endif // searchDebugging
                do // Loop continues until a hit occurs.
                {
                    if (!destSank) // This statement sets the smallest size to look for on the grid.
                                   // If by change the destroyer was sunk early, we don't need to look
                                   // in small spaces.
                    {
                        smallestShip = BS_DESTROYER_SIZE;
                    }
                    else if (!subSank)
                    {
                        smallestShip = BS_SUBMARINE_SIZE;
                    }
                    else if (!cruSank)
                    {
                        smallestShip = BS_CRUISER_SIZE;
                    }
                    else if (!batSank)
                    {
                        smallestShip = BS_BATTLESHIP_SIZE;
                    }
                    else if (!carSank)
                    {
                        smallestShip = BS_CARRIER_SIZE;
                    }
                    pickNew = true;
                    rowGen = (rand() % BS_GRID_ROWS);
                    column = (rand() % BS_GRID_COLS);
                    row = intRowToChar(rowGen);
                    if (tracker[rowGen][column] != 0)
                    {
                        pickNew = true;
                    }
                    // This Algorithm currently under construction.
                    else if (turn > switchAlg)
                    {
                        bool greenZone, vertSearch;
                        int totalSpots, initRow, initCol, endRow, endCol, fireRow, fireCol;
                        #ifdef searchDebugging
                        cout << "Now inside alternative Seach algorithm." << endl;
                        cout << "The smallest ship is set to " << smallestShip << endl;
                        #endif // searchDebugging
                        totalSpots = 0;
                        greenZone = false;
                        initRow = 0;
                        initCol = 0;
                        endRow = 0;
                        endCol = 0;
                        vertSearch = false;
                        while (totalSpots < smallestShip)
                        {
                            for (i = 0; i < BS_GRID_ROWS && !greenZone; i++)
                            {
                                totalSpots = 0;
                                for (j = 0; j < BS_GRID_COLS && !greenZone; j++)
                                {
                                    #ifdef searchDebugging
                                    cout << "I'm reading " << tracker[i][j] << " at position "
                                         << intRowToChar(i) << (j+1) << endl;
                                    #endif // searchDebugging
                                    if (tracker[i][j] == 0 && totalSpots == 0)
                                    {
                                        initRow = i;
                                        initCol = j;
                                        totalSpots++;
                                        #ifdef searchDebugging
                                        cout << "Initial: " << intRowToChar(initRow) << (initCol + 1) << endl;
                                        #endif // searchDebugging
                                    }
                                    else if (tracker[i][j] == 0 && totalSpots > 0 && totalSpots < smallestShip)
                                    {
                                        totalSpots++;
                                    }
                                    else
                                    {
                                        totalSpots = 0;
                                    }
                                    if (totalSpots == smallestShip)
                                    {
                                        greenZone = true;
                                        endRow = i;
                                        endCol = j;
                                        #ifdef searchDebugging
                                        cout << "End: " << intRowToChar(endRow) << (endCol + 1) << endl;
                                        #endif // searchDebugging
                                    }
                                }
                            }
                            if (!greenZone)
                            {
                                vertSearch = true;
                            }
                            // Search vertically
                            #if searchDebugging
                            cout << "Now searching vertically..." << endl;
                            #endif // searchDebugging
                            if (vertSearch)
                            {
                                for (i = 0; i < BS_GRID_COLS && !greenZone; i++)
                                {
                                    totalSpots = 0;
                                    for (j = 0; j < BS_GRID_ROWS && !greenZone; j++)
                                    {
                                        #ifdef searchDebugging
                                        cout << "I'm reading " << tracker[j][i] << " at position "
                                             << intRowToChar(j) << (i+1) << endl;
                                        #endif // searchDebugging
                                        if (tracker[j][i] == 0 && totalSpots == 0)
                                        {
                                            initRow = j;
                                            initCol = i;
                                            totalSpots++;
                                            #ifdef searchDebugging
                                            cout << "Initial: " << intRowToChar(initRow) << (initCol + 1) << endl;
                                            #endif // searchDebugging
                                        }
                                        else if (tracker[j][i] == 0 && totalSpots > 0 && totalSpots < smallestShip)
                                        {
                                            totalSpots++;
                                        }
                                        else
                                        {
                                            totalSpots = 0;
                                        }
                                        if (totalSpots == smallestShip)
                                        {
                                            greenZone = true;
                                            endRow = j;
                                            endCol = i;
                                            #ifdef searchDebugging
                                            cout << "End: " << intRowToChar(endRow) << (endCol + 1) << endl;
                                            #endif // searchDebugging
                                        }
                                    }
                                }
                            }
                        }
                        if (!vertSearch)
                        {
                            if (initCol != 0 && tracker[initRow][initCol-1] != 0)
                            {
                                initCol++;
                            }
                            if (initCol !=0 && initRow != 0 && !topClear(tracker, initRow, initCol) && !botClear(tracker, initRow, initCol) && leftClear(tracker, initRow, initCol))
                            {
                                initCol--;
                            }
                        }
                        else
                        {
                            if (initRow != 0 && tracker[initRow - 1][initCol] != 0)
                            {
                                initRow++;
                            }
                        }
                        fireRow = (initRow + endRow) / 2;
                        fireCol = (initCol + endCol) / 2;
                        pickNew = false;
                        row = intRowToChar(fireRow);
                        column = fireCol;
                        rowGen = fireRow;
                        #ifdef searchDebugging
                        cout << "Fire coords set to " << intRowToChar(fireRow) << (fireCol + 1) << endl;
                        #endif // searchDebugging
                    }
                    // End algorithm under construction.
                    else if (turn <= switchAlg)
                    {
                        if (rowGen == 0 && column == 0)
                        {
                            if (rightClear(tracker, rowGen, column))
                            {
                                pickNew = true;
                                if (botClear(tracker, rowGen, column))
                                    pickNew = false;
                            }
                        }
                        else if (rowGen == 0 && column == BS_GRID_COLS - 1)
                        {
                            if (leftClear(tracker, rowGen, column))
                            {
                                pickNew = true;
                                if (botClear(tracker, rowGen, column))
                                    pickNew = false;
                            }
                        }
                        else if (rowGen == BS_GRID_ROWS - 1 && column == 0)
                        {
                            if (rightClear(tracker, rowGen, column))
                            {
                                pickNew = true;
                                if (topClear(tracker, rowGen, column))
                                    pickNew = false;
                            }
                        }
                        else if (rowGen == BS_GRID_ROWS - 1 && column == BS_GRID_COLS - 1)
                        {
                            if (leftClear(tracker, rowGen, column))
                            {
                                pickNew = true;
                                if (topClear(tracker, rowGen, column))
                                    pickNew = false;
                            }
                        }
                        else if (rowGen == 0)
                        {
                            if (leftClear(tracker, rowGen, column))
                            {
                                pickNew = true;
                                if (rightClear(tracker, rowGen, column))
                                {
                                    pickNew = true;
                                    if (botClear(tracker, rowGen, column))
                                        pickNew = false;
                                }
                            }
                        }
                        else if (rowGen == BS_GRID_ROWS - 1)
                        {
                            if (leftClear(tracker, rowGen, column))
                            {
                                pickNew = true;
                                if (rightClear(tracker, rowGen, column))
                                {
                                    pickNew = true;
                                    if (topClear(tracker, rowGen, column))
                                        pickNew = false;
                                }
                            }
                        }
                        else if (column == 0)
                        {
                            if (topClear(tracker, rowGen, column))
                            {
                                pickNew = true;
                                if (rightClear(tracker, rowGen, column))
                                {
                                    pickNew = true;
                                    if (botClear(tracker, rowGen, column))
                                        pickNew = false;
                                }
                            }
                        }
                        else if (column == BS_GRID_COLS - 1)
                        {
                            if (leftClear(tracker, rowGen, column))
                            {
                                pickNew = true;
                                if (botClear(tracker, rowGen, column))
                                {
                                    pickNew = true;
                                    if (topClear(tracker, rowGen, column))
                                        pickNew = false;
                                }
                            }
                        }
                        else if (leftClear(tracker, rowGen, column))
                        {
                            pickNew = true;
                            if (topClear(tracker, rowGen, column))
                            {
                                pickNew = true;
                                if (rightClear(tracker, rowGen, column))
                                {
                                    pickNew = true;
                                    if (botClear(tracker, rowGen, column))
                                        pickNew = false;
                                }
                            }
                        }
                    }
                } // end of do loop
                while (pickNew);
                ret = fire[opponent](row, (column + 1));
                #ifdef debugging
                cout << "Just fired on " << row << (column + 1) << "." << endl;
                cout << "fired from search" << endl;
                #endif // debugging
                if (!(ret))
                {
                    tracker[charRowToInt(row)][column] = -1;
                    #ifdef debugging
                    cout << "SUCCESSFULLY ASSIGNED -1 to grid on " << row << (column + 1) << endl;
                    cout << row << (column + 1) << " is now " << (tracker[charRowToInt(row)][column]) << endl;
                    #endif // debugging
                }
                else
                {
                    numHits++;
                    tracker[charRowToInt(row)][column] = (ret&BS_SHIP_MASK);
                    #ifdef debugging
                    cout << "SUCCESSFULLY ASSIGNED " << (ret&BS_SHIP_MASK) << " to grid on " << row << (column + 1) << endl;
                    cout << row << (column + 1) << " is now " << (tracker[charRowToInt(row)][column]) << endl;
                    #endif // debugging
                }
            }
        // Check for hit or sink and assign tracking appropriately.
        if (ret && tracking1 && numHits > 1)
        {
            hit = true;
        }
        else if (ret && tracking2 && numHits > 1)
        {
            hit = true;
        }
        else if (ret && tracking3 && numHits > 1)
        {
            hit = true;
        }
        else if (ret && tracking5 && numHits > 1)
        {
            hit = true;
        }
        if (ret && !adjShipFnd)
        {
            hit = true;
            if ((ret&BS_SHIP_MASK) == BS_CARRIER)
            {
                tracking1 = true;
            }
            else if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
            {
                tracking2 = true;
            }
            else if ((ret&BS_SHIP_MASK) == BS_CRUISER)
            {
                tracking3 = true;
            }
            else if ((ret&BS_SHIP_MASK) == BS_DESTROYER)
            {
                tracking4 = true;
            }
            else if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
            {
                tracking5 = true;
            }
        }
        if ((ret&BS_SHIP_SANK) == BS_SHIP_SANK)
        {
            numHits = 0;
            hit = false;
            tracking1 = false;
            tracking2 = false;
            tracking3 = false;
            tracking4 = false;
            tracking5 = false;
            trackAtmpt = 0;
            if ((ret&BS_SHIP_MASK) == BS_CARRIER)
            {
                carSank = true;
            }
            else if ((ret&BS_SHIP_MASK) == BS_BATTLESHIP)
            {
                batSank = true;
            }
            else if ((ret&BS_SHIP_MASK) == BS_CRUISER)
            {
                cruSank = true;
            }
            else if ((ret&BS_SHIP_MASK) == BS_DESTROYER)
            {
                destSank = true;
            }
            else if ((ret&BS_SHIP_MASK) == BS_SUBMARINE)
            {
                subSank = true;
            }
            if (adjShipFnd)
            {
                adjShipFnd = false;
                hit = true;
                rowGen = adjShipRow;
                column = adjShipCol;
                if (shipID == BS_CARRIER)
                {
                    tracking1 = true;
                }
                else if (shipID == BS_BATTLESHIP)
                {
                    tracking2 = true;
                }
                else if (shipID == BS_CRUISER)
                {
                    tracking3 = true;
                }
                else if (shipID == BS_DESTROYER)
                {
                    tracking4 = true;
                }
                else if (shipID == BS_SUBMARINE)
                {
                    tracking5 = true;
                }
                #ifdef debugging
                cout << "==============================" << endl;
                cout << "FOUND ADJACENT SHIP.  NOW STORING COORDINATES: " << endl;
                cout << "Original Row is now " << intRowToChar(origRow) << endl;
                cout << "Original Col is now " << (origCol + 1) << endl;
                cout << "The ship that was found is " << shipID << endl;
                #endif // debugging
            }
        }
        #ifdef printGrid
        cout << endl << "C: ";
        for (i = 0; i < 10; i++)
        {
            cout << setw(2) << (i + 1);
        }


        for (i = 0; i < BS_GRID_ROWS; i++)
        {
            cout << endl << static_cast<char>(intRowToChar(i)) << ": ";
            for (j = 0; j < BS_GRID_COLS; j++)
            {
                cout <<  setw(2) << tracker[i][j];
            }
        }
        cout << endl;
        #endif // printGrid
        break; // end if the "Continue game" switch statement branch
    }
    return 0;
}
int (*fire[MaxPlayerCount])(char, int)= {NULL, scafidiI};
int (*battleship[MaxPlayerCount])(int, int) = {NULL, scafidiA};
char const *playerName[MaxPlayerCount] = {"", "Richard Scafidi"};
