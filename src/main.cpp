#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sstream>
#include <vector>
int main()
{
   int last_status {0};

    while(1)
    {
	if(last_status != 0)
	{
	   std::cout << "\033[31m";
	}

        std::cout << "MyShell>";

	std::cout << "\033[0m";

	std::cout << "last status: "<< last_status << "\n";

        std::string input{};

	bool negation_flag {false};

	size_t command_index = 0;

        if(std::getline(std::cin,input))//getline是否提取成功
        {
		std::string command {};

		std::string op_before {";"};

		struct Command{
		    std::string command;
		    std::string op_before;
		};

		std::vector<Command> commands;

		if (input.empty()){continue;}

		
		size_t start {0};
		size_t i {0};
		for(i = 0;i < input.size();++i)
		{
			if(input[i] == '&'||input[i] == '|'||input[i] == ';')
			{
			    command = input.substr(start,i - start);
			    commands.push_back({command,op_before});
			    start = i+1;
			    if(input[i] == '&'||input[i] == '|')
			    {
				if(i+1 < input.size() && input[i+1] == input[i])
				{
					if(input[i] == '&')
					{
						op_before = "&&";
					}else{
						op_before = "||";
					}
					start = i+2;
					i++;
				}
			    }else{op_before = ";" ; }
			}
		}
	       if(i == input.size()){
	       		command  = input.substr(start,i-start);
			commands.push_back({command,op_before});
		}

		for(const auto& cmd :commands)
		{
			std::cout << "command: [" << cmd.command << "]"
			<< "op_before: [" << cmd.op_before << "]\n";
		}

	    	std::istringstream iss(input);
		std::vector<std::string> issv{};
		std::string out;

		while(iss >> out){issv.push_back(out);}

		if(issv.empty())
		{
    			continue;
		}

		if(issv[0] == "!")
		{
		   negation_flag = !negation_flag;

		   command_index = 1;
		}

		if(issv[0] == "exit")
		{	std::cout << "logout\n";
			std::cin.clear();
			if(negation_flag)
			  {
				exit(!last_status);
			  }else{
				exit(last_status);
			  }
		}

		if(issv[0] == "cd")
		{
			if(static_cast<int>(std::size(issv)) == 1){
			  last_status = 1;
			  std::cout << "用法错误\n";
			  continue;
			}else if(static_cast<int>(std::size(issv) == 2)){
			  if(chdir(issv[1].data()) == -1){
			  last_status = 1;
			  perror("chdir error");
			  continue;
			  }else{last_status = 0;}
			}else{
			  last_status = 1;
			  std::cout << "用法错误\n";
			  continue;
			}
			continue;
		}

		std::vector<char*> argv{};

		if(issv.size()<=1)
		{
			if(issv[0]=="!")
			{
 				last_status = 1;
			        std::cout << "!无参数，请重新输入\n";
				continue;
			}else if(issv[0]=="exec")
			{
				last_status = 1;
				std::cout << "exec()无参数，请重新输入\n";
				continue;
			}
		}

		if(strcmp(issv[0].data(),"exec")==0)
		    {
			command_index = 1;
		    }

		if(issv.size()!=0)
		    {
		    	for(size_t i = command_index;i < issv.size();i++)
			{
				argv.push_back(issv[i].data());
			}
		    	argv.push_back(nullptr);
		    }

		if(strcmp(issv[0].data(),"exec")==0)
		{
			int rc_execve = execvp(issv[1].data(),argv.data());

			if(rc_execve == -1)
			{
			   last_status = 1;
			   perror("execvp");
		           continue;
			}
		 }

                pid_t rc = fork();

                if(rc < 0)
                {
                    fprintf(stderr,"fork failed\n");
	            exit(1);
                }
                else if(rc == 0)
                {
                    int rc_execvp = execvp(issv[command_index].data(),argv.data());

                    if(rc_execvp == -1)
                    {
                        perror("execvp");
                        return 1;
                    }
                }
                else
                {
		    int wstatus;

		    pid_t child_pid = wait(&wstatus);

		    if(child_pid == -1)
		    {
			perror("wait");
			return 1;
		    }

		    if(WIFEXITED(wstatus))
		    {
			if(negation_flag)
		  	{
				last_status = WEXITSTATUS(wstatus);

				last_status = !last_status;
	          	}else{
				last_status = WEXITSTATUS(wstatus);
			}
		    }else{
			last_status = 1;
			if(negation_flag) last_status = !last_status;
		    }
                }
            }
        else//提取失败
        {
            if(std::cin.eof())//判断提取失败是否因为输入结束
            {
		std::cout << "logout\n";
		std::cin.clear();
		exit(last_status);
            }else{
		exit(1);
	    }
        }
    }
}
