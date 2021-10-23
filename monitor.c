/*
* Author: Karthik Reddy Pagilla
* CSE username: kpagilla
* Description: Sleeping Barber Problem 2
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define CHAIRS 7

/*
* Defining the condition struct
*/
typedef struct {
  int numOfThreadsBlocked;
  sem_t suspendThread;
}cond;

sem_t mutex;
cond stylistAvailable, customerAvailable;

int customer = 0 ;
int stylist = 0 ;

int salonFull = 0;
int stylistSleeping = 0;

int customerQueue[CHAIRS];

/*
* This function initializes the semaphores and conditions
*/
void mon_init() {
  sem_init(&mutex,0,1);
  sem_init(&(customerAvailable.suspendThread), 0, 0);
  sem_init(&(stylistAvailable.suspendThread), 0, 0);
  stylistAvailable.numOfThreadsBlocked = 0;
  customerAvailable.numOfThreadsBlocked = 0;
}

/*
 * Implementing count function
 */
int count(cond *cv) {
  return cv->numOfThreadsBlocked;
}

/*
 * Implementing wait function
 */
void wait(cond *cv) {
  cv->numOfThreadsBlocked++;
  sem_post(&mutex);
  sem_wait(&(cv->suspendThread));
}

/*
 * Implementing signal function
 */
void signal(cond *cv) {
  if(count(cv) == 0) {
    sem_post( &mutex );
  }
  else {
    cv->numOfThreadsBlocked--;
    sem_post(&(cv->suspendThread));
  }
}

void mon_checkCustomer() {
  sem_wait(&mutex);

  stylist++;
  signal(&stylistAvailable);

  if(customer == 0) {
    wait(&customerAvailable);
  }

  customer--;

  sem_post(&mutex);
}

int mon_checkStylist() {
  sem_wait(&mutex);

  int status = 0;
  if(customer < CHAIRS) {
    customer++;
    signal(&customerAvailable);

    if(stylist == 0) {
      wait(&stylistAvailable);
    }

    stylist--;
    status = 1;
  }

  sem_post(&mutex);

  return status;
}

void mon_debugPrint(int customersServed) {

  sem_wait(&mutex);

  switch(customer){
    case 0: printf("\n|0|0|0|0|0|0|0| => %d\n",customer);
    stylistSleeping++;
    break;
    case 1:  printf("\n|1|0|0|0|0|0|0| => %d\n",customer);
    break;
    case 2:  printf("\n|1|1|0|0|0|0|0| => %d\n",customer);
    break;
    case 3:  printf("\n|1|1|1|0|0|0|0| => %d\n",customer);
    break;
    case 4:  printf("\n|1|1|1|1|0|0|0| => %d\n",customer);
    break;
    case 5:  printf("\n|1|1|1|1|1|0|0| => %d\n",customer);
    break;
    case 6:  printf("\n|1|1|1|1|1|1|0| => %d\n",customer);
    break;
    case 7:  printf("\n|1|1|1|1|1|1|1| => %d\n",customer);
    salonFull++;
    break;
    default: printf("\nNumber of customers not in range\n");
  }

  printf("Given Haircuts = %d\n", customersServed);
  printf("Salon full = %d times\n", salonFull);
  printf("Salon Empty = %d times\n", stylistSleeping);

  sem_post(&mutex);
}
