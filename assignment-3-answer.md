##作业3：  
解：  
&nbsp;&nbsp;&nbsp;&nbsp;stop表示当一方提出上桥后，应阻止另一方未上桥的后继车辆  

	semaphore stop = 1, mutex = 1, mutexA = 1, mutexB = 1;
	int countA = 0, countB = 0;

	A()                                B()
	{                                  {
		P(stop);                           P(stop);
		P(mutexA);                         P(mutexA);
		countA++;                          countA++;
		if(countA == 1) P(mutex);          if(countA == 1) P(mutex);
		V(mutexA);                         V(mutexA);
		OnBridge();                        OnBridge();
		P(mutexA);                         P(mutexA);
		countA--;                          countA--;
		if(countA == 0) V(mutex);          if(countB == 0) V(mutex);
		V(mutexA);                         V(mutexB);
	}                                   }


