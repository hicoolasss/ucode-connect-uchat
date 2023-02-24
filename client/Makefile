CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -pthread -gdwarf-4

OBJDIR = obj
SRCDIR = src
NAMEBIN = ../uchat

SRC_FILES = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
OBJS = $(OBJDIR)/*.o

LIBMX_DIR = ../libs/libmx
LIBMX_NAME = $(LIBMX_DIR)/libmx.a
LIB_FLAGS = -lmx -lssl -lcrypto -L$(LIBMX_DIR) $(LIBMX_NAME)

all:  $(NAMEBIN) clean

$(NAMEBIN): 
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) `pkg-config --cflags gtk4` -c $(SRC_FILES)
	mv *.o $(OBJDIR)
	$(CC) $(CFLAGS) `pkg-config --cflags gtk4` -o $(NAMEBIN) $(OBJS) $(LIB_FLAGS) `pkg-config --libs gtk4`

clean:
	rm -f $(OBJS)
	rm -df $(OBJDIR)

uninstall:
	make clean
	rm -f $(UCHAT)

reinstall:
	make uninstall
	make all
