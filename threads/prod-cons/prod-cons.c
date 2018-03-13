#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define COUNT_TO_PRODUCE (1000000)
#define PROD_CONS_CHECK_VAL (-1)
#define MAX_BUFFER_LEN (20)

/* Semaphores to help control buffer content */
struct pc_buff {
  int buffer[MAX_BUFFER_LEN];
  sem_t items, empty;
};

void pthread_ret_error(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(-1);
}

/* Producer thread function */
void *producer_thread_func(void *p) {
  struct pc_buff *pcb = (struct pc_buff *)p;
  int i, pi = 0;

  for(i = 0; i < COUNT_TO_PRODUCE; i++) {
    /* Let the semaphore perform the spinlock for an empty space as necessary */
    sem_wait(&pcb->empty);

    /* Just put the number one in the buffer */
    pcb->buffer[pi] = 1;
    /* Increment the producer buffer index by one circularly */
    pi = (pi + 1) % MAX_BUFFER_LEN;

    /* Indicate that one more item has been added to the buffer */
    sem_post(&pcb->items);
  }
  /* Done producing. Indicate this is finished */
  sem_wait(&pcb->empty);
  /* The check value -1 should never occur during production */
  pcb->buffer[pi] = PROD_CONS_CHECK_VAL;
  /* Let the consumers know there's one last value to use */
  pi = (pi + 1) % MAX_BUFFER_LEN;
  sem_post(&pcb->items);

  return NULL;
}

/* Consumer thread function */
void *consumer_thread_func(void *p) {
  struct pc_buff *pcb = (struct pc_buff *)p;
  int val, ci = 0;
  int *result = malloc(sizeof(int));
  *result = 0;

  /* Loop forever and only stop when the producer indicates */
  for(;;) {
    /* Let the semaphore perform the spinlock for items as necessary */
    sem_wait(&pcb->items);

    val = pcb->buffer[ci];
    /* Check value found, stop looping */
    if(PROD_CONS_CHECK_VAL == val) break;
    /* Increment the consumer buffer index by one circularly */
    ci = (ci + 1) % MAX_BUFFER_LEN;

    /* Bump the empty semaphore to indicate there is one more free space to use */
    sem_post(&pcb->empty);

    *result += val;
  }
  /* Pretend that this thread never removed the item from the buffer */
  sem_post(&pcb->items);

  return result;
}

int main(int argc, char *argv[]) {
  pthread_t pth, cth;
  int sum, ret, i;
  void *result;
  struct pc_buff *pcb = malloc(sizeof(struct pc_buff));
  /* Keep track of the number of items in the buffer */
  sem_init(&pcb->items, 0, 0);
  /* Keep track of the number of available spaces in the buffer */
  sem_init(&pcb->empty, 0, MAX_BUFFER_LEN);

  ret = pthread_create(&pth, NULL, producer_thread_func, pcb);
  if(ret != 0) pthread_ret_error("Unable to create producer thread");

  ret = pthread_create(&cth, NULL, consumer_thread_func, pcb);
  if(ret != 0) pthread_ret_error("Unable to create consumer thread");

  ret = pthread_join(pth, NULL);
  if(ret != 0) pthread_ret_error("Unable to join producer thread");

  sum = 0;
  ret = pthread_join(cth, &result);
  if(ret != 0) pthread_ret_error("Unable to join consumer thread");
  
  sum += *(int *)result;

  printf("Sum result: %d\n", sum);

  return 0;
}
