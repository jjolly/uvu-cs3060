#include <stdio.h>
#include <pthread.h>

/* the thread function */
void *printxs(void *unused) {
  while(1)
    fputc('x', stderr);

  return NULL;
}

int main(int argc, char *argv[]) {
  /* the thread handle */
  pthread_t x_thread;

  pthread_create(&x_thread, NULL, &printxs, NULL);

  while(1)
    fputc('o', stderr);

  return 0;
}
