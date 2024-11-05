/*
A201 - Group Project
Version: 1.0
Log (Please Log all changes):
Tuesday November 5th - Primary game loop and input validation created - Axel

*/

#include <iostream>
using namespace std;

void invalid_input() // Error function
{
    cin.clear();
    cin.ignore(1000, '\n');
    cout << "Invalid Input. Please enter a valid number." << endl;
}


void shop_menu() // This is where you spend money on items for your pet.
{
    while (true)
    {
        /* code */
    }
}

void pet_menu() // This is where you feed, water, and play with the pet. It also serves as an inventory.
{
    while (true)
    {
        /* code */
    } 
}

void minigame_menu() // This is where you earn money. Possibly some real time little minigame, but just some basic math can be placeholder.
{
    while (true)
    {
        /* code */
    }
}


int main()
{
    bool exit_game = false;
    //        ---Implement saving pet data from file later---
    int coins = 20; // Default Money
    int hunger = 100, thirst = 100, happyness = 100; // Default Pet Values

    while (true) // Main Game Loop
    {
        int user_input;
        cout << "------------------------------------------" << endl;
        cout << "1) Shop Menu" << endl;
        cout << "2) Pet Menu" << endl;
        cout << "3) MiniGame Menu" << endl;
        cout << "4) Exit Game" << endl;

        while (true) // Input Validation
        {
            cout << "Enter Input: ";
            cin >> user_input;

            if (cin.fail() || !(cin.peek() == '\n') || user_input > 4 || user_input < 1)
            {
                invalid_input();
            }else
                break;
        }

        switch (user_input) // Send User to Proper Menu
        {
        case 1:
            shop_menu();
            break;
        case 2:
            pet_menu();
            break;
        case 3:
            minigame_menu();
            break;
        case 4:
            exit_game = true;
            break;
        
        default:
            break;
        }

        if (exit_game)
        {
            cout << "---Quitting Game---" << endl;
            break;
        }
        
    }


    return 0;
}