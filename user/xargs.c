#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
readOneLine(char *buf){  // Reading a line from pipe
	char c;
	int i = 0;
	memset(buf, 0, 512);

	while(read(0, &c, 1) == 1){
		if(c == '\n'){
			buf[i] = '\0';
			return i;  // return the index of \0
		}else{
			buf[i] = c;
			i++;
		}
	}
	return i;
}

int
main(int argc, char *argv[]){
	char buf[512];
	int i;
	while(readOneLine(buf)){
		if(fork() == 0){
			char *args[32];
			for(i = 1; i < argc; i++){
				args[i - 1] = argv[i];
			}
			args[i - 1] = buf;
			args[i] = 0;
			exec(argv[1], args);
			exit(1);
		}else{
			wait(0);
		}
	}
	exit(0);
}
