OBJDIR = obj
SERVER_DIR = server
CLIENT_DIR = client
SERVER = uchat_server
CLIENT = uchat

LIBMX_DIR = libs/libmx
LIBMX = $(LIBMX_DIR)/libmx.a

SRC = $(SRCDIR)/*.c
OBJS = $(OBJDIR)/*.o

all: $(LIBMX) $(SERVER) $(CLIENT) reinstall

$(LIBMX):
	make -sC $(LIBMX_DIR)

$(SERVER):
	make -sC $(SERVER_DIR)

$(CLIENT):
	make -sC $(CLIENT_DIR)

clean:
	@rm -rf $(SERVER)
	@rm -rf $(CLIENT)

uninstall:
	make -sC $(SERVER_DIR) 
	make -sC $(CLIENT_DIR) 
	make -sC $(LIBMX_DIR)
	@rm -rf $(OBJS)
	@rm -rf $(OBJDIR)

reinstall: clean uninstall