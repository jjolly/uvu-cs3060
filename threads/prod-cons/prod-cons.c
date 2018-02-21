#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define COUNT_TO_PRODUCE (1000000)
#define PROD_CONS_CHECK_VAL (-1)
#define MAX_BUFFER_LEN (20)
#define COUNT_OF_CONSUMERS (2)

/* Lockless producer-consumer is possible, but only for */
/* single producer/single consumer */
struct pc_buff {
  int pi, ci;
  int buffer[MAX_BUFFER_LEN];
  pthread_mutex_t mutex;
  sem_t items, empty;
};

void pthread_ret_error(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(-1);
}

/* Producer thread function */
void *producer_thread_func(void *p) {
  struct pc_buff *pcb = (struct pc_buff *)p;
  int i;

  for(i = 0; i < COUNT_TO_PRODUCE; i++) {
    /* Let the semaphore perform the spinlock for an empty space as necessary */
    sem_wait(&pcb->empty);

    /* Just put the number one in the buffer */
    pcb->buffer[pcb->pi] = 1;
    /* Increment the producer buffer index by one circularly */
    pcb->pi = (pcb->pi + 1) % MAX_BUFFER_LEN;

    /* Indicate that one more item has been added to the buffer */
    sem_post(&pcb->items);
  }
  /* Done producing. Indicate this is finished */
  sem_wait(&pcb->empty);
  /* The check value -1 should never occur during production */
  pcb->buffer[pcb->pi] = PROD_CONS_CHECK_VAL;
  /* Let the consumers know there's one last value to use */
  pcb->pi = (pcb->pi + 1) % MAX_BUFFER_LEN;
  sem_post(&pcb->items);

  return NULL;
}

/* Consumer thread function */
void *consumer_thread_func(void *p) {
  struct pc_buff *pcb = (struct pc_buff *)p;
  int val;
  int *result = malloc(sizeof(int));
  *result = 0;

  /* Loop forever and only stop when the producer indicates */
  for(;;) {
    /* Let the semaphore perform the spinlock for items as necessary */
    sem_wait(&pcb->items);

    /* Use the mutex to protect the consumer index from a race condition */
    pthread_mutex_lock(&pcb->mutex);
    val = pcb->buffer[pcb->ci];
    /* Check value found, stop looping */
    if(PROD_CONS_CHECK_VAL == val) break;
    /* Increment the consumer buffer index by one circularly */
    pcb->ci = (pcb->ci + 1) % MAX_BUFFER_LEN;
    pthread_mutex_unlock(&pcb->mutex);

    /* Bump the empty semaphore to indicate there is one more free space to use */
    sem_post(&pcb->empty);

    *result += val;
  }
  pthread_mutex_unlock(&pcb->mutex);
  /* Pretend that this thread never removed the item from the buffer */
  sem_post(&pcb->items);

  return result;
}

int main(int argc, char *argv[]) {
  pthread_t pth, cth[COUNT_OF_CONSUMERS];
  int sum, ret, i;
  void *result;
  struct pc_buff *pcb = malloc(sizeof(struct pc_buff));
  pcb->pi = 0;
  pcb->ci = 0;
  pthread_mutex_init(&pcb->mutex, NULL);
  /* Keep track of the number of items in the buffer */
  sem_init(&pcb->items, 0, 0);
  /* Keep track of the number of available spaces in the buffer */
  sem_init(&pcb->empty, 0, MAX_BUFFER_LEN);

  ret = pthread_create(&pth, NULL, producer_thread_func, pcb);
  if(ret != 0) pthread_ret_error("Unable to create producer thread");

  for(i = 0; i < COUNT_OF_CONSUMERS; i++) {
    ret = pthread_create(&cth[i], NULL, consumer_thread_func, pcb);
    if(ret != 0) pthread_ret_error("Unable to create consumer thread");
  }

  ret = pthread_join(pth, NULL);
  if(ret != 0) pthread_ret_error("Unable to join producer thread");

  sum = 0;
  for(i = 0; i < COUNT_OF_CONSUMERS; i++) {
    ret = pthread_join(cth[i], &result);
    if(ret != 0) pthread_ret_error("Unable to join consumer thread");
  
    sum += *(int *)result;
  }

  printf("Sum result: %d\n", sum);

  return 0;
}
