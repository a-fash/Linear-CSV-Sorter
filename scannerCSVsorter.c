#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include "directoryAndFile.h"

char *sortColumn;
char *sortDirectory;
char *outputDirectory;
int hasSortColumn;
int hasSortDirectory;
int hasOutputDirectory;

void freeData();
int procCount(char *);

int sortColumnType(char *);

int NULLTERMINATOR = 1;
//int LINEBUFFER = sizeof(char) * 512;
//int DATABUFFER = sizeof(char) * 128;

int LINEBUFFER;
int DATABUFFER;

int main(int argc, char *argv[])
{
	clock_t start;
	start = clock();

	struct timeval s;
	gettimeofday(&s, NULL);

	int maxFileAndDirectories = 255;
	int maxPIDlength = 5;

	hasSortColumn = 0;
	hasSortDirectory = 0;
	hasOutputDirectory = 0;
	char *flag;

	LINEBUFFER = atoi(argv[7]);
	DATABUFFER = atoi(argv[8]);

	/*
	if(argc-1 > 6)
	{
		fprintf(stderr, "ERROR: Input fields is greater than 6\n");
		freeData();
		return -1;
	}
	*/

	if((argc-1) % 2 == 1)
	{
		fprintf(stderr, "ERROR: Flag missing field or field missing flag\n");
		freeData();
		return -1;
	}

	int i;
	for(i=1; i < 6; i+=2)
	{
		flag = (char *)malloc(sizeof(char) * (strlen(argv[i]) + NULLTERMINATOR) );
		strcpy(flag, argv[i]);

		if(strcmp("-c", flag) == 0)
		{
			sortColumn = (char *)malloc(sizeof(char) * (strlen(argv[i+1]) + NULLTERMINATOR) );
			strcpy(sortColumn, argv[i+1]);
			hasSortColumn =	1;
		}else if(strcmp("-d", flag) == 0)
		{
			sortDirectory = (char *)malloc(sizeof(char) * (strlen(argv[i+1]) + NULLTERMINATOR) );
			strcpy(sortDirectory, argv[i+1]);
			hasSortDirectory = 1;
		}else if(strcmp("-o", flag) == 0)
		{
			outputDirectory = (char *)malloc(sizeof(char) * (strlen(argv[i+1]) + NULLTERMINATOR) );
			strcpy(outputDirectory, argv[i+1]);
			hasOutputDirectory = 1;
		} else
		{
			fprintf(stderr, "ERROR: INCORRECT FLAG %s \n", argv[i+1]);
			free(flag);
			freeData();
			return -1;
		}
		free(flag);
	}

	if(hasSortColumn == 0)
	{
		fprintf(stderr, "FATAL ERROR: No sort column given\n");
		freeData();
		return -1;
	}
	if(sortColumnType(sortColumn) < 0)
	{
		fprintf(stderr, "FATAL ERROR: Sort column incorrect\n");
		freeData();
		return -1;
	}

	char cwd[PATH_MAX + NULLTERMINATOR];

        if(hasSortDirectory == 0)
        {
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			sortDirectory = (char *)malloc(sizeof(char) * (strlen(cwd) + NULLTERMINATOR) );
			strcpy(sortDirectory, cwd);
			printf("sort directory: %s\n", sortDirectory);
		} else
		{
			fprintf(stderr, "Fatal ERROR: cannot get current working directory for sort directory\n");
			freeData();
			return 1;
		}
        }
        if(hasOutputDirectory == 0)
        {
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			outputDirectory = (char *)malloc(sizeof(char) * (strlen(cwd) + NULLTERMINATOR) );
			strcpy(outputDirectory, cwd);
		} else
		{
			fprintf(stderr, "Fatal ERROR: cannot get current working directory for output directory\n");
			freeData();
			return 1;
		}
        }

        fprintf(stdout, "Initial PID: %d\n", getpid());

	FILE *temp = tmpfile();
	
	fprintf(temp, "PIDS of all child processes: ");

	//Use stdout as buffer for child pids
	//fprintf(stdout, "%s", "PIDS of all child processes: ");
	//fflush(stdout);

	//fflush(temp);

        directoryHandler(sortDirectory, outputDirectory, sortColumn, temp);

	rewind(temp);

	char *childProcs = (char *)malloc(sizeof(char) * (maxFileAndDirectories * (maxPIDlength + 1)) );

	fgets(childProcs, maxFileAndDirectories * (maxPIDlength + 1), temp);

	fclose(temp);

        fprintf(stdout, "%s\n", childProcs);

        fprintf(stdout,	"Total number of processes: %d\n", procCount(childProcs));

	clock_t end;
	end = clock();

	struct timeval e;
	gettimeofday(&e, NULL);

	double diff;
	diff = (double)(end - start) / CLOCKS_PER_SEC;

	double d = (double) (e.tv_usec - s.tv_usec) / 1000000 +(double) (e.tv_sec - s.tv_sec);

	fprintf(stdout, "CPU Time taken %f\n", diff);
	fprintf(stdout, "USER Time taken %f\n", d);

	char resultName[50];

	sprintf(resultName, "result/result-L%d-D%d.txt", LINEBUFFER, DATABUFFER); 

	FILE *result = fopen(resultName, "a+");

	fprintf(result, "%f\n", diff);

	fclose(result);

	free(childProcs);
	freeData();

	return 0;
}

void freeData()
{

	if(hasSortColumn == 1)
		free(sortColumn);
	if(hasSortDirectory == 1)
		free(sortDirectory);
	if(hasOutputDirectory == 1)
		free(outputDirectory);

	return;
}

int procCount(char *CSpid)
{
	char *ptr;
	int count = 0;;

	ptr = CSpid + strlen("PIDS of all child processess: ");

	while(*ptr != '\0')
	{
		ptr = strchr(ptr, ',');
		count++;
		ptr++;
	}

	return count;
}
