#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_ARGS 20
#define BUFSIZ2 1024

/*
* 	Nicolette Celli
* 	4174075
* 	I affirm that this program is entirely my own work and none of it is the work of any other person.
*/

// This program is an extension of the myshell.c program, and can now pipeline and redirect input and output.

// Cmdline arguments
char *args[MAX_ARGS];
char *temp[MAX_ARGS];
char *post[MAX_ARGS];

// Flags
int outf = 0;
int outaf = 0;
int inf = 0;

// Input and output
char *inFile = NULL;
char *outFile = NULL;

// Call methods
void execute(char* cmdline);
int get_args(char* cmdline, char* args[]);
int copyArg(char **args, char **argsTemp, char* symb, int mode);
void resetVar();
void swap(char ** args2, char **argsTemp2);
int process(int in, int out, char **inData, char **outData);
int countPipes();
int fileIO(char **args);

int main(int argc, char **argv)
{
	char cmdline[BUFSIZ2];

	for (;;)
	{
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

int get_args(char *cmdline, char *args[])
{
	int i = 0;

	// if no args
	if ((args[0] = strtok(cmdline, "\n\t")) == NULL)
		return 0;

	while ((args[++i] = strtok(NULL, "\n\t")) != NULL)
	{
		if (i >= MAX_ARGS)	// error, greater than number of max arguments
		{
			printf("Too many arguments.\n");
			exit(1);
		}
	}
	// The last one is always null
	return i;
}

int copyArg(char **args, char **temp, char *symbol, int mode)
{
	int i = 0;
	int j;
	int k = 0;

	while (args[i] != NULL)
	{
		if (mode == 0)
		{
			if (!strcmp(args[i], symbol))
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
		if (mode == 1)
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

void resetVar() // resets variables
{
	*args = NULL;
	*temp = NULL;
	*post = NULL;
	outf = 0;
	outaf = 0;
	inf = 0;
	inFile = NULL;
	outFile = NULL;
}

void execute(char *cmdline)
{
	int num = get_args(cmdline, args);

	if (num <= 0)
		return;

	if (!strcmp(args[0], "quit") || !strcmp(args[0], "exit"))
		exit(0);

	int numPipes = countPipes();

	int in;
	int out;
	in = dup(1);
	out = dup(0);

	if (numPipes == 0) // If there are no pipes
	{
		int pid;
		pid = process(in, out, args, post);
		waitpid(pid, NULL, 0);
		resetVar();
	}
	if (numPipes > 0) // If there are pipes
	{
		int numProc;
		int pipefd[2];
		int n = 0;
		int pid2;

		for (numProc = 0; numProc < numPipes; numProc++)
		{
			copyArg(args, temp, "|", 0);

			pipe(pipefd);
			pid2 = process(n, pipefd[1], args, post);
			waitpid(pid2, NULL, 0);
			close(pipefd[1]);

			swap(args, temp);
			n = pipefd[0];
		}

		if (n != 0)
			dup2(n, 0);

		pid2 = process(n, out, args, post);
		waitpid(pid2, NULL, 0);
	}

	resetVar();
	dup2(in, 0);
	dup2(out, 1);
}

void swap(char **args, char **temp)
{
	char *t[MAX_ARGS];

	int i = 0;
	while (i < MAX_ARGS)
	{
		t[i] = args[i];
		i++;
	}

	int j = 0;
	while (j < MAX_ARGS)
	{
		args[j] = temp[j];
		j++;
	}

	int k = 0;
	while (k < MAX_ARGS)
	{
		temp[k] = t[k];
		k++;
	}
}

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

		err = fileIO(inData);
		if (err != 0)
			exit(1);	// error

		if (inf == 1 && (outf == 1 || outaf == 1))
			copyArg(inData, outData, "", 1);
		else if (inf == 1 && outf == 0 && outaf == 0)
			copyArg(inData, outData, "<", 0);
		else if ((outf == 1 || outaf == 1) && inf == 0)
		{
			if (outf == 1)
				copyArg(inData, outData, ">", 0);
			if (outaf == 1)
				copyArg(inData, outData, ">>", 0);
		}

		// Set file for output redirection
		if (outf == 1)
		{
			outr = open(outFile, O_CREAT | O_RDWR | O_TRUNC, 0644);
			if (outr < 0)
			{
				printf("File could not be opened.\n");
				exit(1);
			}
		}
		else if (outaf == 1)
		{
			outr = open(outFile, O_CREAT | O_RDWR | O_APPEND, 0644);
			if (outr < 0)
			{
				printf("File could not be opened.\n");
				exit(1);
			}
		}

		// Set file for input redirection
		if (inf == 1)
		{
			inr = open(inFile, O_RDONLY, 0644);
			if (inr < 0)
			{
				printf("File could not be opened.\n");
				exit(1);
			}
		}

		// Redirect Output
		if (outf == 1 || outaf == 1)
		{
			if (dup2(outr, fileno(stdout)) < 0)
			{
				printf("dup2 error\n");
				exit(1);
			}
			close(outr);
		}

		// Redirect Input
		if (inf == 1)
		{
			if (dup2(inr, fileno(stdin)) < 0)
			{
				printf("dup2 error\n");
				exit(1);
			}
			close(inr);
		}

		execvp(inData[0], inData);

		printf("Execution failed.\n");
		exit(-1);
	}

	if (pid < 0)
	{
		printf("Forking error\n");
		exit(1);
	}

	close(inPipe);
	close(outPipe);
	return pid;
}

int countPipes()
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

int fileIO(char **args)
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

	return 0; // to check for errors
}