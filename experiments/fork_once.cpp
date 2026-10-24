#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
	printf("before (pid:%d)\n",(int)getpid());
	fflush(stdout);
	pid_t rc = fork();
	if(rc < 0)
 	{
		fprintf(stderr,"fork failed\n");
		exit(1);
	}else if(rc == 0){
		printf("rc %d child (pid: %d)\n",rc,(int)getpid());
	}else{
		printf("rc %d parent (pid: %d)\n",rc,(int)getpid());
	}
	return 0;
}
