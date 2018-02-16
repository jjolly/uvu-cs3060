#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define CHANGE_AMOUNT (1000000)

/* Increment thread function */
void *inc_thread_func(void *p) {
  int *num = (int *)p;
  int i;

  for(i = 0; i < CHANGE_AMOUNT; i++) {
    (*num)++;
  }

  return NULL;
}

/* Decrement thread function */
void *dec_thread_func(void *p) {
  int *num = (int *)p;
  int i;

  for(i = 0; i < CHANGE_AMOUNT; i++) {
    (*num)--;
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t ith, dth;
  int ret, num = 0;

  ret = pthread_create(&ith, NULL, inc_thread_func, &num);
  if(ret != 0) {
    fprintf(stderr, "Unable to create increment thread\n");
    return -1;
  }

  ret = pthread_create(&dth, NULL, dec_thread_func, &num);
  if(ret != 0) {
    fprintf(stderr, "Unable to create decrement thread\n");
    return -1;
  }

  ret = pthread_join(ith, NULL);
  if(ret != 0) {
    fprintf(stderr, "Unable to join increment thread\n");
    return -1;
  }

  ret = pthread_join(dth, NULL);
  if(ret != 0) {
    fprintf(stderr, "Unable to join decrement thread\n");
    return -1;
  }

  printf("Final result: %d\n", num);

  return 0;
}
