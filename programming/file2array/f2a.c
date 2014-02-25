#include<stdio.h>

#define MAX_ARRAY_SIZE 100

int read2array(FILE *fp, int *array, int max) {
	int count = 0;

	while(count < max && fscanf(fp, "%d", &array[count]) == 1) {
		count++;
	}

	return count;
}

int main() {
	int array[MAX_ARRAY_SIZE];
	int count;

	count = read2array(stdin, array, MAX_ARRAY_SIZE);

	printf("I read %d values\n", count);

	return 0;
}
