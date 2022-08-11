#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * main - a function that reads in commands from the user,
 * interpret them, and act on them
 * Return: Always 0 success
*/

char* kash_read_line();
char** kash_split_line(char *line);
void kash_exec(char **args);
void kash_exit(char **args);
void kash_cd(char **args);
void kash_help(char **args);
int kash_num_builtins();

int main(void)
{
	while (true)
	{
		printf("> ");
		char *line = kash_read_line();
		char **tokens = kash_split_line(line);

		if (tokens[0] != NULL)
		{
			kash_exec(tokens);
		}

		free(tokens);
		free(line);
	}
}

/**
 * kash_read_line - a function that uses the getline() function from <stdio.h> to read a single line of input from stdin.
 * Return: Always 0 success
 */

char* kash_read_line(void)
{

	char *line = NULL;
	size_t buflen = 0;
	getline(&line, &buflen, stdin);
	return (line);
}

/**
 * kash_split_line - a function that split the input string into an array of individual tokens.
 * @line: a character pointer being used
 * Return: Always 0 success
*/

char** kash_split_line(char *line)
{
	int length = 0;
	int capacity = 16;
	char **tokens = malloc(capacity * sizeof(char*));

	char *delimiters = " \t\r\n";
	char *token = strtok(line, delimiters);

	while (token != NULL)
	{
		tokens[length] = token;
		length++;

		if (length >= capacity)
		{
			capacity = (int) (capacity * 1.5);
			tokens = realloc(tokens, capacity * sizeof(char*));
		}

		token = strtok(NULL, delimiters);
	}

	tokens[length] = NULL;
	return (tokens);
}


/**
 * kash_exit - a function that exits a shell
 * @args: array of string pointers
*/

void kash_exit(char **args)
{
	exit(0);
}


/**
 * kash_cd - a function that supports the cd linux command
 * @args: array of string pointers
 * Return: 0 success
*/

void kash_cd(char **args)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "kash: cd: missing argument\n");
	}
	else
	{
		if (chdir(args[1]) != 0)
		{
			perror("kash: cd");
		}
	}
}


/**
 * kash_help - a function that implements the linux help function
 * which simply print a list of available builtins.
 * @args: array of string pointers
 * Return: Always 0 success
*/

void kash_help(char **args)
{

	char *helptext =
        "Kash - the Kinda Aimless Shell. "
        "The following commands are available:\n"
        "  cd       Change the working directory.\n"
        "  exit     Exit the shell.\n"
        "  help     Print this help text.\n";
	printf("%s", helptext);
}


/**
 * struct builtin - a function for registering the association between a command name and its handler function
 * @name: name of the function
 * @args: array of structs
*/

struct builtin
{
	char *name;
	void (*func)(char **args);
};

struct builtin builtins[] = {
    {"help", kash_help},
    {"exit", kash_exit},
    {"cd", kash_cd},
};

/**
 * kash_ num_builtins - a function that handles the command name and its handler function, that is the builtin commands
 * Return: always 0 success
*/

int kash_num_builtins()
{
	return sizeof(builtins) / sizeof(struct builtin);
}


/**
 * kash_exec - a function that launches a new process which checks for a 
 * builtin command before launching an external process.
 * @args: array of string pointers
 * Return: alway 0 success
*/

void kash_exec(char **args)
{
	for (int i = 0; i < kash_num_builtins(); i++)
	{
		if (strcmp(args[0], builtins[i].name) == 0)
		{
			builtins[i].func(args);
			return;
		}
	}

	pid_t child_pid = fork();

	if (child_pid == 0)
	{
		execvp(args[0], args);
		perror("kash");
		exit(1);
	}
	else if (child_pid > 0)
	{
		int status;
		do {
		waitpid(child_pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	else
	{
		perror("kash");
	}
}
