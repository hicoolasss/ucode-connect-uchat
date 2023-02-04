#include <gtk/gtk.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include "../../libs/libmx/inc/libmx.h"

#define MAX_CLIENTS 100
#define BUFFER_SZ 2048
#define LENGTH 2048


//gtk windows structure
typedef struct t_screen{
    GtkWidget *screen;
    GtkCssProvider *provider;
} t_screen;

t_screen start_screen;
t_screen login_screen;

void show_loginscreen();
void loadstyles();
