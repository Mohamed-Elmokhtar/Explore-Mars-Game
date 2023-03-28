#include "Mars.h"
#include <iostream>
#include <iomanip>
#include <time.h>
#include <cmath>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <string>
#include "Colors.h"
#include "Validation.h"
using namespace std;

Map::Map():Map(0, 0){}      // Default Map Constructor

Map::Map(int Y, int X)      // Constructor To Set Map Dimension Y (dimY)
                            // and Dimension X (dimX) Into Dimensions Given By User
{
    this->dimY = Y;
    this->dimX = X;
}

Map::~Map()                 // Destructor To Free Memory Occupied By Dynamic Pointer
{
    for (int i = 0; i < dimX; i++)
        delete[] map[i];
    
    delete[] map; 
}

int Map::getDimY()          // Function To Return The Value Of Map Dimension Y
{
    return dimY;
}

int Map::getDimX()          // Function To Return The Value Of Map Dimension X
{
    return dimX;
}

int Map::getGold()          // Function To Return The Number Of Golds
{
    return gold;
}

// Function To Check If Rover Got Stuck By Obstacles
bool Map::roverStuck(int y, int x)
{
    bool roverBlocked = false;

    if ( !(isEmpty(x, y-1) || isEmpty(x, y+1) ||
           isEmpty(x-1, y) || isEmpty(x+1, y)) )
           roverBlocked = true;
    
    return roverBlocked;
}

// Function To Return Character at Given Map Position
char Map::getObject(int Y, int X)
{
    char ch;
    ch = map[dimY-Y][X-1];
    
    return ch;
}

// Function To Validate Positioning of Obstacles To Avoid
// The Chance of Having a Wall In The Map
bool Map::validPosition(int Y, int X)
{
    bool valid = false;

    if ( dimY - Y == 0 )        // First Row Is Always Valid
        valid = true;

    else if ( X == 1 ){
        if( isEmpty(X, Y+1) && isEmpty(X+1, Y+1) )      // Condition For First Column
            valid = true;
    }

    else if ( X == dimX ){
        if( isEmpty(X, Y+1) && isEmpty(X-1, Y+1) && isEmpty(X-1, Y) )      // Condition For Last Column
            valid = true;
    }

    else if ( isEmpty(X, Y+1) && isEmpty(X-1, Y+1) &&
              isEmpty(X-1, Y) && isEmpty(X+1, Y+1) ) // Condition For Any Other Position
        valid = true;

    return valid;
}

// Function To Ensure The Gold Object Is Not Blocked
bool Map::goldNotBlocked(int x, int y)
{
    char H = 240;
    bool goldReachable = false;

    if ( isEmpty(x, y-1) || isEmpty(x, y+1) ||
         isEmpty(x-1, y) || isEmpty(x+1, y) )
         goldReachable = true; 

    return goldReachable;
}

// Function To Check If Given Position Is Empty or Not
bool Map::isEmpty(int X, int Y)
{
    bool empty = false;
    
    if (isInsideMap(X, Y))      // Check First If Position is Inside Map
        if ( map[dimY-Y][X-1] == ' ')
            empty = true;

    return empty;
}

// Function To Check If Object at Given Position is Hole or Not
bool Map::isHole(int X, int Y)
{
    bool flag = false;

    if ( map[dimY-Y][X-1] == char(233) )
        flag = true;
    
    return flag;
}

// Function To Check If Object at Given Position is Trap or Not
bool Map::isTrap(int X, int Y)
{
    bool flag = false;

    if ( map[dimY-Y][X-1] == char(35) )
        flag = true;
    
    return flag;
}

// Function To Check If Object at Given Position is Gold or Not
bool Map::isGold(int X, int Y)
{
    bool flag = false;

    if ( map[dimY-Y][X-1] == char(4))
        flag =  true;

    return flag;
}

// Function To Check If Object at Given Position is Hill or Not
bool Map::isHill(int X, int Y)
{
    bool flag = false;

    if ( map[dimY-Y][X-1] == char(178))
        flag =  true;

    return flag;
}

// Function To Check If Object at Given Position is Rock or Not
bool Map::isRock(int X, int Y)
{
    bool flag = false;

    if ( map[dimY-Y][X-1] == char(237))
        flag =  true;

    return flag;
}

// Function To Check If Object at Given Position is Fixer/Repair or Not
bool Map::isRepair(int X, int Y)
{
    bool flag = false;

    if ( map[dimY-Y][X-1] == char(224))
        flag =  true;

    return flag;
}

