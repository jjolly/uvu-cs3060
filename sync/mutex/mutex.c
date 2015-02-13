/*
Mutex Example - UVU CS3060 exmaple of using mutex primatives
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
	THREAD_COUNT defines how many of each thread type will be created
*/
#define THREAD_COUNT (20)
/*
	INC_COUNT defines how many times each thread should increment or
	decrement the counter
*/
#define INC_COUNT    (0xFFFF)

/*
	The global count variable is the victim of this program. Should be
	zero by the end of the program. Rarely is.
*/
int count = 0;

/*
	add_thread_func - Simply increments the count variable INC_COUNT times
*/
void *add_thread_func(void *p) {
	/*
		This mutex was provided by the main thread to protect the count
		variable.
	*/
	pthread_mutex_t *pmutex = (pthread_mutex_t *)p;
	int i;
	for(i = 0; i < INC_COUNT; i++) {
		/*
			The modification of the count variable is the critical section
			of this program. Surrounding the critical section with the
			mutex lock and unlock should prevent the count variable from
			becoming corrupted.
		*/
		pthread_mutex_lock(pmutex);
		/* BEGIN CRITICAL SECTION */
		count++;
		/* END CRITICAL SECTION*/
		pthread_mutex_unlock(pmutex);
	}
	return NULL;
}

/*
	sub_thread_func - Simply decrements the count variable INC_COUNT times
*/
void *sub_thread_func(void *p) {
	/*
		This mutex was provided by the main thread to protect the count
		variable.
	*/
	pthread_mutex_t *pmutex = (pthread_mutex_t *)p;
	int i;
	for(i = 0; i < INC_COUNT; i++) {
		/*
			The modification of the count variable is the critical section
			of this program. Surrounding the critical section with the
			mutex lock and unlock should prevent the count variable from
			becoming corrupted.
		*/
		pthread_mutex_lock(pmutex);
		/* BEGIN CRITICAL SECTION */
		count--;
		/* END CRITICAL SECTION*/
		pthread_mutex_unlock(pmutex);
	}
	return NULL;
}

/*
	Main method created THREAD_COUNT * 2 threads to increment and
	decrement the count variable.
*/
int main() {
	pthread_t add_threads[THREAD_COUNT];
	pthread_t sub_threads[THREAD_COUNT];
	int i;

	/*
		Mutex primative that will be used to protect the global count
		variable. Will be shared with all the threads.
	*/
	pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

	printf("Main thread started\n");

	/*
		Create the THREAD_COUNT * 2 threads to abuse the global count
		variable. Each thread will be given a mutex that will be used
		to protect the global count variable.
	*/
	for ( i = 0; i < THREAD_COUNT; i++ ) {
		pthread_create(&add_threads[i], NULL, add_thread_func, &count_mutex);
		pthread_create(&sub_threads[i], NULL, sub_thread_func, &count_mutex);
	}

	printf("Main thread created child threads\n");

	/*
		Join all the threads, just to make sure they get the work done
	*/
	for ( i = 0; i < THREAD_COUNT; i++ ) {
		pthread_join(add_threads[i], NULL);
		pthread_join(sub_threads[i], NULL);
	}

	/*
		The same number of increments and decrements have occured by this
		point. Now display the count variable - which should be zero.
	*/
	printf("Main thread count is %d\n", count);

	return 0;
}

