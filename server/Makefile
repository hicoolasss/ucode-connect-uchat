CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -pthread -gdwarf-4

OBJDIR = obj
SRCDIR = src
NAMEBIN = ../uchat_server

SRC_FILES = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
OBJS = $(OBJDIR)/*.o

LIBMX_DIR = ../libs/libmx
LIBMX_NAME = $(LIBMX_DIR)/libmx.a

CJSON_DIR = ../libs/cjson
CJSON_NAME = $(CJSON_DIR)/cjson.a

SSL_DIR = ../local-lib/include/openssl
SSL_FILES:= ../local-lib/lib
SSL_INC_FILES:= $(SSL_DIR)

LIB_FLAGS = -lmx -lssl -lcjson -lsqlite3 -lcrypto\
			-L$(LIBMX_DIR) $(LIBMX_NAME)\
			-L$(CJSON_DIR) $(CJSON_NAME)\
			-L$(SSL_FILES) -I$(SSL_INC_FILES)

all: $(NAMEBIN) clean

$(NAMEBIN): 
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(shell pkg-config --cflags --libs glib-2.0) -I$(SSL_DIR) -c $(SRC_FILES)
	mv *.o $(OBJDIR)
	$(CC) $(CFLAGS) -I$(SSL_INC_FILES) $(shell pkg-config --cflags --libs glib-2.0) -o $(NAMEBIN) $(OBJS) $(LIB_FLAGS) $(shell pkg-config --cflags --libs glib-2.0)

clean:
	rm -f $(OBJS)
	rm -df $(OBJDIR)

uninstall:
	make clean
	rm -f $(UCHAT)

reinstall:
	make uninstall
	make all