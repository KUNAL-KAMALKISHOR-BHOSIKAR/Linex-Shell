//
// Created by kunal on 28/8/22.
//

#include "header.h"
#include "username.h"
#include "display.h"

//prints the prompt for input with username, hostname and working dir
void PrintPrompt(char* homeDir, long int time)
{
    //get current working directory
    char cwd[MAX_TOK_SIZE];
    getcwd(cwd, MAX_TOK_SIZE); // get the pathname of the current working directory

    //get username
    char usrName[MAX_TOK_SIZE];
    strcpy(usrName, getUsername()); // copies the username to usrName

    //get hostname
    char hName[MAX_TOK_SIZE];
    gethostname(hName,MAX_TOK_SIZE); // put the name of the current host in no more than MAX_TOK_SIZE bytes into hName

    //substitute home directory with '~'
    char* wdWithHome;
    wdWithHome = strstr(cwd, homeDir); // The strstr() function finds the first occurrence of the substring needle in the string haystack.
    if(wdWithHome != NULL)
    {
        wdWithHome += strlen(homeDir)-1; // add the length of homeDir to wdWithHome
        wdWithHome[0] = '~';

        //print prompt for foreground process with time
        if(time>0)
            printf("\n\033[0;32m[\033[0;35m%s@%s\033[0;32m]\033[0;35m:\033[0;32m[\033[0;35m%s\033[0;32m]\033[0;35m took %ld s >>\033[0m", usrName, hName, wdWithHome, time);
        //print prompt for other processes
        else
            printf("\n\033[0;32m[\033[0;35m%s@%s\033[0;32m]\033[0;35m:\033[0;32m[\033[0;35m%s\033[0;32m]\033[0;35m>>\033[0m", usrName, hName, wdWithHome);
    }
    //not in home directory
    else
    {
        //print prompt for foreground process with time
        if(time>0)
            printf("\n\033[0;32m[\033[0;35m%s@%s\033[0;32m]\033[0;35m:\033[0;32m[\033[0;35m%s\033[0;32m]\033[0;35m took %ld s >>\033[0m", usrName, hName, cwd, time);
        //print prompt for other processes
        else
            printf("\n\033[0;32m[\033[0;35m%s@%s\033[0;32m]\033[0;35m:\033[0;32m[\033[0;35m%s\033[0;32m]\033[0;35m>>\033[0m", usrName, hName, wdWithHome);
    }
}