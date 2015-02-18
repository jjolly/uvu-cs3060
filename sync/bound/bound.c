/*
Bounded Buffer Example - UVU CS3060 example of the bounded buffer problem
Copyright (C) 2015  John L. Jolly

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

/*
	BUFFER_SIZE defines how many elements are available in the buffer
*/
#define BUFFER_SIZE (20)

/*
	The global count variable is the victim of this program. Should be
	zero by the end of the program. Rarely is.
*/
int count = 0;
int buffer[BUFFER_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*
	producer_thread_func - Add elements to the buffer
*/
void *producer_thread_func(void *p) {
	int in = 0;
	int number = 1;
	int limit = atoi((char *)p);

	while (number <= limit) {
		while (count >= BUFFER_SIZE);

		buffer[in] = number;

		pthread_mutex_lock(&mutex);
		/* CRITICAL SECTION */
		count++;
		/* END CRITICAL SECTION */
		pthread_mutex_unlock(&mutex);

		number++;

		in = (in + 1) % BUFFER_SIZE;
	}

	while (count >= BUFFER_SIZE);

	buffer[in] = 0;

	pthread_mutex_lock(&mutex);
	/* CRITICAL SECTION */
	count++;
	/* END CRITICAL SECTION */
	pthread_mutex_unlock(&mutex);

	return NULL;
}

/*
	consumer_thread_func - Sum the content of the buffer
*/
void *consumer_thread_func(void *p) {
	int out = 0;
	int sum = 0;

	while(1) {
		while(count < 1);

		if ( buffer[out] == 0 ) break;

		sum += buffer[out];

		pthread_mutex_lock(&mutex);
		/* CRITICAL SECTION */
		count--;
		/* END CRITICAL SECTION */
		pthread_mutex_unlock(&mutex);

		out = (out + 1) % BUFFER_SIZE;
	}

	*(int *)p = sum;

	return p;
}

/*
	Main method created THREAD_COUNT * 2 threads to increment and
	decrement the count variable.
*/
int main(int argc, char *argv[]) {
	pthread_t prod_thread;
	pthread_t cons_thread;
	int i, *pi;

	printf("Main thread started\n");

	if ( argc < 2 ) {
		printf("You need to give me a number to work with\n");
		exit(-1);
	}

	pthread_create(&prod_thread, NULL, producer_thread_func, argv[1]);
	pthread_create(&cons_thread, NULL, consumer_thread_func, &i);

	printf("Main thread created child threads\n");

	pthread_join(prod_thread, NULL);
	pthread_join(cons_thread, (void **)&pi);

	printf("Sum of buffer is %d\n", *pi);

	return 0;
}

