#include <pthread.h>
#include <stdio.h>

void* printxs (void* unused)  // the thread function
{
   while(1)
      fputc('x', stderr);
   return NULL;
}
int main ( )
{
   pthread_t x_thread;  // the thread handle
   pthread_create (&x_thread, NULL, &printxs, NULL);

   while(1)
      fputc('o', stderr);
   return 0;
}

