#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ARGS 20
#define BUFSIZ2 1024

/*
 * 	Nicolette Celli
 * 	4174075
 * 	I affirm that this program is entirely my own work and none of it is the work of any other person.
 */

// This program is a shell program which can pipeline and redirect input and output.

// Cmdline arguments
char *args[MAX_ARGS];
char *temp[MAX_ARGS];
char *tempAfter[MAX_ARGS]; 

// Input and output flags
int outf = 0; // redirect output
int outaf = 0; // append output
int inf = 0; // redirect input

char *inFile = NULL;
char *outFile = NULL;

// Call methods
void execute(char *cmdline);
int get_args(char *cmdline, char *args[]);
int copyArgs(char **args, char **temp, char *s, int mode);
void resetVar();
void swap(char **args2, char **temp2);
int process(int in, int out, char **inData, char **outData);
int pipeCount();
int fileNames(char **args);

// main method from myshell.c
int main(int argc, char **argv)
{
	char cmdline[BUFSIZ2];

	for (;;) {
		printf("COP4338$ ");
		if (fgets(cmdline, BUFSIZ2, stdin) == NULL)
		{
			perror("fgets failed");
			exit(1);
		}
		execute(cmdline);
	}
	return 0;
}

// get_args from myshell.c
int get_args(char *cmdline, char *args[])
{
	int i = 0;

	/* if no args */
	if ((args[0] = strtok(cmdline, "\n\t ")) == NULL)
		return 0;

	while ((args[++i] = strtok(NULL, "\n\t ")) != NULL) {
		if (i >= MAX_ARGS) {
			printf("Too many arguments!\n");
			exit(1);
		}
	}
	/* the last one is always NULL */
	return i;
}

// Seperates argument on a symbol into 2 parts and places them in args and temp respectively
int copyArgs(char **args, char **temp, char *s, int mode)
{

	int i = 0;
	int j;
	int k = 0;

	while (args[i] != NULL)
	{
		if (mode == 0) // Compare to s
		{
			if (!strcmp(args[i], s))
			{
				args[i] = NULL;
				i++;
				while (args[i] != NULL)
				{
					temp[k] = args[i];
					args[i] = NULL;
					i++;
					k++;
				}
				i--;

			}
			i++;
		}
		if (mode == 1) // Compare to > >> or <
		{
			if (!strcmp(args[i], ">") || !strcmp(args[i], ">>") || !strcmp(args[i], "<"))
			{
				args[i] = NULL;
				i++;
				while (args[i] != NULL)
				{
					temp[k] = args[i];
					args[i] = NULL;
					i++;
					k++;
				}
				i--;

			}
			i++;
		}
	}
	
	while (k < MAX_ARGS)
	{
		temp[k] = NULL;
		k++;
	}


	return 0;
}

// Resets variables
void resetVar()
{
	*args = NULL;
	*temp = NULL;
	*tempAfter = NULL;
	outf = 0;
	outaf = 0;
	inf = 0;
	inFile = NULL;
	outFile = NULL;
}

void execute(char *cmdline)
{
	int n = get_args(cmdline, args);
	if (n <= 0) // no arguments 
		return;

	if (!strcmp(args[0], "quit") || !strcmp(args[0], "exit")) 
		exit(0);

	int numPipes = pipeCount();

	int input;
	int output;
	input = dup(1); // stdin
	output = dup(0); // stdout
					 
	if (numPipes == 0)
	{
		int pid1;
		pid1 = process(input, output, args, tempAfter);
		waitpid(pid1, NULL, 0);
		resetVar();
	}

	if (numPipes > 0)
	{
		int proc;
		int pipefd[2];
		int nextIn = 0; 
		int pid2;

		for (proc = 0; proc < numPipes; proc++)
		{
			copyArgs(args, temp, "|", 0);
			pipe(pipefd);

			pid2 = process(nextIn, pipefd[1], args, tempAfter);
			waitpid(pid2, NULL, 0);
			close(pipefd[1]); 

			swap(args, temp);
			nextIn = pipefd[0];
		}

		if (nextIn != 0)
			dup2(nextIn, 0);

		pid2 = process(nextIn, output, args, tempAfter);
		waitpid(pid2, NULL, 0);
	}
	// reset for next execution
	resetVar(); 
	dup2(input, 0); 
	dup2(output, 1);
}

