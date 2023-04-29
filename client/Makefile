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

SSL_DIR:= ../libs/openssl
SSL_FILES = ../libs/openssl/openssl
SSL_INC_FILES:= $(SSL_DIR)
LCRYPTO = $(SSL_DIR)/libcrypto.a
LIBSSL = $(SSL_DIR)/libssl.a

LIB_FLAGS =	-L$(LIBMX_DIR) $(LIBMX_NAME) \
			-L$(CJSON_DIR) $(CJSON_NAME) \
			-L$(SSL_FILES) $(LCRYPTO) $(LIBSSL) -lcrypto -lssl

all:  $(NAMEBIN) clean
$(NAMEBIN): 
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) `pkg-config --cflags gtk4` -c $(SRC_FILES) -I $(SSL_DIR)
	mv *.o $(OBJDIR)
	$(CC) $(CFLAGS) `pkg-config --cflags gtk4` -o $(NAMEBIN) $(OBJS) $(LIB_FLAGS) -lpthread -lcurl `pkg-config --libs gtk4`

clean:
	rm -f $(OBJS)
	rm -df $(OBJDIR)

uninstall:
	make clean
	rm -f $(UCHAT)

reinstall:
	make uninstall
	make all