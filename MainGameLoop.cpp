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
Friday November 29th - Fixed missing semicolon in my code (typical, classic) - Charlene
Saturday November 30th - Bug fixes, rearranging, organization.
Saturday November 30th - Fixed inventory and coin saving system (wasn't loading before) - Charlene
*/

#include <iostream>
#include <fstream> 
#include <string> 
#include <cstdlib> // ("cls" and "clear")
#include <ctime> 
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

void invalid_input(string message = "Invalid Input. Please enter a valid number.") // Error function
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << message << "\n" << endl;
}

class playerPet
{
public:
    int hunger = 100, thirst = 100, happiness = 100;

    string name; // Pet Name

    void displayPet()
    {
        if (hunger >= 50 && thirst >= 50 && happiness >= 50)
        {
            cout << "/\\_/\\ ♥\n"
                << ">^W^<\n"
                << " / \\\n"
                << "(___)_/\n" << endl;
        }
        else
        {
            cout << "/\\_/\\ ☹\n"
                << ">=A=<\n"
                << " / \\\n"
                << "(___)_/\n" << endl;
        }
    }

    void displayStats() 
    {
        cout << "Hunger: " << hunger << " | Thirst: " << thirst << " | Happiness: " << happiness << "\n" << endl;
    }

    void statDecay(time_t& decayTime)
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
};

// Shop menu display
void displayShopMenu(playerPet& pet, int coins)
{
    clearScreen();
    pet.displayPet();
    pet.displayStats();

    cout << "----------------[ Shop Menu ]-----------------" << endl;
    if (coins >= 10)
        cout << "                [ Coins: " << coins << " ]" << endl;
    else
        cout << "                [ Coins: " << coins << "  ]" << endl;
    cout << "1) Water - 3 coins\n"
        << "2) Food - 5 coins\n"
        << "3) Toy - 10 coins\n"
        << "4) Sell item\n"
        << "5) Exit Menu\n" << endl;
}

