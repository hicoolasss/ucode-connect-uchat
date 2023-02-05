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
	$(info $@ compiling...)
	@make -sC $(LIBMX_DIR)

$(SERVER): 
	$(info $@ compiling...)
	@make -sC $(SERVER_DIR)

$(CLIENT): 
	$(info $@ compiling...)
	@make -sC $(CLIENT_DIR)

clean:
	@rm -rf $(SERVER)
	@rm -rf $(CLIENT)
	@rm -rf $(OBJS)
	@rm -rf $(OBJDIR)

uninstall:
	@printf "\r\33[2K$(SERVER_DIR) \033[32;1mcompiling...\033[0m\n"
	@make -sC $(SERVER_DIR)
	@printf "\r\33[2K$(SERVER) \033[32;1mcreated\033[0m\n"
	@printf "\r\33[2K$(CLIENT_DIR) \033[32;1mcompiling...\033[0m\n"
	@make -sC $(CLIENT_DIR) 
	@printf "\r\33[2K$(CLIENT) \033[32;1mcreated\033[0m\n"
	@printf "\r\33[2K$(LIBMX_DIR) \033[32;1mcompiling...\033[0m\n"
	@make -sC $(LIBMX_DIR)
	@printf "\r\33[2K$(LIBMX) \033[32;1mcreated\033[0m\n"

reinstall: clean uninstall