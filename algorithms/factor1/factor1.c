/*
factor1 : Factor one number given on the command line
	Copyright (C) 2017  John L. Jolly

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

#define LARGEST_FACTOR ((int)((1 << ((8 * sizeof(int)) / 2)) - 1))

/*
	Very simple factor algorithm. Only use for small integers
*/
void factor1(int n) {
	int i = 2;

	printf("%d:", n);
	/* Just keep looping until i cannot factor n anymore */
	while ( i <= LARGEST_FACTOR && i * i <= n ) {
		/* Is i a factor of n */
		if ( n % i == 0 ) {
			/* Yes, print it */
			printf(" %d", i);
			/* ...and "remove" the factor from n */
			n /= i;
		} else {
			/* i is not a factor of n, so move on to the next odd integer */
			/* An optimization of this would be to only use primes */
			/* Always starting at two, the next odd integer is three */
			if ( i == 2) {
				i++;
			} else {
				/* Odd number increment by two */
				i += 2;
				/* Check that our factor doesn't have a simple factor as well */
				/* This is an optimization that does not affect the result */
				while(i > 7 && (i % 3 == 0 || i % 5 == 0 || i % 7 == 0)) {
					i += 2;
				}
			}
		}
	}

	/* Display the last factor */
	printf(" %d\n", n);
}

int main(int argc, char *argv[]) {
	int n;

	/*
	   Show usage and exit if no number was provided or the number
	   is not a positive integer greater than 1
	*/
	if (argc < 2 || (n = atoi(argv[1])) < 2) {
		printf("Usage: %s <positive integer>\n", argv[0]);
		exit(-1);
	}

	factor1(n);

	return 0;
}
