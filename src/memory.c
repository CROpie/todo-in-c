
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "memory.h"

MemEntry* memEntry = NULL;
int memCtr = 0;

// Custom malloc for logging
void* trackMalloc(size_t size, char* type) {
  void *ptr = malloc(size);
  if (!ptr) return NULL;

  MemEntry *entry = malloc(sizeof(MemEntry));
  entry->ptr = ptr;
  entry->size = size;
  entry->next = memEntry->head;
  
  ptrdiff_t diff = ptr - memEntry->basePtr;
  entry->dist = diff; 
  entry->id = memCtr++; 
  strcpy(entry->type, type);
  memEntry->head = entry;

  return ptr;
}

// Custom free for logging
void trackFree(void* ptr) {
  if (!ptr) return;

  MemEntry *curr = memEntry->head, *prev = NULL;
  while (curr) {
    if (curr->ptr == ptr) {
      if (prev) {
        prev->next = curr->next;
      } else {
        memEntry->head = curr->next;
      }
      free(curr);
      break;
    }
    prev = curr;
    curr = curr->next;
  }
  free(ptr);
}

void print_allocations() {
  MemEntry* curr = memEntry->head;
  printf("Allocated Memory Blocks:\n");
  void* lastAddress = NULL;
  while (curr) {
    ptrdiff_t diff = 0; 
    if (lastAddress) {
      diff = -1 * ((char*)curr->ptr - (char*)lastAddress);
    }
    printf(" %-2d:  Type: %-10s | Dist: %-7td | Malloc'd: %-8zu bytes | Diff: %-5td\n", curr->id, curr->type, curr->dist, curr->size, diff);
    // printf(" Type: %-10s | Dist: %-7td | Malloc'd: %-8zu bytes | Diff: %-5td\n", curr->type, curr->dist, curr->size, diff);
    lastAddress = curr->ptr;
    curr = curr->next;
  }
}
