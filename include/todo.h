#ifndef TODO_H
#define TODO_H

#define MAX_TODOS 20

// Structs
typedef struct {
  int id;
  char* message;
} Todo;

typedef struct {
  int numTodos;
  Todo* todos[MAX_TODOS];
} TodoList;

// Methods
void addTodo(TodoList* todoList, char* input);
TodoList* deleteTodo(TodoList* todoList, int todoId);
void printAllTodos(TodoList* todoList); 
void freeAll(TodoList* todoList);

#endif
