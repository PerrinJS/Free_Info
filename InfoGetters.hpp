#ifndef INFOGETTERS
#define INFOGETTERS

#include <stdbool.h>
#include <stdio.h>

extern "C" {
char *getKernelVer(void);
char *getLinNmFrom(const char *fileNm);
char *getTimeStr(void);
char *getLuaScriptString(const char *scriptFileName);
};

#endif
