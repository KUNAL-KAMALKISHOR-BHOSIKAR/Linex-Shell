
// main function

#include "header.h"
#include "username.h"
#include "display.h"
#include "parse.h"
#include "input.h"
#include "builtin.h"
#include "process.h"
#include "discover.h"
#include "process_functions.h"
#include "jobs.h"

#define MAX_HIST 20

int timeinterval = 0;
char hist[MAX_HIST][MAX_TOK_SIZE] = {""};
int histBott = -1;
char homeDir[MAX_TOK_SIZE];

//loads history from file
void loadHistory()
{
    FILE* fd;
    char path[MAX_TOK_SIZE];
    sprintf(path, "/home/%s/ShellHist.txt", getUsername());
    fd = fopen(path, "r");
    if(!fd)
    {
        return;
    }
    for(int i = 0; 1; i++)
    {
        if(fgets(path, MAX_TOK_SIZE, fd) != NULL)
        {
            strcpy(hist[i], path);
            histBott++;
        }
        else
            break;
    }
    fclose(fd);
}

//save history to file
void saveHist()
{
    FILE* fd;
    char path[MAX_TOK_SIZE];
    sprintf(path, "/home/%s/ShellHist.txt", getUsername());
    fd = fopen(path, "w");
    if(!fd)
    {
        perror("failed to save history to file.");
        return;
    }
    for(int i = 0; i<=histBott; i++)
    {
        fputs(hist[i], fd);
    }
    fclose(fd);
}

//pushes a commsList to hist
int pushToHist(char** commsList)
{
    char in[MAX_TOK_SIZE] = "";

    //if only one command then no ;
    if(commsList[1] == NULL)
    {
        strcat(in, commsList[0]);
    }
        //else concatenate ; as well
    else
    {
        for(int i = 0; commsList[i] != NULL; i++)
        {
            strcat(in, commsList[i]);
            if(commsList[i+1] != NULL)
                strcat(in, "; ");
        }
    }

    //if at max capacity, push aSelector hist up once
    if(histBott>=MAX_HIST-1)
    {
        for(int i = 0; i<histBott; i++)
            strcpy(hist[i], hist[i+1]);
    }
    else
    {
        histBott++;
    }
    strcpy(hist[histBott], in);
}

//prints the latest n commands
void printHist(int n)
{
    //due to 0 indexing
    n--;

    //if user is asking for more history than is saved
    if(n > histBott)
    {
        n = histBott;
    }
    for(int i = histBott - n; i<=histBott; i++)
    {
        printf("> %s", hist[i]);
    }
}

//prototype for runJob since it's used in two following functions (replayComm, ExecCommand)
void runJob(char*);

//handles the sigchld signal when a child process terminates
void sigchldHandler(siginfo_t *info)
{
    pid_t pID = info->si_pid;
    int st;
    int code = info->si_code;

    //get name before waitpid
    char name[MAX_TOK_SIZE];
    pidToName(pID, name);
    waitpid(pID, &st, WUNTRACED | WNOHANG | WCONTINUED);

    //if foreground process exited properly don't display a message
    if(pID == getForeProc() && (code == CLD_EXITED))
    {
        removeProc(pID);
        return;
    }
    if(code == CLD_STOPPED)
    {
        editProc(pID, 1);		//toggle status to stopped
        fprintf(stderr, "\n%s with pid %d stopped.\n", name, pID);
    }
    else if(code == CLD_CONTINUED)
    {
        editProc(pID, 0);		//toggle status to running
    }
    else if(code == CLD_EXITED)
    {
        removeProc(pID);
        fprintf(stderr, "\n%s with pid %d exited normally.\n", name, pID);
    }
    else
    {
        removeProc(pID);
        fprintf(stderr, "\n%s with pid %d exited abnormally.\n", name, pID);
    }
    return;
}

//send sigint to all processes in the foreground
void sigintHandler()
{
    int pID = getForeProc();
    if(pID>0)
    {
        kill(-pID, SIGINT);
    }
    else
    {
        printf("\n");
        PrintPrompt(homeDir, 0);
    }
}

