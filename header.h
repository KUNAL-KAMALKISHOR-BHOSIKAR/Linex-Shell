//
// Created by kunal on 28/8/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <fcntl.h>
#include <bits/types/siginfo_t.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>
#include <grp.h>
#include <time.h>
#include <sys/stat.h>

extern char homeDir[1000];

//OSN_ASSIGNMENT2_HEADER_H