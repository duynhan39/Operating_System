#include "buffer.h"

#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

using namespace std;

void *producer(void *param);
void *consumer(void *param);

pthread_mutex_t mutex;

sem_t empty;
sem_t full;

int insert_item(buffer_item item);
int remove_item(buffer_item &item);

int numOfProducers;
int numOfConsumers;

/* the buffer */
buffer_item buffer[BUFFER_SIZE];

int main(int argc, char *argv[]) {
/* 1. Get command line arguments argv[1],argv[2],argv[3] */

  if(argc!=4)
  {
    cout << "Usage: " <<argv[0]<<"<Wait time (in seconds)><Number of Producer Threads><Number of Consumer Threads>" <<endl;
    exit(1);
  }
  int sleepTime = atoi(argv[1]);
  numOfProducers = atoi(argv[2]);
  numOfConsumers = atoi(argv[3]);

//  cout<<"CONS :"<<numOfConsumers<<endl;
//  cout<<"Prod :"<<numOfProducers<<endl;

/* 2. Initialize */
  sem_init(&empty, 0, BUFFER_SIZE);
  sem_init(&full, 0, 0);


/* 3. Create producer thread(s) */
  pthread_mutex_init(&mutex, NULL);

  pthread_t prodId;
  pthread_attr_t pAttr;

  pthread_attr_init(&pAttr);

  for(int i=0; i<numOfProducers; i++) {
    pthread_create(&prodId, &pAttr, producer, &i);
  }

/* 4. Create consumer thread(s) */
  pthread_t consId;
  pthread_attr_t cAttr;

  pthread_attr_init(&cAttr);

  for(int i=0; i<numOfConsumers; i++)
    pthread_create(&consId, &cAttr, consumer, &i);

/* 5. Sleep */
  sleep(sleepTime);

/* 6. Exit */
  return 0;
}

int insert_item(buffer_item item) {
  int f;
  sem_getvalue(&full, &f);
  if(f >= BUFFER_SIZE or f < 0)
    return -1;
  buffer[f] = item;
  return 0;
}

int remove_item(buffer_item &item) {
  int f;
  sem_getvalue(&full, &f);
  if(f >= BUFFER_SIZE or f < 0)
    return -1;

  item = buffer[f];
  return 0;
}



void *producer(void *param) {
  int *num = (int*) param;
  int n = *num;

  buffer_item item;
  while (true) {
    
    //cout<<"prod num 1 :"<<*num<<endl;
    //cout<<"prod num 2 :"<<n<<endl;
    

    srand (time(NULL));
    //usleep((rand() % 10) * 400000);
    sleep(rand()%5);
    int k;
    sem_getvalue(&empty, &k);
    //cout<<"EMPTY: "<<k<<endl;

    sem_wait(&empty);
    cout<<"Producer - wait on empty"<<endl;
    pthread_mutex_lock(&mutex); 
    cout<<"Producer - wait on mutex lock"<<endl;
    
    srand (time(NULL));

    item = (rand() * n) % 10000;
    if (insert_item(item)) {
      cout<<"Producer "<<n<<" report error condition"<<endl;
      sem_post(&empty);
    }
    else {
      sem_post(&full);
      cout<<"Producer "<<n<<" produced "<<item<<endl;
    }
    pthread_mutex_unlock(&mutex);
 }

  pthread_exit(0); 
}


void *consumer(void *param) {
  int *num = (int*) param;
  int n = *num; 
  buffer_item item;
  while (true) {
    
//    cout<<"cons ITER :"<<iter<<endl;
    //iter--;

    /* sleep for a random period of time */
    srand (time(NULL));
    //usleep((rand() % 10) * 400000);
    sleep(rand()%5);

    sem_wait(&full);
    cout<<"Consumer - wait on full"<<endl;
    pthread_mutex_lock(&mutex); 
    cout<<"Consumer - wait on mutex lock"<<endl;

    //int k;
    //sem_getvalue(&empty, &k);
    //cout<<"EMPTY: "<<k<<endl;

    if (remove_item(item)) {
      cout<<"consumer "<<n<<" report error condition"<<endl;
      sem_post(&full);
    }
    else {
      sem_post(&empty);
      cout<<"\e[1mConsumer\e[0m "<<n<<" consumed "<<item<<endl;
    }

    
    //sem_getvalue(&empty, &k);
    //cout<<"EMPTY: "<<k<<endl;

    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(0);
}
