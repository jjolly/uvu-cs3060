#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>

int main(int argc, char *argv[]) {
	int rc;
	struct stat mystat;
	char *mypath;

	if(argc > 1) {
		asprintf(&mypath, "%s", argv[1]);

		rc = stat(mypath, &mystat);

		free(mypath);

		fprintf(stdout, "File %s is inode %d, size %d allocated in %d blocks of size %d\n", argv[1], (int)mystat.st_ino, (int)mystat.st_size, (int)mystat.st_blocks, (int)mystat.st_blksize);
	}

	return rc;
}
