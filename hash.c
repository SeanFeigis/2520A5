#include "hash.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define POINTERSIZE 8
//#define DEBUG

struct Performance *newPerformance() {
  struct Performance *perf;
  if ((perf = malloc(sizeof(struct Performance))) == NULL) {
    fprintf(stderr, "%s\n", "Error, malloc failed");
    exit(1);
  }
  perf->reads=0;
  perf->writes=0;
  perf->mallocs=0;
  perf->frees=0;
  return perf;

}

struct HashTable *createTable( struct Performance *performance, unsigned int capacity, int (*hash)(void *, int), int (*compar)(const void *, const void*)) {
  struct HashTable *tempHash = malloc(sizeof(struct HashTable));
  if (!tempHash) {
    fprintf(stderr, "%s\n", "Error, malloc failed");
    exit(1);
  }
  tempHash->capacity = capacity;
  tempHash->hash = hash;
  tempHash->compar = compar;
  tempHash->nel = 0;
  tempHash->data = malloc((capacity) * POINTERSIZE);
  if (!(tempHash->data)) {
    fprintf(stderr, "%s\n", "Error, malloc failed");
    exit(1);
  }
  //printf("%s\n", "before nulling" );
  for (int i = 0; i < capacity; i++) {
    tempHash->data[i] = NULL;
  }
  //printf("%s\n", "after nulling" );
  performance->reads+=capacity;
  performance->mallocs++;
  return(tempHash);
}

void addElement( struct Performance *performance, struct HashTable *table, void *src) {
  if (table->nel == table->capacity) {
    fprintf(stderr, "%s\n", "Error, nel == capacity");
    exit(1);
  }
  int index = 0;

  index = table->hash(src, table->capacity);

    while (1) {

      if (!table->data[index]) {
        table->data[index] = src;
        break;
      }
      performance->reads++;
      index++;
      if (index == table->capacity) {
        index = 0;
      }
    }

    table->nel++;
    performance->writes++;

}


int getIdx( struct Performance *performance, struct HashTable *table, void *src) {

  int index = 0;

  index = table->hash(src, table->capacity);
  int og_index = index;
  while (1) {
  //  printf("%s: %d\n", "loop", index);
    if (table->data[index]) {
      if (!(table->compar(table->data[index], src))) {
          return(index);
      }
    }
    performance->reads++;
    index++;
    if (index == table->capacity) {
      index = 0;
      //printf("%s\n", "zeroing index " );
    }
    if (index == og_index) {
      return(-1);
    //  printf("%s\n", "not found" );
    }
  }

  return(0);

}


void freeTable( struct Performance *performance, struct HashTable *table) {
    free(table->data);
    free(table);
    performance->frees++;
}

//DERIVED functions

void *getElement( struct Performance *performance, struct HashTable *table, void *src) {
  int idx = getIdx(performance, table, src);
//  printf("%s\n", "after index");
  if (idx == -1) {
    return (NULL);
  }
  return (table->data[idx]);
}

void removeElement( struct Performance *performance, struct HashTable *table, void *target) {
  int idx = getIdx(performance, table, target);
  table->data[idx] = NULL;
  performance->writes++;
  table->nel--;
}


int hashAccuracy( struct HashTable *table) {
  int total = 0;


}
