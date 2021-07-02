#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

typedef struct node node;
struct node {
  struct node* next;
  int value;
};

struct node* volatile stack_lock_free = 0;
struct node* stack_lock = 0;



void push_lock_free(int value)
{
  node* new = malloc(sizeof(*new));
  node* old;
  new->value = value;

do{

  old = stack_lock_free;
  new->next = old;
}while(__sync_val_compare_and_swap(&stack_lock_free, old, new) != old);

}

int pop_lock_free()
{

node* new;
int val;

do{
  new = stack_lock_free;
  new->next = stack_lock_free;
}while(!new && __sync_val_compare_and_swap(&stack_lock_free, new, new->next) != new);

//free(new);
val = new->value;
return val;
}

void* say_hello() {
  printf("hello \n" );
  int i;
  for(i=0; i<1000000; i++) {
    push_lock_free(i);
  }

  node* actu = stack_lock;


  for(i=0; i<1000000; i++) {
    pop_lock_free();
  }

  actu = stack_lock_free;


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