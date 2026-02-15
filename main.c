#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mapUtl.h"
#include "globals.h"
#include "game_logic.h"
#include "PnP.h"
#include "save_load.h"
#include "main.h"
#include "ai.h"

int optionsMenu()
{
    clear_screen();

    while (1)
    {
        printf("========================================\n");
        printf("           OPTIONS MENU                \n");
        printf("========================================\n");
        printf("  [1] Resume Game\n");
        printf("  [2] Save Game\n");
        printf("  [3] Game Settings\n");
        printf("  [0] Exit to Main Menu\n");
        printf("========================================\n");
        printf(">> Enter your choice: ");

        int cmd;
        scanf("%d", &cmd);
        getchar();

        switch (cmd)
        {
        case 1:
            printf("\n>> Resuming game...\n");
            getchar();
            clear_screen();
            return 0;

        case 2:
            printf("\n>> Saving game...\n");
            saveTo();
            printf("\n>> Press Enter to continue...");
            getchar();
            clear_screen();
            break;

        case 3:
            printf("\n>> Opening game settings...\n");
            gamesSettings();
            clear_screen();
            break;

        case 0:
            printf("\n+--------------------------------------+\n");
            printf("|  Are you sure you want to exit?     |\n");
            printf("|  [1] Yes, return to Main Menu       |\n");
            printf("|  [0] No, stay in game               |\n");
            printf("+--------------------------------------+\n");
            printf(">> ");

            int tcmd;
            scanf("%d", &tcmd);
            getchar();

            if (tcmd == 1)
            {
                printf("\n>> Returning to main menu...\n");
                getchar();
                clear_screen();
                return 1;
            }
            else
            {
                printf("\n>> Continuing game...\n");
                getchar();
                clear_screen();
            }
            break;

        default:
            printf("\n>> Invalid choice! Please enter 0-3.\n");
            printf(">> Press Enter to continue...");
            getchar();
            clear_screen();
        }
    }
}
void credits()
{
    clear_screen();

    printf("========================================\n");
    printf("              CREDITS                  \n");
    printf("========================================\n\n");
    printf("Created by:\n");
    printf("  • Sajjad Nemati Farooji \n");
    printf("  • Reza Nosrati \n\n");
    printf("GitHub Contributors:\n");
    printf("  • github.com/rzanst\n");
    printf("  • github.com/ItIsZEFiX\n\n");
    printf("  • Fundamentals of Programming - Fall 1404\n");
    printf("  • All who tested the game\n\n");
    printf("========================================\n");
    printf("     THANK YOU FOR PLAYING!\n");
    printf("========================================\n");

    printf("\n>> Press Enter to return to menu...");
    getchar();
    clear_screen();
}

void loadDefaultSettings()
{
    seed = 0;
    isPnP = 1;
    MinMaxActivate = 1;
    DEPTH = 3; // Your default depth
    preciseEvaluations = 0;

    printf("\n Default settings loaded.\n");
}

