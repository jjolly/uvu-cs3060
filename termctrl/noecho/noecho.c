#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<termios.h>

int main()
{
	int fd_tty;
	FILE *fp_tty;
	struct termios attrs;
	char keygood = 0;

	fd_tty = open("/dev/tty", O_RDONLY);
	fp_tty = fdopen(fd_tty, "r");

	tcgetattr(fd_tty, &attrs);

	attrs.c_lflag &= ~ECHO & ~ICANON;

	tcsetattr(fd_tty, TCSANOW, &attrs);

	fprintf(stderr, "Please press 'q', space or enter:");

	while(!keygood)
	{
		keygood = fgetc(fp_tty);
		if ( keygood != '\n' && keygood != ' ' && keygood != 'q' )
			keygood = 0;
	}

	switch(keygood)
	{
		case '\n':
			fprintf(stderr, "\n\nPressed enter\n");
			break;
		case ' ':
			fprintf(stderr, "\n\nPressed space\n");
			break;
		case 'q':
			fprintf(stderr, "\n\nPressed 'q'\n");
			break;
	}

	attrs.c_lflag |= ECHO | ICANON;

	tcsetattr(fd_tty, TCSANOW, &attrs);

	fclose(fp_tty);
	close(fd_tty);

	return 0;
}
