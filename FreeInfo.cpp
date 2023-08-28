#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "FreeInfo.h"
#include "InfoGetters.h"
#include "argparse/include/argparse/argparse.hpp"
#include "InfoStrParser.hpp"

bool cleanUp(void **to_clean, int to_clean_len) {
  bool bad = 0;
  for (int i = 0; i < to_clean_len; i++) {
    if (!IS_ZERO(to_clean[i]))
      free(to_clean[i]);
    else
      bad += 1;
  }
  return bad;
}

int main(int argc, char **argv) {
  //FIXME: make this c accessable and change the build process so c is still built by a c compiler
  //keep things sepparable.
  auto args = argparse::parse<Args>(argc, argv);

  int bad = false;

  char *kVerStr = getKernelVer();
  bad += IS_ZERO(kVerStr);

  /* TODO: add features to read from ~/.inflocal when can't find
   * /etc/os-release or are told to by cmd args */
  char *linName = getLinNmFrom(args.verinfo.c_str());
  bad += IS_ZERO(linName);

  char *timeStr = getTimeStr();
  bad += IS_ZERO(timeStr);

  if (!bad) {
    printf("Running kernel %s on %s\n", kVerStr, linName);
    // this culminates in us having a blank line below
    printf("%s\n", timeStr);
  } else
    printf("Oops something went wrong error #%d\n", bad);

  void *to_clean[] = {(void *)kVerStr, (void *)linName, (void *)timeStr};
  return cleanUp(to_clean, sizeof(to_clean) / sizeof(void *));
}
