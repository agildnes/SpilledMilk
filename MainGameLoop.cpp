/*
A201 - Group Project
Version: 1.0
Log (Please Log all changes):
Tuesday, November 5th - Primary game loop and input validation created - Axel
Thursday, November 7th - Save data/loading save file implementation (Subject to change as the game becomes more developed) - Charlene
Sunday, November 10th - Set up basic minigame menu (Assuming there will be at least a few mini-games, therefore multiple options) - Charlene
Tuesday November 12th - Set up the shop menu, the inventory system still isn't up yet, so it isn't very useful. - Axel
Sunday November 17th - Set up basic inventory, but I expect this to be modified again in the future once game is more developed - Charlene

*/

#include <iostream>
#include <fstream> 
#include <string> 
// #include <cstdlib> (Might use for minigames)
// #include <ctime>  (Might use for minigames)
// #include <limits> (For inventory)

using namespace std;

void invalid_input() // Error function
{
    cin.clear();
    cin.ignore(1000, '\n');
    cout << "Invalid Input. Please enter a valid number." << endl;
}


void shop_menu(int &coins) // This is where you spend money on items for your pet.
{
    while (true)
    {
        int user_input;
        bool leave_shop = false;

        cout << "----------------[ Shop Menu ]-----------------" << endl;
        cout << "1) Water - 3 coins" << endl;
        cout << "2) Food - 5 coins" << endl;
        cout << "3) Toy - 10 coins" << endl;
        cout << "4) Exit Menu" << endl;

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
        
        switch (user_input)
        {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                leave_shop = true;
                break;
            
            default:
                break;
        }

        if (leave_shop)
        {
            break;
        }
        
    }
}

void pet_menu(int &hunger, int &thirst, int &happiness) // This is where you feed, water, and play with the pet. It also serves as an inventory.
{
    while (true)
    {
        /*

        // Inventory Size
        const int MAX_INVENTORY_SIZE = 5; 
        string inventory[MAX_INVENTORY_SIZE];
        int itemCount = 0; 

        //Determine if inventory is full 
        if (itemCount >= MAX_INVENTORY_SIZE) {
            cout << "Inventory full!" << endl; 
            return; 
        }

        //Determine if inventory is empty
        if (itemCount == 0) {
            cout << "Your inventory is empty." << endl; 
            return; 
        }

        // Iterate through inventory 
        cout << "Your inventory: "; 
        for (int i = 0; i < itemCount; ++i) {
            cout << inventory[i] << " "; 
        }
        cout << endl; 
        
        */
    } 
}


void minigame_menu() 
{
   while (true) {
        // Display the menu
        cout << "-------------[ Mini Games ]---------------" << endl;
        cout << "1) Game Name 1" << endl;
        cout << "2) Game Name 2" << endl;
        cout << "3) Game Name 3" << endl;
        cout << "4) Exit" << endl;

        int choice;
        cin >> choice; 

        switch (choice) {
            case 1: {
                cout << "Welcome to Game Name 1!" << endl; // Possible game idea: Blackjack 
                break; 
            }
            case 2: {
                cout << "Welcome to Game Name 2!" << endl; // Possible game idea: Dice Rolls
                break;
            }
            case 3: {
                cout << "Welcome to Game Name 3!" << endl; // Possible game idea: Daily Spin 
                break;
            }
            case 4: {
                cout << "Exiting Mini Games... Goodbye!" << endl;
                return; 
            }
            default: {
                cout << "Invalid choice. Please try again!" << endl;
                break; 
            }
        }
    }
}


int main()
{
    bool exit_game = false;
    string name; // Pet Name
    int coins = 20; // Default Money
    int hunger = 100, thirst = 100, happiness = 100; // Default Pet Values 
    // decay - (thirst 3x) (hunger 2x) (happiness 1x) exact difference in value is subject to change, however they should not decay at the same rate

    // cout << "\033[H\033[J"; (Will put here for now, but this is used to clear screen)
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
        cout << "----------------[ Main Menu ]-----------------" << endl;
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
            shop_menu(coins);
            break;
        case 2:
            pet_menu(hunger, thirst, happiness);
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
