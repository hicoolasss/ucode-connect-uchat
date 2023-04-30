CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -gdwarf-4

OBJDIR = obj
SRCDIR = src
NAMEBIN = ../uchat

SRC_FILES = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
OBJS = $(OBJDIR)/*.o

LIBMX_DIR = ../libs/libmx
LIBMX_NAME = $(LIBMX_DIR)/libmx.a

CJSON_DIR = ../libs/cjson
CJSON_NAME = $(CJSON_DIR)/cjson.a


LIB_FLAGS =	-L$(LIBMX_DIR) $(LIBMX_NAME) \
			-L$(CJSON_DIR) $(CJSON_NAME) \
			-I /opt/homebrew/Cellar/openssl@3/3.1.0/include \
		    -L /opt/homebrew/Cellar/openssl@3/3.1.0/lib \
			-lpthread -lcurl

INCLUDES = -I./
INCS =  $(INCLUDES) `pkg-config --cflags gtk4`  `pkg-config --cflags libevent` `pkg-config --cflags jansson` -I `brew --prefix openssl@3`/include
LIBS =  `pkg-config --libs gtk4` `pkg-config --libs libevent` `pkg-config --libs jansson`  -L `brew --prefix openssl@3`/lib -lsqlite3 $(LIB_FLAGS) -lssl -lcrypto

all: $(NAMEBIN) clean

$(NAMEBIN): 
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(shell pkg-config --cflags --libs glib-2.0) $(INCS) -c $(SRC_FILES)
	mv *.o $(OBJDIR)/
	$(CC) $(CFLAGS) $(shell pkg-config --cflags --libs glib-2.0) $(INCS) $(LIBS) -o $(NAMEBIN) $(OBJS) $(shell pkg-config --cflags --libs glib-2.0) -lcurl -lpthread

clean:
	rm -f $(OBJS)
	rm -df $(OBJDIR)

uninstall:
	make clean
	rm -f $(UCHAT)

reinstall:
	make uninstall
	make all