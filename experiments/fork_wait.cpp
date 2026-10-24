#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc,char* grgv[])
{
	printf("before (pid:%d)\n",(int)getpid());
	fflush(stdout);
	pid_t rc = fork();
	if(rc < 0){
		fprintf(stderr,"fork failed\n");
		exit(1);
	}else if(rc == 0){
		printf("child done,rc %d,child (pid:%d)\n",rc,(int)getpid());
	}else{
		printf("before wait\n");
		fflush(stdout);
		pid_t rc_wait = wait(NULL);
		if(rc_wait == -1){
			perror("wait");
			fprintf(stderr,"wait failed, rc=%d, rc_wait=%d, parent(pid:%d)\n",rc, rc_wait, (int)getpid());
			return 1;
		}else{
				
	        	printf("after wait,rc %d,rc_wait %d,parent (pid:%d)\n",rc,rc_wait,(int)getpid());
		}
	}
	return 0;
}
