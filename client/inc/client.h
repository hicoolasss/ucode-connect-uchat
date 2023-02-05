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

// gtk windows structure
typedef struct
{
    GtkWidget *screen;
    GtkCssProvider *provider;
} t_screen;


// show login screen
void show_loginscreen();

// show registration screen
void show_registrationscreen();

void show_home_page_screen(void);

// load styles
void loadstyles();

// add style to widget
void widget_styling(GtkWidget *widget, t_screen screen, const char *name_of_css_class);