void gamesSettings()
{
    clear_screen();

    printf("========================================\n");
    printf("          GAME SETTINGS                \n");
    printf("========================================\n");

    FILE *settings = fopen("settings.dat", "rb");
    if (settings == NULL)
    {
        printf("\n[ERROR] Settings file not found!\n");
        printf("Creating new settings file with defaults...\n");

        // Create default settings
        settings = fopen("settings.dat", "wb");
        if (settings == NULL)
        {
            printf("Failed to create settings file!\n");
            printf("Press Enter to continue...");
            getchar();
            return;
        }

        // Use global variables for defaults
        loadDefaultSettings();

        fwrite(&seed, sizeof(int), 1, settings);
        fwrite(&isPnP, sizeof(int), 1, settings);
        fwrite(&MinMaxActivate, sizeof(int), 1, settings);
        fwrite(&DEPTH, sizeof(int), 1, settings);
        fwrite(&preciseEvaluations, sizeof(int), 1, settings);

        fclose(settings);

        // Reopen for reading
        settings = fopen("settings.dat", "rb");
    }

    // Read settings from file into global variables
    fread(&seed, sizeof(int), 1, settings);
    fread(&isPnP, sizeof(int), 1, settings);
    fread(&MinMaxActivate, sizeof(int), 1, settings);
    fread(&DEPTH, sizeof(int), 1, settings);
    fread(&preciseEvaluations, sizeof(int), 1, settings);

    fclose(settings);

    // Display current settings
    printf("\nCURRENT CONFIGURATION:\n");
    printf("----------------------------------------\n");
    printf(" Seed                : %d\n", seed);
    printf(" Mystery Boxes (PnP) : %s\n", isPnP ? "Enabled" : "Disabled");
    printf(" AI Assistant        : %s\n", MinMaxActivate ? "Active" : "Inactive");
    printf(" AI Search Depth     : %d\n", DEPTH);
    printf(" Precise Evaluation  : %s\n", preciseEvaluations ? "On" : "Off");
    printf("----------------------------------------\n");

    // Settings menu
    printf("\nSETTINGS MENU:\n");
    printf(" [1] Toggle Mystery Boxes (PnP)\n");
    printf(" [2] Toggle AI Assistant\n");
    printf(" [3] Adjust AI Search Depth\n");
    printf(" [4] Toggle Precise Evaluation\n");
    printf(" [5] Reset to Default Settings\n");
    printf(" [0] Save and Return\n");

    int choice;
    int modified = 0;

    while (1)
    {
        printf("\nEnter your choice [0-5]: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            isPnP = !isPnP;
            printf(" Mystery Boxes: %s\n", isPnP ? "Enabled" : "Disabled");
            modified = 1;
            break;

        case 2:
            MinMaxActivate = !MinMaxActivate;
            printf(" AI Assistant: %s\n", MinMaxActivate ? "Active" : "Inactive");
            modified = 1;
            break;

        case 3:
            printf(" Current Depth: %d\n", DEPTH);
            printf(" Enter new depth [1-20]: ");
            int new_depth;
            scanf("%d", &new_depth);
            getchar();

            if (new_depth >= 1 && new_depth <= 20)
            {
                DEPTH = new_depth;
                printf(" AI Search Depth set to %d\n", DEPTH);
                modified = 1;
            }
            else
            {
                printf(" Invalid depth! Must be between 1 and 20.\n");
            }
            break;

        case 4:
            preciseEvaluations = !preciseEvaluations;
            printf(" Precise Evaluation: %s\n", preciseEvaluations ? "On" : "Off");
            modified = 1;
            break;

        case 5:
            seed = 0;
            isPnP = 1;
            MinMaxActivate = 1;
            DEPTH = 10;
            preciseEvaluations = 1;
            printf(" Settings restored to defaults.\n");
            modified = 1;
            break;

        case 0:
            if (modified)
            {
                // Save settings to file
                FILE *save_settings = fopen("settings.dat", "wb");
                if (save_settings != NULL)
                {
                    fwrite(&seed, sizeof(int), 1, save_settings);
                    fwrite(&isPnP, sizeof(int), 1, save_settings);
                    fwrite(&MinMaxActivate, sizeof(int), 1, save_settings);
                    fwrite(&DEPTH, sizeof(int), 1, save_settings);
                    fwrite(&preciseEvaluations, sizeof(int), 1, save_settings);
                    fclose(save_settings);

                    printf("\nSettings saved successfully.\n");
                }
                else
                {
                    printf("\nError saving settings!\n");
                }
            }

            printf("\nPress Enter to return to game...");
            getchar();
            clear_screen();
            return;

        default:
            printf("Invalid choice! Please enter a number between 0 and 5.\n");
        }

        // Show updated settings after modification
        if (modified && choice != 0 && choice != 3)
        {
            printf("\nUPDATED CONFIGURATION:\n");
            printf("----------------------------------------\n");
            printf(" Mystery Boxes (PnP) : %s\n", isPnP ? "Enabled" : "Disabled");
            printf(" AI Assistant        : %s\n", MinMaxActivate ? "Active" : "Inactive");
            printf(" AI Search Depth     : %d\n", DEPTH);
            printf(" Precise Evaluation  : %s\n", preciseEvaluations ? "On" : "Off");
            printf("----------------------------------------\n");
        }
    }
}

// Playyy!

// If the whole mechanism of gameplay is launched through play() but if the game is a new game some variables like n,m, ... must be determined
// so we use PrePlay() in that case!

void PrePlay()
{
    seed = time(NULL); // seed used here can be declared globally
    srand(seed);

    greetings();  // Some UI
    getDimen();   // Getting the Dimensions from User
    getRnHnK();   // Getting the number of R: runners/ H: hunters/ k: Walls
    mapGen(seed); // isPnP is set as 1 by default

    clear_screen();
}

