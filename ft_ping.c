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
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv_to, sizeof(tv_to)))
        exit(1);

    return (sock);
}

void ft_ping(struct sockaddr_in addr)
{
    (void) addr;
    t_ping_pckt pckt;
    size_t i = 0;
    size_t msg_n = 0;
    int socket = setsocket();

    setsignal();
    if (!(pckt.content = malloc(64 - sizeof(pckt.hdr)))) //change 64 by -s in the future
        exit(1);
    while (42069)
    {
        pckt.hdr.type = ICMP_ECHO;
        pckt.hdr.un.echo.id = getpid();
        while (i < sizeof(pckt.content))
            pckt.content[i] = '0' + i;
        pckt.content[i] = 0;
        pckt.hdr.un.echo.sequence = msg_n++;
        pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));
    }
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