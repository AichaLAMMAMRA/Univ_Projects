#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

volatile int var_i = 0;
pthread_mutex_t mutex;
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;

void* say_hello(void* d)
{
	if (d<5)
	{
    printf("consomateur! tid=%p\n",(void *) pthread_self());
	}
	if (d>= 5)
	{
    printf("producteur! tid=%p\n",(void *) pthread_self());
	}
   d++;
   printf("%d\n",d);
   //pthread_cond_wait(&cond , &mutex);
   pthread_mutex_lock(&mutex);
   var_i++;
   //printf("%d\n", var_i );
   pthread_mutex_unlock(&mutex);
   //if (d == 10)
   //pthread_cond_signal(&cond);
   printf("[%d] thread quitting\n",d );

   return NULL;
}

 
int main(int argc, char const *argv[])
{
    pthread_t* threads;
    int i,nb;
    int * args;
    int para = 0;
    nb =atoi(argv[1]);
    threads = malloc(nb * sizeof(pthread_t));
    args = malloc(nb*sizeof(int));

    pthread_mutex_init(&mutex, NULL);
 	for (i= 0; i < nb; i++)
 	{
 		args[i] = i;
 		pthread_create(&threads[i],NULL,say_hello,(void*) para);
 		para++;
 	}

 	for (int i = 0; i < nb; ++i)
 	{
 		pthread_join(threads[i], NULL);
 	}
 //   pthread_create(&t1,NULL,say_hello,(void*)var_i);
 //   pthread_create(&t2,NULL,say_hello,(void*)var_i);
//    pthread_create(&t3,NULL,say_hello,NULL);
//    pthread_join(t1,NULL);
//    pthread_join(t2,NULL);
//    pthread_join(t3,NULL);
    printf("la valeur %d\n",var_i);
printf("Main quitting \n");
return 0;
}