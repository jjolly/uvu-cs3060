#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define BUFFER_SIZE (20)

int *buffer;
int counter;
int in;
int out;

void *producer(void *ptr) {
	int value = 1;
	while(1) {
		while(counter == BUFFER_SIZE)
			; /* do nothing */

		buffer[in] = value++;
		in = (in + 1) % BUFFER_SIZE;
		counter++;
	}
}

void consumer() {
	int last = 0;
	while(last < 65536) {
		while(counter == 0)
			; /* do nothing */
		
		last++;

		if ( last != buffer[out] ) {
			printf("Error at value %d! I was expecting %d\n", buffer[out], last);
			exit(-1);
		}

		out = (out + 1) % BUFFER_SIZE;
		counter--;
	}
}

int main() {
	pthread_t child;
	buffer = malloc(sizeof(int) * BUFFER_SIZE);
	counter = 0;
	in = 0;
	out = 0;

	pthread_create(&child, NULL, producer, NULL);

	consumer();

	return 0;
}
