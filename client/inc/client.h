#define min(a, b) ((a) < (b) ? (a) : (b))
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <gtk/gtk.h>
#include <gio/gio.h>
#include <ctype.h>
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
#include <pango/pangocairo.h>
#include <pango/pango.h>
#include <math.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <curl/curl.h>
#include <jansson.h>
#include "../../libs/libmx/inc/libmx.h"
#include "../../libs/cjson/inc/cJSON.h"

#define MAX_CLIENTS 100
#define BUFFER_SZ 2048
#define LENGTH 2048

// gtk windows structure
typedef struct {
    GtkWidget *screen;
    GtkCssProvider *provider;
} t_screen;

typedef struct s_log_in {

    bool is_username_correct;

    bool is_password_correct;
    
    GtkWidget *box;

    GtkWidget *welcome;

    GtkWidget *username;
    GtkWidget *username_error_label;

    GtkWidget *password;
    GtkWidget *password_error_label;

    GtkWidget *log_in_button;

    GtkWidget *dont_have_account;

    GtkWidget *sign_up_button_log_in;

} t_log_in;

typedef struct s_registration {

    GtkWidget *welcome;
    GtkWidget *box;

    GtkWidget *username;
    GtkWidget *username_error_label;

    GtkWidget *password;
    GtkWidget *password_error_label;

    GtkWidget *confirm_password;
    GtkWidget *confirm_password_error_label;

    GtkWidget *sign_up_btn;
    GtkWidget *have_account;
    GtkWidget *sign_in_btn;

} t_registration;

typedef struct s_home {

    GtkWidget *chat_gpt_box_for_label;
    GtkWidget *chat_gpt_label;
    GtkWidget *chat_gpt_desc_label;
    GtkWidget *chat_gpt_response;
    GtkWidget *chat_gpt_entry_box;
    GtkWidget *chat_gpt_send_btn;
    GtkWidget *chat_gpt_entry;

} t_home;


typedef struct {
    //containers for easier display
    GtkWidget *main_grid;
    GtkWidget *chats_container;
    //auth grids
    GtkWidget *log_in_conrainer;
    GtkWidget *registration_container;
    GtkWidget *registration_success_container;

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
    GtkWidget *search_btn;
    GtkWidget *entry_for_search;

    GtkWidget *mini_groups;

    GtkWidget *mini_chats;
    //chats list
    GtkWidget *chats_list_grid_child;
    GtkWidget *goups_list_grid_child;
    //count num of chats in chats list
    int chat_pos_count;
    int group_pos_count;
    bool chat_search;
    bool group_search;

    GtkWidget *chats;
    GtkWidget *user_list;
    GtkWidget *empty_chat;
    GtkWidget *chat_with_friend;

    GtkWidget *chats_scrolled;

    GtkWidget *create_new_chat_with_someone;


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
    char *avatarname;
} t_client;

typedef struct s_chat {
    
    int id;
    char *sender;
    char *message;
    char *timestamp;

} t_chat;

typedef struct s_user
{
    char *username;
    char *avatarname;
    char *lastmessage;
} t_user;

typedef struct s_Friend {
    char *username;
    char *avatarname;
    char *lastmessage;
    bool in_chat;
    
    t_list *chat_history;
} t_Friend;

typedef struct s_main
{
    char *ip;
    int port;
    SSL_CTX *context;
    bool is_run;
    bool connected;
    bool loaded;

}   t_main;

typedef struct s_avatar {
    
    int width;
    int height;
    GdkPixbuf *avatar;

} t_avatar;

typedef struct s_scaled_avatar {

    int width;
    int height;
    GdkPixbuf *scaled_avatar;

} t_scaled_avatar;

typedef struct s_your_profile_avatar {

    int width;
    int height;
    GdkPixbuf *your_profile_avatar;
    GdkPixbuf *avatar;

} t_your_profile_avatar;

typedef struct s_achievements {
    
    bool first_step;    
    bool explorer;
    bool fickle;
    bool milka; //xDDDD
    bool loving;
    //bool vicious; unusable
    bool smart;
    bool mission_impossible;

} t_achievements;

