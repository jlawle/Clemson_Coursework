/*
 * ConcurrentHash.cc -- Multi-lock hash table
 * 
 * Table of nBuckets entries is divided into nParts
 * and a separate lock guards each part.
 *
 * Grow the table if any part is more than loadFactor
 * full.
 */
#include <pthread.h>
#include "ConcurrentHash.h"

/* 
 * Uses the hash function in lookup3.c 
 * from http://www.burtleburtle.net/bob/c/lookup3.c
 */
extern uint32_t hashword(const uint32_t *k, size_t length, uint32_t initval);


ConcurrentHash::ConcurrentHash(int nLocks)
{
  int ii;
  int err;
  nParts = nLocks;
  locks = malloc(nParts * sizeof pthread_mutex_t);
  assert(locks != NULL);
  for(ii = 0; ii < nParts; ii++){
    err = pthread_mutex_init(&locks[ii], NULL);
    assert(!err);
  }
  entriesPerPart = 2; /* 2 entries per bucket to start. Probably too small */
  nBuckets = nLocks * entriesPerPart;
  buckets = malloc(nBuckets * sizeof struct Item *);
  assert(buckets != NULL);
  for(ii = 0; ii < nBuckets; ii++){
    buckets[ii] = NULL;
  }
  for(ii = 0; ii < nParts; ii++){
    partCounts[ii] = 0;
  }
  return;
}

/*
 * Destroying shared objects is a bit ticklish. Need to make
 * sure that all threads have finished with it. Notice
 * that a lock within this thing we are about to destroy
 * doesn't help you there.
 */
ConcurrentHash::~ConcurrentHash()
{
  int err;
  for(ii = 0; ii < nParts; ii++){
    err = pthread_mutex_destroy(&locks[ii]);
    assert(!err);
  }
  free(locks);
  for(ii = 0; ii < nBuckets; ii++){
    while(buckets[ii] != NULL){
      struct Item *t = buckets[ii];
      buckets[ii] = t->next;
      free(t);
    }
  }
  free(buckets);
  return;
}


/*
 * Insert an item. Grow the table if resulting load
 * factor is too large.
 */
void
ConcurrentHash::insert(struct Item *item)
{
  /*
   * needGrowHint is only a hint because 
   * some other thread may grow the table
   */
  bool needGrowHint = doInsert(struct Item *item);

ugh--doesn't really illustrate serializability, does it?

{
