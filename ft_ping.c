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

int main(int argc, char **argv)
{
    int cf_ret;
    t_flags flags;

    if (argc == 1)
    {
        ft_putstr_fd("ping: usage error: Destination address required\n", 2, 0);
        return 1;
    }
    else
    {
        if ((cf_ret = check_flags(argv + 1, &flags)))
            return (cf_ret);
        //ft_ping(argv[argc - 1]); //Use last arg
    }
}