//
// Created by kunal on 7/9/22.
//
#include "header.h"
#include "display.h"
#include "parse.h"

//parse space separated tokens
char** parseCommand(char* command)
{
    char** pString = (char**)calloc(MAX_ARGS, sizeof(char*));
    char* token;
    char* string = command;
    token = strtok_r(command, " \t\r\n", &string); // The strtok() function breaks a string into a sequence of zero or more nonempty tokens.
    int i = 0;
    while(token != NULL)
    {
        pString[i] = (char*)malloc(MAX_TOK_SIZE * sizeof(char));
        strcpy(pString[i++], token); // copies token to pString[i]
        token = strtok_r(NULL, " \t\r\n", &string);
    }

    // if the string is empty or only contains whitespace
    if(pString[0] == NULL)
    {
        pString[0] = (char*)malloc(MAX_TOK_SIZE * sizeof(char));
        strcpy(pString[0], "");
    }
    return pString;
}