TARGET_LIB = libmx.a
CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic -gdwarf-4

MKDIR = mkdir -p
RM = rm -rf
AR = ar rcs

SRCS = *.c
OBJS = *.o
OBJS_D = obj

all: MINILIBMX clean

MINILIBMX:
	@$(MKDIR) $(OBJS_D)
	@$(CC) $(CFLAGS) -c $(addprefix src/, $(SRCS))
	@mv $(OBJS) $(OBJS_D)
	@$(AR) $(TARGET_LIB) $(addprefix obj/,$(OBJS))

clean:
	@$(RM) $(OBJS_D)

uninstall:
	@$(RM) $(OBJS_D)
	@$(RM) $(TARGET_LIB)

reinstall: uninstall all
