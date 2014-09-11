#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]) {
	printf("About to dump to core\n");

	abort();

	printf("This line will never be reached\n");

	return 0;
}