extern GAsyncQueue *message_queue;
extern volatile gboolean running;

extern pthread_mutex_t command_queue_mutex;
extern pthread_mutex_t mutex_send;
extern pthread_mutex_t mutex_recv;
extern _Atomic bool registered;
extern pthread_cond_t auth_cond;

extern t_client current_client;
extern t_main main_client;
extern t_list *friend_list;
extern t_list *user_list;
// extern pthread_mutex_t cl_mutex;
extern int in_chat;
extern GThread *send_thread;
extern GThread *receive_thread;
void load_custom_font(const char* font_path, GtkWidget* widget);

SSL_CTX* CTX_initialize_client();
void open_client_connection(char* server_IP, int port);
void close_connection(SSL *ssl);
int open_ssl_connection();
int send_message_to_server(char *buffer);
unsigned char *base64_decode(const char *input, int *out_length);
char *convert_to_json(char *buffer);
gpointer send_func();
gpointer recv_func();
int stable_sending(SSL *ssl, void *buf, int size);
int stable_recv(SSL *ssl, void *buf, int size);
void add_new_friend(t_list **friend_list, const char *username, const char *avatarname);
char *registration(int status);
int recv_all(SSL *sockfd, char *buf, int len);
int send_all(SSL *sockfd, char *buf, int len);
t_list *receive_list(SSL *ssl);
t_list *deserialize_name_list(const char *json_str);
t_list *deserialize_chathistory_list(const char *json_str);
t_list *process_json_object(cJSON *json_object);
cJSON *create_json_from_group_and_friends(const char *group_name, t_list *friends);
t_list *extract_group_and_friends_from_json(cJSON *json_object, char **group_name);
int update_user_avatar(t_list *list, const char *username, const char *avatarname);
void delete_message(t_list *friend_list, char *username, int message_id, char *message_text);
void update_message(t_list *friend_list, char *username, int old_message_id, char *new_message_text);
void clear_friend_list(t_list *list);
void clear_user_list(t_list *list);
// load styles
void loadstyles(void);
// add style to widget
void widget_styling(GtkWidget *widget, t_screen screen, const char *name_of_css_class);
void widget_restyling(GtkWidget *widget, t_screen screen, const char *name_of_removed_css_class, const char *name_of_css_class);
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
void show_log_in(void);
// show registration screen
void show_registration(void);
void show_success_registration(void);

// void log_in_btn_clicked(GtkWidget *widget, gpointer data);
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
void create_new_chat(GtkToggleButton *toggle_button, gpointer user_data);

void create_new_group(const int i,
                      const char *new_groupname);
//help func to create grid with needed params
GtkWidget *create_grid( const gint width,
                        const gint height,
                        const char *style );

void call_new_chat_and_add_iter();
void search_btn_clicked_chat(void);
void search_btn_clicked_group(void);

void call_new_group_and_add_iter(const gchar *const new_username);

void search_btn_clicked_chat(void);
void search_btn_clicked_group(void);

void get_scaled_image();

void show_create_new_chat_with_someone();
void show_empty_chat();

void show_chats_with_added_friends();

void show_chat_history(t_list *chat_history);
void receive_msg(char *msg);
void update_friend_list();

void update_friend_list();


void show_user_list_scrolled(t_list *current);
void show_chats_with_added_friends(t_list *friend_list);
void update_chat_history(gpointer friend_data);
gboolean send_receive_msg(gpointer user_data);

void add_message_to_chat_history(t_list **friend_list, const char *username, t_chat *new_chat);

void update_show_chats_with_added_friends(t_list *friend_list);

void show_friend_info(gpointer data);

void update_current_chat(t_chat *chat_data, char *friendname);

typedef struct {
    gpointer chat_history;
    gpointer username_copy;
} CallbackArgs;

typedef struct {
    char *friend_name;
    t_list *messages_list;
    GtkWidget *chat_grid;
} t_chat_info;

typedef struct {
    t_chat *chat_data;
    t_Friend *friend_data;
    GtkWidget *sent_box;
    GtkWidget *entry;
    char *friendname;
} SentMessageData;


typedef struct {
    t_Friend *friend_data;
    int id;
} handler_id;






