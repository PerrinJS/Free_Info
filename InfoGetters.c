#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/time.h>
#include <sys/utsname.h>

#include "FreeInfo.h"
#include "InfoGetters.h"

char *getKernelVer(void) {
  struct utsname releaseInfo;
  char *ret = NULL;
  if (!uname(&releaseInfo)) {
    int outputLen = strlen(releaseInfo.release);
    ret = (char *)malloc((outputLen + 1) * sizeof(char));
    if (ret) {
      // create a copy on the heap
      strcpy(ret, releaseInfo.release);
    }
  }

  return ret;
}

int gotoEol(FILE *toMove) {
  char currch = '\0';
  do {
    currch = getc(toMove);
  } while (NOT_EOF_NL(currch));
  return currch;
}

bool findLineStartingWith(FILE *toSearch, char *sLine, int sLineLen) {
  char currch = '\0';
  int sLinePos = 0;
  bool validSearch = true;
  bool found = false;
  while (!found && (currch != EOF)) {
    // if we found it (we need to check here so we don't read an extra char)
    if (validSearch && (sLinePos >= sLineLen)) {
      found = true;
    } else {

      if (currch != '\n') {
        currch = getc(toSearch);
      } else {
        // if it's a new line reset our search
        sLinePos = 0;
        validSearch = true;
        currch = getc(toSearch);
      }

      if (currch != EOF) {
        if (validSearch && (sLinePos < sLineLen)) {
          // make sure the current char is in the correct part of our search str
          validSearch = (sLine[sLinePos] == currch);
        } else
          // if not skip to the end of the line
          currch = gotoEol(toSearch);
        sLinePos += 1;
      }
    }
  }

  return found;
}

char *restOfLineStartingWith(FILE *toSearch, char *sLine, int sLineLen) {
  bool found = findLineStartingWith(toSearch, sLine, sLineLen);
  char *ret = NULL;
  if (found) {
    int retBufLen = 40;
    int currPos = 0;
    ret = (char *)malloc(retBufLen * sizeof(char));

    char currch = '\0';
    while (NOT_EOF_NL(currch)) {
      /* +1 for '\0'
       * we reuse currPos to keep the current pos in ret */
      for (; currPos < retBufLen; currPos++) {
        currch = getc(toSearch);
        if (NOT_EOF_NL(currch)) {
          ret[currPos] = currch;
        } else
          break;
      }
      // we filled the buffer but didn't finish reading
      if (NOT_EOF_NL(currch)) {
        retBufLen += 20;
        ret = (char *)realloc(ret, retBufLen);
      }
    }
    ret[currPos] = '\0';
  }

  return ret;
}

void delStrtChars(char *toClean, int toCleanLen, int toRem) {
  if (toClean) {
    if (toRem <= toCleanLen) {
      char *targetStrt = toClean + toRem;
      while (*targetStrt != '\0') {
        *toClean = *targetStrt;
        toClean++;
        targetStrt++;
      }
      // move the '\0'
      *toClean = *targetStrt;
    }
    // if we are asked to delete more than we have do nothing
  }
}

/* removes the '=' and surrounding quotes from string read
 * from /etc/os-release */
void cleanLinNmStr(char *toClean, int toCleanLen) {
  if (toClean) {
    // remove the quote at the end first as we know where it is atm
    char *lastCh = toClean + (toCleanLen - 1);
    if (*lastCh == '"') {
      *lastCh = '\0';
    }

    // the number of characters to take off the front
    int toMvLft = 0;
    if (*toClean == '=')
      toMvLft += 1;
    if (*(toClean + 1) == '"')
      toMvLft += 1;

    delStrtChars(toClean, toCleanLen, toMvLft);
  }
}

// This only has a parameter so we can make a mock /etc/os-release
char *getLinNmFrom(const char *fileNm) {
  /* THIS IS ONLY INTENDED TO BE USED WITH A /etc/os-release FILE */
  char *ret = NULL;
  FILE *osInfoFile = fopen(fileNm, "r");
  // if we couldn't open the file return null
  if (osInfoFile) {
    // if we can't find the "PRETTY_NAME" we want a backup
    char *name = "NAME";
    char *linNm = restOfLineStartingWith(osInfoFile, name, strlen(name));

    name = "PRETTY_NAME";
    char *prettyLinNm = restOfLineStartingWith(osInfoFile, name, strlen(name));

    // if we successfully got the line we wanted (PRETTY_NAME) then cleanup
    if (prettyLinNm) {
      if (linNm) {
        free(linNm);
        linNm = NULL;
      }
      ret = prettyLinNm;
    } else {
      if (linNm) {
        ret = linNm;
      }
    }

    if (ret) {
      cleanLinNmStr(ret, strlen(ret));
    }

    fclose(osInfoFile);
  }
  return ret;
}

char *getTimeStr(void) {
  // A string to hold the final printed line
  int maxCurrTmLen = 51;
  char *currTime = (char *)malloc(maxCurrTmLen * sizeof(char));
  if (currTime) {
    // To store the current unix time in a timeval struct
    struct timeval thetime;
    // To store the converted unix time to localtime format
    struct tm *timeinfo;
    gettimeofday(&thetime, NULL);          // get the unix time
    timeinfo = localtime(&thetime.tv_sec); // Convert the Unix time to localtime

    // Format the localtime into a string
    strftime(currTime, maxCurrTmLen, "It is currently %A the %d/%m/%G at %R\n",
             timeinfo);
  }
  return currTime;
}
