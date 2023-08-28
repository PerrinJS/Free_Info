#ifndef INFOGETTERS
#define INFOGETTERS

#include <stdbool.h>
#include <stdio.h>

char *getKernelVer(void);
int gotoEol(FILE *toMove);
bool findLineStartingWith(FILE *toSearch, char *sLine, int sLineLen);
char *restOfLineStartingWith(FILE *toSearch, char *sLine, int sLineLen);
void delStrtChars(char *toClean, int toCleanLen, int toRem);
/* removes the '=' and surrounding quotes from string read
 * from /etc/os-release */
void cleanLinNmStr(char *toClean, int toCleanLen);
// This only has a parameter so we can make a mock /etc/os-release
char *getLinNmFrom(const char *fileNm);
char *getTimeStr(void);

#endif
