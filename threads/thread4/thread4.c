#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define THREAD_COUNT (20)

/* A structure is usually the best way to pass data to and from a
   threaded function. In this case, we will use this structure to pass
   the two multiplicands to the threaded function and return the
   result of the multiplication
 */
struct multiply_data {
  int input1;
  int input2;
  int result;
};

/* The threaded function. It simply multiplies two numbers together
   and returns the result. This function does not allocate any memory.
   Instead it expects data in a structure from the thread creator. This
   structure can be placed either on the heap or the stack, it doesn't
   matter
 */
void *multiply_thread(void *ptr) {
  struct multiply_data *data = (struct multiply_data*)ptr;
  int answer = data->input1 * data->input2;
  data->result = answer;
  return data;
}

/* The main function. This function will create all the threads and
   then wait for all the threads to finish, reporting each thread
   result.
 */
int main() {
  /* For this implementation, the main function allocates one structure
     at a time and passes it to each thread. When each thread is
     joined, the expected return value is an allocated structure of
     this type. The structure is freed after the values are retrieved
     from it.
   */
  struct multiply_data *data;
  int i;
  pthread_t threads[THREAD_COUNT];

  for ( i = 0; i < THREAD_COUNT; i++ ) {
    /* Allocate memory for the structure */
    data = malloc(sizeof(struct multiply_data));
    if ( data == NULL ) {
      fprintf(stderr, "Alloc error creating thread #%d\n", i);
      perror("Allocation Error");
      return 1;
    }

    /* Initialize the input values for the thread */
    data->input1 = i;
    data->input2 = i + 3;

    /* Create the thread, passing the allocated and initialized
       structure
     */
    if ( pthread_create(&(threads[i]), NULL, multiply_thread, data) != 0 ) {
      fprintf(stderr, "Unable to create thread instance #%d\n", i);
      return 1;
    }
  }

  for ( i = 0; i < THREAD_COUNT; i++ ) {
    /* Join each thread, expecting a return value of a pointer to the
       allocated structure. In order for my pointer to be set properly,
       I must not use pass-by-value, but rather pass-by-reference. This
       allows the pthread_join method to set my pointer variable. Also,
       I must explicitly case my structure pointer reference to a
       void **.
     */
    if ( pthread_join(threads[i], (void **)&data) != 0 ) {
      fprintf(stderr, "Unable to join thread instance #%d\n", i);
      return 1;
    }
    fprintf(stdout, "%d * %d = %d\n", data->input1, data->input2, data->result);
    /* I allocated the memory, common sense tells me I should also free
       the memory
     */
    free(data);
  }

  return 0;
}
