# Compiler
cc = gcc

# Compiler flags
CFLAGS = -Iinclude

# Source files
SRCS = src/main.c src/memory.c src/todo.c

# Object files
OBJS = $(SRCS:.c=.o)

# Output executable
TARGET = ./build/todo

# Default rule
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compile source files into object files
%.o: $.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)
