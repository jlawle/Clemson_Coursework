/*
 * BarberShop.h -- Barber shop for sleeping barber 
 * problem
 */
#ifndef _BARBERSHOP_H_
#define _BARBERSHOP_H_
#include "Lock.h"
#include "CV.h"

const int NCHAIRS = 5;
const int NO_CUST_CLOSING_TIME = -1;

class BarberShop{
 private:
  Lock lock;
  CV wakeBarber;
  CV nextCustomer;

  bool timeToClose;
  bool open;
  int arrivalCount;
  int cutCount;
  int fullCount;
  
 public:
  BarberShop();
  ~BarberShop() {};
  void barberDay(); // Main loop for barber thread
  bool getHairCut(); // Called by customer thread
  void clockRingsClosingTime(); // Called by clock thread

 private:
  void openStore();
  int waitForCustomer();
  void doneCutting();
  void printFinalStats();

  bool emptyAndOpen();
  bool stillNeedHaircut(int custId);
  bool waitingRoomFull();
};
#endif
