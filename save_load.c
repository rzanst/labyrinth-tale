#include "save_load.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mapUtl.h"
#include "globals.h"
#include "game_logic.h"
#include "PnP.h"
#include "string.h"

int saveTo()
{
    char name[21];
    printf("\n========================================\n");
    printf("           SAVE GAME                    \n");
    printf("========================================\n");
    printf("Enter a name for your save file (max 20 chars):\n");
    printf(">>> ");

    scanf("%s", name);

    if (strlen(name) > 20)
    {
        printf("\n[ERROR] Name too long! Must be 20 characters or less.\n");
        printf("Press Enter to continue...");
        getchar();
        return 999;
    }

    // Add .dat extension
    char filename[25];
    strcpy(filename, name);
    strcat(filename, ".dat");

    // Check if file already exists
    FILE *checkFile = fopen(filename, "rb");
    if (checkFile != NULL)
    {
        fclose(checkFile);
        printf("\n[WARNING] A save file with this name already exists!\n");
        printf("Do you want to overwrite it? (y/n): ");
        char response;
        scanf(" %c", &response);
        if (response != 'y' && response != 'Y')
        {
            printf("Save cancelled.\n");
            printf("Press Enter to continue...");
            getchar();
            return 0;
        }
    }

    FILE *saveFile = fopen(filename, "wb");
    if (saveFile == NULL)
    {
        printf("\n[ERROR] Could not create save file!\n");
        printf("Press Enter to continue...");
        getchar();
        return 10;
    }

    // Write all game data
    fwrite(&n, sizeof(int), 1, saveFile);
    fwrite(&m, sizeof(int), 1, saveFile);
    fwrite(&k, sizeof(int), 1, saveFile);
    fwrite(&core_x, sizeof(int), 1, saveFile);
    fwrite(&core_y, sizeof(int), 1, saveFile);
    fwrite(grid, sizeof(char), (2 * n + 1) * (2 * m + 1), saveFile);
    fwrite(runners, sizeof(int), R * 2, saveFile);
    fwrite(hunters, sizeof(int), H * 2, saveFile);
    fwrite(runnersID, sizeof(int), R, saveFile);
    fwrite(huntersID, sizeof(int), H, saveFile);
    fwrite(&tmpWallsToPlace, sizeof(int), 1, saveFile);
    fwrite(&seed, sizeof(int), 1, saveFile);
    fwrite(&R, sizeof(int), 1, saveFile);
    fwrite(&H, sizeof(int), 1, saveFile);
    fwrite(AllTempWalls, sizeof(struct tmpWall), numberOfTWPlaced, saveFile);
    fwrite(&numberOfTWPlaced, sizeof(int), 1, saveFile);

    fclose(saveFile);

    // Update the list of all save files
    FILE *AllSaveFiles = fopen("AllSaveFiles.dat", "ab");
    if (AllSaveFiles != NULL)
    {
        // Write name length first, then the name (for easier reading)
        int nameLen = strlen(name);
        fwrite(&nameLen, sizeof(int), 1, AllSaveFiles);
        fwrite(name, sizeof(char), nameLen, AllSaveFiles);
        fclose(AllSaveFiles);

        printf("\n[SUCCESS] Game saved as '%s'!\n", name);
    }
    else
    {
        printf("\n[WARNING] Game saved but couldn't update saves list.\n");
    }

    printf("\nPress Enter to continue...");
    getchar();
    return 0;
}

