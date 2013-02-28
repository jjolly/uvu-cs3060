/* stdio for printf */
#include<stdio.h>

/* Since I am using command line arguments, the parameters for main
   must be defined. The argc parameter is the count of options provided,
   including the command itself. The argv parameter is an array of
   character pointers. This means that argv points to an array. This
   in turn is an array of pointers, each pointer pointing to the first
   character in a nullbyte-terminated string. The first pointer of the
   array (argv[0]) points to the actual command, and each subsequent
   array element (argv[1], argv[2], etc.) points to the arguments that
   follow the command, */
int main(int argc, char * argv[]) {
	/* showline will be set to one when line numbers are needed */
	int i, showline = 0;

	/* Loop through all the arguments, even the command */
	for ( i = 0; i < argc; i++ ) {
		/* Is the first character of the current argument a dash? */
		if( argv[i][0] == '-' ) {
			/* The first character of the current argument is a dash.
			   Is that dash followed by an 'n' and a nullbyte? */
			if( argv[i][1] == 'n' && argv[i][2] == '\0' ) {
				/* The dash-'n' was specified. Turn on line numbers */
				showline = 1;
			}
			/* This would be a really good place to check for other
			   command-line arguments or report that the argument
			   specified is invalid. */
		}

		/* Only print the line number if the dash-'n' has been used */
		if( showline ) {
			printf("%d : ", i);
		}

		/* Print the current argument to stdin */
		printf("%s\n", argv[i]);
	}
	return 0;
}
