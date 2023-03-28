#include <iostream>
#include <iomanip>
#include <time.h>
#include <cmath>
#include <cctype>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <limits>
#include "Colors.h"
#include "Rover.h"
using namespace std;

// Enum To Hold Constant Int Values Represnting Character Heading Directin
enum Direction
{
    north = 30,
    east = 16,
    south = 31,
    west = 17
};

// Constructor To Initialeze p_mars pointer To Point To The Original Map Mars
// And Create The Hidden Map
Rover::Rover(Map *mars)
{
    system("cls");
    
    p_mars = mars;

    int Y = p_mars->getDimY();  // For Setting Hidden Map Dimension Y
    int X = p_mars->getDimX();  // For Setting Hidden Map Dimension X

    goldTarget = p_mars->getGold();
    marsHidden.setDimY(Y);
    marsHidden.setDimX(X);
    marsHidden.resizeMap();
    marsHidden.initMap();
    setRoverHealth();
    setRoverFuel();
}

// Function To Send And Land Rover On Planet Mars
void Rover::land()
{
    char possibleHeading[] = {north, east, south, west};

    posX = round(p_mars->getDimX()/(float)2);
    posY = round(p_mars->getDimY()/(float)2);
    
    roverHeading = possibleHeading[ rand() % 4 ];
    marsHidden.setObject(posX,posY,roverHeading);
    threeDisappear();
    marsHidden.display();
    setHealthBar();
    showRoverHealth();
    setFuelBar();
    showRoverFuel();
}

// Function To Set Rover Fuel
void Rover::setRoverFuel()
{
    // Methodolgy Followed In Setting Rover Fuel Is Based On Larger Map Dimension
    if ( p_mars->getDimY() > p_mars->getDimX() )
        maxFuel = p_mars->getDimY();
    else
        maxFuel = p_mars->getDimX();
    
    roverFuel = maxFuel;
}

// Function To Set Rover Health
void Rover::setRoverHealth()
{
    // Methodolgy Followed In Setting Rover Health Is Based On Larger Map Dimension
    if ( p_mars->getDimY() > p_mars->getDimX() )
        maxHealth = p_mars->getDimY();
    else
        maxHealth = p_mars->getDimX();
    
    roverHealth = maxHealth;
}

// Function To Fill The Fule Bar With Fuel Character
void Rover::setFuelBar()
{
    char fuel = 254; 

    for (int i = 0; i < maxFuel; i++)
        fuelBar.push_back(fuel);
}

// Function To Fill The Health Bar With Health Character
void Rover::setHealthBar()
{
    char health = 254; 

    for (int i = 0; i < maxHealth; i++)
        healthBar.push_back(health);
}

// Function To Control Health Incrementation Or Decrementation
void Rover::controlHealth(int Y, int X)
{
    char F = 224;    // Fixer
    char R = 237;   // Rock

    char ch = p_mars->getObject(Y,X);
    
    if ( ch == F && roverHealth != maxHealth )
        roverHealth++;
    
    else if ( ch == R )
    {
        roverHealth--;
        if ( roverHealth == 0 )
            quit();
    }
}

// Function To Control Fuel Incrementation Or Decrementation
void Rover::controlFuel(int Y, int X)
{
    char F = 224;    // Fixer
    char ch = p_mars->getObject(Y,X);
    
    if ( ch == F && roverFuel != maxFuel )
        roverFuel = maxFuel;
    
    // Fuel Decrease By One Point Every 5 moves
    // And Turning Commands Not Counted As Moves
    else if ( moves == 5 )
    {
        moves = 0;
        roverFuel--;
        if ( roverFuel == 0 )
            quit();
    }
}

// Function To Control Health Bar Display
void Rover::showRoverHealth()
{
    // Calculating Remaining Health in Percentage
    int healtRemain = float(roverHealth) / maxHealth * 100; 

    cout << setw(8) << left << "Health :" << " [";
    for (int i = 0; i < maxHealth; i++)
        if (i < roverHealth ){
            if ( healtRemain <= 100 && healtRemain > 70)
                cout << GREEN << healthBar[i] << healthBar[i] << RESET;
            else if ( healtRemain <= 70 && healtRemain > 40 )
                cout << ORANGE << healthBar[i] << healthBar[i] << RESET;
            else
                cout << RED << healthBar[i] << healthBar[i] << RESET;
        }
        else
            cout << ' ' << ' ';
    
    cout << "] " << healtRemain << " %" << endl;  
}

