#include "ft_ping.h"


void intHandle(int i)
{
    (void) i;
    ft_printf("Hey I got interrupted !\n");
    exit(0);
}

void setsignal()
{
    if (signal(SIGINT, intHandle) == SIG_ERR)
        exit(1);
    
}
