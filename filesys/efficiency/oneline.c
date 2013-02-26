#include<stdio.h>
#include<stdlib.h>
#define MAXLINE 4096

int main (void)
{
   char buf[MAXLINE];

   while (fgets(buf, MAXLINE, stdin) != NULL)
      if (fputs(buf, stdout) == EOF)
         perror("Output Error");

   if (ferror(stdin))
      perror("Input Error");

   exit(0);
}
