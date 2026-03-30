#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]){
	int n;
	int len = 0;
	char c;  // inputing a line from pipe
	char buf[512];
	
	while(read(0, &c, 1) == 1){
		if(c == '\n'){
			if(fork() == 0){
				
			}else{

				wait(0);
				exit(0);
			}
			memset(buf, 0, sizeof(buf));
		}
	}
}