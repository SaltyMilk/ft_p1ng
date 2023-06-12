#include "ft_ping.h"

void print_help()
{
    ft_putstr_fd("\nUsage\n  ping [options] <destination>\n\nOptions:\n  <destination>\t\tdns name or ip address\n  -h\t\t\tprint help and exit\n  -v\t\t\tverbose output\n  -q\t\t\tquiet output\n  -s <size>\t\tuse <size> as number of data bytes to be sent\n  -W <timeout>\t\ttime to wait for response\n  -t <ttl>\t\tdefine time to live\n  -a\t\t\tuse audible ping\n  -c <count>\t\tstop after <count> replies\n\nFor more details see ping(8).\n", 2, 0);

}

void print_errop(char c)
{
    ft_putstr_fd("ping: invalid option -- '", 2, 0);
    ft_putchar_fd(c, 2);
    ft_putstr_fd("'\n", 2, 0);
}

void print_erromissarg(char c)
{
    ft_putstr_fd("ping: option requires and argument -- '", 2, 0);
    ft_putchar_fd(c, 2);
    ft_putstr_fd("'\n", 2, 0);
}

void print_errominvarg(char *s)
{
    ft_putstr_fd("ping: invalid argument: '", 2, 0);
    ft_putstr_fd(s, 2,0);
    ft_putstr_fd("'\n", 2, 0);
	exit(1);
}

void print_errominvargrangeS(char *s)
{
    ft_putstr_fd("ping: invalid argument: '", 2, 0);
    ft_putstr_fd(s, 2,0);
    ft_putstr_fd("': out of range: 0 <= value <= 2147483647\n", 2, 0);
	exit(1);
}

void print_errominvargrangeC(char *s)
{
    ft_putstr_fd("ping: invalid argument: '", 2, 0);
    ft_putstr_fd(s, 2,0);
    ft_putstr_fd("': out of range: 1 <= value <= 9223372036854775807\n", 2, 0);
	exit(1);
}

void print_errominvargrangeT(char *s)
{
    ft_putstr_fd("ping: invalid argument: '", 2, 0);
    ft_putstr_fd(s, 2,0);
    ft_putstr_fd("': out of range: 0 <= value <= 255\n", 2, 0);
	exit(1);
}
void print_errominvargrangeW(char *s)
{
    ft_putstr_fd("ping: bad linger time: '", 2, 0);
    ft_putstr_fd(s, 2,0);
    ft_putstr_fd("'\n", 2, 0);
	exit(130);
}
void print_errhost(char *host)
{
    ft_putstr_fd("ping: ", 2, 0);
    ft_putstr_fd(host, 2, 0);
    ft_putstr_fd(": Name or service not known\n", 2, 0);
}