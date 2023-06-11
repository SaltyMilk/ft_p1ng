#include "ft_ping.h"

int stop_send = 0;

void intHandle(int i)
{
    (void) i;
    stop_send = 42069;
    printf("\b\b  \b\b");

}

void setsignal()
{
    if (signal(SIGINT, intHandle) == SIG_ERR)
        exit(1);
    
}


void ft_ping(struct sockaddr_in addr, char *host,char *ip, char *domain, int isip, t_flags flags)
{
    (void) addr;
    t_ping_pckt pckt;
    struct sockaddr_in r_addr;
    size_t i = 0;
    double pckt_n = 0;
    double err_n = 0;
    int ttl = flags.tflag ? flags.tflag_value : 117;//update with -t flag
    int to = flags.Wflag ? flags.Wflag_value: 5 ;// timeout update with -W flag
    int socket = setsocket(ttl, to);
    struct timeval tv_start;
    struct timeval tv_end;
    struct timeval tv_sent;
    struct timeval tv_rcv;
    double rtt_tot = 0;
    double rtt;
    double rtt_min = 2147483647;
    double rtt_sum = 0;
    double rtt_sqsum = 0;
    double rtt_max = 0;
    double rtt_avg = 0;
    double rtt_mdev = 0;
    int pckt_size = flags.sflag ? flags.sflag_value : 56;//change 56 by -s in the future

    setsignal();
    gettimeofday(&tv_start, 0);
    printf("PING %s (%s) %d(%ld) bytes of data\n", host, ip, pckt_size, pckt_size + sizeof(pckt.hdr) + sizeof(struct ip));
    while (stop_send != 42069)
    {
        usleep(1000000);
        ft_bzero(&pckt, sizeof(pckt));
        if (!(pckt.content = malloc(pckt_size - sizeof(pckt.hdr)))) 
            exit(1);
        pckt.hdr.type = ICMP_ECHO;
        pckt.hdr.un.echo.id = getpid();
        i = 0;
        while (i < sizeof(pckt.content))
        {
            pckt.content[i] = '0' + i;
            i++;
        }
        pckt.content[i] = 0;
        pckt.hdr.un.echo.sequence = pckt_n++;
        pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));

        gettimeofday(&tv_sent, 0);
        if (sendto(socket, &pckt, sizeof(pckt), 0, (struct sockaddr*)&addr, sizeof(addr)) <= 0)
        {
            err_n++;
            free(pckt.content);
        }
        else
        {
            free(pckt.content);

            socklen_t len = sizeof(r_addr);
            if (recvfrom(socket, &pckt, sizeof(pckt), 0, (struct sockaddr*)&r_addr, &len) <= 0)
                err_n++;
            else
            {
                gettimeofday(&tv_rcv, 0);
                rtt = ((double)(tv_rcv.tv_sec - tv_sent.tv_sec)) *1000 + ((double)(tv_rcv.tv_usec - tv_sent.tv_usec))/1000;
                rtt_sum += rtt;
                rtt_sqsum += rtt * rtt;
                if (rtt > rtt_max)
                    rtt_max = rtt;
                if (rtt < rtt_min)
                    rtt_min = rtt;
				if (flags.aflag)
					printf("\a");
             	if (!flags.qflag)
				{
				   if (isip)
               	     printf("%ld bytes from %s: icmp_seq=%.0f ttl=%d time=%.3f ms\n", pckt_size + sizeof(pckt.hdr), ip ,pckt_n, ttl, rtt);
               		else
               		    printf("%ld bytes from %s (%s): icmp_seq=%.0f ttl=%d time=%.3f ms\n", pckt_size + sizeof(pckt.hdr), domain,ip ,pckt_n, ttl, rtt);
				}
			}
        }

    }
    gettimeofday(&tv_end, 0);
    free(ip);
    free(domain);
    double pckt_rcv = pckt_n - err_n;
    rtt_tot = ((double)(tv_end.tv_sec - tv_start.tv_sec)) *1000 + ((double)(tv_end.tv_usec - tv_start.tv_usec)) / 1000;
    rtt_avg = rtt_sum / (pckt_n - err_n);
    rtt_mdev = dSqrt((rtt_sqsum / pckt_rcv) - ((rtt_sum/pckt_rcv) *(rtt_sum/pckt_rcv)) );
    int precision = calcPrecision(((pckt_n - (pckt_n - err_n))/pckt_n) * 100);
    printf("^C\n--- %s ping statistics ---\n", host);//placeholder change by arg host
    printf("%.0f packets transmitted, %.0f received, %.*f%% packet loss, time %.0fms\n", pckt_n, pckt_n - err_n, precision, ((pckt_n - (pckt_n - err_n))/pckt_n) * 100, rtt_tot);
    printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", rtt_min, rtt_avg, rtt_max, rtt_mdev);
}




int main(int argc, char **argv)
{
    int cf_ret;
    t_flags flags;
    struct sockaddr_in addr;
    char *host;
    char *ip;
    char *domain;
    int isip;

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
        host = parse_hosts(argc - 1 , argv + 1, &addr);
        if (!addr.sin_addr.s_addr) //should never happen but let's be safe
            return 1;
        if (!(ip = dns(host, &addr)))
            exit(1);
        isip = is_ip(host);
        if (!isip)
        {
            if (!(domain = reverse_dns_lookup(ip)))
                exit(1);
        }
	//	printf("s=%d, W= %d\n", flags.sflag_value, flags.Wflag_value);
        ft_ping(addr, host, ip, domain, isip, flags); //Use last arg
    }
}