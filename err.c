#include "ft_ping.h"

void print_help()
{
    ft_putstr_fd("\nUsage\n  ping [options] <destination>\n\nOptions:\n  <destination>\t\tdns name or ip address\n  -h\t\t\tprint help and exit\n  -v\t\t\tverbose output\n\nFor more details see ping(8).\n", 2, 0);

}

void print_errop(char c)
{
    ft_putstr_fd("ping: invalid option -- '", 2, 0);
    ft_putchar_fd(c, 2);
    ft_putstr_fd("'\n", 2, 0);
}

void print_errhost(char *host)
{
    ft_putstr_fd("ping: ", 2, 0);
    ft_putstr_fd(host, 2, 0);
    ft_putstr_fd(": Name or service not known\n", 2, 0);
}