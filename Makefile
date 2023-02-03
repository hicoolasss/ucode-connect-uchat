OBJDIR = obj
SERVER_DIR = server
CLIENT_DIR = client
SERVER = uchat_server
CLIENT = uchat

LIBMX_DIR = libs/libmx
LIBMX = $(LIBMX_DIR)/libmx.a

SRC = $(SRCDIR)/*.c
OBJS = $(OBJDIR)/*.o

all: $(LIBMX) $(SERVER) $(CLIENT) clean

$(LIBMX):
	make -sC $(LIBMX_DIR)

$(SERVER):
	make -sC $(SERVER_DIR)

$(CLIENT):
	make -sC $(CLIENT_DIR)

clean:
	@rm -f $(OBJS)
	@rm -df $(OBJDIR) 

uninstall:
	make -sC $(SERVER_DIR) $@
	make -sC $(CLIENT_DIR) $@
	make -sC $(LIBMX_DIR) $@
	make clean

reinstall:
	make uninstall
	make all