#include <stdio.h>
#include <termios.h>
#include <stdlib.h>

int main ( int argc, char *argv[] )
{
    struct termios info;
    int rv;

    if ( (rv = tcgetattr( 0, &info ) ) == -1 )
    {
        perror("tcgetattr error");
        exit ( 1 );
    }
    if ( argc > 1 && argv[1][0] == 'y' )
        info.c_lflag |= ECHO;
    else
        info.c_lflag &= ~ECHO;
    if ( tcsetattr ( 0, TCSANOW, &info ) == -1)
    {
        perror("tcsetattr error");
        exit ( 1 );
    }
}
