#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define CHANGE_AMOUNT (1000000)

int lock_var = 0;

/* Mutual Exclusion Lock using test-and-set */
void test_and_set_lock(int *lock) {
  /* Perform XCGH machine instruction on Intel CPUs */
  while(__sync_lock_test_and_set(lock, 1) == 1);
}

/* Mutual Exclusion Unlock for test-and-set */
void test_and_set_unlock(int *lock) {
  *lock = 0;
}

/* Increment thread function */
void *inc_thread_func(void *p) {
  int *num = (int *)p;
  int i, tid = 0;

  for(i = 0; i < CHANGE_AMOUNT; i++) {
    test_and_set_lock(&lock_var);
    (*num)++;
    test_and_set_unlock(&lock_var);
  }

  return NULL;
}

/* Decrement thread function */
void *dec_thread_func(void *p) {
  int *num = (int *)p;
  int i, tid = 1;

  for(i = 0; i < CHANGE_AMOUNT; i++) {
    test_and_set_lock(&lock_var);
    (*num)--;
    test_and_set_unlock(&lock_var);
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