// Function To Check If Given Postion Is Within The Map Boundaries
bool Map::isInsideMap(int X, int Y)
{
    bool inside = true;

    if ( !(( Y <= dimY && Y > 0 ) && ( X <= dimX && X > 0 )) )
        inside = false;

    return inside;
}

// Function To Check If Given Position Is Position Reserved For Rover
bool Map::roverPosition(int Y, int X)
{
    int posX = round(dimX/2.0);
    int posY = round(dimY/2.0);
    bool isRoverPosition = false;

    if ( ( X == posX ) && ( Y == posY ) )
        isRoverPosition = true;
    
    return isRoverPosition;
}

// Function To set Map Dimension X To Dimension Given By User
void Map::setDimX(int X)
{
    this->dimX = X;
}

// Function To set Map Dimension Y To Dimension Given By User
void Map::setDimY(int Y)
{
    this->dimY = Y;
}

// Function To set Number Of Golds To Number Given By User
void Map::setGold()
{
    string data, prompt;
    data = "Gold";
    prompt = "Enter Number of Gold: ";
    gold = validateInput(data, prompt, dimY, dimX);
}

// Function To Enusre At Least One Fixer Is In Map
void Map::ensureFixerInMap()
{
    int X,Y;
    int fixCount = 0;
    char F = 224; // Fixer
    
    // A Loop To Determine How Many Fixers Are In The Map
    for (int i = 0; i > dimY; i++)
    {
        for (int j = 0; j < dimX; j++)
        {
            if ( map[i][j] == F ) ;
                fixCount++;
        }
    }

    // If No Fixers Are In Map
    // Loop Until Finding Valid Position To Place Fixer In It
    while ( fixCount < 1 )
    {
        Y = rand()% dimY + 1;
        X = rand()% dimX + 1;

        if ( !roverPosition(Y, X) && isEmpty(X, Y) && validPosition(Y, X) )
        {
            setObject(X, Y, F);
            fixCount++;
        }
    }
}

// Function To Place Object at The Given Position
void Map::setObject (int X,int Y, char ch)
{
    map[dimY-Y][X-1] = ch;
}

// Function To Resize The Map According To Dimension Y and Dimension X
void Map::resizeMap()
{
    map = new char*[dimY];  // Map Rows

    for (int i = 0; i < dimY; i++)  // Number Of Columns in Each Row
        map[i] = new char[dimX];

    // Initialze All Map Positions To Empty Space For Further Validation Process
    for (int i = 1; i <= dimY; i++)
    {
        for (int j = 1; j <= dimX; j++)
            setObject(j, i, ' ');
    }
}

// Function To Initialize Hidden Map with '?' character
void Map::initMap()
{
    char ch = '?';
    
    for (int i = 1; i <= dimY; i++)
    {
        for(int j = 1; j <= dimX; j++)
            if ( roverPosition(i, j) )
                setObject(j, i, ' ');
            else
                setObject(j, i, ch);
    }
}

// Overload of Fucntion initMap
// To Initialize Original Map With Random Characters From charList
void Map::initMap(char charlist[])
{
    int y = round(dimY/2.0);
    int x = round(dimX/2.0);

    for (int i = dimY; i >= 1; i--)
    {
        for(int j = 1; j <= dimX; j++)
        {
            int r = rand() % 30;
            char ch = charlist[r];

            if ( roverPosition(i, j) || !validPosition(i, j))
                continue;
            else
            {
                if( roverStuck(y,x) )
                    continue;
                else
                    setObject(j, i, ch);
            }
        }
    }

    ensureFixerInMap();
}

// Function To Distribute Gold in Mars Map
void Map::goldScatter()
{
    int goldCount = 0;
    char G = 4;

    while (goldCount != gold)
    {    
        int y = rand()% dimY + 1;
        int x = rand()% dimX + 1;

        // Check If Number of Golds is Equal To Maximum Gold Ammount
        // And If Yes Set All Map Objects To Gold Except Rover Position
        if ( gold == (dimX * dimY - 1) )
        {
            for (int i = 1; i <= dimY; i++)
            {
                for(int j = 1; j <= dimX; j++)
                {
                    if ( roverPosition(i, j) )
                        setObject(j, i, ' ');
                    else
                    {
                        setObject(j, i, G);
                        goldCount++;
                    }
                }
            }
        }

        // Normal Gold Position Validation
        else
        {
            if ( isEmpty(x,y) && !roverPosition(y,x) && goldNotBlocked(x,y) )
            {
                setObject(x, y, G);
                goldCount++;
            }
        }
    }
}

