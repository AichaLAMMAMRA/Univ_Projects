#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

pthread_mutex_t mutex;
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;

typedef struct node node;
struct node {
  struct node* next;
  int value;
};

struct node* volatile stack_lock_free = 0;
struct node* stack_lock = 0;



void push_lock(int value)
{

pthread_mutex_lock(&mutex);

node* new = malloc(sizeof(*new));
node* old;

old = stack_lock;

new->value = value;
new->next = old;
stack_lock = new;

pthread_mutex_unlock(&mutex);

}

int pop_lock()
{

pthread_mutex_lock(&mutex);
node* new = stack_lock;
int val;
stack_lock->next = new->next;
val = new->value;
pthread_mutex_unlock(&mutex);

return val;
}

void* say_hello()
{
 
   printf("hello \n" );
   int i;
   for(i=0; i<1000000; i++) {
    push_lock(i);
  }

   {
    node* actu = stack_lock;
    
   }
   for(i=0; i<1000000; i++) {
    pop_lock();
  }
     {
    node* actu = stack_lock;
    
   }
   
   return NULL;
}

 
int main(int argc, char const *argv[])
{
    struct timeval temps_avant, temps_apres;
    pthread_t* threads;
    int i,nb;
    nb =atoi(argv[1]);
    threads = malloc(nb * sizeof(pthread_t));
  
  gettimeofday (&temps_avant, NULL);
  
  pthread_mutex_init(&mutex, NULL);
  for (i= 0; i < nb; i++)
  {
    pthread_create(&threads[i],NULL,say_hello, NULL);
  }
  for (int i = 0; i < nb; ++i)
  {
    pthread_join(threads[i], NULL);
  }
  gettimeofday (&temps_apres, NULL);
printf("temps en us: %ld us\n", ((temps_apres.tv_sec - temps_avant.tv_sec) * 1000000 + temps_apres.tv_usec) - temps_avant.tv_usec);
return 0;
}