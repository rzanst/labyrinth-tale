#include "save_load.h"
#include "globals.h"
#include "string.h"
#include <stdio.h>

int saveTo()
{
    char filename[50];
    printf("Enter a name for your save file: \n --> ");
    scanf("%s", filename);

    strcat(filename, ".txt");

    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Error creating file!\n");
        return 1;
    }

    fprintf(f, "%d %d\n", n, m);
    fprintf(f, "%d %d\n", core_x, core_y);
}
