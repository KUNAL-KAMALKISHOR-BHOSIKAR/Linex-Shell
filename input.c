//
// Created by kunal on 7/9/22.
//

#include "header.h"
#include "display.h"
#include "input.h"

//reads input from stdin and tokens with ;
char** TakeInput()
{
    fflush(stdin); // flush stdin if stdin is null
    char in[MAX_TOK_SIZE];

    //read input
    //fgets must take input since often if a signal is handled input is skipped
    char* l;
    do
    {
        l = fgets(in, MAX_TOK_SIZE, stdin); // Get a newline-terminated string of finite length from stdin.
    }while(l==NULL);

    //tokens with ; delimiter
    char** list = (char**)calloc(MAX_ARGS, sizeof(char*));
    char* token;
    char* saveptr = in;
    token = strtok_r(in, ";", &saveptr);
    int i = 0;

    // if input is '\n'
    if(strcmp(in, "\n") == 0)
    {
        list[0] = (char*)malloc(MAX_TOK_SIZE*sizeof(char));
        strcpy(list[0], "");
        return list;
    }
    while(token != NULL)
    {
        list[i] = (char*)malloc(MAX_TOK_SIZE * sizeof(char));
        strcpy(list[i++], token);
        token = strtok_r(NULL, ";", &saveptr);
    }

    //if no memory was assigned
    if(list[0] == NULL)
    {
        list[0] = (char*)malloc(MAX_TOK_SIZE * sizeof(char));
        strcpy(list[0], "");
    }
    return list;
}