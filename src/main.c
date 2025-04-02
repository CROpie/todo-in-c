#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MESSAGE_LENGTH 129 
#define MAX_TODOS 10

typedef struct {
  int id;
  char message[MAX_MESSAGE_LENGTH];
} Todo;

typedef struct {
  int numTodos;
  Todo* todos[MAX_TODOS];
} TodoList;

void printTodo(Todo* todo) {
    printf("todo message %d: %s\n", todo->id, todo->message);
}

void printAllTodos(TodoList* todoList) {
  for (int i = 1; i < todoList->numTodos + 1; i++) {
    printTodo(todoList->todos[i]);
  }
}

int main() {
  TodoList* todoList = (TodoList*) malloc(sizeof(TodoList));
  todoList->numTodos = 0;
  int count = 6;
  for (int i = 1; i < count; i++) {   
    Todo* todo = (Todo*) malloc(sizeof(Todo));
    todo->id = i;
    strcpy(todo->message, "Yo, Peanut!");
    todoList->todos[i] = todo;
    todoList->numTodos++;
  }
  printAllTodos(todoList);
  return 1;
}
