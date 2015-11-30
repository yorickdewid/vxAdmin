SRCDIR = src
BINDIR = bin
NAME = vxadmin
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)
CFLAGS = -g -O0 -pedantic-errors -std=c11 -Wall -Werror -Wextra -Winit-self -Wswitch-default -Wshadow
LDFLAGS = -lm

all: $(NAME)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(BINDIR)/$@

clean:
	$(RM) -rf $(SRCDIR)/*.o
	$(RM) $(BINDIR)/$(NAME)

cleandist: clean