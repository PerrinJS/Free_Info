#include <linux/kernel.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <time.h>

void stripReturn(char *inFile) {
  while (*inFile != '\0') {
    if (*inFile == '\n') {
      *inFile = ' ';
    }

    inFile++;
  }
}

char *getLinVer(char *linVer, int buffLength) {
  FILE *sysInfo;
  // link the system-release file to the sysInfo file pointer
  sysInfo = fopen("/etc/system-release", "r");
  fgets(linVer, buffLength, sysInfo);
  fclose(sysInfo);
  stripReturn(linVer);
  return linVer;
}

_Bool systemname() {
  _Bool failure = 0;
  struct utsname namedata;
  failure = (_Bool)uname(&namedata);
  char fileBuff[42];
  int buffLength = 42;

  // This was taken out of this just says what type of cpu "on a %s system\n"
  // namedata.machine
  printf("Running kernel %s on %s\n", namedata.release,
         getLinVer(fileBuff, buffLength));
  return failure;
}

_Bool lintime() {
  char current_time[81];  // A string to hold the final printed line
  struct timeval thetime; // To store the current unix time in a timeval struct
  struct tm
      *timeinfo; // To store the converted time since 1970 to localtime format
  gettimeofday(&thetime, NULL);          // get the unix time
  timeinfo = localtime(&thetime.tv_sec); // Convert the Unix time to localtime
  // Format the localtime into a string
  strftime(current_time, 80, "It is currently %A the %d/%m/%G at %R\n",
           timeinfo);
  puts(current_time);

  return 0;
}

_Bool systeminformation() {
  _Bool failure = 0;
  const double megabyte = 1024 * 1024;
  struct sysinfo information;
  failure = (_Bool)sysinfo(&information);

  puts("Here is your system info:");
  printf("total ram in this system: %5.1f MB\n",
         information.totalram / megabyte);
  printf("free ram in this system: %5.1f MB\n", information.freeram / megabyte);
  printf("%d processess are running\n", information.procs);
  return failure;
}

int main(void) {
  _Bool failure = 0;

  failure = systemname();

  if (failure == 0) {
    failure = lintime();
  }

  /*if(failure == 0) {
          failure = systeminformation();
  }*/

  if (failure == 1) {
    puts("Something went wrong!");
  }
}
