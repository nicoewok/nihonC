#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>

//This file contains all functions used to check the line
void replace_line_ending(char *line, size_t len);



//Replace 。 with ;
void replace_line_ending(char *line, size_t len)
{
    if (len >= 3 && strncmp(&line[len - 3], "。", 3) == 0) { //。is three bytes long
        line[len - 3] = ';';
        line[len - 2] = '\n';  //new line after ;
        line[len - 1] = '\0';
    }
}