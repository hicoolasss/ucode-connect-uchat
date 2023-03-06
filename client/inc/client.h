#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
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
#include <openssl/err.h>
#include <openssl/ssl.h>
#include "../../libs/libmx/inc/libmx.h"
#include "../../libs/cjson/inc/cJSON.h"

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
    GtkWidget *log_in_conrainer;
    GtkWidget *registration_container;

    //intro grid
    GtkWidget *intro_grid;
    GtkWidget *first_intro_screen;
    GtkWidget *second_intro_screen;
    GtkWidget *third_intro_screen;
    bool is_log_in_clicked;

    //container to grout searchbar ghats ang groups
    GtkWidget *three_rows_container;
    //all grid boxes
    GtkWidget *left_menu_bar;
    GtkWidget *your_profile;
    GtkWidget *home;

    GtkWidget *search_bar;
    GtkWidget *entry_for_search;

    GtkWidget *mini_groups;

    GtkWidget *mini_chats;
    //chats list
    GtkWidget *chats_list_grid_child;
    //count num of chats in chats list
    int chat_pos_count;

    GtkWidget *chats;
    GtkWidget *settings;
    GtkWidget *achievements;
    GtkWidget *dialog_with_blur;
} t_grid;

typedef struct s_client {
    int serv_fd;

    SSL *ssl;// client ssl structure with coneection to server

    struct sockaddr_in adr;
    int cl_socket;

    int id;
    char *login;
    const char *password;

} t_client;

typedef struct s_main
{
    char *ip;
    int port;
    SSL_CTX *context;
    bool is_run;
    bool connected;
    bool loaded;

}   t_main;

extern t_client cur_client;
extern t_main main_client;
extern pthread_mutex_t cl_mutex;
extern _Atomic bool registered;

SSL_CTX* CTX_initialize_client();
int send_message_to_server(char *buffer);
char *convert_to_json(char *buffer);
void *recv_func();
char *registration();
int recv_all(SSL *sockfd, char *buf, int len);
int send_all(SSL *sockfd, char *buf, int len);

// load styles
void loadstyles(void);
// add style to widget
void widget_styling(GtkWidget *widget, t_screen screen, const char *name_of_css_class);
void change_scheme_to_any_color(char *bg_win_col,
                                char *bg_auth_main_box,
                                char *bit_text_col,
                                char *small_text_col,
                                char *auth_input_bg_col,
                                char *auth_input_border_col,
                                char *auth_input_border_col_hover,
                                char *big_log_in_button_bg,
                                char *big_log_in_button_shadow_hover,
                                char *big_log_in_button_shadow_active,
                                char *sign_btn_text_col,
                                char *sign_btn_text_col_hover,
                                char *all_containers_bg,
                                char *right_chat_container_bg,
                                char *all_containers_borders_bg,
                                char *left_menu_bar_icon_background,
                                char *left_menu_bar_icon_hover,
                                char *chat_and_group_label_text_color,
                                char *log_out_container_bg,
                                char *log_out_yes_border,
                                char *log_out_no_border);

// show login screen
void show_auth(void);
void show_login(void);
// show registration screen
void show_registration(void);

void log_in_btn_clicked(GtkWidget *widget, gpointer data);
//show intro screens
void first_intro_screen();
void second_intro_screen();
void third_intro_screen();
void set_first_intro_screen_visible(void);
void set_second_intro_screen_visible(void);
void set_third_intro_screen_visible(void);

void show_home(void);


//unvisible grids
void set_unvisible_all(void);
void set_unvisible_auth(void);
void set_unvisible_intro(void);

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

//function to create new chat and
//add it to mini chat grid.
//As a parametr get number of existed chats,
// so it can attach it to grit in a right way
void create_new_chat(const int i,
                     const char *new_username);

//help func to create grid with needed params
GtkWidget *create_grid( const gint width,
                        const gint height,
                        const char *style );

void call_new_chat_and_add_iter(const char *new_username);
void search_btn_clicked(void);

