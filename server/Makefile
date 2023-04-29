CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -gdwarf-4

OBJDIR = obj
SRCDIR = src
NAMEBIN = ../uchat_server

SRC_FILES = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
OBJS = $(OBJDIR)/*.o

LIBMX_DIR = ../libs/libmx
LIBMX_NAME = $(LIBMX_DIR)/libmx.a

CJSON_DIR = ../libs/cjson
CJSON_NAME = $(CJSON_DIR)/cjson.a

SQLITE_DIR = ../libs/SQLite3
SQLITE_NAME = $(SQLITE_DIR)/sql.a

SSL_DIR:= ../libs/openssl
SSL_FILES = ../libs/openssl/openssl
SSL_INC_FILES:= $(SSL_DIR)
LCRYPTO = $(SSL_DIR)/libcrypto.a
LIBSSL = $(SSL_DIR)/libssl.a

LIB_FLAGS =	-L$(LIBMX_DIR) $(LIBMX_NAME) \
			-L$(CJSON_DIR) $(CJSON_NAME) \
			-L$(SQLITE_DIR) $(SQLITE_NAME) \
			-L$(SSL_DIR) $(LCRYPTO) $(LIBSSL)

all: $(NAMEBIN) clean

$(NAMEBIN): 
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(shell pkg-config --cflags --libs glib-2.0) -I$(SSL_INC_FILES) -c $(SRC_FILES)
	mv *.o $(OBJDIR)/
	$(CC) $(CFLAGS) $(shell pkg-config --cflags --libs glib-2.0) -o $(NAMEBIN) $(OBJS) $(LIB_FLAGS) $(shell pkg-config --cflags --libs glib-2.0) -lcurl -lpthread

clean:
	rm -f $(OBJS)
	rm -df $(OBJDIR)

uninstall:
	make clean
	rm -f $(UCHAT)

reinstall:
	make uninstall
	make all