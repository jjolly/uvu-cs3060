#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>

pid_t r_wait (int* status_loc)
{
   int retval;

   while ( (retval = wait(status_loc) ) == -1 && (errno == EINTR) );
   return retval;
}

int main(int argc, char *argv[ ] )
{
   pid_t childpid = 0;
   int i, n;

   n = atoi ( argv[1] );
   for ( i = 1; i < n; i++)
        if ((childpid = fork( ))  <= 0 )
             break;

   while (r_wait(NULL) >  0); 

   fprintf(stderr, "i:%d process ID:%ld  parent ID: %ld child ID %ld\n",
        i, (long)getpid( ), (long)getppid( ), (long)childpid );

   return 0;
}
