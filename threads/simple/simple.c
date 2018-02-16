#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

/* Simple child thread function */
void *child_thread_func(void *p) {
  /* Cast void pointer to an integer pointer */
  int *number = (int *)p;

  /* Allocate integer pointer on the heap */
  int *result = malloc(sizeof(int));

  /* Square number */
  *result = *number * *number;

  printf("Child thread created\n");

  /* Send result (in heap) back to the parent */
  return result;
}

int main(int argc, char *argv[]) {
  /* Thread handle */
  pthread_t thread_handle;
  void *result;
  int ret;
  int number = 10;

  /* Create child thread */
  /* Send pointer to local variable */
  ret = pthread_create(&thread_handle, NULL, child_thread_func, &number);

  if(ret != 0) {
    fprintf(stderr, "Error creating child thread\n");
    return -1;
  }

  /* Retrieve result by passing a reference to a void pointer */
  ret = pthread_join(thread_handle, &result);

  if(ret != 0) {
    fprintf(stderr, "Error joining child thread\n");
    return -1;
  }

  printf("Result of %d squared is %d\n", number, *(int *)result);

  /* Free allocated memory */
  free(result);

  return 0;
}