// Shop menu
void shop_menu(int& coins, string inventory[], int& itemCount, playerPet& pet) // This is where you spend money on items for your pet.
{
    displayShopMenu(pet, coins);

    while (true)
    {
        bool leave_shop = false;
        int user_input;
        

        while (true) // Input Validation
        {
            cout << "Enter Input: ";
            cin >> user_input;


            if (cin.fail() || user_input > 5 || user_input < 1)
            {
                invalid_input();
            }
            else {
                break;
            }
        }

        displayShopMenu(pet, coins);

        switch (user_input) {
        case 1: // Water
            if (coins >= 3) {
                if (itemCount < MAX_INVENTORY_SIZE) {
                    coins -= 3;
                    inventory[itemCount++] = "Water";
                    cout << "You purchased Water! Remaining inventory space: " << MAX_INVENTORY_SIZE - itemCount << "\n " << endl;
                }
                else {
                    cout << "Inventory full!" << endl;
                }
            }
            else {
                cout << "Not enough coins!" << endl;
            }
            break;

        case 2: // Food
            if (coins >= 5) {
                if (itemCount < MAX_INVENTORY_SIZE) {
                    coins -= 5;
                    inventory[itemCount++] = "Food";
                    cout << "You purchased Food! Remaining inventory space: " << MAX_INVENTORY_SIZE - itemCount << "\n " << endl;
                }
                else {
                    cout << "Inventory full!" << endl;
                }
            }
            else {
                cout << "Not enough coins!" << endl;
            }
            break;

        case 3: // Toy
            if (coins >= 10) {
                if (itemCount < MAX_INVENTORY_SIZE) {
                    coins -= 10;
                    inventory[itemCount++] = "Toy";
                    cout << "You purchased a Toy! Remaining inventory space: " << MAX_INVENTORY_SIZE - itemCount << "\n " << endl;
                }
                else {
                    cout << "Inventory full!\n" << endl;
                }
            }
            else {
                cout << "Not enough coins!\n" << endl;
            }
            break;

        case 4: // Sell an item and get a refund
            if (itemCount == 0) {
                cout << "You have no items to sell!\n" << endl;
            }
            else {
                cout << "Your inventory: " << endl;
                for (int i = 0; i < itemCount; ++i) {
                    cout << i + 1 << ") " << inventory[i] << "\n " << endl;
                }

                int item_Sell;
                cout << "Enter the number of the item you want to sell: ";
                cin >> item_Sell;

                // Input validation
                if (cin.fail() || item_Sell > itemCount || item_Sell < 1) {
                    invalid_input();
                    break;
                }

                item_Sell--;
                string item = inventory[item_Sell];

                int refund = 0;
                if (item == "Water") {
                    refund = 2; // User gets half coins (round up) 
                }
                else if (item == "Food") { // User gets half coins (round up)
                    refund = 3;
                }
                else if (item == "Toy") { // User gets half coins (round up)
                    refund = 5;
                }

                coins += refund;
                if (refund > 1) {
                    cout << item << " sold! You received " << refund << " coins." << endl;
                }
                else {
                    cout << item << " sold! You received " << refund << " coin." << endl;
                }


                for (int i = item_Sell; i < itemCount - 1; ++i) {
                    inventory[i] = inventory[i + 1]; // Shift items to left
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

// Item menu display
void displayPetMenu(string inventory[MAX_INVENTORY_SIZE], int& itemCount, playerPet& pet)
{
    clearScreen();
    pet.displayPet();
    pet.displayStats();

    cout << "----------------[ Pet Menu ]-----------------" << endl;

    for (int i = 0; i < itemCount; ++i)
        cout << i + 1 << ") " << inventory[i] << endl;
    for (int i = itemCount; i < 5; ++i)
        cout << i + 1 << ") " << "Empty Slot" << endl;

    cout << "6) Exit Menu\n" << endl;
}

// Item menu
void pet_menu(string inventory[MAX_INVENTORY_SIZE], int& itemCount, playerPet& pet) // This is where you feed, water, and play with the pet. It also serves as an inventory.
{
    displayPetMenu(inventory, itemCount, pet);

    while (true)
    {
        int user_input;

        while (true) // Input Validation
        {
            cout << "Enter Input: ";
            cin >> user_input;

            if (cin.fail() || user_input > MAX_INVENTORY_SIZE + 1 || user_input < 1)
            {
                invalid_input();
            }
            else {
                break;
            }

        }

        displayPetMenu(inventory, itemCount, pet);

        if (user_input == MAX_INVENTORY_SIZE + 1)
        {
            break;
        }


        user_input--;
        if (user_input >= itemCount) {
            cout << "Invalid selection. No item in this slot." << endl;
            continue;
        }
        string item = inventory[user_input];

        if (item == "Water") { // we should probably change these values around
            pet.thirst += 20;
            if (pet.thirst > 100)
                pet.thirst = 100;
        }
        else if (item == "Food") {
            pet.hunger += 20;
            if (pet.hunger > 100)
                pet.hunger = 100;
        }
        else if (item == "Toy") {
            pet.happiness += 20;
            if (pet.happiness > 100)
                pet.happiness = 100;
        }
        else {
            cout << "Invalid, No item at this position." << endl;
            continue;
        }

        for (int i = user_input; i < itemCount - 1; ++i) {
            inventory[i] = inventory[i + 1]; // Shift items to left
        }

        itemCount--;
        if (itemCount < 0)
            itemCount = 0;

        displayPetMenu(inventory, itemCount, pet);
        cout << "Used " << item << ".\n" << endl;
    }
}

//Exploration minigame!
class explorationGame
{
public:
    int numCoins = 0;

    // Displays the whole field. Used for testing.
    void printField()
    {
        for (int iy = 0; iy < ysize; iy++)
        {
            for (int ix = 0; ix < xsize; ix++)
            {
                cout << gameField[ix][iy] << ' ';
            }
            cout << '\n';
        }
    }

    // Used for testing. Runs through the list of encounters.
    void testEncounters()
    {
        int tempvar[27] = { 1, 3, 5, 10, 15, 20, 25, 30, 35, 36, 40, 41, 42, 50, 60, 61, 65, 66, 67, 68, 75, 80, 85, 90, 95, 99, 100 };
        for (int i : tempvar)
            encounter(i);
        cout << "\n";
    }

    // Runs the game
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

    char gameField[xsize][ysize];

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
            gameField[mx][my] = ';';
            for (int i = 0; i <= msize; i++) // Fills foliage
            {
                if (mx + i < xsize && mx + i >= 0) // Right side
                {
                    for (int iy = my; (iy >= 0 && iy < ysize) && iy - my <= msize - i; iy++)
                    {
                        gameField[mx + i][iy] = ';';
                    }
                    for (int iy = my; (iy >= 0 && iy < ysize) && my - iy <= msize - i; iy--)
                    {
                        gameField[mx + i][iy] = ';';
                    }
                }
                if (mx - i < xsize && mx - i >= 0) // Left side
                {
                    for (int iy = my; (iy >= 0 && iy < ysize) && iy - my <= msize - i; iy++)
                    {
                        gameField[mx - i][iy] = ';';
                    }
                    for (int iy = my; (iy >= 0 && iy < ysize) && my - iy <= msize - i; iy--)
                    {
                        gameField[mx - i][iy] = ';';
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
            gameField[mx][my] = 'M';
            for (int i = 0; i <= msize; i++) // Fills mountains
            {
                if (mx + i < xsize && mx + i >= 0) // Right side
                {
                    for (int iy = my; (iy >= 0 && iy < ysize) && iy - my <= msize - i; iy++)
                    {
                        gameField[mx + i][iy] = 'M';
                    }
                    for (int iy = my; (iy >= 0 && iy < ysize) && my - iy <= msize - i; iy--)
                    {
                        gameField[mx + i][iy] = 'M';
                    }
                }
                if (mx - i < xsize && mx - i >= 0) // Left side
                {
                    for (int iy = my; (iy >= 0 && iy < ysize) && iy - my <= msize - i; iy++)
                    {
                        gameField[mx - i][iy] = 'M';
                    }
                    for (int iy = my; (iy >= 0 && iy < ysize) && my - iy <= msize - i; iy--)
                    {
                        gameField[mx - i][iy] = 'M';
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
                if (gameField[ix][iy] == 'M')
                    continue;
                else if (gameField[ix][iy] == '.')
                {
                    if (rand() % 60 == 1) // Coin
                    {
                        gameField[ix][iy] = '@';
                    }
                    else if (rand() % 60 == 1) // Encounter
                    {
                        gameField[ix][iy] = 'E';
                    }
                }
                else if (gameField[ix][iy] == ';')
                {
                    if (rand() % 60 == 1) // Coin
                    {
                        gameField[ix][iy] = '@';
                    }
                    else if (rand() % 30 == 1) // Encounter
                    {
                        gameField[ix][iy] = 'E';
                    }
                }
                else if (gameField[ix][iy] == 'S')
                {
                    if (rand() % 30 == 1) // Coin
                    {
                        gameField[ix][iy] = '@';
                    }
                    else if (rand() % 60 == 1) // Encounter
                    {
                        gameField[ix][iy] = 'E';
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
                gameField[ix][iy] = '.';
            }
        }
        treegen();

        mountaingen(); // Places mountain tiles

        rivergen(); // Places river tiles

        locationgen(); // Places coins & encounters

        for (int i = 0; i < xsize; i++) // Sets the map border
        {
            gameField[i][0] = 'X';
        }
        for (int i = 0; i < ysize; i++)
        {
            gameField[0][i] = 'X';
        }
        for (int i = 0; i < ysize; i++)
        {
            gameField[xsize - 1][i] = 'X';
        }
        for (int i = 0; i < xsize; i++)
        {
            gameField[i][ysize - 1] = 'X';
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
        return 1; // Not implemented, error
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
                cout << "                              " << "|\n";
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
                        cout << gameField[ix][iy] << " ";
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
                tile = gameField[playerCoords[0]][playerCoords[1] - 1];
                pos[1] = -1;
            }
            if (input == 'a' || input == 'A')
            {
                tile = gameField[playerCoords[0] - 1][playerCoords[1]];
                pos[0] = -1;
            }
            if (input == 's' || input == 'S')
            {
                tile = gameField[playerCoords[0]][playerCoords[1] + 1];
                pos[1] = 1;
            }
            if (input == 'd' || input == 'D')
            {
                tile = gameField[playerCoords[0] + 1][playerCoords[1]];
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
                gameField[playerCoords[0]][playerCoords[1]] = '.';
                return COIN;
            }
            if (tile == 'E') // Encounter. Once again, no energy loss because I'm nice :)
            {
                gameField[playerCoords[0]][playerCoords[1]] = '.';
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

void minigame_menu(explorationGame& explore, int& coins, playerPet& pet, time_t& decayTime)
{
    bool exiting = 0;
    while (!exiting) {
        // Display the menu
        cout << "-------------[ Mini Games ]---------------\n";
        cout << "1) Exploration!\n";
        cout << "2) Unavailable\n";
        cout << "3) Unavailable\n";
        cout << "4) Exit\n" << endl;

        int choice;
        cin >> choice;
        if (cin.fail() || choice < 1 || choice > 4)
            invalid_input("Choose from 1-4");

        switch (choice) {
        case 1: {
            explore.runGame();
            coins += explore.numCoins;
            pet.statDecay(decayTime);
            break;
        }
        case 2: {
            break;
        }
        case 3: {
            break;
        }
        case 4: {
            cout << "Exiting Mini Games... Goodbye!" << endl;
            exiting = 1;
            break;
        }
        }
    }
}


int main()
{
    time_t decayTime = time(NULL); // Initialize time 
    bool exit_game = false;
    playerPet pet; // Initialize pet
    explorationGame explore; // Initialize exploration game
    string inventory[MAX_INVENTORY_SIZE];
    int coins = 20; // Default Money
    int itemCount = 0;
    

    // Try to load data from the file
    ifstream inFile("pet_data.txt");
    if (inFile) {
        getline(inFile, pet.name);
        inFile >> pet.hunger >> pet.thirst >> pet.happiness; // Load pet stats
        inFile >> itemCount; // Load inventory count
        for (int i = 0; i < itemCount; ++i) {
            inFile >> inventory[i]; // Load inventory items
        }
        inFile >> coins;  // Load coins
        inFile.close();
    }
    else {
        cout << "No pet data found. Create a new pet? (y/n): ";
        char choice; // Local variable for saving
        while (1)
        {
            cin >> choice;

            if (choice == 'y' || choice == 'Y')
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\nEnter pet name: ";
                getline(cin, pet.name);
                break;
            }
            else if (choice == 'n' || choice == 'N')
            {
                cout << "Exiting..." << endl;
                return 0;
            }
            else
            {
                invalid_input("Invalid choice. Please enter 'Y' or 'N'. ");
            }
        }
    }


    while (true) // Main Game Loop
    {
        clearScreen();

        // Call time decay function
        pet.statDecay(decayTime);

        // Display happy or sad art depending on needs
        pet.displayPet();

        // Display pet stats
        pet.displayStats();

        int user_input;

        cout << "----------------[ Main Menu ]-----------------\n"
        << "1) Shop Menu\n"
        << "2) Pet Menu\n"
        << "3) Minigames Menu\n"
        << "4) Exit Game" << endl;


        while (true) // Input Validation
        {
            cout << "Enter Input: ";
            cin >> user_input;

            if (cin.fail() || user_input > 4 || user_input < 1)
            {
                invalid_input();
            }
            else
                break;
        }

        switch (user_input) // Send User to Proper Menu
        {
        case 1:
            shop_menu(coins, inventory, itemCount, pet);
            break;
        case 2:
            clearScreen();
            pet.displayPet();
            pet.displayStats();
            pet_menu(inventory, itemCount, pet);
            break;
        case 3:
            clearScreen();
            pet.displayPet();
            pet.displayStats();
            minigame_menu(explore, coins, pet, decayTime);
            break;
        case 4:
            exit_game = true;
            break;

        default:
            break;
        }

        if (exit_game) {
            ofstream outFile("pet_data.txt");
            if (!outFile) {
                cout << "Error opening save file! Please check file permissions or disk space. " << endl;
                break;
            }

            outFile << pet.name << endl;
            outFile << pet.hunger << " " << pet.thirst << " " << pet.happiness << endl; // Save stats

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
