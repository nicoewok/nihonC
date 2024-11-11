#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>

//This file contains all functions used to check the line
void replace_line_ending(char *line, size_t len);
void add_tabs(char *line, char *new_line);
int replace_type(char *token, char *new_line);


//Replace 。 with ;
void replace_line_ending(char *line, size_t len)
{
    if (len >= 3 && strncmp(&line[len - 3], "。", 3) == 0) { //。is three bytes long
        line[len - 3] = ';';
        line[len - 2] = '\n';  //new line after ;
        line[len - 1] = '\0';
    }
}

//Count \t in the beginning of the line and append to new_line
void add_tabs(char *line, char *new_line) {
    int i = 0;
    while ((line[i] == ' ' || line[i] == '\t') && i < 256) {
        // Append spaces or tabs as they appear in the original line
        strncat(new_line, &line[i], 1);
        i++;
    }
}

//Check for type names
//Returns 0 for char, 1 for int, 2 for long, 3 for float, 4 for double
//Returns -1 if not a type
int is_type(char *token)
{
    if (strcmp(token, "チャル") == 0)
        return 0;
    if (strcmp(token, "イント") == 0)
        return 1;
    if (strcmp(token, "ロング") == 0)
        return 2;
    if (strcmp(token, "フロート") == 0)
        return 3;
    if (strcmp(token, "ダブル") == 0)
        return 4;
    return -1;
}

//Replace katakan type names with C type names
int replace_type(char *token, char *new_line)
{
    int type = is_type(token);
    if (type != -1)
    {
        switch (type)
        {
        case 0:
            strcat(new_line, "char");
            break;
        case 1:
            strcat(new_line, "int");
            break;
        case 2:
            strcat(new_line, "long");
            break;
        case 3:
            strcat(new_line, "float");
            break;
        case 4:
            strcat(new_line, "double");
            break;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

//Replace brackets
int replace_bracket(char *token, char *new_line)
{
    if (strncmp(token, "「", 3) == 0) {
        strcat(new_line, "{");
        return 1;
    }
    if (strncmp(token, "」", 3) == 0) {
        strcat(new_line, "}");
        return 1;
    }
    return 0;
}