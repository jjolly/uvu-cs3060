#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/stat.h>

void my_readdir(char *pathname) {
	DIR *pdir;
	struct dirent *dent;
	struct stat st;
	char *tmpstr;

	fprintf(stdout, "Directory : %s\n", pathname);
	pdir = opendir(pathname);
	
	while((dent = readdir(pdir)) != NULL) {
		if(dent->d_name[0] == '.') continue;
		asprintf(&tmpstr, "%s/%s", pathname, dent->d_name);
		lstat(tmpstr, &st);
		if(S_ISDIR(st.st_mode)) my_readdir(tmpstr);
		else fprintf(stdout, "Filename : %s\n", dent->d_name);

		free(tmpstr);
	}
}

int main(int argc, char *argv[]) {
	char *pathname = ".";

	if(argc > 1) pathname = argv[1];

	my_readdir(pathname);

	return 0;
}