// Function To Control Fuel Bar Display
void Rover::showRoverFuel()
{
    // Calculating Remaining Fuel in Percentage
    int fuelRemain = float(roverFuel) / maxFuel * 100;
    
    cout << setw(8) << left << "Fuel   :" << " [";
    for (int i = 0; i < maxFuel; i++)
        if (i < roverFuel ){
            if ( fuelRemain <= 100 && fuelRemain > 70)
                cout << GREEN << fuelBar[i] << fuelBar[i] << RESET;
            else if ( fuelRemain <= 70 && fuelRemain > 40 )
                cout << ORANGE << fuelBar[i] << fuelBar[i] << RESET;
            else
                cout << RED << fuelBar[i] << fuelBar[i] << RESET;
        }
        else
            cout << ' ' << ' ';
    
    cout << "] " << fuelRemain << " %" << endl << endl;    
}

// Function To Handle What Happens When Rover Falls Into Hole
void Rover::placeRoverRandomly()
{
    int Y,X;

    do
    {
        Y = rand()% p_mars->getDimY() + 1;
        X = rand()% p_mars->getDimX() + 1;

    } while ( p_mars->roverStuck(Y, X) || !p_mars->isEmpty(X,Y) );

    posX = X;
    posY = Y;
    marsHidden.setObject(X,Y,roverHeading);
}

// Function To Take Command String From User
void Rover::setCommand()
{
    cout << "Example Command: MMMLLRRLMR" << endl;
    cout << "Enter the command sequence => ";
    cin >> command;

    numOfCommandSequence +=1;
    numOfCommands += command.length();

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
}

// Function To Handle Actions Required When User Quit Or Loses The Game
void Rover::quit()
{
    win = false;
    endGame = true;
}

// Function To Display Message After Winning Or Losing
// And Display Player Score Rating
void Rover::message()
{
    double highScore = goldTarget * 50 * 0.75;
    double midScore = goldTarget * 50 * 0.25;

    if ( p_mars->isTrap(posX,posY) )
        cout << BOLDRED << "Trapped...Mission Failed...Heading Back To Planet Earth\n" << RESET << endl;
    else if ( roverHealth == 0 )
        cout << BOLDRED << "Rover Destroyed...No More Heakth\n" << RESET << endl;
    else if ( roverFuel == 0 )
        cout << BOLDRED << "No More Fuel...Rover Cannot Move Any More\n" << RESET << endl;
    else if ( goldCollected == goldTarget )
        cout << BOLDYELLOW << "All Gold Collected...Mission Passed\n" << RESET << endl;
    else
        cout << RED << "Quitted Before Collecting Gold\n" << RESET << endl;
    
    if ( win )
    {
        if ( score < 0 )
        {
            cout << RED << "Score: " << score << RESET << endl;
            cout << RED << "Very Low Score...You Are NOOB\n" << RESET << endl;
        }
        else if ( score < midScore )
        {
            cout << ORANGE << "Score: " << score << RESET << endl;
            cout << ORANGE << "Average Score...You Are an AMATEUR\n" << RESET << endl;
        }
        else if ( score < highScore )
        {
            cout << DARKYELLOW << "Score: " << score << RESET << endl;
            cout << DARKYELLOW << "Great Score...You Are SMART\n" << RESET << endl;
        }
        else if ( score >= highScore )
        {
            cout << GREEN << "Score: " << score << RESET << endl;
            cout << GREEN << "World Class Score...You Are a GENIUS\n" << RESET << endl;
        }
    }
    else
    {
        cout << BOLDRED << "Score: HIDDEN " << RESET << endl; 
        cout << BOLDMAGENTA << "You LOST...Better Luck Next Time\n" << endl;
    }
}

// Function To Check If Given Character In Command Can Be Executed
void Rover::commandExecutable(char ch)
{
    system("cls");
    marsHidden.display();
    showRoverHealth();
    showRoverFuel();

    if (execution)
        cout << "Command: Executed (" << ch << ")" << endl;
    else
        cout << "Command: Failed to execute (" << ch << ")" << endl;   

    Sleep(1200);
    system("cls");
    marsHidden.display();
    showRoverHealth();
    showRoverFuel();
}

