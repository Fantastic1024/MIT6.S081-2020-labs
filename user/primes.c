#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
sieve(int leftPipe[])
{
	int i;
	int firstN;
	int rightPipe[2];

	pipe(rightPipe);

	read(leftPipe[0], &firstN, 4);  //read first number

	if(fork() == 0){

		int hasNext = 0;
		while(read(leftPipe[0], &i, 4) == 4){
			if(i % firstN != 0){
				write(rightPipe[1], &i, 4);
				hasNext = 1;
			}
		}

		close(rightPipe[1]);
		close(leftPipe[0]);

		if(hasNext){
			sieve(rightPipe);
		}
		
		close(rightPipe[0]); // Cannot close before pass to next child, Otherwise dispear
		exit(0);
	}else{
		printf("prime %d\n", firstN);
		close(leftPipe[0]);
		close(leftPipe[1]);
		close(rightPipe[0]);
		close(rightPipe[1]);

		wait(0);
		exit(0);
	}
}

int
main(int argc, char *argv[])
{
	int i;
	int primesPipe[2];

	pipe(primesPipe);
  if(fork() == 0){
		close(primesPipe[1]);
		sieve(primesPipe);
		exit(0);
	}else{
		close(primesPipe[0]);
		for(i = 2; i < 36; i++){
			write(primesPipe[1], &i, 4);
  	}
		close(primesPipe[1]);
		wait(0);
		exit(0);
	}
  
}