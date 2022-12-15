//
// Created by kunal on 7/9/22.
//

#include "header.h"

//get username
char* getUsername()
{
    unsigned int u = getuid(); // returns the real user ID of the calling process
    struct passwd* p = getpwuid(u); // retrieve the user database entry for the given user ID
    return (p->pw_name);
}