// Function To Handle Required Actions To Be Taken For Every
// Character In The Command String
void Rover::takeAction()
{
    for (int i = 0; i < command.length(); i++)
    {
        command[i] = toupper(command[i]);
        
        if ( i == 0 ) // Following Commands Need To Be Executed Here At First Run Only
        {
            system("cls");
            marsHidden.display();
            showRoverHealth();
            showRoverFuel();
        }
        cout << "Command: Executing (" << command[i] << ")..." << endl;
        executeCommand(command[i]);
        if ( !endGame )
        {
            Sleep(1500);
            commandExecutable(command[i]);
        }
        else
        {
            Sleep(1500);
            commandExecutable(command[i]);
            break;
        }
    }
}

// Function To Call Required Rover State Change Function
// And Update The execution Varaible
void Rover::executeCommand(char c)
{
    if (c == 'M'){
        move();
    }
    else if (c == 'R'){
        execution = true; 
        turnRight();
    }
    else if (c == 'L'){
        execution = true; 
        turnLeft();
    }
    else if ( c == 'Q'){
        execution = true;
        quit();
    }
    else
        execution = false;

    if ( execution )
        threeDisappear();
}

// Function To Reveal The Three Positions In Front Of The ROver
void Rover:: threeDisappear()
{
    // Methodology Is Based On Determining Current Rover Direction Then
    // Determine Which of The Three Positions In Front Of It Is Inside Map
    // And Reveal The Positions That Are Inside The Map
    if ( roverHeading == char(east) )
    {
        if ( p_mars->isInsideMap( posX+1,posY+1) )
        {
            char object = p_mars->getObject(posY+1,posX+1);
            marsHidden.setObject(posX+1,posY+1,object);
        }
        if ( p_mars->isInsideMap(posX+1,posY))
        {
            char object = p_mars->getObject(posY,posX+1);
            marsHidden.setObject(posX+1,posY,object);
        }

        if ( p_mars->isInsideMap(posX+1,posY-1))
        {
            char object = p_mars->getObject(posY-1,posX+1);
            marsHidden.setObject(posX+1,posY-1,object);
        }
    } 

    else if ( roverHeading == char(west) )
    {
        if (p_mars->isInsideMap(posX-1,posY+1))
        {
            char object = p_mars->getObject(posY+1,posX-1);
            marsHidden.setObject(posX-1,posY+1,object);
        }
        if ( p_mars->isInsideMap(posX-1,posY))
        {
            char object = p_mars->getObject(posY,posX-1);
            marsHidden.setObject(posX-1,posY,object);
        }

        if ( p_mars->isInsideMap(posX-1, posY-1))
        {
            char object = p_mars->getObject(posY-1,posX-1);
            marsHidden.setObject(posX-1,posY-1,object);
        }
    }

    else if ( roverHeading == char(north) ) 
    {
        if (p_mars->isInsideMap(posX,posY+1))
        {
            char object = p_mars->getObject(posY+1, posX);
            marsHidden.setObject(posX,posY+1,object);
        }
        if ( p_mars->isInsideMap(posX-1,posY+1))
        {
            char object = p_mars->getObject(posY+1, posX-1);
            marsHidden.setObject(posX-1,posY+1,object);
        }
        if ( p_mars->isInsideMap(posX+1,posY+1))
        {
            char object = p_mars->getObject(posY+1, posX+1);
            marsHidden.setObject(posX+1,posY+1,object);
        }
    }

    else if ( roverHeading == char(south) )
    {
        if (p_mars->isInsideMap(posX+1,posY-1))
        {
            char object = p_mars->getObject(posY-1,posX+1);
            marsHidden.setObject(posX+1,posY-1,object);
        }
        if ( p_mars->isInsideMap(posX,posY-1))
        {
            char object = p_mars->getObject(posY-1,posX );
            marsHidden.setObject(posX,posY-1,object);
        }

        if ( p_mars->isInsideMap(posX-1, posY-1))
        {
            char object = p_mars->getObject(posY-1,posX-1);
            marsHidden.setObject(posX-1,posY-1,object);
        }
    }
}

