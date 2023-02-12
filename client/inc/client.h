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

typedef struct {
    //containers for easier display
    GtkWidget *main_grid;
    GtkWidget *home_grid;
    GtkWidget *right_space_grid;

    //all grid boxes
    GtkWidget *profile_box;
    GtkWidget *home_left_box;
    GtkWidget *searchbar_box;
    GtkWidget *groups_box;
    GtkWidget *chats_box;
    GtkWidget *home_right_box;
    GtkWidget *direct_message_box;
    GtkWidget *achievements_box;
    GtkWidget *settings_box;
    GtkWidget *log_out_box;

} t_grid;

GtkWidget *create_hbox( const gint width,
                        const gint height,
                        const gchar *const style );
                        
GtkWidget *create_vbox( const gint width,
                        const gint height,
                        const gchar *const style );
                        
GtkWidget *create_grid( const gint width,
                        const gint height,
                        const gchar *const style );


// show login screen
void show_login(void);

// show registration screen
void show_registration(void);

// load styles
void loadstyles(void);

void show_home(void);

void show_log_out(void);

void show_settings(void);

void show_achievements(void);

// add style to widget
void widget_styling(GtkWidget *widget, t_screen screen, const char *name_of_css_class);
