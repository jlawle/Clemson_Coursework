/*
 * BarberShop.cc -- Barber shop for sleeping barber 
 * problem
 */
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include "BarberShop.h"
#include "Lock.h"
#include "CV.h"
#include "sthread.h"

void BarberShop::barberDay()
{
  // BarberDay is not an atomic action.
  // No lock. Only touch object's state 
  // by calling methods that lock.
  int cust;
  printf("Opening for the day\n");
  openStore();
  while(1){
    cust = waitForCustomer();
    if(cust == NO_CUST_CLOSING_TIME){
      printf("Closing for the day\n");
      printFinalStats();
      return;
    }
    printf("Cut hair %d\n", cust);
    sthread_sleep(1, 0); // Time to cut
    doneCutting();
  }
}

void BarberShop::openStore()
{
  lock.acquire();
  open = true;
  lock.release();
  return;
}

int BarberShop::waitForCustomer()
{
  int custId;
  lock.acquire();
  while(emptyAndOpen()){
    wakeBarber.wait(&lock);
  }
  if(timeToClose){
    open = false; // Stop new arrivals
  }
  if(arrivalCount > cutCount){
    custId = cutCount;
  }
  else{
    custId = NO_CUST_CLOSING_TIME;
  }
  lock.release();
  return custId;
}

void BarberShop::doneCutting()
{
  lock.acquire();
  cutCount++; 
  nextCustomer.broadcast();
  lock.release();
  return;
}

void BarberShop::printFinalStats()
{
  lock.acquire();
  printf("Stats: arr=%d cut=%d full=%d\n",
         arrivalCount,cutCount,fullCount);
  assert(arrivalCount == cutCount); 
  lock.release();
}

bool BarberShop::getHairCut()
{
  int myNumber;
  bool ret;
  lock.acquire();
  if(!open || waitingRoomFull()){
    ret = false;
  }
  else{
    // "Take a number" for FIFO service
    myNumber = ++arrivalCount;
    wakeBarber.signal();
    while(stillNeedHaircut(myNumber)){
      nextCustomer.wait(&lock);
    }
    ret = true;
  }
  lock.release();
  return ret;
}

void BarberShop::clockRingsClosingTime()
{
  lock.acquire();
  timeToClose = true;
  wakeBarber.signal();
  lock.release();
}

// Internal functions for checking status.
//  Always called with lock already held.
bool BarberShop::emptyAndOpen()
{
  if((timeToClose 
      || (arrivalCount > cutCount))){
    return false;
  }
  else{
    return true;
  }
}

bool BarberShop::stillNeedHaircut(int custId)
{
  // Ensure FIFO order by letting customers
  // leave in order they arrive
  if(custId > cutCount){
    return true;
  }
  else{
    return false;
  }
}

bool BarberShop::waitingRoomFull()
{
  // +1 b/c barber chair
  if(arrivalCount-cutCount == NCHAIRS+1){
    return true;
  }
  else{
    return false;
  }
}

// Routine constructor
BarberShop::BarberShop()
{
  open = false;
  timeToClose = false;
  arrivalCount = 0;
  cutCount = 0;
  fullCount = 0;
}

