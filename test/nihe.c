#include "stdio.h"

int main()
{
    float v;
    int n = 0;
    for (n = 0; n <= 20; n++)
    {
        v = 0.5 * n * n + 0.5 * n + 100;
        printf("n = %d, v = %f\n", n, v);
    }
    for (n = 20; n < 121; n++)
    {
        v = 20 * n - 90;
        printf("n = %d, v = %f\n", n, v);
    }
    for (n = 121; n < 141; n++)
    {
        v = 193.5 * n - 0.5 * n * n - 7230;
        printf("n = %d, v = %f\n", n, v);
    }

    return 0;
}