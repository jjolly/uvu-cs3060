#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define CHANGE_AMOUNT (1000000)

/* Identify thread need of critical section */
int need[2] = {0,0};
/* Identify thread allowed to enter critical section */
int turn;

/* Peterson's Solution Lock */
void peterson_lock(int tid) {
  /* Define thread ID of the other thread */
  int otid = 1 - tid;
  /* Declare need for the critical section for this thread */
  need[tid] = 1;
  /* Grant use of critical section to the other thread */
  turn = otid;
  /* Needed to provide a memory cache "fence" */
  __sync_synchronize();
  /* Spinlock as long as the other thread needs and is using
     the critical section */
  while(1 == need[otid] && turn == otid);
}

/* Peterson's Solution Unlock */
void peterson_unlock(int tid) {
  /* Release the need for the critical section for this thread */
  need[tid] = 0;
}

/* Increment thread function */
void *inc_thread_func(void *p) {
  int *num = (int *)p;
  int i, tid = 0;

  for(i = 0; i < CHANGE_AMOUNT; i++) {
    peterson_lock(tid);
    (*num)++;
    peterson_unlock(tid);
  }

  return NULL;
}

/* Decrement thread function */
void *dec_thread_func(void *p) {
  int *num = (int *)p;
  int i, tid = 1;

  for(i = 0; i < CHANGE_AMOUNT; i++) {
    peterson_lock(tid);
    (*num)--;
    peterson_unlock(tid);
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
