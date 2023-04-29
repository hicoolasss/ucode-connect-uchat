CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -pthread -gdwarf-4 -lsqlite3

OBJDIR = obj
SRCDIR = src
NAMEBIN = ../uchat_server

SRC_FILES = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
OBJS = $(OBJDIR)/*.o

LIBMX_DIR = ../libs/libmx
LIBMX_NAME = $(LIBMX_DIR)/libmx.a

CJSON_DIR = ../libs/cjson
CJSON_NAME = $(CJSON_DIR)/cjson.a

SSL_DIR = ../libs/openssl
SSL_FILES:= ../local_lib/lib
SSL_INC_FILES:= $(SSL_DIR)

LIB_FLAGS = -lmx -lssl -lcjson -lsqlite3 -lcrypto\
			-L$(LIBMX_DIR) $(LIBMX_NAME)\
			-L$(CJSON_DIR) $(CJSON_NAME)\

OS := $(shell uname)

GLIB_FLAGS := $(shell pkg-config --cflags --libs glib-2.0)

all: $(NAMEBIN) clean

$(NAMEBIN): 
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(GLIB_FLAGS) -c $(SRC_FILES)
	mv *.o $(OBJDIR)
	$(CC) $(CFLAGS) $(GLIB_FLAGS) -o $(NAMEBIN) $(OBJS) $(LIB_FLAGS) $(GLIB_FLAGS)

clean:
	rm -f $(OBJS)
	rm -df $(OBJDIR)

uninstall:
	make clean
	rm -f $(UCHAT)

reinstall:
	make uninstall
	make all