#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define N       128
#define LOOPS   2000
#define THREADS 20
#define ABORT   0xf0000000 /* f + 7x0 */
#define BACKOFF 500        /* initial backoff in microseconds */

#define STM    0
#define LOCK   1
#define ATOMIC 2

int version;

struct value {
  int value;
  int counter;
};

struct memory {
  pthread_mutex_t        lock;
  struct value* volatile values[N];
  volatile int           clock;
};

struct tx {
  int       readSet[N];      /* bitset */
  int       writeSet[N];     /* bitset */
  int       writenValues[N]; /* valeurs */
  int       clock;
  long long backoff;
};

struct memory      memory;
__thread struct tx tx;

struct value* newValue(int value, int counter) {
  struct value* res = malloc(sizeof(struct value));
  res->value = value;
  res->counter = counter;
  return res;
}

void initMemory() {
  pthread_mutex_init(&memory.lock, 0);
  for(int i=0; i<N; i++)
    memory.values[i] = newValue(0, 0);
  memory.clock = 1;
  srand(time(0));
}

void startTX() {

for (int i = 0; i < N; ++i)
{
tx.writeSet[i] = 0;
tx.readSet[i] = 0;
}

tx.clock = memory.clock;
tx.backoff = BACKOFF;

/*
 *   ABORT: abort, other: ok
 */
}
int writeValue(int idx, int value) {
  tx.writeSet[idx] = 1;
  tx.writenValues[idx] = value;
  return 0;
}

/*
 *   ABORT: abort, other: ok
 */
int readValue(int idx) {
  if (tx.writeSet[idx])
    return tx.writenValues[idx];
  struct value* value = memory.values[idx];
  if (value->counter >= tx.clock)
    return ABORT;

  tx.readSet[idx] = 1;
  return value->value;
}

/*
 *   ABORT: abort, other: ok
 */
int commitTX() {
  pthread_mutex_lock(&memory.lock);
  for (int i = 0; i < N; ++i)
  {  
    if (tx.writeSet[i])
    {
      if (memory.values[i]->counter >= tx.clock)
      {
        pthread_mutex_unlock(&memory.lock);
        return ABORT;
      }    
    }
  }

  for (int i = 0; i < N; ++i)
    if (tx.writeSet[i])
    {
      memory.values[i] = newValue(tx.writenValues[i], tx.clock);
    }
  memory.clock++;
  pthread_mutex_unlock(&memory.lock);
  return 0;
}

void* f(void* arg) {
  for(int i=0; i<LOOPS; i++) {
    if(version == STM) {
restart:
      startTX();

      int x = readValue(0);
      
      if(x == ABORT) {
        goto restart;
      }
    
      if(writeValue(0, x + 1) == ABORT) {
        goto restart;
      }

      if(commitTX() == ABORT) {
        goto restart;
      }
    } else if(version == LOCK) {
      pthread_mutex_lock(&memory.lock);
      memory.values[0]->value++;
      pthread_mutex_unlock(&memory.lock);
    } else {
      __sync_fetch_and_add(&memory.values[0]->value, 1);
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  if(argc != 2) {
    fprintf(stderr, "usage: %s [0: STM, 1: LOCK, 2: ATOMIC_ADD]\n", argv[0]);
    return 1;
  }

  version = atoi(argv[1]);
  
  pthread_t threads[THREADS];
  initMemory();
  
  struct timeval start;
  gettimeofday(&start, 0);
  
  for(int i=0; i<THREADS; i++)
    pthread_create(&threads[i], 0, f, 0);
  
  for(int i=0; i<THREADS; i++)
    pthread_join(threads[i], 0);
  
  struct timeval end;
  gettimeofday(&end, 0);
  
  if(memory.values[0]->value != THREADS*LOOPS) {
    printf("value = %d while we expect %d\n", memory.values[0]->value, THREADS*LOOPS);
    printf("    => error!!!\n");
  } else {
    printf("Elapsed time: %0.3f ms\n",
           1e3*end.tv_sec + end.tv_usec*1e-3 - start.tv_sec*1e3 - start.tv_usec*1e-3);
  }
  
  return 0;
}

/* gcc -Wall -Werror -o main main.c -lpthread */