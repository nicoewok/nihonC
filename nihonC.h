#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>

//This file contains all functions used to check the line
void replace_line_ending(char *line, size_t len);
void add_tabs(char *line, char *new_line);
int replace_token(char *token, char *new_line);
int replace_type(char *token, char *new_line);
int replace_bracket(char *token, char *new_line);
int replace_for(char *line, char *new_line);
int replace_assignment(char *line, char *new_line);

//execute check/replace functions on token in line
int replace_token(char *token, char *new_line) {
    int replacement = 0;
    replacement += replace_type(token, new_line);
    replacement += replace_bracket(token, new_line);
    return replacement;
}

//Replace 。 with ;
void replace_line_ending(char *line, size_t len) {
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
//Returns 0 for char, 1 for int, 2 for long, 3 for float, 4 for double, 5 for const
//Returns -1 if not a type
int is_type(char *token) {
    if (strcmp(token, "文字") == 0)
        return 0;
    if (strcmp(token, "整数") == 0)
        return 1;
    if (strcmp(token, "ロング") == 0)
        return 2;
    if (strcmp(token, "浮動小数点") == 0)
        return 3;
    if (strcmp(token, "倍精度浮動小数点") == 0)
        return 4;
    if (strcmp(token, "定数") == 0)
        return 5;
    return -1;
}

//Replace katakan type names with C type names
int replace_type(char *token, char *new_line) {
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
            case 5:
                strcat(new_line, "const");
                break;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

//Replace 「」brackets
int replace_bracket(char *token, char *new_line) {
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

//Detect kara and made
int detect_for(char *line) {
    int kara_counter = 0, made_counter = 0;
    
    //copy line and tokenize the line
    char *line_copy = malloc(256);
    strncpy(line_copy, line, 256);
    char *token = strtok(line_copy, " ");
    while (token != NULL)
    {
        if (strcmp(token, "から") == 0) {
            kara_counter++;
        }
        if (strcmp(token, "まで") == 0) {
            made_counter++;
        }
        token = strtok(NULL, " ");
    }

    free((void *) line_copy);
    if (kara_counter == 1 && made_counter == 1) {
        return 1;
    }
    return 0;
}

//Replace ... から ... まで with C for loop
int replace_for(char *line, char *new_line) {
    if (detect_for(line) == 0) {
        return 0;
    }
 
    //copy line
    char *line_copy = malloc(256);
    strncpy(line_copy, line, 256);
    
    //getting variable type
    char *token = strtok(line_copy, " ");
    int type = is_type(token);
    if (type == -1) {
        free((void *) line_copy);
        return 0;
    }

    //get lower bound
    token = strtok(NULL, " ");
    char *lower_bound = calloc(256, sizeof(char));
    strncpy(lower_bound, token, strlen(token));

    token = strtok(NULL, " ");
    if (strcmp(token, "から") != 0) {
        free((void *) line_copy);
        return 0;
    }

    //get upper bound
    token = strtok(NULL, " ");
    char *upper_bound = calloc(256, sizeof(char));
    strncpy(upper_bound, token, strlen(token));

    token = strtok(NULL, " ");
    if (strcmp(token, "まで") != 0) {
        free((void *) line_copy);
        return 0;
    }

    //get increment variable name
    token = strtok(NULL, " ");
    char *increment = calloc(256, sizeof(char));
    strncpy(increment, token, strlen(token));

    //write C for loop into new_line
    switch (type)
    {
        case 0:
            strcat(new_line, "for (char ");
            break;
        case 1:
            strcat(new_line, "for (int ");
            break;
        case 2:
            strcat(new_line, "for (long ");
            break;
        case 3:
            strcat(new_line, "for (float ");
            break;
        case 4:
            strcat(new_line, "for (double ");
            break;
    }
    strcat(new_line, increment);
    strcat(new_line, " = ");
    strcat(new_line, lower_bound);
    strcat(new_line, "; ");
    strcat(new_line, increment);
    strcat(new_line, " <= ");
    strcat(new_line, upper_bound);
    strcat(new_line, "; ");
    strcat(new_line, increment);
    strcat(new_line, "++)");
    free(lower_bound);
    free(upper_bound);
    free(increment);

    //write the rest of the line with correct checks
    token = strtok(NULL, " ");
    int replacement = 0;
    while (token != NULL)
    {
        strcat(new_line, " ");
        //do checks and replace if needed
        replacement += replace_token(token, new_line);
        
        if (replacement == 0) {
            strcat(new_line, token);
        }
        replacement = 0;

        //go to next token
        token = strtok(NULL, " ");
    }

    free((void *) line_copy);

    return 1;
}

//Detect は　...　です
int detect_assignment(char *line)
{
    int wa_counter = 0, desu_counter = 0;
    
    //copy line and tokenize the line
    char *line_copy = malloc(256);
    strncpy(line_copy, line, 256);
    char *token = strtok(line_copy, " ");
    while (token != NULL)
    {
        if (strcmp(token, "は") == 0) {
            wa_counter++;
        }
        if (strncmp(token, "です", 6) == 0) {
            desu_counter++;
        }
        token = strtok(NULL, " ");
    }

    free((void *) line_copy);
    if (wa_counter == 1 && desu_counter == 1) {
        return 1;
    }
    return 0;
}


//Replace は ... です with C assignment
int replace_assignment(char *line, char *new_line) {
    if (detect_assignment(line) == 0) {
        return 0;
    }
 
    //copy line
    char *line_copy = malloc(256);
    strncpy(line_copy, line, 256);
    char *token = strtok(line_copy, " ");

    //write the rest of the line with correct checks
    //replace は with =, remove です
    int first_token = 1;
    int replacement = 0;
    while (token != NULL)
    {
        //if we reached the end, no " " needed
        if (strncmp(token, "です", 6) == 0) {
            strcat(new_line, ";");
            replacement++;
        } else {
            if (!first_token) {
                strcat(new_line, " ");
            }
            first_token = 0;

            //check for は
            if (strcmp(token, "は") == 0) {
                strcat(new_line, "=");
                replacement++;
            } else {
                //do checks and replace if needed
                replacement += replace_token(token, new_line);
            }
        }

        if (replacement == 0) {
            strcat(new_line, token);
        }
        replacement = 0;

        //go to next token
        token = strtok(NULL, " ");
    }

    free((void *) line_copy);

    return 1;
}