#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "memory.h"
#include "todo.h"

int todoCtr = 0;

void printTodo(Todo* todo) {
    printf("todo message %d: %s\n", todo->id, todo->message);
}

void printAllTodos(TodoList* todoList) {
  for (int i = 0; i < todoList->numTodos; i++) {
    printTodo(todoList->todos[i]);
  }
}

void freeAll(TodoList* todoList) {
  for (int i = 0; i < todoList->numTodos; i++) {
    trackFree(todoList->todos[i]->message);
    trackFree(todoList->todos[i]);
  }
  trackFree(todoList);
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

void addTodo(TodoList* todoList, char* input) {
    Todo* todo = (Todo*) trackMalloc(sizeof(Todo), "todo");
    todo->id = todoCtr++;
    char* message = trackMalloc(strlen(input) + 1, "message"); 
    strcpy(message, input);
    todo->message = message;
    todoList->todos[todoList->numTodos] = todo;
    todoList->numTodos++;
}
