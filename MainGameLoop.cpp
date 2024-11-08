/*
A201 - Group Project
Version: 1.0
Log (Please Log all changes):
Tuesday November 5th - Primary game loop and input validation created - Axel
Thursday November 7th - Save data/loading save file implementation (Subject to change as the game becomes more developed) - Charlene

*/

#include <iostream>
#include <fstream> 
#include <string> 
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

void minigame_menu() // This is where you earn money. Possibly some real-time little minigame, but just some basic math can be placeholder.
{
    while (true)
    {
        /* code */
    }
}


int main()
{
    bool exit_game = false;
    string name; // Pet Name
    int coins = 20; // Default Money
    int hunger = 100, thirst = 100, happiness = 100; // Default Pet Values

    // Try to load data from the file
    ifstream inFile("pet_data.txt");
    if (inFile) {
        getline(inFile, name);
        inFile >> hunger >> thirst >> happiness; // Takes these values if found
        inFile.close();
        cout << "Pet data loaded: " << name << endl; 
    } else {
        cout << "No pet data found. Create a new pet? (y/n): "; 
        char choice; // Local variable for saving
        cin >> choice;
        cin.ignore(); // Clear newline character from input buffer
        if (choice == 'y' || choice == 'Y') {
            cout << "Enter pet name: ";
            getline(cin, name);
        } else if (choice == 'n' || choice == 'N') {
            cout << "Exiting..." << endl; 
            return 0; 
        } else {
            cout << "Invalid choice. Please enter 'Y' or 'N'. " << endl; 
            return 0;
        }
    }


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
            ofstream outFile ("pet_data.txt");
            if (!outFile) {
                cout << "Error opening save file! Please check file permissions or disk space. " << endl; 
                break; 
            }
            outFile << name << endl; 
            outFile << hunger << " " << thirst << " " << happiness << endl; // Save stats
            
            outFile.close(); // Close after writing
            
            cout << "Pet data saved successfully." << endl; 
            cout << "---Quitting Game---" << endl;
            break;
        }
        
    }


    return 0;
}