//executes a given command string accounting for random whitespace
void ExecCommand(char* command, int inp, int out)
{
    char** inpt = parseCommand(command);
    long long no_of_args = 0;
    char command_strings[200][200];

    //REDIRECTION
    //save current stdout and stdin for restoration later
    int savedOut = dup(1); // The dup() system call creates a copy of the file descriptor oldfd, using the lowest-numbered unused file descriptor for the new descriptor.
    int savedInp = dup(0);

    //check for input redirection
    for(int i = 0; inpt[i] != NULL; i++)
    {
        int temp = -2;
        if(strcmp(inpt[i], "<") == 0)
        {
            if((temp = open(inpt[i+1], O_RDONLY)) == -1)
                perror("Unable to redirect input");
            else
                inp = temp;
        }
        if(strcmp(inpt[i], ">") == 0)
        {
            if((temp = open(inpt[i+1], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) == -1)
                perror("Unable to redirect output");
            else
                out = temp;
        }
        if(strcmp(inpt[i], ">>") == 0)
        {
            if((temp = open(inpt[i+1], O_WRONLY|O_TRUNC)) == -1)
                perror("Unable to redirect append");
            else
                out = temp;
        }

        //remove the two tokens that specify IO redirect
        if(temp != -2)
        {
            int j;
            for(j = i+2; inpt[j] != NULL; j++)
                strcpy(inpt[j-2], inpt[j]);
            inpt[j-2] = NULL;
            i--;
        }
    }

    //dup inp and out to stdin and stdout
    dup2(inp, 0);
    dup2(out, 1);

    //checks the first word in "inpt" for matching commands and executes
    if(strcmp(inpt[0], "cd") == 0)
        changeDir(inpt, homeDir);
    else if(strcmp(inpt[0], "pwd") == 0)
        printWD();
    else if(strcmp(inpt[0], "echo") == 0)
        echoecho(inpt);
    else if(strcmp(inpt[0], "ls") == 0)
        listDriver(inpt);
    else if(strcmp(inpt[0], "pinfo") == 0)
        pinfo(no_of_args, command_strings);
    else if(strcmp(inpt[0], "discover") == 0)
        discover(inpt);
    else if(strcmp(inpt[0], "jobs") == 0)
        AllJobs();
    else if(strcmp(inpt[0], "sig") == 0)
        sendSignal(inpt);
    else if(strcmp(inpt[0], "fg") == 0)
        makeFG(inpt);
    else if(strcmp(inpt[0], "bg") == 0)
        makeBG(inpt);
    else if(strcmp(inpt[0], "history") == 0)
    {
        if(inpt[1]==NULL)
        {
            if(histBott <= MAX_HIST/2 - 1)
                printHist(histBott+1);
            else
                printHist(MAX_HIST/2);
        }
        else
            printHist(atoi(inpt[1]));
    }
    else
    {
        timeinterval=launchProcess(inpt);
    }

    //free memory allocated to inpt
    for(int i = 0; i<MAX_ARGS; i++)
    {
        if(inpt[i] != NULL)
            free(inpt[i]);
    }
    free(inpt);

    //restore original stdout and stdin
    dup2(savedOut,1);
    close(savedOut);
    dup2(savedInp,0);
    close(savedInp);
    return;
}

//runs each ';' separated job
void runJob(char* job)
{
    //handle empty input
    if(strlen(job) == 0) return;
    //tokens with " | "
    char** processList = (char**)calloc(MAX_ARGS, sizeof(char*));

    char* saveptr;
    char* token = strtok_r(job, "|", &saveptr);
    for(int i = 0; token != NULL; i++)
    {
        processList[i] = (char*)malloc(MAX_TOK_SIZE*sizeof(char));
        strcpy(processList[i], token);
        token = strtok_r(NULL, "|", &saveptr);
    }
    int pipeFD[2];
    int inpFD = 0, outFD;
    int i;
    for(i = 0; processList[i+1] != NULL; i++)
    {
        pipe(pipeFD); // pipe() creates a pipe, a unidirectional data channel that can be used for interprocess communication.
        outFD = pipeFD[1];

        //check for IO redirection and run
        ExecCommand(processList[i], inpFD, outFD);
        close(outFD);
        inpFD = pipeFD[0];
    }
    ExecCommand(processList[i], inpFD, 1);

    //free memory allocated to processList
    for(int j = 0; j<MAX_ARGS; j++)
        if(processList[j] != NULL)
            free(processList[j]);
    free(processList);
}

int main() {
    loadHistory();

    // initialize shell
    pid_t pID = getpid();
    setpgid(pID, pID);
    tcsetpgrp(STDIN_FILENO, pID);

    //define SIGCHLD signal handler for background process exits
    struct sigaction act = {0};
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = (void (*)(int, siginfo_t *, void *)) sigchldHandler;
    sigaction(SIGCHLD, &act, NULL);

    // handle SIGINT and SIGTSTP signals
    signal(SIGINT, sigintHandler);
    signal(SIGTSTP, SIG_IGN);

    // ignore SIGTTIN and SIGTTOU signals
    signal(SIGTTOU, SIG_IGN);

    //copy current directory as home directory
    getcwd(homeDir, sizeof(homeDir));
    char** commsList;
    int toExit = 0;
    do
    {
        signal(SIGCHLD, done);
        if(timeinterval> 0)
        {
            PrintPrompt(homeDir,timeinterval);
            timeinterval=0;
        }
        else
        {
            PrintPrompt(homeDir,0);
        }
        commsList = TakeInput();
        pushToHist(commsList);
        for(int i = 0; commsList[i] != NULL != 0; i++)
        {
            if(strcmp(commsList[i], "exit\n") == 0)
            {
                toExit = 1;
                break;
            }
            runJob(commsList[i]);
        }

        //free memory allocated to commsList
        for(int i = 0; i<MAX_ARGS; i++)
        {
            if(commsList[i] != NULL)
                free(commsList[i]);
        }
        free(commsList);
    }while(toExit == 0);

    // free memory allocated to procList
    clearProc();

    commsList = NULL;
    saveHist();
    printf("\n");
    return 0;
}