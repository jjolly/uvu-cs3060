/* Reverse video control example
   This example uses the \033[ terminal control commands. The bulk of
   these can be found at this website:
   http://ascii-table.com/ansi-escape-sequences.php
   Basically, this example turns on reverse video and returns the
   console to normal */

#include<stdio.h>

int main()
{
	/* This code does not actually display anything. It merely
	   activates the reverse video on the console for the subsequent
	   text. */
	printf("\033[7m");
	/* This text will be affected by the previous control character,
	   or, it will be in reverse video (white background, black text) */
	printf("Reverse On");
	/* Not putting any value between the square bracket and the 'm'
	   will restore the console video to "normal" */
	printf("\033[m");
	printf(" Reverse Off ");
	printf("\033[7m");
	printf("Reverse On");
	printf("\033[m");
	printf("\n");

	return 0;
}
