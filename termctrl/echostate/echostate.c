#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
int main ( )
{   
   struct termios info;
   int rv;
   
   if ( (rv = tcgetattr( 0, &info ) ) == -1 )
   {
        perror("tcgetattr error");
        exit ( 1 );
   }
   
   if ( info.c_lflag & ECHO)
      printf(" echo flag is set\n");
   else
      printf(" echo flag is not set\n");

   return 0;
}
