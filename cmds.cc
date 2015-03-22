#include <iostream>

using namespace std;

#include <assert.h>
#include <stdlib.h>
//#include <stdio.h>
//#include <stdint.h>
//#include <limits.h>
//#include <ctype.h>
//#include <string.h>
#include <strings.h>
//#include <stdlib.h>
//#include <errno.h>

#include <stdbool.h>
#include "game.h"
#include "q_trie.h"
#include "cvar.h"
#include "cmds.h"

typedef struct {
    const char *keyword;
    void (*handler)(Game*);
} Command;

void Trace(void)
{
    FILE *fp = fopen("Trace.out", "a");
    fprintf(fp, "Got here\n");
    fclose(fp);

}

static Command s_commands[] =
{
    {   
        "close", 
        [](Game*g)
        { Cvar_Set("g_console", "0"); }
    },
    { 
        "down",  
        [](Game*g)
        { Trace();g->move(Down); }
    },
    { 
        "left",
        [](Game*g)
        { g->move(Left); }
    },
    { 
        "quit",
        [](Game*g)
        { exit(0); }
    },
    { 
        "reset",  
        [](Game*g)
        { g->reset( ); }
    },
    { 
        "right",
        [](Game*g)
        { g->move(Right); }
    },
    { 
        "set",
        [](Game*g)
        { Cvar_Set(Cmd_Argv(1),Cmd_Argv(2)); }
    },
    { 
        "up",
        [](Game*g)
        { g->move(Up); }
    },
};


#define ARRAY_LEN(x) (sizeof(x) / sizeof(*(x)))
const size_t s_commands_cnt = ARRAY_LEN(s_commands);

#define  MAX_ARGS 128
static char *argv[ MAX_ARGS ];
static int   argc;

int Cmd_Argc(void)
{
    return argc;
}

const char * Cmd_Argv(int n)
{
    if (Cmd_Argc() >= n)
        return argv[ n ];

    return NULL;
}

static void _whiteout(char s[])
{
    int i;
    for(i = 0; s[i] != '\0'; i++)
        if(s[i] == '#')
            break;
    for(; s[i] != '\0'; i++)
        s[i] = ' '; // whiteout
}

int Cmd_MapArgs(char **argv, int *argc, char *input)
{
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

static int CmdCmp(const void *a, const void *b)
{
    return strcasecmp((const char*)a,((Command*)b)->keyword);
}

Command *Cmd_GetCmd(const char *cmd)
{
    return (Command*)bsearch(cmd, s_commands, s_commands_cnt, sizeof(Command), CmdCmp);
}

int Cmd_Exec(Game *g, char *input) 
{
    int ret = 0;

    Cmd_MapArgs(argv, &argc, input);

    if (Cmd_Argc() >= 1)
    {
        Command *cmd = Cmd_GetCmd(Cmd_Argv(0));
        if (cmd && cmd->handler)
        {
            cmd->handler(g);
        }
    }

    return ret;
}


