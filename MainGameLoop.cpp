/*
A201 - Group Project
Version: 1.0
Log (Please Log all changes):
Tuesday, November 5th - Primary game loop and input validation created - Axel
Thursday, November 7th - Save data/loading save file implementation (Subject to change as the game becomes more developed) - Charlene
Sunday, November 10th - Set up basic minigame menu (Assuming there will be at least a few mini-games, therefore multiple options) - Charlene
Tuesday November 12th - Set up the shop menu, the inventory system still isn't up yet, so it isn't very useful. - Axel
Sunday November 17th - Set up basic inventory, but I expect this to be modified again in the future once game is more developed - Charlene
Sunday November 24th - Clear screen/terminal function, ASCII art, shop menu inventory/switch case handles coins and inventory, inventory saved to file implemented - Charlene
Sunday November 24th - Fixed ASCII art, added pet_Stats_Decay function to keep track of needs decay, added pet_Stats function to display stats, implemented inventory in shop menu for a refund 
(so users can delete items), created global const variable for inventory, added a condition to show happy pet art or sad pet art based on needs - Charlene
Monday November 25th - Add screen clear for apple
Tuesday November 26th - Added incomplete exploration minigame
Tuesday November 26th - Fixed Shop Menu exit, reworked Shop Menu to be more user friendly, other minor QOL changes. - Axel
*/

#include <iostream>
#include <fstream> 
#include <string> 
#include <cstdlib> // ("cls" and "clear")
#include <ctime>  // (Might use for minigames)
#include <limits> // (For inventory)

using namespace std;

const int MAX_INVENTORY_SIZE = 5; 

void clearScreen() 
{
// Clear screen based on platform
#if defined _WIN32
	system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	system("clear");
#elif defined (__APPLE__)
	system("clear");
#endif
}

void invalid_input() // Error function
{
    cin.clear();
    cin.ignore(1000, '\n');
    cout << "Invalid Input. Please enter a valid number." << endl;
}

void pet_Avatar_Happy() 
{
    cout << "/\\_/\\ ♥\n"
         << ">^W^<\n"
         << " / \\\n"
         << "(___)_/\n" << endl;
}

void pet_Avatar_Sad() 
{
    cout << "/\\_/\\ ☹\n"
         << ">=A=<\n"
         << " / \\\n"
         << "(___)_/\n" << endl;
}

void pet_Stats_Decay(int &hunger, int &thirst, int &happiness) // this needs to be signaled by another program if we want constant change
{

    thirst = thirst - 3; 
    hunger = hunger - 2; 
    happiness = happiness - 1;
    
    if (hunger < 0) hunger = 0; // Hunger can't go past zero
    if (thirst < 0) thirst = 0; // Thirst can't go past zero
    if (happiness < 0) happiness = 0; // Happiness can't go past zero
}

void pet_Stats(int hunger, int thirst, int happiness) {
cout << "Hunger: " << hunger << " | Thirst: " << thirst << " | Happiness: " << happiness << endl; }


