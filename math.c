#include "ft_ping.h"

double dSqrt(double x)
{
    double z = 1.0;

    for (int i = 1; i <= 10; i++)
        z -= (z*z - x) / (2 * z);
    return (z);

}

int calcPrecision(double n)
{
    int ret = 0;
    if (n == 0.0)
        return 0;
    n -= (int)n;
    while (n != 0.0 && ret < 4)
    {
        ret++;
        n *= 10;
        n -= (int)n;
    }
    return ret;
}