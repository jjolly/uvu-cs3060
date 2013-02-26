#include <stdio.h>
#include <stdlib.h>

#define LINELEN 256

void send_to_stdout( FILE*);

int main (int argc, char* argv[ ])
{
    FILE *fp;

    if (argc == 1)
        send_to_stdout ( stdin);
    else if (argc == 2)
        if ((fp = fopen(*++argv, "r") ) != NULL)
        {
            send_to_stdout ( fp );
            fclose ( fp );
        }
        else
        {
            perror("could not open the file.");
            exit(1);
        }
    else
    {
        perror("Invalid command – too many arguments");
        exit(1);
    }
    return 0;
}

void send_to_stdout(FILE *fp)
{
    char line[LINELEN];

    while ( fgets (line, LINELEN, fp) )
    {
        if (fputs ( line, stdout ) == EOF )
        {
            perror("Write to stdout failed");
            exit(1);
        }
    }
}
