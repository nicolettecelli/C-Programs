#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
	Nicolette Celli
	4174075
	I affirm that this program is entirely my own work and none of it is the work of any other person.
*/

// This program forks into two processes, ls and sort, and terminates once both processes are complete.
int main(int argc, char **argv) // may not need argc argv
{
	// Children
	pid_t child_ls, child_sort;
	int pipefd[2];

	pipe(pipefd);

	// Commands
	char *ls_args[] = { "ls", NULL };
	char *sort_args[] = { "sort", NULL };

	// Execute ls and pipe to stdout
	if ((child_ls = fork()) == 0)
	{
		printf("I AM THE FIRST CHILD PROCESS\n");
		dup2(pipefd[1], 1);
		close(pipefd[0]);
		execvp("ls", ls_args);
		fprintf(stderr, "'ls' failed.\n");
	}
	else if (child_ls == -1)
	{
		fprintf(stderr, "First fork failed.\n");
		exit(1);
	}
	else
	{
		// Execute sort from the output of ls
		if ((child_sort = fork()) == 0)
		{
			printf("I AM THE SECOND CHILD PROCESS\n");
			dup2(pipefd[0], 0);
			close(pipefd[1]);
			execvp("sort", sort_args);
			fprintf(stderr, "'sort' failed.\n");
		}
		else if (child_sort == -1)
		{
			fprintf(stderr, "Second fork failed.\n");
			exit(1);
		}
		else	// Parent
		{
			// Close file descriptors
			close(pipefd[0]);
			close(pipefd[1]);
			// Wait
			waitpid(child_ls, NULL, 0);
			waitpid(child_sort, NULL, 0);
			printf("PARENT FINISHES\n");
		}
	}
}