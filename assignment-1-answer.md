##作业1
解：  

	/*************************************************************************
	> File Name: launch.c
	> Author: yin
	> Mail: 943675853@qq.com
	> Created Time: 2016年04月24日 星期日 16时21分39秒
	 ************************************************************************/
	
	#include<stdio.h>
	#include <sys/types.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <sys/wait.h>
	
	int main(int argc, char* argv[])
	{
		pid_t pid;
		int status;
		int flag;
		if(argc < 2)
		{
			printf("Usage : %s process\n",argv[0]);
			exit(0);
		}
		char* args[argc];
		for(int i = 1; i < argc; i++)
		{
			args[i-1] = argv[i];
		}
		args[argc-1] = (char*)0;
		if((pid = fork()) < 0)
		{
			printf("fork error!\n");
			exit(0);
		}else if(pid == 0){
			flag = execvp(args[0],args);
			if(flag == -1)
			{
				printf("execvp error!\n");
			}
		}else{
			pid = wait(&status);
			printf("The child process %d exit normally!\n",pid);
		}
		return 0;
	}

1. 使用**`gcc launch.c -o launch`**编译； 
2. 运行`./launch `+要启动的进程，如`./launch ls`， `./launch ls -a`;
3. 进程执行完毕后提示进程正常退出。
4. 结果如下：   
![result](images/assignment-1.png)