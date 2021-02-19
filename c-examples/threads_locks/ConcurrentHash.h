/*
 * ConcurrentHash.h -- Multi-lock hash table
 */
#ifndef _CONCURRENTHASH_H_
#define _CONCURRENTHASH_H_
#include <pthread.h>

struct Item{
  int key;
  void *value;
  struct Item *next;
}

static const float loadFactor = 0.75;

class ConcurrentHash{
 private: 
  // Synchronization variables
  pthread_mutex_t locks[];
 
  // State variables 
  int nParts;
  int nBuckets;
  int entriesPerPart;
  struct Item *buckets[];
  int partCounts[];

 public:
  ConcurrentHash(int nLocks);
  ~ConcurrentHash();
  void insert(struct Item *item);
  struct Item *remove(int key);
};
#endif
