#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	int i;

  if(argc < 2){
    fprintf(2, "Usage: sleep int...\n");
    exit(1);
  }

	i = atoi(argv[1]);

	if(sleep(i) < 0){
		fprintf(2, "sleep: failed to sleep\n");
		exit(1);
	}

	exit(0);
}