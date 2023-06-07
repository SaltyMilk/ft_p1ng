#include "ft_ping.h"

unsigned short checksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned int sum=0;
    unsigned short result;
 
    for ( sum = 0; len > 1; len -= 2 )
        sum += *buf++;
    if ( len == 1 )
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
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

//find ip
char *dns(char *host, struct sockaddr_in *addr)
{
    struct hostent *host_entity;
    char *ip;
 
    if ((host_entity = gethostbyname(host)) == NULL)

        return NULL;
     
    if (!(ip = ft_calloc(64, 1)))
        return NULL;
    inet_ntop(AF_INET, &addr->sin_addr, ip, 64);
    ip[63] = 0;

    return ip;
}

//find domain name
char* reverse_dns_lookup(char *ip)
{
    struct sockaddr_in addr;
    char buf[NI_MAXHOST];

    ft_bzero(buf, sizeof(buf));
    
    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0)
        exit(1);
    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    int rc;
    if ( (rc = getnameinfo((struct sockaddr *) &addr, sizeof(addr), buf, sizeof(buf), NULL, 0, NI_NAMEREQD)))
        return NULL;
    return( ft_strdup(buf));

}
