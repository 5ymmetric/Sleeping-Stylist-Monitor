/*
 * Author: Karthik Reddy Pagilla
 * CSE username: kpagilla
 * Description: Sleeping Barber Problem 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define DELAY 10000
#define NUMBER_OF_STYLISTS 1
#define NUMBER_OF_CUSTOMERS 120

int customersServed = 0;

/*
 * Function definitions
 */
void customerFunction(void);
void stylistFunction(void);
extern void mon_init();
extern void mon_checkCustomer();
extern int mon_checkStylist();
extern void mon_debugPrint(int);

/*
 * Stylist and customer threads are created
 * And joined here
 */
void main(void) {

  mon_init();

  int i;
  pthread_t stylistThreads[NUMBER_OF_STYLISTS];
  pthread_t customerThreads[NUMBER_OF_CUSTOMERS];

  for(i = 0; i < NUMBER_OF_STYLISTS; i++){
    pthread_create(&stylistThreads[i], NULL, (void *) stylistFunction, NULL);
  }


  for(i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
    pthread_create(&customerThreads[i], NULL, (void *) customerFunction, NULL);
  }

  for(i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
    pthread_join(customerThreads[i], NULL);
  }

  for(i = 0; i < NUMBER_OF_STYLISTS; i++) {
    pthread_join(stylistThreads[i], NULL);
  }

}

/*
 * This function is the start_routine for the stylist thread
 * and this function serves the customer when the stylist is available
 */
void stylistFunction(void) {
  int j;
  while(1) {
    mon_debugPrint(customersServed);
    mon_checkCustomer();
    for(j = 0; j < DELAY; j++); // cut hair
    customersServed++;

    if(customersServed == NUMBER_OF_CUSTOMERS) {
      mon_debugPrint(customersServed);
      printf("\n%d Customers got served! \n", NUMBER_OF_CUSTOMERS);
      break;
    }
  }
}

/*
 * This fucntion is the start_routine for the customer threads
 * and this function either adds the customer to the waiting queue or
 * sends them for shopping
 */
void customerFunction(void) {
  int j;
  while(1) {
    mon_debugPrint(customersServed);
    if(mon_checkStylist()){
      break;
    }
    for(j = 0; j < DELAY; j++); // go shopping
  }
}
