    #include <stdio.h> 
    #include <stdlib.h>  
    #include <unistd.h> 
    #include <sys/shm.h>  
    #include <sys/sem.h>  
    #define SHM_SIZE (1024*1024)  
    #define SHM_MODE 0600  
    #define SEM_MODE 0600  
      
    #if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)  
    /*   union   semun   is   defined   by   including   <sys/sem.h>   */   
    #else   
    /*   according   to   X/OPEN   we   have   to   define   it   ourselves   */   
    union semun{  
        int val;  
        struct semid_ds *buf;  
        unsigned short *array;  
    };  
    #endif  
      
    struct ShM{  
        int value;  
    }* pSM;  
     
    const int N_BUFFER = 10; 
    int cn ;//消费者数量  
    int pn;
    int shmId = -1,semSetId=-1;  
    union semun su;//sem union，用于初始化信号量  
      
    //semSetId 表示信号量集合的 id  
    //semNum 表示要处理的信号量在信号量集合中的索引  
    void waitSem(int semSetId,int semNum)  
    {  
        struct sembuf sb;  
        sb.sem_num = semNum;  
        sb.sem_op = -1;  
        sb.sem_flg = SEM_UNDO;//  
       
        if(semop(semSetId,&sb,1) < 0){  
            perror("waitSem failed");  
            exit(1);  
        }  
    }  
    void sigSem(int semSetId,int semNum)  
    {  
        struct sembuf sb;  
        sb.sem_num = semNum;  
        sb.sem_op = 1;  
        sb.sem_flg = SEM_UNDO;  
       
        if(semop(semSetId,&sb,1) < 0){  
            perror("waitSem failed");  
            exit(1);  
        }  
    }  
   
    void produce()  
    {  
    	int val = pSM->value;
        pSM->value++;
        printf("produce : %d -> %d\n",val,pSM->value);  
    }  
    
    void consume()  
    {  
        int val = pSM->value;
        pSM->value--;
        printf("consume : %d -> %d\n",val,pSM->value); 
    }  
      
    void init()  
    {  
       
        if((shmId = shmget(IPC_PRIVATE,SHM_SIZE,SHM_MODE)) < 0)  
        {  
            perror("create shared memory failed");  
            exit(1);  
        }  
        pSM = (struct ShM *)shmat(shmId,0,0);  
        pSM->value = 0;  
        
          
        //信号量创建  
      
        if((semSetId = semget(IPC_PRIVATE,3,SEM_MODE)) < 0)  
        {  
            perror("create semaphore failed");  
            exit(1);  
        }  
        //信号量初始化,其中 su 表示 union semun   
        su.val = N_BUFFER; 
        if(semctl(semSetId,0,SETVAL, su) < 0){  
            perror("semctl failed");  
            exit(1);  
        }  
        su.val = 0; 
        if(semctl(semSetId,1,SETVAL,su) < 0){  
            perror("semctl failed");  
            exit(1);  
        }  
        su.val = 1;  
        if(semctl(semSetId,2,SETVAL,su) < 0){  
            perror("semctl failed");  
            exit(1);  
        }  
    }  
    int main()  
    {  
        int i = 0,parent = -1,child = -1; 
        int pn,cn;
		printf("please input the sum of producer:");
		scanf("%d",&pn);
		printf("please input the sum of consumer:");
		scanf("%d",&cn); 
        init(); 
        
        for(i = 0; i < cn+pn; i++)  
        {  
            if((child = fork()) < 0)//调用fork失败  
            {  
                perror("the fork failed");  
                exit(1);  
            }  
            else if(child == 0)//子进程  
            {  
            	if(i < cn){
	                printf("consumer process %d, PID = %d\n",i,getpid());  
	                while(1)  
	                {  
	                    waitSem(semSetId,1);
	                    waitSem(semSetId,2);  
	                    consume();
	                    sigSem(semSetId,2);  
	                    sigSem(semSetId,0); 
	                    sleep(2);
	                }
	                break;  
            		
            	}else{
            		printf("producer process %d, PID = %d\n",i,getpid());  
	                while(1)  
	                {  
	                    waitSem(semSetId,0);  
	                    waitSem(semSetId,2);  
	                    produce(); 
	                    sigSem(semSetId,2);
	                    sleep(1);  
	                    sigSem(semSetId,1);  
	                } 
	                break; 
            	}
                
            }
             
        }      
        return 0;  
    }  

