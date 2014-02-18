#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define DEBUG_OUTPUT
#undef DEBUG_OUTPUT

#define MAX_ITEMS (20)
#define TRUE (1)
#define FALSE (0)

/* primecheck_data
	 Structure to pass into the child thread.
	 Data Members :
	 buffer : Producer/Consumer buffer
	 count  : Number of elements in the buffer
	 done   : Flag when the producer is finished
*/
struct primecheck_data {
	int buffer[MAX_ITEMS];
	int count;
	int done;
};

/* int isPrime(int)
	 Function to determine the prime-ness of a number
	 Input:
	 int number : Number to test.
	 Output:
	 	TRUE - is prime
		FALSE- is not prime
	 */
int isPrime(int number) {
	int i, result = TRUE;

	/* Anything less than two is not prime */
	if ( number < 2 ) result = FALSE;
	else
		/* The i loop variable is my test. First two is tested, followed
			 by all the odd integers starting with three. If the loop
			 variable exceed the square root of the number being tested, then
			 all the possible divisors have been tested and the number must
			 be prime */
		for ( i = 2; result == TRUE && i * i <= number; i += (i == 2) ? 1 : 2 )
			if ( number % i == 0 ) result = FALSE;

	return result;
}

/* void printIsPrime(int)
	 Function to print "<number> is [not ]prime"
	 */
void printIsPrime(int number) {
	fprintf(stdout, "%d is ", number);
	if ( isPrime(number) == FALSE ) fprintf(stdout, "not ");
	fprintf(stdout, "prime\n");
}

/* void *isPrimeConsumer(void *)
	 Consumer thread function to test the primeness of all numbers sent
	 by a producer thread
	 Input:
	 ptr : Pointer to the primecheck_data structure used between threads
	 */
void *isPrimeConsumer(void *ptr) {
	int count = 0, number, next_out = 0;
	struct primecheck_data *data = (struct primecheck_data *)ptr;

	while(data->done == FALSE || data->count > 0) {
		if ( data->count > 0 ) {
			number = data->buffer[next_out];
			next_out = (next_out + 1) % MAX_ITEMS;
			data->count--;

			fprintf(stdout, "%d :", ++count);
			printIsPrime(number);
		}
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
	
	/* Initialize the data structure */
	data->count = 0;
	data->done = FALSE;

	if ( pthread_create(&consumer, NULL, isPrimeConsumer, data) != 0 ) {
		fprintf(stderr, "Failure to create consumer thread\n");
		return(1);
	}

	/* Read each number from stdin and send it to the consumer */
	while ( fscanf(stdin, "%d", &number) == 1 ) {
		while ( data->count == MAX_ITEMS );
		data->buffer[next_in] = number;
		next_in = (next_in + 1) % MAX_ITEMS;
		data->count++;
	}

	/* Notify the consumer that all the data has been read */
	data->done = TRUE;

	/* Wait for the consumer to finish */
	pthread_join(consumer, NULL);

	free(data);

	return 0;
}