// Function To Move Rover From Its Position
void Rover::move()
{
    moves++;    // Increment Moves For Fuel Control

    // Methodology Is Based On Determining Current Rover Direction Then
    // Then Enure The Position To Move To Is Inside Map And Does Not A Hill
    if ( roverHeading == char(east) )
    {       
        if( p_mars->isInsideMap(posX+1, posY) && !p_mars->isHill(posX+1,posY) )
        {
            if ( p_mars->isRock(posX,posY) || p_mars->isRepair(posX,posY))
                marsHidden.setObject(posX, posY, p_mars->getObject(posY, posX));
            
            else
                marsHidden.setObject(posX, posY,' ');
            
            posX += 1;
            execution = true; 
        }
        else
            execution = false;
    }

    else if ( roverHeading == char(west) )
    {
        
        if ( p_mars->isInsideMap(posX-1, posY) && !p_mars->isHill(posX-1,posY) )
        {        
            if ( p_mars->isRock(posX,posY) || p_mars->isRepair(posX,posY) )
                marsHidden.setObject(posX, posY, p_mars->getObject(posY, posX));
            
            else
                marsHidden.setObject(posX, posY,' ');

            posX -= 1;
            execution = true; 
        }
        else
            execution = false;
    }

    else if (roverHeading == char(north) )
    {    
        if( p_mars->isInsideMap(posX, posY+1) && !p_mars->isHill(posX,posY+1) )
        {
            if ( p_mars->isRock(posX,posY) || p_mars->isRepair(posX,posY) )
                marsHidden.setObject(posX, posY, p_mars->getObject(posY, posX));

            else
                marsHidden.setObject(posX, posY,' ');

            posY += 1;
            execution = true;
        }
        else
            execution = false;
    }

    else if ( roverHeading == char(south) )
    {
        if( p_mars->isInsideMap(posX, posY-1) && !p_mars->isHill(posX,posY-1) )
        {
            if ( p_mars->isRock(posX,posY) || p_mars->isRepair(posX,posY) )
                marsHidden.setObject(posX, posY, p_mars->getObject(posY, posX));

            else
                marsHidden.setObject(posX, posY,' ');
            
            posY -= 1;
            execution = true; 
        }
        else
            execution = false;
    }

    // Section Responisbe For Determining Whar Action To Take After Rover Position Updated
    if ( p_mars->isGold(posX, posY) )
    {
        p_mars->setObject(posX, posY, ' ');
        goldScore();
    }

    else if ( p_mars->isHole(posX,posY) )
    {
        marsHidden.setObject(posX, posY, p_mars->getObject(posY, posX));
        placeRoverRandomly();
    }
    
    if ( p_mars->isTrap(posX,posY) )
    {
        marsHidden.setObject(posX, posY, '@');
        quit();
    }
    else
        marsHidden.setObject(posX, posY, roverHeading);
    
    controlHealth(posY, posX);
    controlFuel(posY, posX);
}

// Function To Turn Rover Right
void Rover::turnRight()
{
    if ( roverHeading == char(east) )
        roverHeading = char(south);
    else
        if ( roverHeading == char(west) )
            roverHeading = char(north);
        else
            if ( roverHeading == char(north) )
                roverHeading = char(east);
            else
                roverHeading = char(west);
    
    marsHidden.setObject(posX, posY, roverHeading);
}

// Function To Turn Rover Left
void Rover::turnLeft()
{
    if ( roverHeading == char(east) )
        roverHeading = char(north);
    else
        if ( roverHeading == char(west) )
           roverHeading = char(south);
        else
            if ( roverHeading == char(north) )
                roverHeading = char(west);
            else
                roverHeading = char(east);

    marsHidden.setObject(posX, posY, roverHeading);
}

// Function To Control Gold Score And Keep Track of Collected Golds
void Rover::goldScore()
{
    goldCollected += 1;
    if ( goldCollected == goldTarget )
    {
        win = true;
        endGame = true;
    }
}

// Function To Display Score Calculation Formula And Current User Score
void Rover::displayScore()
{
    cout << "Total Commands Sequence: " << numOfCommandSequence << " [S]" << endl;
    cout << "Total Commands: " << numOfCommands<< " [C]" <<endl;
    cout << "Total Golds: " << goldCollected << " [G] out of " << goldTarget << endl;
    
    score = (goldCollected*50)-(numOfCommandSequence*5)-(numOfCommands*1);
    
    cout<< "Total Score: [G] x 50 - [S] x 5 - [C] x 1 = " << score << endl << endl;
}