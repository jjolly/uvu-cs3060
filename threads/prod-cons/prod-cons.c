#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define COUNT_TO_PRODUCE (1000000)
#define PROD_CONS_CHECK_VAL (-1)
#define MAX_BUFFER_LEN (20)

struct pc_buff {
  int count;
  int buffer[MAX_BUFFER_LEN];
};

void pthread_ret_error(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(-1);
}

/* Producer thread function */
void *producer_thread_func(void *p) {
  struct pc_buff *pcb = (struct pc_buff *)p;
  int i, pi;

  for(i = 0; i < COUNT_TO_PRODUCE; i++) {
    /* spinlock while buffer is full */
    while(COUNT_TO_PRODUCE == pcb->count);

    /* Just put the number one in the buffer */
    pcb->buffer[pi] = 1;
    /* Increment the buffer count after adding the item */
    pcb->count++;
    /* Increment the producer buffer index by one circularly */
    pi = (pi + 1) % MAX_BUFFER_LEN;
  }
  /* Done producing. Indicate this is finished */
  while(COUNT_TO_PRODUCE == pcb->count);
  /* The check value -1 should never occur during production */
  pcb->buffer[pi] = PROD_CONS_CHECK_VAL;
  /* Let the consumers know there's one last value to use */
  pcb->count++;


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
    /* spinlock while buffer is empty */
    while(0 == pcb->count);
    val = pcb->buffer[ci];
    /* Check value found, stop looping */
    if(PROD_CONS_CHECK_VAL == val) break;
    /* Decrement count on removed value */
    pcb->count--;
    /* Increment the consumer buffer index by one circularly */
    ci = (ci + 1) % MAX_BUFFER_LEN;

    *result += val;
  }

  return result;
}

int main(int argc, char *argv[]) {
  pthread_t pth, cth;
  int *sum, ret;
  void *result;
  struct pc_buff *pcb = malloc(sizeof(struct pc_buff));
  pcb->count = 0;

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
