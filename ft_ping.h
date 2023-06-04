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

typedef struct s_ping_pckt
{
    struct icmphdr hdr;
    char *content;
}               t_ping_pckt;

//parser.c
int check_flags(char **argv, t_flags *flags);
void parse_hosts(size_t argc, char **argv, struct sockaddr_in *addr);
//err.c
void print_errhost(char *host);
void print_help();
void print_errop(char c);
//signal.c
void intHandle(int i);
void setsignal();
//icmp.c
unsigned short checksum(void *b, int len);

#endif