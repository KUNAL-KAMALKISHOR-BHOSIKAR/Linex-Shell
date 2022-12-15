**README**

CS3.301 - Operating Systems and Networks Assignment 2 Submission by Kunal Bhosikar (roll no: 2022121005)

Instructions to Run

When in /2022121005_Assignment2. Run make and then ./out.

Special Symbols

    The following symbols are used to represent the following:
    '<' indicates Input Redirection. The input for the command will be given by the file specified on the right hand.
    '>' indicates Output Redirection. The output of the command will be written to the file specified on the right hand.The file is created if it does not exist (with 0644 permissions).
    '>>' indicates Append Redirection. The output of the command will be appended to the file specified on the right hand. The output is appended to the existing file contents and the file must exist.

Username

    The source code for display is in ./username.c.
    We get the username of the user.

Display

    The source code for display is in ./display.c.

    PrintPrompt() prints the prompt for the shell along with the current working directory.

    The home directory of the shell (signified by ~) is the directory in which shell starts.
    The prompt consists of [<username>@<hostname>]:[<workingDirectory>]>>.
    Input can consist of ; separated commands, followed by arguments.
    Maximum input size is 1000 characters.

Parse

    The source code for parse is in ./parse.c.

    The parse function takes the input string and parses it into space separated tokens.
    The tokens are then passed to the execute function.

Input

    The source code for input is in ./input.c.
    
    TakeInput() reads input from the user and parses it into ; separated tokens.
    The tokens are then passed to the execute function.

Built-in Commands

    The source code for aSelector built-in commands is in ./builtin.c.

    cd <path> - Changes the current working directory of the shell to path. If no path is specified, changes to the home directory.
    pwd - Prints the current working directory.
    echo <args> - Prints args exactly as given after processing whitespaces.
    ls <path> - Lists the contents of the directory path. If no path is specified, lists the contents of the current working directory.
    exit - Exits the shell.

Process

    The source code for process is in ./process.c.

    In launchProcess(), the following processes are performed:
    Foreground process - A process that is executed in the foreground of the shell. The shell waits for the process to finish before accepting new input.
    Background process - A process that is executed in the background of the shell. The shell does not wait for the process to finish before accepting new input.
    The process is executed using execvp().
    
    In pinfo() the information about the process is printed.
    The process id is obtained using getpid().
    The process status is obtained using /proc/<pid>/stat.
    The memory used by the process is obtained using /proc/<pid>/statm.
    The executable path is obtained using /proc/<pid>/exe.

Discover

    The source code for discover is in ./discover.c.

    The discover function takes the input string and parses it into space separated tokens.
    The tokens are then passed to the execute function.
    A custom discover command emulates the basics of the find command. The command searches for files in a directory hierarchy.

Process Functions

    The source code for process functions is in ./process.c.

    The following functions are used to perform operations on processes:
    pidToName() - Returns the executable name of the process with the given pid.
    editProc() - Change status of a proc to 0-running 1-stopped 3-foreground.
    addProc() - Adds an item to the process list, if it exists toggle between running and stopped.
    clearProc() - Clears procList and frees memory.
    removeProc() - Removes pid from the procList.
    sendSignal() - Send signal to a child process.
    getForeProc() - Returns pid of foreground process.
    getProc() - Returns pid of process with given name.
    fg <jobNumber> - Brings the job with the given job number to the foreground.
    bg <jobNumber> - Brings the job with the given job number to the background.

Jobs

    The source code for jobs is in ./jobs.c.

    The jobs function prints the list of jobs running in the background.
    The jobs are printed in the format: <jobNumber> <processName> <status> <pid>.
    The status is printed as Running or Stopped.
    The jobNumber is the index of the job in the process list.

Main

    The source code for main file is in ./main.c.

    The function for history of the commands is written in main.c.
    Recent 10 commands are displayed on the terminal.
    But recent 20 commands are stored in the ShellHist.txt file.
    The commands are stored in the file in the reverse order.
    Oldest commands are replaced by newer ones.
    main() is the entry point for the shell.
    All the commands are run from the main file.
    The shell runs in an infinite loop.
    The shell accepts input from the user.
    The input is parsed into tokens.
    The tokens are then passed to the execute function.
    The execute function checks if the command is a built-in command or a process.
    If the command is a built-in command, the corresponding function is called.
    If the command is a process, the launchProcess() function is called.
    The shell exits when the user enters exit.
    Each input is parsed for ; and run separately. Each token is then considered a job.
    If a job has | then pipes are created in runJob() and each command is executed in ExecCommand() with particular input and output file descriptors.
    ExecCommand() also checks for Input and Output redirection (<, >, >>) and respectively modifies the input and output file descriptors. Then it runs the particular command.