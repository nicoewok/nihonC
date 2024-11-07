//This is the main program. It translates .nihon files to .c files and compiles them.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: nihonC <file>.nihonC\n");
        return 1;
    }

    char *filename = malloc(strlen(argv[1]) + 1);
    strcpy(filename, argv[1]);

    //check if file ending is .nihonC
    if (strcmp(filename + strlen(filename) - 7, ".nihonC") != 0)
    {
        fprintf(stderr, "Error: File must end with .nihonC\n");
        return 1;
    }

    //open file for reading
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return 1;
    }

    //create a new file with the same name but ending with .c
    char *new_filename = malloc(strlen(filename) - 6);
    strncpy(new_filename, filename, strlen(filename) - 7);
    new_filename[strlen(filename) - 7] = '\0';
    strcat(new_filename, ".c");

    FILE *new_file = fopen(new_filename, "w");
    free(filename);
    free(new_filename);

    //write the header of the new file
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        fputs(line, new_file);
    }

    fclose(file);
    fclose(new_file);
    

    return 0;
}
