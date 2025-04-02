#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MESSAGE_LENGTH 129 

typedef struct {
  int id;
  char message[MAX_MESSAGE_LENGTH];
} Todo;

int main() {
  Todo* todo = (Todo*) malloc(sizeof(Todo));
  todo->id = 1;
  strcpy(todo->message, "Yo, Peanut!");
  printf("todo message: %s\n", todo->message);
  return 1;
}