// Function To Display The First Line Border Of The Map
void Map::firstLine()
{
    // char(205) ==> '═'
    // char(201) ==> '╔'
    // char(203) ==> '╦'
    // char(187) ==> '╗'
    cout << "  ";
    cout << CYAN << char(201) << RESET;
    for (int i = 0; i < dimX; i++)
    {
        if (i < dimX-1)
            cout << CYAN << char(205) << char(205) << char(205) << char(203) << RESET;
        else
            cout << CYAN << char(205) << char(205) << char(205) << char(187) << RESET;
    }
    cout << endl;
}

// Function To Display The In Between Map Line Borders
void Map::middleLine()
{
    // char(205) ==> '═'
    // char(206) ==> '╬'
    // char(204) ==> '╠'
    // char(185) ==> '╣'
    cout << "  ";
    cout << CYAN << char(204) << RESET;
    for (int i = 0; i < dimX; i++)
    {
        if (i < dimX-1)
            cout << CYAN << char(205) << char(205) << char(205) << char(206) << RESET;
        else
            cout << CYAN << char(205) << char(205) << char(205) << char(185) << RESET;
    }

    cout << endl;
}

// Function To Display The Last Line Border Of The Map
void Map::lastLine()
{
    // char(205) ==> '═'
    // char(202) ==> '╩'
    // char(200) ==> '╚'
    // char(188) ==> '╝'
    cout << "  ";
    cout << CYAN << char(200) << RESET;
    for (int i = 0; i < dimX; i++)
    {
        if (i < dimX-1)
            cout << CYAN << char(205) << char(205) << char(205) << char(202) << RESET;
        else
            cout << CYAN << char(205) << char(205) << char(205) << char(188) << RESET;
    }

    cout << endl;
}

// Function To Manage Map Objects Colors
void Map::colorManager(int i, int j)
{
    if ( map[i][j] == char(4) ) // Gold
        cout << YELLOW << setw(2) << map[i][j] << RESET;
    
    else if ( map[i][j] == '#' ) // Trap
        cout << BOLDRED << setw(2) << map[i][j] << RESET;
    
    else if ( map[i][j] == char(237) ) // Rock
        cout << RED << setw(2) << map[i][j] << RESET;
    
    else if ( map[i][j] == char(224) ) // Fixer
        cout << GREEN << setw(2) << map[i][j] << RESET;
    
    else if ( map[i][j] == char(178) ) // Hill
        cout << MAGENTA << setw(2) << map[i][j] << RESET;

    else if ( map[i][j] == char(233) ) // Hole
        cout << BOLDMAGENTA << setw(2) << map[i][j] << RESET;
    
    else if ( map[i][j] == '?' ) // Hidden
        cout << BLUE << setw(2) << map[i][j] << RESET;

    else if ( map[i][j] == '@' ) // Trapped
        cout << BOLDRED << setw(2) << map[i][j] << RESET;

    else if ( map[i][j] == char(16) || map[i][j] == char(17) ||
              map[i][j] == char(30) || map[i][j] == char(31) ) // Rover
        cout << BOLDGREEN << setw(2) << map[i][j] << RESET;
    else
        cout << setw(2) << map[i][j];
}

// Function To Diplay The Map
void Map::display()
{
    // char(186) ==> '║'
    cout << right;  // Reset Justification To Right
    firstLine();    // Display First Line Border
    for (int i = 0; i < dimY; i++)
    {
        cout << setw(2) << (dimY-i) << CYAN << char(186) << RESET;
        for (int j = 0; j < dimX; j++){
            colorManager(i, j);   
            cout << CYAN << setw(2) << char(186) << RESET;  
        }
        cout << endl;
        if (i < dimY - 1)
            middleLine();
        else
            lastLine(); 
    }

    if ( dimX >= 10 )
    {
        cout << " ";
        for (int i = 1; i <= dimX; i++)
        {
            int num = i/10;
            if ( num == 0)
                cout << setw(4) << " ";
            else
                cout << setw(4) << num;
        }
        cout << endl;
    }

    cout << " ";
    for (int i = 1; i <= dimX; i++)
        cout << setw(4) << i%10;

    cout << endl << endl;
}