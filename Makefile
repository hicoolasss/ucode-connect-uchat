OBJDIR = obj
SERVER_DIR = server
CLIENT_DIR = client
SERVER = uchat_server
CLIENT = uchat

LIBMX_DIR = libs/libmx
LIBMX = $(LIBMX_DIR)/libmx.a

CJSON_DIR = libs/cjson
CJSON = $(CJSON_DIR)/cjson.a

SQLITE_DIR = libs/SQLite3
SQLITE = $(SQLITE_DIR)/sql.a

SRC = $(SRCDIR)/*.c
OBJS = $(OBJDIR)/*.o

OS := $(shell uname)

all: $(LIBMX) $(CJSON) $(SQLITE) $(SERVER) $(CLIENT) reinstall

install:
ifeq ($(OS), Darwin)
	brew update
	brew install gtk4
	brew install libevent
	brew install openssl@3
	brew install jansson
	brew install sqlite3
	brew install glib
	brew install pkg-config
else ifeq ($(OS), Linux)
	sudo apt update
	sudo apt install libsqlite3-dev
	sudo apt install libcurl4-openssl-dev
	sudo apt install libgtk-4-dev
	sudo apt install libevent-dev
	sudo apt install libssl-dev
	sudo apt install libjansson-dev
	sudo apt install libglib2.0-dev
	sudo apt-get install pkg-config
	sudo apt-get install libc6-dev
endif

$(LIBMX): 
	$(info $@ compiling...)
	@make -sC $(LIBMX_DIR)

$(CJSON):
	$(info $@ compiling...)
	@make -sC $(CJSON_DIR)

$(SQLITE_DIR)/sql.a:
	$(info $@ compiling...)
	@make -C $(SQLITE_DIR)

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
	@make -sC $(CJSON_DIR)

reinstall: clean uninstall