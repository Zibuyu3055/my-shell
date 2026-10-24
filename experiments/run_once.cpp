#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "No command parameter\n");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        execvp(argv[1], &argv[1]);

        // 只有 execvp 失败时才会执行到这里
        perror("execvp failed");
        return 1;
    }
    else
    {
        if (wait(NULL) == -1)
        {
            perror("wait failed");
            return 1;
        }

        printf("Child process is finished.\n");
        return 0;
    }
}
