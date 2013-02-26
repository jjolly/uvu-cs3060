#include <stdio.h>
#include<stdlib.h>

int main (void)
{
   int c;
   while ( (c =getc(stdin)) != EOF)
      if (putc(c, stdout) == EOF)
         perror("Error writing output");

      if(ferror(stdin))
         perror("Error reading input");
   exit(0);
}
