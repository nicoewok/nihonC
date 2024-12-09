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
int replace_if_else(char *token, char *new_line);
int replace_for(char *line, char *new_line);
int replace_assignment(char *line, char *new_line, int operator, int particle);
int complex_replacement(char *line, char *new_line);

char* operator_particles[] = {"は", "と"};
char* operators[] = {"=", "==", "!=", ">", "<", ">=", "<="};
char* operator_jap[] = {"です", "が等しい", "と異なる", "より大きい", "より小さい", "と等しいかより大きい", "と等しいかより小さい"};
int operator_jap_len[] = {6, 12, 12, 15, 15, 30, 30};


//execute check/replace functions on token in line
int replace_token(char *token, char *new_line) {
    int replacement = 0;
    replacement += replace_type(token, new_line);
    replacement += replace_bracket(token, new_line);
    replacement += replace_if_else(token, new_line);
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

//Check and replace if-else statements
int replace_if_else(char *token, char *new_line) {
    if (strcmp(token, "もし") == 0) {
        strcat(new_line, "if");
        return 1;
    }
    if (strcmp(token, "それ以外") == 0) {
        strcat(new_line, "else");
        return 1;
    }
    return 0;
}

//Check and replace complex grammatical structures
int complex_replacement(char *line, char *new_line) {
    int complex_replacement = 0;

    //Detection
    //for-loop
    int kara_counter = 0, made_counter = 0;
    //operators
    int operator_particle_counter[] = {0, 0};
    int operator_counter[] = {0, 0, 0, 0, 0, 0, 0};

    //copy line and tokenize the line
    char *line_copy = malloc(256);
    strncpy(line_copy, line, 256);
    char *token = strtok(line_copy, " ");
    while (token != NULL)
    {
        if (strcmp(token, "から") == 0) {
            kara_counter++;
        } else if (strcmp(token, "まで") == 0) {
            made_counter++;
        } else if (strcmp(token, "は") == 0) {
            operator_particle_counter[0]++;
        } else if(strcmp(token, "と") == 0) {
            operator_particle_counter[1]++;
        }
        for (int i = 0; i < 7; i++) {
            if (strncmp(token, operator_jap[i], operator_jap_len[i]) == 0) {
                operator_counter[i]++;
            }
        }
        token = strtok(NULL, " ");
    }
    free((void *) line_copy);

    //replace if needed
    if (kara_counter == 1 && made_counter == 1) {
        replace_for(line, new_line);
        complex_replacement++;
    }
    if (operator_particle_counter[0] == 1) {
        for (int i = 0; i < 7; i++) {
            if (i == 1) //only if particle is と
                i++;
            if (operator_counter[i] == 1) {
                replace_assignment(line, new_line, i, 0);
                complex_replacement++;
                break;
            }
        }
    } else if (operator_particle_counter[1] == 1) {
        if (operator_counter[1] == 1) {
            replace_assignment(line, new_line, 1, 1);
            complex_replacement++;
        }
    }

    return complex_replacement;
}

//Replace ... から ... まで with C for loop
int replace_for(char *line, char *new_line) {
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

//Replace は ... です with C assignment
//operator is 0 for =, 1 for >, 2 for <, 3 for >=, 4 for <=
int replace_assignment(char *line, char *new_line, int operator, int particle) {
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
        //if "deleted", no space needed
        if (strncmp(token, operator_jap[operator], operator_jap_len[operator]) == 0) {
            //make copy of token after the keyword
            char *remaining_part = token + operator_jap_len[operator];
            char *token_rest = calloc(strlen(remaining_part) + 1, sizeof(char));

            strncpy(token_rest, remaining_part, strlen(remaining_part));
            strcat(new_line, token_rest);            
            replacement++;

            free(token_rest);
        } else {
            if (!first_token) {
                strcat(new_line, " ");
            }
            first_token = 0;
            //check for は/と and replace with new operator
            if (strcmp(token, operator_particles[particle]) == 0) {
                strcat(new_line, operators[operator]);
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