#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void *print_msg(void *);

int main()
{
	pthread_t thr1, thr2;

	pthread_create(&thr1, NULL, print_msg, (void *)"hello ");
	pthread_create(&thr2, NULL, print_msg, (void *)"world\n");
	pthread_join(thr1, NULL);
	pthread_join(thr2, NULL);

	return 0;
}

void *print_msg(void *m)
{
	char *cp = (char *)m;
	int i;
	for ( i = 0 ; i < 5; i++ )
	{
		printf("%s", cp);
		fflush(stdout);
		sleep(1);
	}

	return NULL;
}
