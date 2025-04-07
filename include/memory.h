
#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// Structs
typedef struct MemEntry {
  void* ptr;
  size_t size;
  struct MemEntry* next;
  char type[12];
  ptrdiff_t dist;
  struct MemEntry* head;
  int id;
  void* basePtr;
} MemEntry;

// Methods
void* trackMalloc(size_t size, char* type);
void trackFree(void* ptr);
void print_allocations();

#endif
