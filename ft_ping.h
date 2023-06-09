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
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>

typedef struct		s_flags
{
    char    		vflag;
    char 			sflag;
	char			Wflag;
	char			aflag;
	char			tflag;
	char			qflag;
	char			cflag;
	char			Dflag;
	long long		cflag_value;
	int				tflag_value;
    int				sflag_value;
    int				Wflag_value;
}               	t_flags;

typedef struct		s_ping_pckt
{
    struct icmphdr	hdr;
    char			*content;
}               	t_ping_pckt;

//parser.c
int          	  	check_flags(char **argv, t_flags *flags);
char*				parse_hosts(size_t argc, char **argv, struct sockaddr_in *addr);
int         	    is_ip(char *s);
//err.c
void				print_errhost(char *host);
void     	    	print_help();
void            	print_errop(char c);
void				print_erromissarg(char c);
void				print_errominvarg(char *s);
void				print_errominvargrangeS(char *s);
void				print_errominvargrangeW(char *s);
void				print_errominvargrangeT(char *s);
void				print_errominvargrangeC(char *s);

//icmp.c
unsigned short  	checksum(void *b, int len);
char*            	dns(char *host, struct sockaddr_in *addr);
char*            	reverse_dns_lookup(char *ip);
int             	setsocket(int ttl, int to);

//math.c
double          	dSqrt(double x);
int             	calcPrecision(double n);
#endif