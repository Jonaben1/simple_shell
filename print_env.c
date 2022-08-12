#include <stdio.h>

/**
 * main - a function that prints the current environment
 * @argc: argument count
 * @argv: arrays of strings pointers
 * @envp: environment variables
 * Return: Always 0 success
*/

int main(int argc, char *argv[], char *envp[])
{
	int i;

	for (i = 0; envp[i] != NULL; i++)
		printf("\n%s", envp[i]);
	getchar();
	return (0);
}

