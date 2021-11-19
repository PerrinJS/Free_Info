#include <stdio.h>
#include <time.h>
#include <linux/kernel.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/utsname.h>

void stripReturn(char *inString)
{
    while(*inString != '\0')
    {
        if(*inString == '\n')
        {
            *inString = ' ';
        }
        
        inString++;
    }
}

void stripSpace(char *inString)
{
    while(*inString != '\0')
    {
        if(*inString == ' ')
        {
            *inString = '\0';
        }
        else inString++;
    }
}

/*int strLength(char *inString)
{
    int length = 0;

    while(*inString != '\0')
    {
        length++;
        inString++;
    }

    return length;
}*/

_Bool getFileData(char *fileBuff, char *fileWorkingDir, int bufferLength)
{
    FILE *fileLocal;
    _Bool failure = 0;
    
    fileLocal = fopen(fileWorkingDir, "r");
    if(fileLocal == NULL)
    {
        failure = 1;
    }
    else
    {
        fgets(fileBuff, bufferLength, fileLocal);
        failure = (_Bool)fclose(fileLocal);
        if(!failure)
        {
            stripReturn(fileBuff);
        }
    }
    return failure;
    
}

_Bool getVerData(char *fileBuff, int buffLength)
{
    _Bool failure = 0;
    char settingsLocal[] = "/home/sand49/.inflocal";
    char infoLocation[42];
    int locationLength = 42;

    failure = getFileData(infoLocation, settingsLocal, locationLength);
    if(failure != 1)
    {
        stripSpace(infoLocation);
	failure = getFileData(fileBuff, infoLocation, buffLength);
    }
    

    return failure;
}

_Bool systemname() {
    _Bool failure = 0;
    struct utsname namedata;
    failure = (_Bool)uname(&namedata);
    char fileBuff[81];
    int buffLength = 81;

    _Bool verDataFailure = getVerData(fileBuff, buffLength);

    if(!failure && !verDataFailure)
    {
        //This was taken out of this just says what type of cpu "on a %s system\n" namedata.machine
        printf("Running kernel %s on %s\n", namedata.release, fileBuff);
    }
    else failure = 1;

    return failure;
}

    _Bool lintime() {
	char current_time[81]; //A string to hold the final printed line
	struct timeval thetime; //To store the current unix time in a timeval struct
	struct tm * timeinfo; //To store the converted time since 1970 to localtime format
	gettimeofday(&thetime, NULL); //get the unix time
	timeinfo = localtime (&thetime.tv_sec); //Convert the Unix time to localtime
    //Format the localtime into a string
	strftime (current_time,80,"It is currently %A the %d/%m/%G at %R\n",timeinfo);
	puts(current_time);

	return 0;
}

_Bool systeminformation() {
	_Bool failure = 0;
	const double megabyte = 1024 * 1024;
	struct sysinfo information;
	failure = (_Bool)sysinfo(&information);

	puts("Here is your system info:");
 	printf("total ram in this system: %5.1f MB\n", information.totalram / megabyte);
 	printf("free ram in this system: %5.1f MB\n", information.freeram / megabyte);
 	printf("%d processess are running\n", information.procs);
 	return failure;
}

int main(void)
{
	_Bool failure = 0;

	failure = systemname();

	if(failure == 0) {
        failure = lintime();
	}

	/*if(failure == 0) {
		failure = systeminformation();
	}*/
	
	if(failure == 1) {
        puts("Something went wrong!");
    }
	
}
