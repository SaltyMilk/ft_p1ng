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
//    t_ping_pckt pckt;
    struct sockaddr_in r_addr;
    int i = 0;
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
	long long c = 0;
    int pckt_size = flags.sflag ? flags.sflag_value : 56;//change 56 by -s in the future
	unsigned long long icmp_err = 0;

	char *pckt;
    setsignal();
    gettimeofday(&tv_start, 0);
    printf("PING %s (%s) %d(%ld) bytes of data\n", host, ip, pckt_size, pckt_size + sizeof(struct icmphdr) + sizeof(struct ip));
    while (stop_send != 42069 && ((flags.cflag ? c :flags.cflag_value - 1) < flags.cflag_value ))
    {
        usleep(1000000);
        if (!(pckt = ft_calloc(sizeof(struct ip) + sizeof(struct icmphdr)+ pckt_size, 1))) 
            exit(1);
        ((struct icmphdr *)pckt)->type = ICMP_ECHO;
        ((struct icmphdr *)pckt)->un.echo.id = getpid();
        i = 0;
        while (i < pckt_size - 1)
        {
            *(pckt + sizeof(struct icmphdr)+ i) = '0' + i;
            i++;
        }
        *(pckt + sizeof(struct icmphdr) + i) = 0;
        ((struct icmphdr *)pckt)->un.echo.sequence = pckt_n++;
        ((struct icmphdr *)pckt)->checksum = checksum(pckt, sizeof(struct icmphdr) + pckt_size);

        gettimeofday(&tv_sent, 0);
        if (sendto(socket, pckt, sizeof(struct icmphdr)+ pckt_size, 0, (struct sockaddr*)&addr, sizeof(addr)) <= 0)
        {
            err_n++;
            free(pckt);
        }
        else
        {
            

            socklen_t len = sizeof(r_addr);

            if (recvfrom(socket, pckt, sizeof(struct ip) + sizeof(struct icmphdr) + pckt_size, 0, (struct sockaddr*)&r_addr, &len) <= 0)
            {
				if (flags.vflag)
				{
					if (isip)
               		    printf("From %s: icmp_seq=%.0f Received no answer.\n", ip ,pckt_n);
               		else
               			printf("From %s (%s): icmp_seq=%.0f Received no answer.\n", domain,ip ,pckt_n);
				}
				err_n++;
			}
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
				int type = ((struct icmphdr *)(pckt + sizeof(struct ip)))->type;
				if(type == 11)
				{
					
					int code = ((struct icmphdr *)(pckt + sizeof(struct ip)))->code;
					if (type == 11 && code == 0)
					{
						struct in_addr ttladdr;
						ttladdr.s_addr = ((struct ip*)pckt)->ip_src.s_addr;
						char *from_ip = inet_ntoa(ttladdr);//for ttl bonus -t
						char *from = reverse_dns_lookup(from_ip);
						if (flags.vflag)
						{
							if (!from)
								printf("From %s (%s): icmp_seq=%0.f Time to Live exceeded\n",from_ip,from_ip, pckt_n);
							else	
								printf("From %s (%s): icmp_seq=%0.f Time to Live exceeded\n",from,from_ip, pckt_n);
						}
						icmp_err++;
						err_n++;
						free(from);
					}
				}
             	else if (!flags.qflag)
				{
					if (flags.Dflag)
					{
						struct timeval tv;
						gettimeofday(&tv, NULL);
						printf("[%lu.%06lu] ",(unsigned long)tv.tv_sec, (unsigned long)tv.tv_usec);
					}
					if (isip)
               	    	printf("%ld bytes from %s: icmp_seq=%.0f ttl=%d time=%.3f ms\n", pckt_size + sizeof(struct icmphdr), ip ,pckt_n, ttl, rtt);
               		else
               		    printf("%ld bytes from %s (%s): icmp_seq=%.0f ttl=%d time=%.3f ms\n", pckt_size + sizeof(struct icmphdr), domain,ip ,pckt_n, ttl, rtt);
				}
			}
			free(pckt);
        }
		c++;
    }
    gettimeofday(&tv_end, 0);
    free(ip);
    free(domain);
    double pckt_rcv = pckt_n - err_n;
    rtt_tot = ((double)(tv_end.tv_sec - tv_start.tv_sec)) *1000 + ((double)(tv_end.tv_usec - tv_start.tv_usec)) / 1000;
    rtt_avg = rtt_sum / (pckt_n - err_n);
    rtt_mdev = dSqrt((rtt_sqsum / pckt_rcv) - ((rtt_sum/pckt_rcv) *(rtt_sum/pckt_rcv)) );
    int precision = calcPrecision(((pckt_n - (pckt_n - err_n))/pckt_n) * 100);
	if (!flags.cflag)
		printf("^C");
    printf("\n--- %s ping statistics ---\n", host);
	if (!icmp_err)
    	printf("%.0f packets transmitted, %.0f received, %.*f%% packet loss, time %.0fms\n", pckt_n, pckt_n - err_n, precision, ((pckt_n - (pckt_n - err_n))/pckt_n) * 100, rtt_tot);
	else
		printf("%.0f packets transmitted, %.0f received, +%lld errors, %.*f%% packet loss, time %.0fms\n", pckt_n, pckt_n - err_n,  icmp_err, precision,((pckt_n - (pckt_n - err_n))/pckt_n) * 100, rtt_tot);
	if (pckt_n - err_n > 0)
		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", rtt_min, rtt_avg, rtt_max, rtt_mdev);
	puts("");
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
	ft_bzero(&flags, sizeof(t_flags));
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
        ft_ping(addr, host, ip, domain, isip, flags); //Use last arg
    }
}