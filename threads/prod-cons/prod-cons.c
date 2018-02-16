#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define COUNT_TO_PRODUCE (1000000)
#define PROD_CONS_CHECK_VAL (-1)
#define MAX_BUFFER_LEN (20)

/* Lockless producer-consumer is possible, but only for */
/* single producer/single consumer */
struct pc_buff {
  int pi, ci;
  int buffer[MAX_BUFFER_LEN];
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
    /* spinlock while buffer is full */
    while((pcb->pi + 1) % MAX_BUFFER_LEN == pcb->ci);

    /* Just put the number one in the buffer */
    pcb->buffer[pcb->pi] = 1;
    /* Increment the producer buffer index by one circularly */
    pcb->pi = (pcb->pi + 1) % MAX_BUFFER_LEN;
  }
  /* Done producing. Indicate this is finished */
  while((pcb->pi + 1) % MAX_BUFFER_LEN == pcb->ci);
  /* The check value -1 should never occur during production */
  pcb->buffer[pcb->pi] = PROD_CONS_CHECK_VAL;
  /* Let the consumers know there's one last value to use */
  pcb->pi = (pcb->pi + 1) % MAX_BUFFER_LEN;


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
    /* spinlock while buffer is empty */
    while(pcb->pi == pcb->ci);
    val = pcb->buffer[pcb->ci];
    /* Check value found, stop looping */
    if(PROD_CONS_CHECK_VAL == val) break;
    /* Increment the consumer buffer index by one circularly */
    pcb->ci = (pcb->ci + 1) % MAX_BUFFER_LEN;

    *result += val;
  }

  return result;
}

int main(int argc, char *argv[]) {
  pthread_t pth, cth;
  int *sum, ret;
  void *result;
  struct pc_buff *pcb = malloc(sizeof(struct pc_buff));
  pcb->pi = 0;
  pcb->ci = 0;

  ret = pthread_create(&pth, NULL, producer_thread_func, pcb);
  if(ret != 0) pthread_ret_error("Unable to create producer thread");

  ret = pthread_create(&cth, NULL, consumer_thread_func, pcb);
  if(ret != 0) pthread_ret_error("Unable to create consumer thread");

  ret = pthread_join(pth, NULL);
  if(ret != 0) pthread_ret_error("Unable to join producer thread");

  ret = pthread_join(cth, &result);
  if(ret != 0) pthread_ret_error("Unable to join consumer thread");
  
  sum = (int *)result;

  printf("Sum result: %d\n", *sum);

  return 0;
}
