#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]){
	int i;
	char buf[512];  // input from pipe
	int n = read(0, &buf, sizeof(buf));
	if(n > 0 && buf[n-1] == '\n'){
		buf[n-1] = '\0'; 
	}

	char *argvP[32];
	char *argvX[32];
	for(i = 1; i < argc; i++){
		argvX[i-1] = argv[i];  // end of xargs' argv
	}

	argvP[0] = buf;
	argvP[1] = 0;

	argvX[i-1] = 0;

	if(fork() == 0){
		exec(argv[1], argvX[2]);
	}else{
		wait(0);
		exec(argv[1], argvP);
	}
}