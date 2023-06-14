#include "ft_ping.h"


int is_numflag(char c)
{
	return (c == 's' || c == 'W' || c == 't' || c == 'c');
}

char *parse_hosts(size_t argc, char **argv, struct sockaddr_in *addr)
{
    struct addrinfo hints;
    struct addrinfo *res;

    ft_bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET; //IpV4

    size_t i = argc - 1;
    while (i != 0)
    {
        if (argv[i][0] == '-') //it's an option
        {
            i--;
            continue;
        }
        if (getaddrinfo(argv[i], 0, &hints, &res))
        {
			print_errhost(argv[i]);
            exit(2);
        }
        freeaddrinfo(res);
        i--;
    }
	char b = 0;
	if (argv[i][0] == '-') //it's an option
		b = 1;
    else if (getaddrinfo(argv[i], 0, &hints, &res))
    {
		print_errhost(argv[i]);
        exit(2);
    }
	if (!b)
    	freeaddrinfo(res);
    i = argc - 1;
    while (i != 0)
    {
        if (argv[i][0] == '-')//it's an option
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
	if (argv[i][0] == '-')//it's an option
	{
        freeaddrinfo(res);
		return NULL;
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

int parse_numflags(char **argv, size_t i, size_t j, char flag)
{
	if (!argv[i][j] && !argv[i + 1])
	{
		print_erromissarg(flag);
		print_help();
		exit(2);
	}
	if (argv[i][j])//form -s64
	{
		for (int x = j; argv[i][x]; x++)
		{
			if (!ft_isdigit(argv[i][x]) && argv[i][x] != '+' && argv[i][x] != '-')
				print_errominvarg(argv[i] +j);
		}
		long long l = ft_atois(argv[i] + j);
		long long max = INT_MAX;
		if ((l <= 0 || l > 9223372036854775807) && flag == 'c')
			print_errominvargrangeC(argv[i]);
		else if (flag == 's' && ( l < 0 || l > max))
			print_errominvargrangeS(argv[i] + j);
		else if (flag == 'W' && (l < 0 || l > max/1000000))
			print_errominvargrangeW(argv[i] + j);
		else if (flag == 't' && (l < 0 || l > 255))
				print_errominvargrangeT(argv[i] + j);
		
		return l;
	}
	else//form -s 64
	{
		i++;
		for (int x = 0; argv[i][x]; x++)
		{
			if (!ft_isdigit(argv[i][x]) && argv[i][x] != '+' && argv[i][x] != '-')
				print_errominvarg(argv[i]);
		}
		long long l = ft_atois(argv[i]);
		long long max = INT_MAX;
		if ((l <= 0 || l > 9223372036854775807) && flag == 'c')
			print_errominvargrangeC(argv[i]);
		else if (flag == 's' && ( l < 0 || l > max))
			print_errominvargrangeS(argv[i]);
		else if (flag == 'W' && (l < 0 || l > max/1000000))
			print_errominvargrangeW(argv[i]);
		else if (flag == 't' && (l < 0 || l > 255))
				print_errominvargrangeT(argv[i]);
		return l;
	}

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
				else if (argv[i][j] == 'a')
					flags->aflag = 1;
				else if (argv[i][j] == 'q')
					flags->qflag = 1;
				else if (argv[i][j] == 'D')
					flags->Dflag = 1;
                else if (argv[i][j] == 'h')
                {
                
                    print_help();
                    return (1);
                }
				else if (argv[i][j] == 's' || argv[i][j] == 'W' || argv[i][j] == 't' || argv[i][j] == 'c')
				{

					long long ret = parse_numflags(argv, i , j + 1, argv[i][j]);
					if (argv[i][j] == 's')
					{
						flags->sflag = 1;
						flags->sflag_value = ret;
					}
					else if (argv[i][j] == 'W')
					{
						flags->Wflag = 1;
						flags->Wflag_value = ret;
					}
					else if (argv[i][j] == 't')
					{
						flags->tflag = 1;
						flags->tflag_value = ret;
					}
					else if (argv[i][j] == 'c')
					{
						flags->cflag = 1;
						flags->cflag_value = ret;
					}
					break; // we're done with this option
				}
                else 
                {
                    print_errop(argv[i][j]);
                    print_help();
                    return (1);
                }
            }
        }
        else if (!(i > 0 && argv[i - 1][0] == '-' && is_numflag(argv[i - 1][ft_strlen(argv[i - 1])-1])))// Basically if the previous argv is a numerical flag))
            one_valid = 1;
		i++;
    }
    if (!one_valid)
    {
        ft_putstr_fd("ping: usage error: Destination address required\n", 2, 0);
        return (1);
    }
    return (0);
}