int loadFrom()
{
    clear_screen();
    FILE *AllSaveFiles = fopen("AllSaveFiles.dat", "rb");
    if (AllSaveFiles == NULL)
    {
        printf("\n========================================\n");
        printf("           LOAD GAME                    \n");
        printf("========================================\n");
        printf("\nNo saved games found!\n");
        printf("Press Enter to continue...");
        getchar();
        return 11;
    }

    printf("\n========================================\n");
    printf("           LOAD GAME                    \n");
    printf("========================================\n");
    printf("\nYour Saved Files:\n");
    printf("----------------------------------------\n");

    // Read all save file names
    char saveNames[100][21]; // Maximum 100 saves
    int fileCount = 0;
    int nameLen;

    while (fread(&nameLen, sizeof(int), 1, AllSaveFiles) == 1)
    {
        if (nameLen > 0 && nameLen < 21)
        {
            fread(saveNames[fileCount], sizeof(char), nameLen, AllSaveFiles);
            saveNames[fileCount][nameLen] = '\0'; // Null terminate
            printf("[%d] %s\n", fileCount + 1, saveNames[fileCount]);
            fileCount++;
        }
    }
    fclose(AllSaveFiles);

    if (fileCount == 0)
    {
        printf("No saved games found!\n");
        printf("Press Enter to continue...");
        getchar();
        return 11;
    }

    printf("----------------------------------------\n");
    printf("Choose a file to load [1-%d]: ", fileCount);

    int choice;
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > fileCount)
    {
        printf("\n[ERROR] Invalid choice!\n");
        printf("Press Enter to continue...");
        getchar();
        return 999;
    }

    // Construct filename
    char filename[25];
    strcpy(filename, saveNames[choice - 1]);
    strcat(filename, ".dat");

    FILE *loadFile = fopen(filename, "rb");
    if (loadFile == NULL)
    {
        printf("\n[ERROR] Could not open save file '%s'!\n", filename);
        printf("Press Enter to continue...");
        getchar();
        return 10;
    }

    fread(&n, sizeof(int), 1, loadFile);
    fread(&m, sizeof(int), 1, loadFile);
    fread(&k, sizeof(int), 1, loadFile);
    fread(&core_x, sizeof(int), 1, loadFile);
    fread(&core_y, sizeof(int), 1, loadFile);
    fread(grid, sizeof(char), (2 * n + 1) * (2 * m + 1), loadFile);

    fread(&R, sizeof(int), 1, loadFile); // Note: Read R before using it
    fread(&H, sizeof(int), 1, loadFile); // Note: Read H before using it

    fread(runners, sizeof(int), R * 2, loadFile);
    fread(hunters, sizeof(int), H * 2, loadFile);
    fread(runnersID, sizeof(int), R, loadFile);
    fread(huntersID, sizeof(int), H, loadFile);

    fread(&tmpWallsToPlace, sizeof(int), 1, loadFile);
    fread(&seed, sizeof(int), 1, loadFile);
    fread(AllTempWalls, sizeof(struct tmpWall), numberOfTWPlaced, loadFile);
    fread(&numberOfTWPlaced, sizeof(int), 1, loadFile);

    fclose(loadFile);

    printf("\n[SUCCESS] Game loaded successfully!\n");
    printf("Press Enter to start playing...");
    getchar();
    return 0;
}

int savedGames()
{
    clear_screen();
    FILE *AllSaveFiles = fopen("AllSaveFiles.dat", "rb");
    if (AllSaveFiles == NULL)
    {
        printf("\n========================================\n");
        printf("           SAVED GAMES                  \n");
        printf("========================================\n");
        printf("\nNo saved games found!\n");
        printf("\nPress Enter to continue...");
        getchar();
        getchar();
        clear_screen();
        return 11;
    }

    printf("\n========================================\n");
    printf("           SAVED GAMES                  \n");
    printf("========================================\n\n");

    int nameLen;
    int counter = 1;
    char currName[21];

    while (fread(&nameLen, sizeof(int), 1, AllSaveFiles) == 1)
    {
        if (nameLen > 0 && nameLen < 21)
        {
            fread(currName, sizeof(char), nameLen, AllSaveFiles);
            currName[nameLen] = '\0';
            printf("  [%d] %s\n", counter, currName);
            counter++;
        }
    }
    fclose(AllSaveFiles);

    if (counter == 1)
    {
        printf("  No saved games found!\n");
    }

    printf("\n----------------------------------------\n");
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
    clear_screen();
    return 0;
}