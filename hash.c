#include "hash.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define POINTERSIZE 8

struct Performance *newPerformance() { //Create newPerformance
  struct Performance *perf;
  if ((perf = malloc(sizeof(struct Performance))) == NULL) { //Null check
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
  if (!tempHash) { //Null check
    fprintf(stderr, "%s\n", "Error, malloc failed");
    exit(1);
  }
  tempHash->capacity = capacity; //Assign the data to the appropriate struct member
  tempHash->hash = hash;
  tempHash->compar = compar;
  tempHash->nel = 0;
  tempHash->data = malloc((capacity) * POINTERSIZE);
  if (!(tempHash->data)) { //Null check
    fprintf(stderr, "%s\n", "Error, malloc failed");
    exit(1);
  }
  //printf("%s\n", "before nulling" );
  for (int i = 0; i < capacity; i++) { //Null every member of the data array
    tempHash->data[i] = NULL;
  }
  //printf("%s\n", "after nulling" );
  performance->reads+=capacity;
  performance->mallocs++;
  return(tempHash);
}

void addElement( struct Performance *performance, struct HashTable *table, void *src) {
  if (table->nel == table->capacity) { //Full table check
    fprintf(stderr, "%s\n", "Error, nel == capacity");
    exit(1);
  }
  int index = 0;

  index = table->hash(src, table->capacity); //Get the index from the hash function

  while (1) {
    if (!table->data[index]) { //If the index is null, the data gets inserted
      table->data[index] = src;
      break;
    }
    performance->reads++;
    index++;
    if (index == table->capacity) { //Loop back around to 0 if it reaches capacity
      index = 0;
    }
  }

  table->nel++;
  performance->writes++;
}


int getIdx( struct Performance *performance, struct HashTable *table, void *src) {

  int index = 0;

  index = table->hash(src, table->capacity); //Get the index from the hash function
  int og_index = index;
  while (1) {
    //  printf("%s: %d\n", "loop", index);
    if (table->data[index]) { //If the index is null, and the data cmp is 0, return index
      if (!(table->compar(table->data[index], src))) {
        return(index);
      }
    }
    performance->reads++;
    index++;
    if (index == table->capacity) { //If index at capacity, loop around
      index = 0;
      //printf("%s\n", "zeroing index " );
    }
    if (index == og_index) { //If index meets original index, return -1
      return(-1);
      //  printf("%s\n", "not found" );
    }
  }

  return(0);

}


void freeTable( struct Performance *performance, struct HashTable *table) {
  free(table->data); //Free the data
  free(table); //Free the table
  performance->frees++;
}

//DERIVED functions

void *getElement( struct Performance *performance, struct HashTable *table, void *src) {
  int idx = getIdx(performance, table, src); //Call getIdx
  if (idx == -1) { //return null if it returns -1
    return (NULL);
  }
  return (table->data[idx]); //return the data based on the index
}

void removeElement( struct Performance *performance, struct HashTable *table, void *target) {
  int idx = getIdx(performance, table, target); //get the index of the data
  table->data[idx] = NULL; //Set it to null
  performance->writes++; //increment performance and nel accordingly
  table->nel--;
}

int hashAccuracy( struct HashTable *table) {
  int total = 0;
  int diff = 0;
  int capacity = table->capacity;
  for (int i = 0; i < table->capacity; i ++) { //Loop through the whole HashTable
    if (table->data[i]) { //if not null data
      diff = i - table->hash(table->data[i],capacity); //calculate the diff
      if (diff < 0) { //if the data looped around
        total += i + (table->hash(table->data[i], capacity) - capacity); //get the now total if it looped around
      } else {
        total += diff; //add diff if normal
      }
    }
  }
  return(total);
}

void rehash( struct HashTable *table) {
  int currHash = 0;
  for (int i = 0; i < table->capacity; i++) { //Loop through the whole table
    if (table->data[i]) { //if the data isnt empty
      currHash = table->hash(table->data[i], table->capacity); //get the ideal hash
      //printf("%s%d, %d\n", "Current index, hash of index ", i, currHash );
      if (currHash < i) { //case where ideal is smaller then current index
        for (int j = currHash; j < i; j++) {  //start the loop at the ideal location, move close to the current
          if (!table->data[j]) { //if the location is null
            table->data[j] = table->data[i]; //swap data
            table->data[i] = NULL;
            break;
          }
        }
      } else if (currHash > i) { //case where ideal is larger then current index
        for (int j = currHash; j > i; j--) { //start the loop at the ideal location, move closer to the current
          if (!table->data[j]) { //if the location is null
            table->data[j] = table->data[i]; //swap data
            table->data[i] = NULL;
            break;
          }
        }
      }
    }
  }
}
