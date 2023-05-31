#include "ft_ping.h"


void ft_ping(char *dest)
{
    puts(dest);
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

void parse_hosts(size_t argc, char **argv, struct sockaddr_in *addr)
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
            ft_putstr_fd("ping: ", 2, 0);
            ft_putstr_fd(argv[i], 2, 0);
            ft_putstr_fd(": Name or service not known\n", 2, 0);
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
            return;
        }
        freeaddrinfo(res);

        i--;
    }
    if (!getaddrinfo(argv[i], 0, &hints, &res))
    {
        addr->sin_addr.s_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
        freeaddrinfo(res);
        return;
    }
    freeaddrinfo(res);
}

int main(int argc, char **argv)
{
    int cf_ret;
    t_flags flags;
    struct sockaddr_in addr;

    ft_bzero(&addr, sizeof(addr));
    if (argc == 1)
    {
        ft_putstr_fd("ping: usage error: Destination address required\n", 2, 0);
        return 1;
    }
    else
    {
        if ((cf_ret = check_flags(argv + 1, &flags)))
            return (cf_ret);
        parse_hosts(argc - 1 , argv + 1, &addr);
        if (!addr.sin_addr.s_addr) //should never happen but let's be safe
            return 1;
//        ft_ping(argv[argc - 1]); //Use last arg
    }
}