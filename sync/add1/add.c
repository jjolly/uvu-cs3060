#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

static int globalValue = 0;

void *funny_add(void *);

int main()
{
	int i;

	pthread_t mythread;
	pthread_create(&mythread, NULL, funny_add, NULL);

	for ( i = 0; i < 10; i++ )
	{
		globalValue++;
		sleep(1);
	}

	pthread_join(mythread, NULL);
	printf("\nDone ...global value = %d\n", globalValue);

	return 0;
}

void *funny_add(void * m)
{
	int j;

	for ( j = 0; j < 10; j++ )
	{
		int temp = globalValue + 1;
		sleep(1);
		globalValue = temp;
	}

	return NULL;
}
