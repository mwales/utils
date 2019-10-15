#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

void printUsage(char* appName)
{
	printf("%s -h        # For help\n", appName);
	printf("%s numOutputFiles bytesPerLine inputFilename  # Deinterlace a file\n", appName);
	printf("%s numOutputFiles bytesPerLine -              # Deinterlace stdin\n", appName);
	printf("\n");
	printf("For example 3 files, 2 bytes per line of the following data:\n");
	printf(" 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0\n\n");
	printf("Creates the following files:\n\n");
	printf("deint1.bin:\n");
	printf("01 02 07 08 0d 0e\n\n");
	printf("deint2.bin:\n");
	printf("03 04 09 0a 0f\n\n");
	printf("deint3.bin:\n");
	printf("05 06 0b 0c\n\n");
}



int main(int argc, char** argv)
{
	if (argc != 4)
	{
		printUsage(argv[0]);
		return 1;
	}

	int inputFileFd;
	if (strcmp(argv[3], "-") == 0)
	{
		// Input file is stdin!
		printf("Reading input from stdin\n");
		inputFileFd = 1;
	}
	else
	{
		inputFileFd = open(argv[3], O_RDONLY);
		if (inputFileFd == -1)
		{
			printf("Error opening file %s\n", argv[3]);
			printf("Error = %s\n", strerror(errno));
			return 2;
		}

		printf("Opened file %s for reading\n", argv[3]);
	}

	// Allocate memory for all the fds for output
	int numFiles = atoi(argv[1]);
	int bytesPerChunk = atoi(argv[2]);

	// Validate values passed by the user
	if (numFiles <= 1)
	{
		printf("Invalid number of files specified: %s\n", argv[1]);
		return 3;
	}

	if (bytesPerChunk <= 0)
	{
		printf("Invalid number of bytes specified: %s\n", argv[2]);
		return 3;
	}

	// Allocate memory for all the fds for output
	int* outputFds = (int*) malloc(sizeof(int) * numFiles);

	// Open all the output file
	for(int i = 0; i < numFiles; i++)
	{
		char filename[100];
		sprintf(filename, "deint_%d.bin", i);
		outputFds[i] = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);

		if (outputFds[i] == -1)
		{
			printf("Error trying to open output file %s\n", filename);
			printf("Error = %s\n", strerror(errno));

			// Close the files we already opened
			for(int j = 0; j < i; j++)
			{
				close(outputFds[j]);
			}

			return 4;
		}
	}

	int currentOutputFile = 0;
	int curByteInBlock = 0;
	int totalBytesRead = 0;
	while(1)	
	{
		char curByte;
		if (1 != read(inputFileFd, &curByte, 1) )
		{
			printf("Failed to read any more bytes at byte %d\n", totalBytesRead);
			break;
		}

		if (1 != write(outputFds[currentOutputFile], &curByte, 1))
		{
			printf("Failed ot write any more bytes at byte %d\n", totalBytesRead);
			break;
		}

		// Update counts and current pointers
		totalBytesRead++;
		curByteInBlock++;

		if (curByteInBlock == bytesPerChunk)
		{
			curByteInBlock = 0;
			currentOutputFile++;
		}

		if (currentOutputFile == numFiles)
		{
			currentOutputFile = 0;
		}
	}

	printf("Exiting after %d bytes copied\n", totalBytesRead);	
		
		
	if (inputFileFd != 1)
	{
		// If the input file is a file (and not stdin), close it
		printf("Closing intput file\n");
		close(inputFileFd);
		inputFileFd = -1;
	}

	// Close the output files
	for(int i = 0; i < numFiles; i++)
	{
		printf("Closing output file %d\n", i);
		close(outputFds[i]);
	}
	
	return 0;
}
