#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define DEBUG_OUTPUT

#define MAX_ITEMS (20)
#define TRUE (1)
#define FALSE (0)

struct primecheck_data {
	int buffer[MAX_ITEMS];
	int count;
	int done;
};

void *isPrimeConsumer(void *ptr) {
	int count = 0, i, number, isPrime, next_out = 0;
	struct primecheck_data *data = (struct primecheck_data *)ptr;

	while(data->done == FALSE || data->count > 0) {
		while(data->count == 0) {
			if (data->done == TRUE) {
				return NULL;
			}
		}

		number = data->buffer[next_out];
		next_out = (next_out + 1) % MAX_ITEMS;
		data->count--;
#ifdef DEBUG_OUTPUT
		fprintf(stdout, "buffer count to consume is %d\n", data->count);
#endif /* DEBUG_OUTPUT */

		isPrime = TRUE;
		for ( i = 2; isPrime == TRUE && i * i <= number; i += 2 ) {
			if ( number % i == 0 ) {
				isPrime = FALSE;
			}
			if ( i == 2 ) i = 1;
		}

		fprintf(stdout, "%d : %d is ", ++count, number);
		if ( isPrime == FALSE ) {
			fprintf(stdout, "not ");
		}
		fprintf(stdout, "prime\n");
	}

	return NULL;
}

int main() {
	pthread_t consumer;
	int number, next_in = 0;
	struct primecheck_data *data = malloc(sizeof(struct primecheck_data));

	if ( data == NULL ) {
		perror("Unable to allocate shared data structure");
		return(1);
	}
	
	data->count = 0;
	data->done = FALSE;

	if ( pthread_create(&consumer, NULL, isPrimeConsumer, data) != 0 ) {
		fprintf(stderr, "Failure to create consumer thread\n");
		return(1);
	}

	while ( fscanf(stdin, "%d", &number) == 1 ) {
		while ( data->count == MAX_ITEMS );
		data->buffer[next_in] = number;
		next_in = (next_in + 1) % MAX_ITEMS;
		data->count++;
#ifdef DEBUG_OUTPUT
		fprintf(stdout, "buffer count read is %d\n", data->count);
#endif /* DEBUG_OUTPUT */
	}

	data->done = TRUE;
#ifdef DEBUG_OUTPUT
	fprintf(stdout, "done set to true\n");
#endif /* DEBUG_OUTPUT */

	pthread_join(consumer, NULL);

	free(data);

	return 0;
}
