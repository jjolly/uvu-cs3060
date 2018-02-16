#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define CHANGE_AMOUNT (1000000)

pthread_mutex_t lock_var = PTHREAD_MUTEX_INITIALIZER;

/* Mutual Exclusion Lock using pthread mutex */
void mutex_lock(pthread_mutex_t *lock) {
  int ret = pthread_mutex_lock(lock);
  if(ret != 0) {
    fprintf(stderr, "Failed to lock mutex\n");
    exit(-1);
  }
}

/* Mutual Exclusion Unlock for pthread mutex */
void mutex_unlock(pthread_mutex_t *lock) {
  int ret = pthread_mutex_unlock(lock);
  if(ret != 0) {
    fprintf(stderr, "Failed to unlock mutex\n");
    exit(-1);
  }
}

/* Increment thread function */
void *inc_thread_func(void *p) {
  int *num = (int *)p;
  int i, tid = 0;

  for(i = 0; i < CHANGE_AMOUNT; i++) {
    mutex_lock(&lock_var);
    (*num)++;
    mutex_unlock(&lock_var);
  }

  return NULL;
}

/* Decrement thread function */
void *dec_thread_func(void *p) {
  int *num = (int *)p;
  int i, tid = 1;

  for(i = 0; i < CHANGE_AMOUNT; i++) {
    mutex_lock(&lock_var);
    (*num)--;
    mutex_unlock(&lock_var);
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
