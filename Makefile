CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -std=c11 -g -I./src
SRC = $(wildcard src/*.c)
OBJDIR = build
OBJ = $(SRC:src/%.c=$(OBJDIR)/%.o)
TARGET = bin/zasm

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@

# Regola per compilare ciascun .c in .o nella directory build/
$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)