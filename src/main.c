#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "memory.h"
#include "todo.h"

extern MemEntry* memEntry;

int main() {
  memEntry = malloc(sizeof(MemEntry)); 
  memEntry->head = NULL;
  memEntry->basePtr = malloc(8);

  TodoList* todoList = (TodoList*) trackMalloc(sizeof(TodoList), "todolist");
  todoList->numTodos = 0;
  addTodo(todoList, "First");
  addTodo(todoList, "Second");
  addTodo(todoList, "Third");
  printAllTodos(todoList);
  print_allocations();
  todoList = deleteTodo(todoList, 1);
  printAllTodos(todoList);
  print_allocations();
  
  freeAll(todoList);
  return 0;
}

