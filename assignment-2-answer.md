##作业2：
解：  

	semaphore empty = k, full = 0, mutex = 1;
	int j = 0;

	producer()               consumer()
	{                        {
		while(1){            	while(1){
			生产;
			P(empty);           	P(full);
			P(mutex);               P(mutex);
			填充buffer;             消费buffer;
			V(mutex);               j++;
			V(full);                if(j >= n) 
		}                           {    
	}                                  V(empty);
                                       j = 0;
                                    }
                                    V(mutex);
                                 }
                             }