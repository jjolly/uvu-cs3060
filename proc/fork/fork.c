#include <stdio.h>
#include <unistd.h>

int main ( )
{
   int x,ret;

   x = 0;
   ret = fork( );
   x = 1;
   printf("I am process %ld and my x value is %d and fork returned %d\n",
             (long)getpid( ), x, ret);

   return 0;
}

