#include <stdio.h>
#include "BarberShop.h"
#include "sthread.h"

#define NCUST 10
#define SIMTIME 10

void *barberMain(void *bsPtr)
{
  BarberShop *bs = (BarberShop *)bsPtr;
  bs->barberDay();
  return NULL;
}

void *custMain(void *bsPtr)
{
  BarberShop *bs = (BarberShop *)bsPtr;
  while(1){
    bs->getHairCut();
    sthread_sleep(1, 0);
  }
  return NULL;
}

void *clockMain(void *bsPtr)
{
  BarberShop *bs = (BarberShop *)bsPtr;
  sthread_sleep(SIMTIME, 0);
  printf("CLOCK: Closing time\n");
  bs->clockRingsClosingTime();
  return NULL;
}


int main(int argc, char **argv)
{
  int ii;
  BarberShop *bs = new BarberShop();
  sthread_t barber;
  sthread_t customers[NCUST];
  sthread_t clock;

  sthread_create_p(&barber, barberMain, bs);
  sthread_create_p(&clock, clockMain, bs);
  for(ii = 0; ii < NCUST; ii++){
    sthread_create_p(&customers[ii], custMain, bs);
  }
  sthread_join(barber);
}
