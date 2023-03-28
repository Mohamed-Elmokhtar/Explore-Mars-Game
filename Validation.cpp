#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <conio.h>
#include <limits>
#include "Validation.h"
using namespace std;

// Function To Display User Guide To Valid Input
void validInput(string data)
{
    system("cls");

    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    
    if ( data == "Gold" )
    {
        cout << "| Gold Number Input Instructions:                      |" << endl;
        cout << "|   1. Number Of Golds Cannot Be Less Than 0.          |\n";
        cout << "|   2. Number of Golds Cannot Be Greater Than          |\n";
        cout << "|      Number Of Cells In The Map - 1.                 |\n";
        cout << "|   3. Number Of Golds Cannot Be Greater Than          |\n";
        cout << "|      Half The Number Of Cells In The Map             |\n";
        cout << "|-.-.-.-..-..-..-..-..-..-..-..-..-..-..-..-..-..-..-.-|\n";
        cout << "| Important Notes:                                     |\n";
        cout << "|   1. Please Follow Intsruction To Enusre The Game    |\n";
        cout << "|      Always Runs Smoothly And Error-Free             |\n";
        cout << "|   2. For Instruction Number 3, Note Down That Any    |\n";
        cout << "|      Decimal Number Obtained After Dividing Number   |\n";
        cout << "|      Of Cells In The Map By 2 Is Rounded Down        |\n";  
    }
    
    else if ( data == "Dimensions")
    {
        cout << "| Map Dimensions Input Instructions:                   |" << endl;
        cout << "|   Dimension Y And Dimension X Must Be Greater Than 0.|\n";
        cout << "|   If Dimension Y is 1 then Dimension X Must Be       |\n";
        cout << "|   Greater Than 1.                                    |\n";
        cout << "|   If Dimension X is 1 then Dimension Y Must Be       |\n";
        cout << "|   Greater Than 1.                                    |\n";
    }

    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
}

void instructionRequest(string data)
{    
    char ch;
    do
    {
        cout << "Would You Like To See Valid Input Instructions (Y/N) => ";
        ch = toupper(getch());
        cout << ch << endl;

        if ( ch == 'Y' )
        {    
            validInput(data);
            system("pause");
            system("cls");
            break;    
        }
    } while (ch != 'N');
}

// Input Validation Function
int validateInput( string data, string userPrompt, int Y, int X )
{
    string data_str, error = "Invalid Input Detected!!";        // String To Take The User Input
    int checked_data;    // To Hold Accepted Data Type From The User Input
    
    while(true)        // Keeps Asking User For Input And Display Error Until Valid Data Given
    {
        cout << userPrompt;
        getline(cin, data_str);         // Captures The User Input
        stringstream entry(data_str);   // Convert The Input String Into String Stream
        
        entry >> checked_data;                  // Put Numerical Literals In Stream Into checked_data
        
        if (!(entry.fail() || !entry.eof()))    // Checks If User Input Into Stream Has Only Numbers or Stream is Empty
            if ( data != "Dimensions" )     // Validating Gold Number
                if( (checked_data > 0) && ( (checked_data ==  (Y * X - 1)) ||
                    (checked_data <= ((Y * X)/2.0)) ) )
                    break;
                else
                {   
                    cout << error << endl;
                    cin.clear();
                }
            else        // Extra Validation For Dimensions
                if ( Y == 0 && checked_data > 0 )   // Validates Dimension Y is Greater Than 0
                    break;
                else if ( Y == 1 && checked_data > 1)   // Validates Dimension X is Greater Than 1 When Dimension Y is 1
                    break;
                else if ( Y > 1 && checked_data > 0 )   // Validates Both Dimension Y and Dimension X Are Greater Than 0
                    break;
                else
                {   
                    cout << error << endl;
                    cin.clear();
                }       
        else
        {
            cout << error << endl;
            cin.clear();
        }

        instructionRequest(data);
    }
    // Return checked_data If It Is Valid
    return checked_data;
}