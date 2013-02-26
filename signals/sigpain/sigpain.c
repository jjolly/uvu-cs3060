#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void f(int);

int main ( )
{
    int i;
    signal ( SIGINT, f);
    for (i = 0; i < 10; i++)
    {
        printf("hello\n");
        sleep(1);
    }
    return 0;
}

void f ( int signum )
{
    printf("\nOUCH");
}
