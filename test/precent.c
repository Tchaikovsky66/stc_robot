#include "stdio.h"
#include <unistd.h>

void precent(int a)
{
    int all = 100;
    int i;

    if (a > 100)
        a = 100;
    i = a * 100 / all;
    printf("precent:");
    printf("[");
    for (int j = 0; j < i; j = j)
    {
        printf("*");
    }
    for (int j = 0; j < 100 - i; j = j)
    {
        printf(" ");
    }
    printf("]%d%%", i);
    printf("\n");
}

int main()
{
    // int a;
    // scanf("%d", &a);
    // precent(a);

    for (int j = 0; j < 100; j++)
    {
        precent(j);
        sleep(1);
    }
}