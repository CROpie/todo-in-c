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
int todoId = 0;

typedef struct {
  int id;
  char* message;
} Todo;

typedef struct {
  int numTodos;
  Todo* todos[MAX_TODOS];
} TodoList;

typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

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
  for (int i = 0; i < todoList->numTodos; i++) {
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

void addTodoOld(TodoList* todoList, int index) {
    Todo* todo = (Todo*) trackMalloc(sizeof(Todo), "todo");
    todo->id = index;
    char* message = createMessage(index);
    todo->message = message;
    todoList->todos[index] = todo;
    todoList->numTodos++;
}

// delete by re-creating todolist
// conceptually just like todos.filter((todo) => todo.id !== deleteId)
TodoList* deleteTodoOld(TodoList* todoList, int todoId) {
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

void printCL() {
  printf("> ");
}

InputBuffer* new_input_buffer() {
  InputBuffer* IB = (InputBuffer*) trackMalloc(sizeof(InputBuffer), "buffer");
  IB->buffer = NULL;
  IB->buffer_length = 0;
  IB->input_length = 0;
  return IB;
}

void getUserInput(InputBuffer* IB) {
  ssize_t bytes_read = getline(&(IB->buffer), &(IB->buffer_length), stdin);
  IB->input_length = bytes_read - 1;
  IB->buffer[bytes_read - 1] = 0;
}

void addTodo(TodoList* todoList, char* input) {
    Todo* todo = (Todo*) trackMalloc(sizeof(Todo), "todo");
    todo->id = todoId++;
    char* message = trackMalloc(strlen(input + 4) + 1, "message"); 
    strcpy(message, input + 4);
    todo->message = message;
    todoList->todos[todoList->numTodos] = todo;
    todoList->numTodos++;
}

void deleteTodo(TodoList* todoList, char* input) {
  // extract id to delete
  char* message = trackMalloc(strlen(input + 7) + 1, "message"); 
  char* end;
  strcpy(message, input + 7);
  int id = strtol(message, &end, 10);
  printf("id: %d\n", id);
  if (*end != '\0') {
    printf("Invalid number\n");
  }
  TodoList* updatedTodoList = (TodoList*) trackMalloc(sizeof(TodoList), "todolist");
  updatedTodoList->numTodos = 0;
  for (int i = 0; i < todoList->numTodos; i++) {
    if (todoList->todos[i]->id == id) {
      trackFree(todoList->todos[i]->message);
      trackFree(todoList->todos[i]);
      continue;
    }
    updatedTodoList->todos[updatedTodoList->numTodos++] = todoList->todos[i];
  }

  printAllTodos(updatedTodoList);
  // trackFree(todoList);
  // Doesn't work: need to replace a pointer to this pointer
  // todoList = updatedTodoList;
}

void parseUserInput(TodoList* todoList, char* input) {
    if (strncmp(input, "add ", 4) == 0) {
      addTodo(todoList, input);
    }
    if (strncmp(input, "delete ", 7) == 0) {
      deleteTodo(todoList, input);
    }
}

int main() {
  basePtr = malloc(8);
  InputBuffer* IB = new_input_buffer();
  TodoList* todoList = (TodoList*) trackMalloc(sizeof(TodoList), "todolist");
  todoList->numTodos = 0;
  while (1) {
    printAllTodos(todoList);
    printCL();
    getUserInput(IB);
    if (strcmp(IB->buffer, "exit") == 0) {
      break;
    }
    parseUserInput(todoList, IB->buffer);
  }
  return 0;
}