// The main game
int play(int isLoaded)
{
    getchar();
    clear_screen();
    if (isLoaded == 0)
    {
        PrePlay();
    }

    while (1)
    {
        int acted = 0;
        refresh_tmp_walls();
        print_map_UI();

        if (runner_won())
        {
            print_map_UI();
            printf("\nYOU WON! Runner reached the Core.\n");
            break;
        }

        if (runner_lost())
        {
            print_map_UI();
            printf("\nYOU LOST! Hunter caught the Runner.\n");
            break;
        }

        printf("\nMove: W/A/S/D | Stay: X | Put Temporary Wall : T | Help : H | Exit To Main Menu: Q | Options Menu: M\n");
        printf(">>> ");

        int cmd = read_non_newline_char();
        while (getchar() != '\n')
            ;

        if (cmd == 'q' || cmd == 'Q')
        {
            printf("Exiting the Labyrinth...\n");
            break;
        }

        if (cmd == 'm' || cmd == 'M')
        {
            int output = optionsMenu();
            if (output == 1)
                break;
        }

        if (cmd == 'T' || cmd == 't')
        {
            int userInputX, userInputY;
            printf("Where do you want to put a temporary wall? Enter its cords: (x,y) (based on instructions\n");
            print_map_numbered();

            scanf("%d %d", &userInputX, &userInputY);

            // FIX: Swap them when accessing the grid.
            // Row is Y, Column is X.
            int row = userInputY;
            int col = userInputX;

            if (grid[row][col] == '+' || grid[row][col] == 'R' || grid[row][col] == 'H' || grid[row][col] == 'C')
            {
                printf("That aint a wall\n");
                continue;
            }
            if (grid[row][col] == '-' || grid[row][col] == '|')
            {
                printf("There's a wall standing there! \n");
                continue;
            }

            // Pass row (y) first, then col (x) because your insert_tmp_wall expects (row, col)
            insert_tmp_wall(row, col);
            acted = 1;
        }

        else if (cmd == 'w' || cmd == 'a' || cmd == 's' || cmd == 'd' ||
                 cmd == 'W' || cmd == 'A' || cmd == 'S' || cmd == 'D' ||
                 cmd == 'x' || cmd == 'X')
        {
            int result = moveRunner((char)cmd);

            if (result == 0)
            {
                printf(">> Invalid command! <<\n");
                continue;
            }

            acted = 1;
        }

        if (cmd == 'h' || cmd == 'H' || cmd == '?')
        {
            suggest_best_move(DEPTH);
        }

        if (acted)
        {
            if (runner_won())
                continue;

            if (moveHunter())
            {
                clear_screen();
                printf("\nYOU LOST! Hunter caught the Runner.\n");
                break;
            }
        }
        else
        {
            printf(">> Invalid command! <<\n");
            continue;
        }
    }

    getchar();
    return 0;
}

// The same concept of Main Menu in most AAA games
int main()
{
    clear_screen();

    printf("========================================\n");
    printf("     LABYRINTH OF ARKA - MAIN MENU     \n");
    printf("========================================\n\n");

    loadDefaultSettings();

    int cmd = 0;
    do
    {
        printf("+---------------------------------------+\n");
        printf("|  [1] Play New Game                    |\n");
        printf("|  [2] Custom Board                     |\n");
        printf("|  [3] Load Saved Game                  |\n");
        printf("|  [4] Credits                          |\n");
        printf("|  [0] Settings                         |\n");
        printf("|  [9] Quit                             |\n");
        printf("+---------------------------------------+\n");
        printf("\n>> Enter your choice: ");

        scanf("%d", &cmd);
        printf("\n");

        switch (cmd)
        {
        case 1:
            printf("[Starting new game...]\n\n");
            clear_screen();
            play(0);
            clear_screen();
            break;

        case 2:
            printf("[Entering custom board editor...]\n\n");
            clear_screen();
            customBoard();
            clear_screen();
            break;

        case 3:
            printf("[Loading saved game...]\n\n");
            if (loadFrom() == 0)
            {
                printf(">> Game loaded successfully!\n");
                play(1);
            }
            else
            {
                printf(">> Failed to load game.\n");
                printf(">> Press Enter to continue...");
                getchar();
                getchar();
            }
            clear_screen();
            break;

        case 4:
            credits();
            clear_screen();
            break;

        case 0:
            printf("[Opening settings...]\n\n");
            gamesSettings();
            clear_screen();
            break;

        case 9:
            printf("+--------------------------------------+\n");
            printf("|  Are you sure you want to quit?      |\n");
            printf("|  (y/n):                              |\n");
            printf("+--------------------------------------+\n");
            printf(">> ");

            char certainty[10];
            scanf("%s", certainty);

            if (certainty[0] == 'y' || certainty[0] == 'Y')
            {
                printf("\n========================================\n");
                printf("  Thank you for playing! Goodbye!\n");
                printf("========================================\n\n");
                exit(0);
                break;
            }
            else
            {
                printf("\n>> Returning to main menu...\n\n");
                continue;
            }

        default:
            printf(">> Invalid choice! Please enter 0-9.\n");
            printf(">> Press Enter to continue...");
            getchar();
            getchar();
            clear_screen();
        }

    } while (1);

    return 0;
}