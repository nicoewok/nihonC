//This is the main program. It translates .nihon files to .c files and compiles them.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>

int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "");
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
    

    //write translate and write new file
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            len--;
        }

        // Check if the line ends with a 。
        if (len >= 3 && strncmp(&line[len - 3], "\xE3\x80\x82", 3) == 0) {
            // Replace the last three bytes with a single semicolon
            line[len - 3] = ';';
            line[len - 2] = '\n';  // Null-terminate the string after replacing
            line[len - 1] = '\0';
        }

        fputs(line, new_file);
    }

    fclose(file);
    fclose(new_file);
    

    
    
    //compile the new file using gcc
    size_t base_length = strlen(new_filename) - 2;
    size_t command_length = strlen("gcc ") + strlen(new_filename) + strlen(" -o ") + base_length + strlen(".exe") + 1;

    char *command = malloc(command_length);
    strcpy(command, "gcc ");
    strcat(command, new_filename);
    strcat(command, " -o ");
    strncat(command, new_filename, strlen(new_filename) - 2);
    strcat(command, ".exe");
    system(command);


    free(filename);
    free(new_filename);
    return 0;
}