void shop_menu(int &coins, string inventory[], int &itemCount) // This is where you spend money on items for your pet.
{
    while (true)
    {
        int user_input;
        bool leave_shop = false;

        cout << "----------------[ Shop Menu ]-----------------" << endl;
        if (coins >= 10)
            cout << "                [ Coins: " << coins << " ]" << endl;
        else
            cout << "                [ Coins: " << coins << "  ]" << endl;
        cout << "1) Water - 3 coins" << endl;
        cout << "2) Food - 5 coins" << endl;
        cout << "3) Toy - 10 coins" << endl;
        cout << "4) Sell item" << endl;
        cout << "5) Exit Menu" << endl; 

        while (true) // Input Validation
        {
            cout << "Enter Input: ";
            cin >> user_input;

            if (cin.fail() || !(cin.peek() == '\n') || user_input > 5 || user_input < 1)
            {
                invalid_input();
            } else {
                break;
        }
     }
        
      switch (user_input) {
            case 1: // Water
                if (coins >= 3) {
                    if (itemCount < MAX_INVENTORY_SIZE) {
                    coins -=3;
                    inventory[itemCount++] = "Water"; 
                    cout << "You purchased Water! Remaining inventory space: " << MAX_INVENTORY_SIZE - itemCount << endl; 
                    } else {
                    cout << "Inventory full!" << endl; 
                    }
                } else {
                cout << "Not enough coins!" << endl; 
                }
                break;
            
            case 2: // Food
                if (coins >= 5) {
                    if (itemCount < MAX_INVENTORY_SIZE) {
                    coins -=5;
                    inventory[itemCount++] = "Food";
                    cout << "You purchased Food! Remaining inventory space: " << MAX_INVENTORY_SIZE - itemCount << endl; 
                     } else {
                    cout << "Inventory full!" << endl; 
                    }
                } else {
                cout << "Not enough coins!" << endl; 
                }
                break;
            
            case 3: // Toy
                if (coins >= 10) {
                    if (itemCount < MAX_INVENTORY_SIZE) {
                    coins -= 10;
                    inventory[itemCount++] = "Toy";
                    cout << "You purchased a Toy! Remaining inventory space: " << MAX_INVENTORY_SIZE - itemCount << endl; 
                    } else {
                    cout << "Inventory full!" << endl; 
                    }
                } else {
                cout << "Not enough coins!" << endl; 
                }
                break;

            case 4: // Sell an item and get a refund
              if (itemCount == 0) {
                  cout << "You have no items to sell!" << endl; 
              } else {
                  cout << "Your inventory: " << endl; 
                      for (int i = 0; i < itemCount; ++i) {
                          cout << i + 1 << ") " << inventory[i] << endl; }
                  
                int item_Sell; 
                cout << "Enter the number of the item you want to sell: " << endl;
                cin >> item_Sell; 

                // Input validation
                if (cin.fail() || !(cin.peek() == '\n') || item_Sell > itemCount) {
                    invalid_input(); 
                    break;
                }
                  
                item_Sell--; 
                string item = inventory[item_Sell]; 

                int refund = 0; 
                if (item == "Water") {
                    refund = 2; // User gets half coins (round up) 
                } else if (item == "Food") { // User gets half coins (round up)
                    refund = 3; 
                } else if (item == "Toy") { // User gets half coins (round up)
                    refund = 5;
                }

                coins += refund; 
                if (refund > 1) {
                    cout << item << " sold! You received " << refund << " coins." << endl;
                } else {
                    cout << item << " sold! You received " << refund << " coin." << endl;
                }


                for (int i = item_Sell; i < itemCount - 1; ++i) {
                    inventory[i] = inventory[i+1]; // Shift items to left
                }
                itemCount--; 
              }
              break; 
          
            case 5:
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

void pet_menu(int &hunger, int &thirst, int &happiness, string inventory[MAX_INVENTORY_SIZE], int &itemCount) // This is where you feed, water, and play with the pet. It also serves as an inventory.
{
    while (true)
    {
        /*
        // Inventory Size
        string inventory[MAX_INVENTORY_SIZE];
        int itemCount = 0; 

        //Determine if inventory is full 
        if (itemCount >= MAX_INVENTORY_SIZE) {
            cout << "Inventory full! " << endl; 
            return; 
        }

        //Determine if inventory is empty
        if (itemCount == 0) {
            cout << "Your inventory is empty. " << endl; 
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

//Exploration minigame!
class explorationGame 
{
public:
    int numCoins = 0;
    
    //Displays the whole field. Currently used for testing.
    void printField() 
    {
        for (int iy = 0; iy < ysize; iy++)
        {
            for (int ix = 0; ix < xsize; ix++)
            {
                cout << field[ix][iy] << ' ';
            }
            cout << '\n';
        }
    }

    // Constructor. When you make the class, this runs automatically.
    explorationGame() 
    {
        srand(time(0)); // Randomize game
        setField();
        exploreLoop();
    }
private: 
    // Map size in tiles.
    static const int xsize = 50;
    static const int ysize = 50;
    static const int visionRadius = 5;
    enum eventKey { EXIT = 0, BORDER = 1, COIN = 2, ENCOUNTER = 3 };
    int energy = 50;

    char field[xsize][ysize];

    int playerCoords[2] = { xsize / 2, ysize / 2 };

    // Each function ending with gen adds a layer to the map.
    void rivergen()
    {
        ;
    }

    void treegen()
    {
        int mnum = rand() % 6 + 24; // Generates random amount of foliage
        for (int im = 0; im <= mnum; im++)
        {
            int mx = rand() % 50;
            int my = rand() % 50;
            int msize = rand() % 7 + 2; // Foliage radius
            while (((mx - msize <= 25) && (mx + msize >= 25)) && ((my - msize <= 25) && (my + msize >= 25)))
            {
                mx = rand() % 50;
                my = rand() % 50;
            }
            field[mx][my] = ';';
            for (int i = 0; i <= msize; i++) // Fills foliage
            {
                if (mx + i < 50 && mx + i >= 0) // Right side
                {
                    for (int iy = my; (iy >= 0 && iy <= 49) && iy - my <= msize - i; iy++)
                    {
                        field[mx + i][iy] = ';';
                    }
                    for (int iy = my; (iy >= 0 && iy <= 49) && my - iy <= msize - i; iy--)
                    {
                        field[mx + i][iy] = ';';
                    }
                }
                if (mx - i < 50 && mx - i >= 0) // Left side
                {
                    for (int iy = my; (iy >= 0 && iy <= 49) && iy - my <= msize - i; iy++)
                    {
                        field[mx - i][iy] = ';';
                    }
                    for (int iy = my; (iy >= 0 && iy <= 49) && my - iy <= msize - i; iy--)
                    {
                        field[mx - i][iy] = ';';
                    }
                }
            }
        }
    }

    void mountaingen()
    {
        int mnum = rand() % 6 + 4; // Generates random number of mountains
        for (int im = 0; im <= mnum; im++)
        {
            int mx = rand() % 50;
            int my = rand() % 50;
            int msize = rand() % 4 + 4; // Mountain radius
            while (((mx - msize <= 25) && (mx + msize >= 25)) && ((my - msize <= 25) && (my + msize >= 25)))
            {
                mx = rand() % 50;
                my = rand() % 50;
            }
            field[mx][my] = 'M';
            for (int i = 0; i <= msize; i++) // Fills mountains
            {
                if (mx + i < 50 && mx + i >= 0) // Right side
                {
                    for (int iy = my; (iy >= 0 && iy <=49) && iy - my <= msize - i; iy++)
                    {
                        field[mx + i][iy] = 'M';
                    }
                    for (int iy = my; (iy >= 0 && iy <= 49) && my - iy <= msize - i; iy--)
                    {
                        field[mx + i][iy] = 'M';
                    }
                }
                if (mx - i < 50 && mx - i >= 0) // Left side
                {
                    for (int iy = my; (iy >= 0 && iy <= 49) && iy - my <= msize - i; iy++)
                    {
                        field[mx - i][iy] = 'M';
                    }
                    for (int iy = my; (iy >= 0 && iy <= 49) && my - iy <= msize - i; iy--)
                    {
                        field[mx - i][iy] = 'M';
                    }
                }
            }
        }
    }

    void locationgen()
    {
        for (int iy = 0; iy < 50; iy++)
        {
            for (int ix = 0; ix < 50; ix++)
            {
                if (field[ix][iy] == 'M')
                    continue;
                else if (field[ix][iy] == '.')
                {
                    if (rand() % 60 == 1) // Coin
                    {
                        field[ix][iy] = '@';
                    }
                    else if (rand() % 60 == 1) // Encounter
                    {
                        field[ix][iy] = 'E';
                    }
                }
                else if (field[ix][iy] == ';')
                {
                    if (rand() % 60 == 1) // Coin
                    {
                        field[ix][iy] = '@';
                    }
                    else if (rand() % 30 == 1) // Encounter
                    {
                        field[ix][iy] = 'E';
                    }
                }
                else if (field[ix][iy] == 'S')
                {
                    if (rand() % 30 == 1) // Coin
                    {
                        field[ix][iy] = '@';
                    }
                    else if (rand() % 60 == 1) // Encounter
                    {
                        field[ix][iy] = 'E';
                    }
                }
            }
        }
    }
   
    void setField()
    {
        for (int iy = 0; iy < ysize; iy++) //Initializes board with flat ground
        {
            for (int ix = 0; ix < xsize; ix++)
            {
                field[ix][iy] = '.';
            }
        }
        treegen();

        mountaingen(); // Places mountain tiles

        rivergen(); // Places river tiles

        locationgen(); // Places coins & encounters

        for (int i = 0; i < 50; i++) // Sets the map border
        {
            field[i][0] = 'X';
        }
        for (int i = 0; i < 50; i++)
        {
            field[0][i] = 'X';
        }
        for (int i = 0; i < 50; i++)
        {
            field[49][i] = 'X';
        }
        for (int i = 0; i < 50; i++)
        {
            field[i][49] = 'X';
        }
    }

    void encounter()
    {
        ;
    }

    // Only prints player vision
    void printVision()
    {
        for (int iy = playerCoords[1] - visionRadius; iy <= playerCoords[1] + visionRadius; iy++)
        {
            if (iy)
                cout << '\n';
            if (iy < 0 || iy >= 50)
            {
                for (int i = 0; i <= 10; i++)
                    cout << "  ";
                continue;
            }
            else
            {
                for (int ix = playerCoords[0] - visionRadius; ix <= playerCoords[0] + visionRadius; ix++)
                {
                    if (iy < 0 || iy >= 50)
                    {
                        cout << "  ";
                    }
                    else if (ix == playerCoords[0] && iy == playerCoords[1])
                        cout << 'O ';
                    else
                    {
                        cout << field[ix][iy] << " ";
                    }
                }
            }
        }
        cout << endl;
    }

    int getInput()
    {
        while (1)
        {
            char tile = ' '; // Type of tile.
            int pos[2] = { 0, 0 }; // Change in player position.
            char input = getchar();
            if (input == '!')
            {
                return EXIT;
            }
            if (input == 'w' || input == 'W')
            {
                tile = field[playerCoords[0]][playerCoords[1] - 1];
                pos[1] = -1;
            }
            if (input == 'a' || input == 'A')
            {
                tile = field[playerCoords[0] - 1][playerCoords[1]];
                pos[0] = -1;
            }
            if (input == 's' || input == 'S')
            {
                tile = field[playerCoords[0]][playerCoords[1] + 1];
                pos[1] = 1;
            }
            if (input == 'd' || input == 'D')
            {
                tile = field[playerCoords[0] + 1][playerCoords[1]];
                pos[0] = 1;
            }

            if (tile == 'X') // Border
                return BORDER;
            // Apply change in player position.
            playerCoords[0] += pos[0];
            playerCoords[1] += pos[1];
            if (tile == '@') // Coin
            {
                field[playerCoords[0]][playerCoords[1]] = '.';
                return COIN;
            }
            if (tile == 'E') // Encounter
            {
                field[playerCoords[0]][playerCoords[1]] = '.';
                return ENCOUNTER;
            }
            if (tile == '.') // Grass
                energy -= 1;
            if (tile == ';') // Foliage
                energy -= 2;
            if (tile == 'S') // River
                energy -= 3;
            
        }
    }
    // Game loop
    int exploreLoop() 
    {
        int tempCoins;
        // You can think of it as the max amount of time in the day, if you like.
        int escape = 0;
        while (escape < 300)
        {
            clearScreen();
            printVision();
            switch (getInput())
            {
            case EXIT:
                return 0;
            case BORDER:
                clearScreen();
                printVision();
                cout << "You shouldn't stray too far from home.";
                break;
            case COIN:
                tempCoins = rand() % 3 + 1;
                numCoins += tempCoins;
                clearScreen();
                printVision();
                cout << "\nYou found " << tempCoins << " coins!" << endl;
                break;
            case ENCOUNTER:
                clearScreen();
                printVision();
                encounter();
                break;
            default:
                break;
            }


        }
        return 0;
    }
    
};
// --- END OF EXPLORATION MINIGAME ---

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
                explorationGame *fish = new explorationGame;
                delete fish;
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
    string inventory[MAX_INVENTORY_SIZE];
    int itemCount = 0; 
    int hunger = 100, thirst = 100, happiness = 100; // Default Pet Values 
    // decay - (thirst 3x) (hunger 2x) (happiness 1x) exact difference in value is subject to change, however they should not decay at the same rate

    
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
            clearScreen(); 
           
            // Display happy or sad art depending on needs
            if (hunger >= 50 || thirst >= 50 || happiness >= 50) {
                pet_Avatar_Happy(); 
            } else {
                pet_Avatar_Sad(); 
            }

            //pet_Stats_Decay(hunger, thirst, happiness);
            pet_Stats(hunger, thirst, happiness); 
            
            
            int user_input;
     
            cout << "----------------[ Main Menu ]-----------------" << endl;
            cout << "1) Shop Menu" << endl;
            cout << "2) Pet Menu" << endl;
            cout << "3) Minigames Menu" << endl;
            cout << "4) Exit Game" << endl;
        
    
            while (true) // Input Validation
            {
                cout << "Enter Input: ";
                cin >> user_input;
    
                if (cin.fail() || !(cin.peek() == '\n') || user_input > 4 || user_input < 1)
                {
                    invalid_input();
                } else
                    break;
            }
    
            switch (user_input) // Send User to Proper Menu
            {
            case 1:
                shop_menu(coins, inventory, itemCount);
                break;
            case 2:
                pet_menu(hunger, thirst, happiness, inventory, itemCount);
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
    
            if (exit_game) {
                ofstream outFile ("pet_data.txt");
                if (!outFile) {
                    cout << "Error opening save file! Please check file permissions or disk space. " << endl; 
                    break; 
                }
                
                outFile << name << endl; 
                outFile << hunger << " " << thirst << " " << happiness << endl; // Save stats
    
                outFile << itemCount << endl; // Inventory saving
                for (int i = 0; i < itemCount; ++i) {
                    outFile << inventory[i] << endl; 
                }
                outFile.close(); // Close after writing
                
                cout << "Pet data saved successfully." << endl; 
                cout << "---Quitting Game---" << endl;
                break;
            }
        
        }
    
    return 0;
}
