#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>

#include <stdbool.h>
#include "cvar.h"
typedef enum {
  oCmdUnknown, oCmdSet,
} TokId;

typedef struct {
    const char *name;
    TokId tokid;
    void *option;
    int (*set)(const char *, const char *, unsigned, void *);
} Command;

static Command s_commands[] = {
    { "set",  oCmdSet       },
    { NULL,   oCmdUnknown   }
};

int Cmd_Exec(const char *, char *);
static TokId _get_tokId(const char *);
static void _whiteout(char s[]);

#define  MAX_ARGS 128
static char *argv[ MAX_ARGS ];
static int argc;

int Cmd_Argc()
{
    return argc;
}

const char * Cmd_Argv(int n)
{
    if (Cmd_Argc() >= n)
        return argv[ n ];

    return NULL;
}

int Cmd_MapArgs(char *argv[static MAX_ARGS], int *argc, char *input)
{
    int err;
    int idx = 0;

    assert(input);
    *argc = 0;

    _whiteout(input);

    while (*input && strchr("\t\n", *input))
        ++input;

    for (;;) {
        char *arg = input;    
        while (*input && !strchr(" \t\n", *input))
            ++input;

        if (arg)
            argv[ (*argc)++ ] = arg;

        if (*input)
            *input++ = '\0';
        else
            return 0;
    }

    return 0;
}

int Cmd_Exec( const char *prefix, char *line ) 
{
    int ret = 0;

    Cmd_MapArgs(argv, &argc, line);

    if (argc >= 1)
    {
        switch (_get_tokId(argv[0])) {
            default:
                break;
            case oCmdSet:
                {
                    cvar_t * tmp = NULL;

                    if (argc >= 3)
                    {
                        tmp = Cvar_Set(argv[1], argv[2]);
                    }

                    if (tmp)
                    {
                        printf("Ok\n");
                    }
                    else
                    {
                        printf("Error\n");
                    }
                }
                break;
        }
    }
    return ret;
}

static TokId
_get_tokId( const char *src ) {
    int i;
    for(i = 0; s_commands[i].name; i++)
        if (strcasecmp(src, s_commands[i].name) == 0)
            return s_commands[i].tokid;
    return oCmdUnknown;
}

static void
_whiteout( char s[] ) {
    int i;
    for(i = 0; s[i] != '\0'; i++)
        if(s[i] == '#')
            break;
    for(; s[i] != '\0'; i++)
        s[i] = ' '; // whiteout
}

