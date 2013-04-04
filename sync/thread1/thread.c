#include<stdio.h>
#include<unistd.h>

void print_msg(char *);

int main()
{
	print_msg("hello ");
	print_msg("world\n");

	return 0;
}

void print_msg(char *m)
{
	int i;
	for ( i = 0 ; i < 5; i++ )
	{
		printf("%s", m);
		fflush(stdout);
		sleep(1);
	}
}
