#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
int main()
{
	pid_t rc = fork();
	if(rc < 0)
	{
		fprintf(stderr,"fork failed\n");
		exit(1);
	}else if(rc == 0){
		printf("child pid:%d \n",(int)getpid());
		fflush(stdout);
		char name[] {"pwd"};
		char* argv[]{name,NULL};
		int rc_exec = execvp("/bin/myshell_no_such_program",argv);
		if(rc_exec == -1){perror("execvp");return 1;}
	}else{
		pid_t rc_wait = wait(NULL);
		if(rc_wait == -1){perror("wait");return 1;}
		else{printf("wait after,parent pid:%d \n",(int)getpid());}
	     }
}

