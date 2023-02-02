SERVER = uchat_server
CLIENT = uchat

SERVER_DIR = server
CLIENT_DIR = client
INC_DIR = inc

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC_FILES:%.c=%.o)))
INC_FILES = $(wildcard $(INC_DIR)/*.h)
LIB_FILES = $(wildcard $(LIB_DIR)/$(LIB_DIR).a)
LIB_INC_FILES = $(wildcard $(LIB_DIR)/$(INC_DIR))

CC = clang
CFLAGS = -std=c11 $(addprefix -W, all extra error pedantic) -lncurses -g3 -fsanitize=address -pthread

RM = rm -rf

all: $(NAMEBIN) MINILIBMX clean

$(NAMEBIN): $(OBJ_FILES)
	@$(MAKE) $(LIB_DIR)
	@$(CC) $(CFLAGS) $^ -L$(LIB_DIR) -lmx -o $@

$(OBJ_FILES): | $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_FILES)
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR) -I $(LIB_INC_FILES)

$(OBJ_DIR):
	@$(MKDIR) $@

MINILIBMX:
	@$(CC) $(CFLAGS) `pkg-config --cflags gtk4` $(SERVER_DIR)/server.c `pkg-config --libs gtk4` -I $(INC_DIR) -o $(SERVER)
	@$(CC) $(CFLAGS) `pkg-config --cflags gtk4` $(CLIENT_DIR)/client.c `pkg-config --libs gtk4` -I $(INC_DIR) -o $(CLIENT)

clean:
	@$(RM) $(OBJ_DIR)

uninstall: clean
	@$(RM) $(NAME)

reinstall: uninstall all
