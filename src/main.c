#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define MAX_MESSAGE_LENGTH 100 
#define MAX_TODOS 20

typedef struct MemEntry {
  void* ptr;
  size_t size;
  struct MemEntry* next;
  char type[12];
  ptrdiff_t dist;
  int id;
} MemEntry;

MemEntry *head = NULL;
void* basePtr;
int memCtr = 0;

typedef struct {
  int id;
  char* message;
} Todo;

typedef struct {
  int numTodos;
  Todo* todos[MAX_TODOS];
} TodoList;

// Custom malloc for logging
void* trackMalloc(size_t size, char* type) {
  void *ptr = malloc(size);
  if (!ptr) return NULL;

  MemEntry *entry = malloc(sizeof(MemEntry));
  entry->ptr = ptr;
  entry->size = size;
  entry->next = head;
  
  ptrdiff_t diff = ptr - basePtr;
  entry->dist = diff; 
  entry->id = memCtr++; 
  strcpy(entry->type, type);
  head = entry;

  return ptr;
}

// Custom free for logging
void trackFree(void* ptr) {
  if (!ptr) return;

  MemEntry *curr = head, *prev = NULL;
  while (curr) {
    if (curr->ptr == ptr) {
      if (prev) {
        prev->next = curr->next;
      } else {
        head = curr->next;
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
  MemEntry* curr = head;
  printf("Allocated Memory Blocks:\n");
  void* lastAddress = NULL;
  while (curr) {
    ptrdiff_t diff = 0; 
    if (lastAddress) {
      diff = -1 * ((char*)curr->ptr - (char*)lastAddress);
    }
    printf(" %-2d:  Type: %-10s | Dist: %-7td | Malloc'd: %-8zu bytes | Diff: %-5td\n", curr->id, curr->type, curr->dist, curr->size, diff);
    lastAddress = curr->ptr;
    curr = curr->next;
  }
}

void printTodo(Todo* todo) {
    printf("todo message %d: %s\n", todo->id, todo->message);
}

void printAllTodos(TodoList* todoList) {
  for (int i = 1; i < todoList->numTodos + 1; i++) {
    printTodo(todoList->todos[i]);
  }
}

char* createMessage(int i) {
  // int size = i * 10;
  int size = 64;
  char* message = trackMalloc(size, "message");
  memset(message, 'a', size);
  return message;
}

void freeAll(TodoList* todoList) {
  for (int i = 0; i < todoList->numTodos; i++) {
    trackFree(todoList->todos[i]->message);
    trackFree(todoList->todos[i]);
  }
  trackFree(todoList);
}

void addTodo(TodoList* todoList, int index) {
    Todo* todo = (Todo*) trackMalloc(sizeof(Todo), "todo");
    todo->id = index;
    char* message = createMessage(index);
    todo->message = message;
    todoList->todos[index] = todo;
    todoList->numTodos++;
}

// delete by re-creating todolist
// conceptually just like todos.filter((todo) => todo.id !== deleteId)
TodoList* deleteTodo(TodoList* todoList, int todoId) {
  TodoList* updatedTodoList = (TodoList*) trackMalloc(sizeof(TodoList), "todolist");
  updatedTodoList->numTodos = 0;
  for (int i = 0; i < todoList->numTodos; i++) {
    if (todoList->todos[i]->id == todoId) {
      trackFree(todoList->todos[i]->message);
      trackFree(todoList->todos[i]);
      continue;
    }
    updatedTodoList->todos[updatedTodoList->numTodos++] = todoList->todos[i];
  }
  trackFree(todoList);
  return updatedTodoList;
}

int main() {
  // reference pointer for monitoring memory
  basePtr = malloc(8);
  TodoList* todoList = (TodoList*) trackMalloc(sizeof(TodoList), "todolist");
  todoList->numTodos = 0;
  int count = 10;
  for (int i = 0; i < count; i++) {
    addTodo(todoList, i);
  }
  print_allocations();
  todoList = deleteTodo(todoList, 3);
  print_allocations();
  freeAll(todoList); 
  print_allocations();
  return 1;
}
