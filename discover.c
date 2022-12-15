//
// Created by kunal on 8/9/22.
//

#include "header.h"
#include "display.h"
#include "discover.h"

//function used for sorting the files in alphabetic order
int alphasortAll(const struct dirent **a, const struct dirent **b) {
    return strcasecmp((*a)->d_name, (*b)->d_name);
}

//returns only 1, used for scandir
int aSelector(__attribute__((unused)) const struct dirent *a)
{return 1;}

//used by listDriver to list files at a given path
void printDirectory(char *dir, int for_dir)
{
    char fileName[MAX_TOK_SIZE] = "";

    //to check if dir is a file
    struct stat s;
    if(stat(dir,&s) == 0)
    {
        if(s.st_mode & S_IFREG)
        {
            //seek pos to location of last '/'
            int pos;
            for(pos = strlen(dir)-1; pos>=0 && dir[pos] != '/'; pos--);

            //copy file name to fileName
            strcpy(fileName, dir+pos+1);
            if(pos >= 0)
            {
                //modify dir to not have the fileName
                char temp[MAX_TOK_SIZE];
                strncpy(temp, dir, pos+1);
                strcpy(dir, temp);
            }
                //else dir is a file in the working dir
            else
            {
                strcpy(dir, "./");
            }
        }
            //not a dir so print the path
        else
        {
            printf("\n%s:\n", dir);
        }
    }
    else
    {
        perror("Could not read the given path");
        return;
    }

    //save current directory
    char savedDir[MAX_TOK_SIZE];
    getcwd(savedDir, MAX_TOK_SIZE);

    //change cwd to dir
    if(chdir(dir) != 0)
    {
        perror("Could not list given directory");
        return;
    }

    //declare necessary variables for listing
    struct dirent** f;
    int noOfFiles = scandir(".", &f, aSelector, alphasortAll); // The scandir() function scans the directory dir, calling filter() on each directory entry.

    //iterate through each file and print details
    if(noOfFiles > 0)
    {
        for (int i=0; i<noOfFiles; i++)
        {
            stat(f[i]->d_name, &s);
            if(S_ISDIR(s.st_mode))

            if(f[i]->d_name[0] != '.' && for_dir == 0)
                continue;

            printf("%s\n", f[i]->d_name);
        }
    }
    else
    {
        perror("Invalid path or empty directory");
    }

    //change directory back to savedDir
    if(chdir(savedDir) != 0)
    {
        perror("error: could not come back to original directory.");
    }
    free(f);
}

void discover(char** inpt)
{
    char* dir;
    int for_dir=-1, for_file=-1;
    if(inpt[1]==NULL)
    {
        dir = (char*)malloc(MAX_TOK_SIZE*sizeof(char));
        getcwd(dir,MAX_TOK_SIZE);
        printDirectory(dir, for_dir);
        free(dir);
    }
    for(int i=1;i<MAX_ARGS;i++)
    {
        if(inpt[i]==NULL)
            break;
        if(strcmp(inpt[i],"-")==0 && strcmp(inpt[i+1],"d")==0)
        {
            for_dir=1;
        }
        else if(strcmp(inpt[i],"-")==0 && strcmp(inpt[i+1],"f")==0)
        {
            for_file=1;
        }
    }
    struct stat s;
    int i=1;
    while(inpt[i]!=NULL)
    {
        if(strcmp(inpt[i],"-d")==0 || strcmp(inpt[i],"-f")==0)
        {
            i++;
            continue;
        }

        if(for_file==1 && S_ISREG(s.st_mode))
        {
            printf("%s",inpt[i]);
        }
        else if(S_ISDIR(s.st_mode))
        {
            printDirectory(inpt[i], for_dir);
        }
        i++;
    }
}