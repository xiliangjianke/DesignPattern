#include <iostream>
#include "Singleton.h"

/*单例模式简单实现*/
/*
int main()
{
	Singleton *s1 = Singleton::getInstance();
	Singleton *s2 = Singleton::getInstance();

	system("pause");
	return 0;
}
*/

#ifdef win32
/*非线程安全 单例模式*/
#include <process.h>
#include <Windows.h>

//多线程，线程数目：5
#define THREAD_NUM 5

unsigned int __stdcall CallSingleton(void *pPM)
{
	Singleton *s = Singleton::getInstance();
	int nThreadNum = *(int *)pPM; 
	Sleep(50);
	//printf("线程编号为%d\n", nThreadNum);
	return 0;
}


int main()
{
	HANDLE  handle[THREAD_NUM];

	//线程编号
	int threadNum = 0;
	while (threadNum < THREAD_NUM)
	{
		handle[threadNum] = (HANDLE)_beginthreadex(NULL, 0, CallSingleton, &threadNum, 0, NULL);
		//等子线程接收到参数时主线程可能改变了这个i的值
		threadNum++;
	}
	//保证子线程已全部运行结束
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);

	system("pause");
	return 0;
}
/* for linux platform */
#else
#define THREAD_NUM 5
#include<pthread.h>
void* callSingleton_Lazy(void *pPM)
{
	Singleton_Lazy *s = Singleton_Lazy::getInstance();
	pthread_t nThreadNum = pthread_self();
	// sleep(50);
	printf("线程编号为%ld\n", nThreadNum);
	return 0;
}

void* callSingleton_Hungry(void *pPM)
{
	Singleton_Hungry *s = Singleton_Hungry::getInstance();
	pthread_t nThreadNum = pthread_self();
	// sleep(50);
	printf("线程编号为%ld\n", nThreadNum);
	return 0;
}

int main()
{
	pthread_t threads_pool[THREAD_NUM];
	int tids[THREAD_NUM];
	printf("Singleton Lazy mode:\n");
	for(int i = 0; i < THREAD_NUM; i++) 
	{
		tids[i] = pthread_create(&threads_pool[i], NULL, callSingleton_Lazy, NULL);
		pthread_join(threads_pool[i], (void**)&tids[i]);
	}

	printf("Singleton Hungry mode:\n");
	for(int i = 0; i < THREAD_NUM; i++) 
	{
		tids[i] = pthread_create(&threads_pool[i], NULL, callSingleton_Hungry, NULL);
		pthread_join(threads_pool[i], (void**)&tids[i]);
	}

	return 0;
}

#endif