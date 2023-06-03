#include "ft_ping.h"

int setsocket()
{
    int ttl = 117; //update with -t flag
    struct timeval tv_to; 
    tv_to.tv_sec = 5;// timeout update with -W flag
    tv_to.tv_usec = 0;
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0)
        exit(1);
    //set TTL
    if (setsockopt(sock, SOL_IP, IP_TTL, &ttl, sizeof(ttl)))
        exit(1);
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,&tv_to, sizeof(tv_to)))
        exit(1);
    return (sock);
}

void ft_ping(struct sockaddr_in addr)
{
    (void) addr;
    setsocket();
    setsignal();
    while (1)
        ;
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
        ft_ping(addr); //Use last arg
    }
}