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
Wednesday November 27th - Added stats decay function (decay every 5 seconds) - Charlene (I hope I didn't break it LOL)
Friday November 29th - Fixed game, resolved some errors
Friday November 29th - Added functioning pet menu, using items on pet and whatnot - Axel
*/

#include <iostream>
#include <fstream> 
#include <string> 
#include <cstdlib> // ("cls" and "clear")
#include <ctime> 
#include <limits> // (For inventory)
#include <iomanip>

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

void pet_Stats_Decay(int &hunger, int &thirst, int &happiness, time_t &decayTime) // this needs to be signaled by another program if we want constant change
{
 time_t currentTime = time(NULL); // Current time; 
 double secondsElapsed = difftime(currentTime, decayTime); // Time difference

// Every 5 seconds, there is a decay in stats (we can change this if needed)
if (secondsElapsed >= 5) {
    thirst = thirst - 3; 
    hunger = hunger - 2; 
    happiness = happiness - 1;
    
	if (hunger < 0) hunger = 0; // Hunger can't go past zero
	if (thirst < 0) thirst = 0; // Thirst can't go past zero
	if (happiness < 0) happiness = 0; // Happiness can't go past zero
	
	decayTime = currentTime; 
	}
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

        cout << "----------------[ Pet Menu ]-----------------" << endl;

        for (int i = 0; i < itemCount; ++i)
            cout << i + 1 << ") " << inventory[i] << endl; 
        for (int i = itemCount; i < 5; ++i)
            cout << i + 1 << ") " << "Empty Slot" << endl; 
        
        cout << "6) Exit Menu" << endl; 

        int user_input;

        while (true) // Input Validation
        {
            cout << "Enter Input: ";
            cin >> user_input;

            if (cin.fail() || !(cin.peek() == '\n') || user_input > 6 || user_input < 1)
            {
                invalid_input();
            } else {
                break;
            }

        }

        if (user_input == 6)
        {
            break;
        }


        user_input--; 
        if (user_input >= itemCount){
            cout << "Invalid selection. No item in this slot." << endl;
            continue;
        }
        string item = inventory[user_input]; 

        if (item == "Water"){ // we should probably change these values around
            thirst += 20;
            if (thirst > 100) 
                thirst = 100;

        }else if (item == "Food") {
            hunger += 20;
            if (hunger > 100) 
                hunger = 100;

        }else if (item == "Toy"){
            happiness += 20;
            if (happiness > 100) 
                happiness = 100;
                
        }else{
            cout << "Invalid, No item at this position." << endl;
            continue;
        }

        for (int i = user_input; i < itemCount - 1; ++i) {
            inventory[i] = inventory[i+1]; // Shift items to left
        }

        itemCount--; 
        if (itemCount < 0)
            itemCount = 0;
        
        break;
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

    //Used for text formatting. Runs through the list of encounters.
    void testEncounters()
    {
        int tempvar[5] = { 1, 3, 5, 10, 15 };
        for (int i : tempvar)
            encounter(i);
    }

    void runGame()
    {
        srand(time(0));
        setField();
        defaultValues();
        if (exploreLoop())
            cout << "Tired from a day of adventure, you return home.\n";

    }

private:
    // Map size in tiles.
    static const int xsize = 50;
    static const int ysize = 50;

    static const int visionRadius = 7;

    enum eventKey { EXIT = 0, BORDER = 1, COIN = 2, ENCOUNTER = 3, TILE = 4, MOUNTAIN = 5 };

    int energy = 50; // Starting energy.

    char field[xsize][ysize];

    int playerCoords[2] = { xsize / 2, ysize / 2 };

    // Resets variables to base values.
    void defaultValues()
    {
        numCoins = 0;
        energy = 50;
        playerCoords[0] = xsize / 2;
        playerCoords[1] = ysize / 2;
    }

    // Map generation functions.
    void rivergen()
    {
        ;
    }

    void treegen()
    {
        int mnum = rand() % 6 + 24; // Generates random amount of foliage
        for (int im = 0; im <= mnum; im++)
        {
            int mx = rand() % xsize;
            int my = rand() % ysize;
            int msize = rand() % 7 + 2; // Foliage radius
            while (((mx - msize <= xsize / 2) && (mx + msize >= xsize / 2)) && ((my - msize <= ysize / 2) && (my + msize >= ysize / 2)))
            {
                mx = rand() % xsize;
                my = rand() % xsize;
            }
            field[mx][my] = ';';
            for (int i = 0; i <= msize; i++) // Fills foliage
            {
                if (mx + i < xsize && mx + i >= 0) // Right side
                {
                    for (int iy = my; (iy >= 0 && iy < ysize) && iy - my <= msize - i; iy++)
                    {
                        field[mx + i][iy] = ';';
                    }
                    for (int iy = my; (iy >= 0 && iy < ysize) && my - iy <= msize - i; iy--)
                    {
                        field[mx + i][iy] = ';';
                    }
                }
                if (mx - i < xsize && mx - i >= 0) // Left side
                {
                    for (int iy = my; (iy >= 0 && iy < ysize) && iy - my <= msize - i; iy++)
                    {
                        field[mx - i][iy] = ';';
                    }
                    for (int iy = my; (iy >= 0 && iy < ysize) && my - iy <= msize - i; iy--)
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
            int mx = rand() % xsize;
            int my = rand() % ysize;
            int msize = rand() % 4 + 4; // Mountain radius
            while (((mx - msize <= xsize / 2) && (mx + msize >= xsize / 2)) && ((my - msize <= ysize / 2) && (my + msize >= ysize / 2)))
            {
                mx = rand() % xsize;
                my = rand() % ysize;
            }
            field[mx][my] = 'M';
            for (int i = 0; i <= msize; i++) // Fills mountains
            {
                if (mx + i < xsize && mx + i >= 0) // Right side
                {
                    for (int iy = my; (iy >= 0 && iy < ysize) && iy - my <= msize - i; iy++)
                    {
                        field[mx + i][iy] = 'M';
                    }
                    for (int iy = my; (iy >= 0 && iy < ysize) && my - iy <= msize - i; iy--)
                    {
                        field[mx + i][iy] = 'M';
                    }
                }
                if (mx - i < xsize && mx - i >= 0) // Left side
                {
                    for (int iy = my; (iy >= 0 && iy < ysize) && iy - my <= msize - i; iy++)
                    {
                        field[mx - i][iy] = 'M';
                    }
                    for (int iy = my; (iy >= 0 && iy < ysize) && my - iy <= msize - i; iy--)
                    {
                        field[mx - i][iy] = 'M';
                    }
                }
            }
        }
    }

    void locationgen()
    {
        for (int iy = 0; iy < ysize; iy++)
        {
            for (int ix = 0; ix < xsize; ix++)
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

        for (int i = 0; i < xsize; i++) // Sets the map border
        {
            field[i][0] = 'X';
        }
        for (int i = 0; i < ysize; i++)
        {
            field[0][i] = 'X';
        }
        for (int i = 0; i < ysize; i++)
        {
            field[xsize - 1][i] = 'X';
        }
        for (int i = 0; i < xsize; i++)
        {
            field[i][ysize - 1] = 'X';
        }
    }

    // Encounter generator.
    bool encounter(int var = rand() % 100 + 1)
    {
        int tempCoins;
        // May the best if statement win!
        if (var <= 1)
        {
            cout << "You fall into an inconveniently placed pitfall trap."
                << "\n After a while you manage to climb out, leaving you"
                << "\ntired from whe whole ordeal.\n\n-20 energy";
            energy -= 20;
            return 0;
        }
        if (var <= 3)
        {
            cout << "You are chased up a tree by wild animals! They eventually"
                << "\nleave you alone, but you feel drained from the encounter.\n\n-15 energy";
            energy -= 15;
            return 0;
        }
        if (var <= 5)
        {
            cout << "You encounter someone during your travels who drags you"
                << "\ninto a long-winded conversation about uninteresting things."
                << "\nYou finally escape, but not unscathed.\n\n-10 energy";
            energy -= 10;
            return 0;
        }
        if (var <= 10)
        {
            cout << "You follow a long path that leads nowhere. Disappointed,"
                << "\nyou walk back up the path with nothing to show for it\n\n-5 energy";
            energy -= 5;
            return 0;
        }
        if (var <= 15)
        {
            cout << "You hear a sound in the distance. You go to investigate,"
                << "\nand find someone stuck in a pit. You help them out."
                << "\nAs thanks, they give you a coin.\n\n-5 energy\n+1 coin";
            energy -= 5;
            numCoins++;
            return 0;
        }
        if (var <= 20)
        {
            cout << "You stop by a small stream and take a drink of water."
                << "\nYou feel reinvigorated.\n\n+15 energy";
            energy += 15;
            return 0;
        }
        if (var <= 25)
        {
            cout << "You stop by a small stream and take a drink of water."
                << "\nYou see something glittering in the stream."
                << "After closer inspection, it turns out to be a coin!\n\n+10 energy\n+1 coin";
            energy += 10;
            numCoins++;
            return 0;
        }
        if (var <= 30)
        {
            tempCoins = rand() % 7 + 3;
            cout << "You find a buried chest! inside is a bunch ofjunk, but"
                << "\nafter some digging around, you collect a few coins.\n\n+"
                << tempCoins << " coins";
            numCoins += tempCoins;
            return 0;
        }
        if (var <= 35)
        {
            tempCoins = rand() % 5 + 2;
            cout << "You feel air rush by and something brush up against you."
                << "\nYou look around, but see nothing. Your pockets feel ligher\n\n-"
                << tempCoins << " coins";
            numCoins -= tempCoins;
            if (numCoins < 0)
                numCoins = 0;
            return 0;
        }
        if (var <= 36)
        {
            tempCoins = rand() % 4 + 3;
            cout << "A rich person sees you wandering and assumes you're homeless."
                << "\nOut of pity, they give you a couple coins. You're not sure how"
                << "\nto feel about this interaction, but at least you got coins.\n\n+"
                << tempCoins << " coins";
            numCoins += tempCoins;
            return 0;
        }
        if (var <= 40)
        {
            tempCoins = rand() % 4 + 3;
            cout << "You find some coins lying on the ground. Nice!"
                << "\n\n+" << tempCoins << " coins";
            numCoins += tempCoins;
            return 0;
        }
        if (var <= 41)
        {
            cout << "A saudi prince offers you a million coins in exchange for"
                << "\nan initial investment. Knowing they must be telling the"
                << "\ntruth, you give them some of your coins. You wonder when"
                << "\nthey'll contact you.\n\n-10 coins";
            numCoins -= 10;
            if (numCoins < 0)
                numCoins = 0;
            return 0;
        }
        if (var <= 42)
        {
            cout << "You're confronted by a bandit looking to steal your coins."
                << "\nLuckily, they trip on a rock and fall over, unconscious."
                << "\nYou hurry away.";
            return 0;
        }
        if (var <= 50)
        {
            cout << "You find some berry bushes, and decide to eat some berries."
                << "\nThey're very tasty, and you feel invigorated!\n\n+10 energy";
            energy += 10;
            return 0;
        }
        if (var <= 60)
        {
            cout << "After reaching the crest of a hill, you stop to admire"
                << "\nthe view. You're filled with determination!\n\n+10 energy";
            energy += 10;
            return 0;
        }
        if (var <= 61)
        {
            cout << "Despite the sunny weather, you're struck by lightning."
                << "\nNaturally, this energizes you!\n\n+50 energy";
            energy += 50;
            return 0;
        }
        if (var <= 65)
        {
            cout << "You find a discarded piece of candy. Surely it's edible..."
                << "\n\n+10 energy";
            energy += 10;
            return 0;
        }
        if (var <= 66)
        {
            cout << "You find a discarded piece of candy. Surely it's edible..."
                << "\nYou bite into it, but it turns out to be a coin. Ow."
                << "\n\n-1 energy\n+1 coin";
            energy--;
            numCoins++;
            return 0;
        }
        if (var <= 67)
        {
            cout << "You take a sip from your trusty vault 13 canteen.\n\n+15 energy";
            energy += 15;
            return 0;
        }
        if (var <= 68)
        {
            cout << "Someone offers you a cake. Naturally, you accept."
                << "\nTurns out it wasn't a lie.\n\n+30 energy";
            energy += 30;
            return 0;
        }
        if (var <= 75)
        {
            cout << "You decide to listen to some music, and your favorite"
                << "\nsong comes on. It makes the journey easier.\n\n+15 energy";
            energy += 15;
            return 0;
        }
        if (var <= 80)
        {
            cout << "You find a wishing well, and toss a coin in. You feel hopeful."
                << "\n\n+ 10 energy\n-1 coin";
            energy += 10;
            numCoins--;
            if (numCoins < 0)
                numCoins = 0;
            return 0;
        }
        if (var <= 85)
        {
            cout << "You encounter a very short man on the road with a pack-laden pony"
                << "\nat his side. He quite adamantly offers you coins, as his pony is"
                << "\ntoo tired to carry them. You graciously accept.\n\n+10 coins";
            numCoins += 10;
            return 0;
        }
        if (var <= 90)
        {
            cout << "A goose walks across your path and lays a golden egg."
                << "You're sure you could fetch a good price for it.\n\n+10 coins";
            numCoins += 10;
            return 0;
        }
        if (var <= 95)
        {
            tempCoins = rand() % 11 + 5;
            cout << "You approach a group of people watching a magician perform"
                << "\ntricks, and join the crowd. For the final trick, the"
                << "\nmagician makes coins rain from the sky. That was fun!"
                << "\n\n+15 energy\n+" << tempCoins << " coins";
            energy += 15;
            numCoins += tempCoins;
            return 0;
        }
        if (var <= 99)
        {
            cout << "A man offers you an energy drink, saying it'll make you run"
                << "\nfaster and jump higher. Despite your sudden and inexplicable"
                << "\nurge to throw it on the ground, you drink it.\n\n+15 energy";
            energy += 15;
            return 0;
        }
        if (var <= 100)
        {
            cout << "A little something for playing :)\n\n+50 energy\n+20 coins";
            energy += 50;
            numCoins += 20;
            return 0;
        }




    }

    // Only prints player vision.
    void printVision()
    {
        cout << "/-------------------------------\\\n";
        for (int iy = playerCoords[1] - visionRadius; iy <= playerCoords[1] + visionRadius; iy++)
        {
            cout << "| ";
            if (iy < 0 || iy >= ysize)
            {
                cout << setw(30) << "" << "|\n";
                continue;
            }
            else
            {
                for (int ix = playerCoords[0] - visionRadius; ix <= playerCoords[0] + visionRadius; ix++)
                {
                    if (ix < 0 || ix >= xsize)
                    {
                        cout << "  ";
                    }
                    else if (ix == playerCoords[0] && iy == playerCoords[1])
                        cout << "O ";
                    else
                    {
                        cout << field[ix][iy] << " ";
                    }
                }
                cout << "|\n";
            }
        }
        cout << "\\-------------------------------/\n" << endl;
        cout << "Energy: " << energy << "     Coins: " << numCoins << "\n" << endl;
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
            if (tile == 'M') // Mountain
                return MOUNTAIN;

            // Apply change in player position.
            playerCoords[0] += pos[0];
            playerCoords[1] += pos[1];
            if (tile == '@') // Coin. No energy loss because you're happy you got money! It's intentional.
            {
                field[playerCoords[0]][playerCoords[1]] = '.';
                return COIN;
            }
            if (tile == 'E') // Encounter. Once again, no energy loss because I'm nice :)
            {
                field[playerCoords[0]][playerCoords[1]] = '.';
                return ENCOUNTER;
            }
            if (tile == '.') // Grass
            {
                energy -= 1;
                return TILE;
            }
            if (tile == ';') // Foliage
            {
                energy -= 2;
                return TILE;
            }
            if (tile == 'S') // River
            {
                energy -= 3;
                return TILE;
            }

            return 6;
        }
    }
    // Game loop
    int exploreLoop()
    {
        int tempCoins;
        // You can think of it as the max amount of time in the day, if you like.
        int escape = 0;
        clearScreen();
        printVision();
        while (escape < 300)
        {
            switch (getInput())
            {
            case EXIT:
                return 0;
            case BORDER:
                clearScreen();
                printVision();
                cout << "You shouldn't stray too far from home.";
                escape++;
                break;
            case COIN:
                tempCoins = rand() % 3 + 1;
                numCoins += tempCoins;
                clearScreen();
                printVision();
                cout << "You found " << tempCoins << " coins!" << endl;
                escape++;
                break;
            case ENCOUNTER:
                clearScreen();
                printVision();
                encounter();
                if (energy < 0) // Tired after the encounter? Go home!
                {
                    energy = 0;
                    return 2;
                }

                escape++;
                break;
            case TILE:
                if (energy < 0) // Too much walking? Go home!
                {
                    energy = 0;
                    clearScreen();
                    printVision();
                    return 1;
                }
                clearScreen();
                printVision();
                escape++;
                break;
            case MOUNTAIN:
                clearScreen();
                printVision();
                cout << "You are blocked by impassable mountains." << endl;
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
    time_t decayTime = time(NULL); // Initialize time 
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

	    // Call time decay function
	    pet_Stats_Decay(hunger, thirst, happiness, decayTime)

            // Display happy or sad art depending on needs
            if (hunger >= 50 || thirst >= 50 || happiness >= 50) {
                pet_Avatar_Happy(); 
            } else {
                pet_Avatar_Sad(); 
            }

            // Display pet stats
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
