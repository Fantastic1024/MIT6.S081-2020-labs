#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
sieve(int readFd)
{
	int i;
	int p;
	int rightPipe[2];

	// 读取第一个数即为素数
	if(read(readFd, &p, 4) == 0){
		exit(0);
	}
	printf("prime %d\n", p);

	pipe(rightPipe);

	if(fork() == 0){
		// 子进程：关闭写端和读端，递归处理右管道
		close(rightPipe[1]);
		close(readFd);
		sieve(rightPipe[0]);
		exit(0);
	}

	// 父进程：过滤非 p 倍数，写入右管道
	close(rightPipe[0]);
	while(read(readFd, &i, 4) == 4){
		if(i % p != 0){
			write(rightPipe[1], &i, 4);
		}
	}
	close(readFd);
	close(rightPipe[1]);

	// 等待子进程（整条链）完全结束
	wait(0);
	exit(0);
}

int
main(int argc, char *argv[])
{
	int i;
	int primesPipe[2];

	pipe(primesPipe);
	if(fork() == 0){
		close(primesPipe[1]);
		sieve(primesPipe[0]);
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