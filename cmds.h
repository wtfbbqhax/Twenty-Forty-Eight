#ifndef CMDS_H
#define CMDS_H

int          Cmd_Exec(Game*, char *);
int          Cmd_Argc(void);
const char * Cmd_Argv(int);
int          Cmd_MapArgs(char *argv[], int *argc, char *input);

#endif /* end of include guard: CMDS_H */

