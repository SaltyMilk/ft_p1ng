#ifndef FT_PING_H
#define FT_PING_H
#include "libft/libft.h"
#include <sys/time.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

typedef struct s_flags
{
    char    vflag;
}               t_flags;



//err.c
void print_help();
void print_errop(char c);

#endif