#include "ft_ping.h"

int stop_send = 0;

void intHandle(int i)
{
    (void) i;
    stop_send = 42069;
}

void setsignal()
{
    if (signal(SIGINT, intHandle) == SIG_ERR)
        exit(1);
    
}


int setsocket(int ttl, int to)
{
    struct timeval tv_to; 

    tv_to.tv_sec = to;
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
    struct sockaddr_in r_addr;
    size_t i = 0;
    size_t pckt_n = 0;
    size_t err_n = 0;
    int ttl = 117;//update with -t flag
    int to = 5;// timeout update with -W flag
    int socket = setsocket(ttl, to);
    struct timeval tv_start;
    struct timeval tv_end;
    struct timeval tv_sent;
    struct timeval tv_rcv;
    double rtt_tot = 0;
    double rtt;
    double rtt_min = 2147483647;
    double rtt_max = 0;
    double rtt_avg = 0;
    double rtt_mdev = 0;
    int pckt_size = 64;//change 64 by -s in the future

    setsignal();
    if (!(pckt.content = malloc(pckt_size - sizeof(pckt.hdr)))) 
        exit(1);
    gettimeofday(&tv_start, 0);
    puts("getting in !");
    while (stop_send != 42069)
    {
        printf("stopsend=%d\n", stop_send);
        pckt.hdr.type = ICMP_ECHO;
        pckt.hdr.un.echo.id = getpid();
        while (i < sizeof(pckt.content))
            pckt.content[i] = '0' + i;
        pckt.content[i] = 0;
        pckt.hdr.un.echo.sequence = pckt_n++;
        pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));

        gettimeofday(&tv_sent, 0);
        if (sendto(socket, &pckt, sizeof(pckt), 0, (struct sockaddr*)&addr, sizeof(addr)) <= 0)
            err_n++;
        else //we were able to send !
        {
            socklen_t len = sizeof(r_addr);
            if (recvfrom(socket, &pckt, sizeof(pckt), 0, (struct sockaddr*)&r_addr, &len) <= 0)
                err_n++;
            else
            {
                gettimeofday(&tv_rcv, 0);
                rtt = ((double)(tv_rcv.tv_usec - tv_sent.tv_usec))/1000;
                if (rtt > rtt_max)
                    rtt_max = rtt;
                if (rtt < rtt_min)
                    rtt_min = rtt;
                printf("%d bytes from {} (h:{}) icmp_seq=%ld ttl=%d time=%f ms\n",pckt_size, pckt_n, ttl, rtt);
            }
        }

    }
    gettimeofday(&tv_end, 0);
    rtt_tot = ((float)(tv_end.tv_usec - tv_start.tv_usec)) / 1000;
    rtt_avg = rtt_tot / (pckt_n - err_n);
    printf("--- {} ping statistics ---\n");//placeholder change by arg host
    printf("%ld packets transmitted, %ld received, %ld%% packet loss, time %.0fms\n", pckt_n, pckt_n - err_n, ((pckt_n - (pckt_n - err_n))/pckt_n) * 100, rtt_tot);
    printf("rtt min/avg/max/mdev = %f/%f/%f/%f ms\n", rtt_min, rtt_avg, rtt_avg, rtt_mdev);
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