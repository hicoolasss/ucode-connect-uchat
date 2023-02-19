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
    GtkWidget *chats_container;
    //auth grids
    GtkWidget *auth_grid;
    GtkWidget *log_in_conrainer;
    GtkWidget *registration_container;

    //container to grout searchbar ghats ang groups
    GtkWidget *three_rows_container;
    //all grid boxes
    GtkWidget *left_menu_bar;
    GtkWidget *your_profile;
    GtkWidget *home;
    GtkWidget *search_bar;
    GtkWidget *mini_groups;

    GtkWidget *mini_chats;
    //chats list
    GtkWidget *chats_list_grid_child;
    //count num of chats in chats list
    int chat_pos_count;

    GtkWidget *chats;
    GtkWidget *settings;
    GtkWidget *achievements;
} t_grid;


// load styles
void loadstyles(void);
// add style to widget
void widget_styling(GtkWidget *widget, t_screen screen, const char *name_of_css_class);

// show login screen
void show_auth(void);
void show_login(void);
// show registration screen
void show_registration(void);

void show_home(void);


//unvisible grids
void set_unvisible_all(void);
void set_unvisible_auth(void);

void show_left_menu_bar(void);
void show_your_profile(void);
void show_home_grid(void);
void show_chats(void);
void show_achievements(void);
void show_settings(void);
void show_log_out(void);

void show_mini_groups(void);
void show_mini_chats(void);
void show_search_bar(void);

//help func to create grid with needed params
GtkWidget *create_grid( const gint width,
                        const gint height,
                        const gchar *const style );

