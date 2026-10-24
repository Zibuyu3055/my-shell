#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{
	char buf[100];
	
	//获取当前的工作目录
	if(getcwd(buf,sizeof(buf))==NULL){
	perror("getcwd error");
	exit(EXIT_FAILURE);
	}
	printf("更改前的工作目录：%s\n",buf);
	
	//更改工作目录
	if(chdir("/tmp") == -1){
	perror("chdir error");
	exit(EXIT_FAILURE);
	}

	if(getcwd(buf,sizeof(buf))==NULL){
	perror("getcwd error");
	exit(EXIT_FAILURE);
	}
	printf("更改后的工作目录：%s\n",buf);

	return 0;
}
