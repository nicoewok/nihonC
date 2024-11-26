//This is the main program. It translates .nihon files to .c files and compiles them.

#include "nihonC.h"
#define COMPILE

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
    char *new_line = calloc(256, sizeof(char));
    while (fgets(line, sizeof(line), file))
    {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            len--;
        }

        replace_line_ending(line, len);
        add_tabs(line, new_line);
        //complex replacements
        int complex_replacement = 0;
        complex_replacement += replace_for(line, new_line);
        complex_replacement += replace_assignment(line, new_line);

        //tokenize the line
        char *token = strtok(line, " ");
        int first_token = 1; //for adding spaces between tokens
        int replacement = 0;
        if (complex_replacement == 0) {
            while (token != NULL)
            {
                if (!first_token) {
                    strcat(new_line, " ");
                }
                first_token = 0;
                
                //do checks and replace if needed
                replacement += replace_token(token, new_line);
                
                if (replacement == 0) {
                    strcat(new_line, token);
                }
                replacement = 0;

                //go to next token
                token = strtok(NULL, " ");
            }
        }

        //check if \n is actually there
        if (new_line[strlen(new_line) - 1] != '\n')
            strcat(new_line, "\n");

        fputs(new_line, new_file);
        memset(new_line, 0, 256);
    }

    free((void *) new_line);
    fclose(file);
    fclose(new_file);
    

    
    
    //compile the new file using gcc
    #ifdef COMPILE
    size_t base_length = strlen(new_filename) - 2;
    size_t command_length = strlen("gcc ") + strlen(new_filename) + strlen(" -o ") + base_length + strlen(".exe") + 1;

    char *command = malloc(command_length);
    strcpy(command, "gcc ");
    strcat(command, new_filename);
    strcat(command, " -o ");
    strncat(command, new_filename, strlen(new_filename) - 2);
    strcat(command, ".exe");
    system(command);
    #endif


    free(filename);
    free(new_filename);
    return 0;
}
