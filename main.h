#ifndef _MAIN_N_
#define _MAIN_N_



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
#endif
