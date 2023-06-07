#include "ft_ping.h"

char *parse_hosts(size_t argc, char **argv, struct sockaddr_in *addr)
{
    struct addrinfo hints;
    struct addrinfo *res;

    ft_bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET; //IpV4

    size_t i = 0;
    while (i < argc)
    {
        if (argv[i][0] == '-') //it's an option
        {
            i++;
            continue;
        }
        if (getaddrinfo(argv[i], 0, &hints, &res))
        {
            exit(2);
        }
        freeaddrinfo(res);
        i++;
    }
    i = argc - 1;
    while (i != 0)
    {
        if (argv[i][0] == '-') //it's an option
        {
            i--;
            continue;
        }
        if (!getaddrinfo(argv[i], 0, &hints, &res))
        {
            addr->sin_addr.s_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
            freeaddrinfo(res);
            return argv[i];
        }
        freeaddrinfo(res);

        i--;
    }
    if (!getaddrinfo(argv[i], 0, &hints, &res))
    {
        addr->sin_addr.s_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
        freeaddrinfo(res);
        return argv[i];
    }
    freeaddrinfo(res);
    return NULL;
}

int is_ip(char *s)
{
    size_t i = 0;

    while (s[i])
    {
        if (s[i] != '.' && !ft_isdigit(s[i]))
            return 0;
        i++;
    }
    return 1;
}

int check_flags(char **argv, t_flags *flags)
{
    size_t i = 0;
    char one_valid = 0;

    while (argv[i])
    {
        if (argv[i][0] == '-')
        {
            for (size_t j = 1; argv[i][j]; j++)
            {
                if (argv[i][j] == 'v')
                    flags->vflag = 1;
                else if (argv[i][j] == 'h')
                {
                
                    print_help();
                    return (1);
                }
                else 
                {
                    print_errop(argv[i][j]);
                    print_help();
                    return (1);
                }
            }
        }
        else
            one_valid = 1;
        argv++;
    }
    if (!one_valid)
    {
        ft_putstr_fd("ping: usage error: Destination address required\n", 2, 0);
        return (1);
    }
    return (0);
}