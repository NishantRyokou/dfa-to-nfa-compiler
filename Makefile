CC = gcc
CFLAGS = -Wall -std=c99
LEX = flex
BISON = bison

SRCDIR = src
OBJDIR = obj

SOURCES = $(SRCDIR)/dfa.c $(SRCDIR)/nfa.c $(SRCDIR)/converter.c $(SRCDIR)/main.c
GENERATED = $(OBJDIR)/lex.yy.c $(OBJDIR)/parser.tab.c
OBJECTS = $(OBJDIR)/dfa.o $(OBJDIR)/nfa.o $(OBJDIR)/converter.o $(OBJDIR)/lex.yy.o $(OBJDIR)/parser.tab.o $(OBJDIR)/main.o

TARGET = dfa_converter

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/lex.yy.c: $(SRCDIR)/lexer.l $(OBJDIR)/parser.tab.h $(OBJDIR)
	$(LEX) -o $@ $(SRCDIR)/lexer.l

$(OBJDIR)/parser.tab.c $(OBJDIR)/parser.tab.h: $(SRCDIR)/parser.y $(OBJDIR)
	$(BISON) -d -o $(OBJDIR)/parser.tab.c $(SRCDIR)/parser.y

$(OBJDIR)/lex.yy.o: $(OBJDIR)/lex.yy.c $(OBJDIR)/parser.tab.h
	$(CC) $(CFLAGS) -I$(SRCDIR) -I$(OBJDIR) -c -o $@ $<

$(OBJDIR)/parser.tab.o: $(OBJDIR)/parser.tab.c
	$(CC) $(CFLAGS) -I$(SRCDIR) -I$(OBJDIR) -c -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(OBJDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) -I$(OBJDIR) -c -o $@ $<

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^

clean:
	rm -rf $(OBJDIR) $(TARGET)

run: $(TARGET)
	./$(TARGET) < tests/test_input.dfa

.PHONY: all clean run
