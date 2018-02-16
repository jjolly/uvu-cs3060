#include<stdio.h>
#include<pthread.h>

/* Simple child thread function */
void *child_thread_func(void *p) {
  printf("Child thread created\n");
  return NULL;
}

int main(int argc, char *argv[]) {
  /* Thread handle */
  pthread_t thread_handle;
  int ret;

  /* Create child thread */
  ret = pthread_create(&thread_handle, NULL, child_thread_func, NULL);

  if(ret != 0) {
    fprintf(stderr, "Error creating child thread\n");
    return -1;
  }

  ret = pthread_join(thread_handle, NULL);

  if(ret != 0) {
    fprintf(stderr, "Error joining child thread\n");
    return -1;
  }

  return 0;
}
