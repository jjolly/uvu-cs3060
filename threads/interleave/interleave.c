#include <stdio.h>
#include <pthread.h>

struct char_print_params {
  char print_char;
  int number_to_print;
};

/* the thread function */
void *char_print(void *params) {
  struct char_print_params *p;
  p = (struct char_print_params *) params;

  int i;
  for (i = 0; i < p->number_to_print; i++)
    fputc(p->print_char, stderr);

  return NULL;
}

int main(int argc, char *argv[]) {
  /* the thread handle */
  pthread_t thread1_handle;
  pthread_t thread2_handle;

  struct char_print_params thread1_args;
  struct char_print_params thread2_args;

  thread1_args.print_char = 'x';
  thread1_args.number_to_print = 3000;
  pthread_create(&thread1_handle, NULL, char_print, &thread1_args);

  thread2_args.print_char = 'y';
  thread2_args.number_to_print = 5000;
  pthread_create(&thread2_handle, NULL, char_print, &thread2_args);

  return 0;
}