void swap(char **args2, char **temp2)
{
	char *tempCopy[MAX_ARGS];
	int i = 0;
	while (i < MAX_ARGS)
	{
		tempCopy[i] = args2[i];
		i++;
	}

	i = 0;
	while (i < MAX_ARGS)
	{
		args2[i] = temp2[i];
		i++;
	}

	i = 0;
	while (i < MAX_ARGS)
	{
		temp2[i] = tempCopy[i];
		i++;
	}
}

// Controls input and output redirection
int process(int inPipe, int outPipe, char **inData, char **outData)
{
	int pid;

	if ((pid = fork()) == 0)
	{
		if (inPipe != 0)
		{
			dup2(inPipe, 0);
			close(inPipe);
		}

		if (outPipe != 1)
		{
			dup2(outPipe, 1);
			close(outPipe);
		}

		int err = 0;
		int outr;
		int inr;

		err = fileNames(inData);
		if (err != 0)
			exit(1);

		// in/out redirection
		if (inf == 1 && (outf == 1 || outaf == 1))
			copyArgs(inData, outData, "", 1); 
		// in redirection
		else if (inf == 1 && outf == 0 && outaf == 0)
			copyArgs(inData, outData, "<", 0);
		// out redirection
		else if ((outf == 1 || outaf == 1) && inf == 0)
		{	
			if (outf == 1)
				copyArgs(inData, outData, ">", 0);
			if (outaf == 1)
				copyArgs(inData, outData, ">>", 0);
		}

		if (outf == 1)
		{
			outr = open(outFile, O_CREAT | O_RDWR | O_TRUNC, 0644);
			if (outr < 0)
			{
				perror("File cannot be opened.");
				exit(1);
			}
		}
		else if (outaf == 1)
		{
			outr = open(outFile, O_CREAT | O_RDWR | O_APPEND, 0644);
			if (outr < 0)
			{
				perror("File cannot be opened.");
				exit(1);
			}
		}

		if (inf == 1)
		{
			inr = open(inFile, O_RDONLY, 0644);
			if (inr < 0)
			{
				perror("File cannot be opened.");
				exit(1);
			}
		}

		if (outf == 1 || outaf == 1)
		{
			if (dup2(outr, fileno(stdout)) < 0)
			{
				perror("dup2 error");
				exit(1);
			}
			close(outr);
		}

		if (inf == 1)
		{
			if (dup2(inr, fileno(stdin)) < 0)
			{
				perror("dup2 error");
				exit(1);
			}
			close(inr);
		}

		execvp(inData[0], inData);
		perror("Execution failed");
		exit(-1);
	}
	if (pid < 0)
	{
		perror("Forking error");
		exit(1);
	}

	close(inPipe); 
	close(outPipe);
	return pid;
}

// Counts the number of pipes
int pipeCount()
{
	int i = 0;
	int n = 0;

	while (args[i] != NULL)
	{
		if (!strcmp(args[i], "|"))
			n++;

		i++;
	}

	return n;
}

// Sets flags and filenames
int fileNames(char **args)
{
	int i = 0;

	while (args[i] != NULL)
	{
		if (!strcmp(args[i], ">"))
		{
			outFile = args[i + 1];
			outf = 1;
		}
		if (!strcmp(args[i], ">>"))
		{
			outFile = args[i + 1];
			outaf = 1;
		}
		if (!strcmp(args[i], "<"))
		{
			inFile = args[i + 1];
			inf = 1;
		}
		i++;
	}

	return 0;
}
