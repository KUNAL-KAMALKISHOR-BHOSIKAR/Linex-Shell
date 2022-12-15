//
// Created by kunal on 25/9/22.
//

#ifndef OSN_ASSIGNMENT2_PROCESS_FUNCTIONS_H
#define OSN_ASSIGNMENT2_PROCESS_FUNCTIONS_H

#define MAX_JOBS 20

void pidToName(int, char*);
void editProc(int, int);
void addProc(int, char **);
void clearProc();
void removeProc(int);
void sendSignal(char**);
int getForeProc();
void makeFG(char**);
void makeBG(char**);

#endif //OSN_ASSIGNMENT2_PROCESS_FUNCTIONS_H