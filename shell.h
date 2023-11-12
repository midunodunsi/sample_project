#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for cnvrt_func() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 * struct passinfo - contains pseudo-arguements to pass into a function,
 * allowing uniform prototype for function pointer struct
 * @arg: a string generated from getline containing arguements
 * @argv:an array of strings generated from arg
 * @path: a string path for the current command
 * @argc: the argument count
 * @line_count: the error count
 * @err_num: the error code for exit()s
 * @linecount_flag: if on count this line of input
 * @fname: the program filename
 * @env: linked list local copy of environ
 * @environ: custom modified copy of environ from LL env
 * @history: the history node
 * @alias: the alias node
 * @env_changed: on if environ was changed
 * @status: the return status of the last exec'd command
 * @cmd_buf: address of pointer to cmd_buf, on if chaining
 * @cmd_buf_type: CMD_type ||, &&, ;
 * @readfd: the fd from which to read line input
 * @histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
		0, 0, 0}

/**
 * struct builtin - contains a builtin string and related function
 * @type: the builtin command flag
 * @func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;



int hsh(info_t *, char **);
int find_builtin(info_t *);
void find_cmd(info_t *);
void run_cmd(info_t *);

int exec_cmd(info_t *, char *);
char *dupl_chars(char *, int, int);
char *find_path(info_t *, char *, char *);

int loophsh(char **);

void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);


int _strlen(char *);
int _strcmp(char *, char *);
char *check_needle(const char *, const char *);
char *_strcat(char *, char *);

char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

char **strtow(char *, char *);
char **strtow2(char *, char);

char *_memset(char *, char, unsigned int);
void free_str(char **);
void *_realloc(void *, unsigned int, unsigned int);

int free_ptr(void **);

int int_mode(info_t *);
int delim_check(char, char *);
int _isalpha(int);
int _atoi(char *);

int _erratoi(char *);
void print_error(info_t *, char *);
int print_d(int, int);
char *cnvrt_func(long int, int, int);
void replce_instce(char *);

int exit_shell(info_t *);
int curr_dec(info_t *);
int change_cd(info_t *);

int my_htry_node(info_t *);
int my_alias_node(info_t *);

ssize_t gets_line(info_t *);
int gets_input(info_t *, char **, size_t *);
void block_ctrl(int);

void init_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);

char *init_env(info_t *, const char *);
int _envcopy(info_t *);
int _setenv(info_t *);
int unset_env(info_t *);
int populates_env(info_t *);

char **copy_env(info_t *);
int rem_env(info_t *, char *);
int set_env(info_t *, char *, char *);

char *get_htry(info_t *info);
int create_file(info_t *info);
int reads_htry(info_t *info);
int adds_entry(info_t *info, char *buf, int linecount);
int renmbr_htry(info_t *info);

list_t *adds_listnode(list_t **, const char *, int);
list_t *adds_nodeend(list_t **, const char *, int);
size_t print_strlist(const list_t *);
int delete_nodeidx(list_t **, unsigned int);
void free_nodelist(list_t **);

size_t lnkd_len(const list_t *);
char **array_str(list_t *);
size_t prnt_listt(const list_t *);
list_t *node_prefix(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

int char_chain(info_t *, char *, size_t *);
void stat_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_token(info_t *);
int token_str(info_t *);
int replace_string(char **, char *);

#endif
