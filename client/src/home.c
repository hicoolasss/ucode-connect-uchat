#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;
t_home current_home;
extern t_avatar current_avatar;
extern t_scaled_avatar current_scaled_avatar;

int pos = 0;

struct MemoryStruct
{
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL)
    {
        /* Ошибка выделения памяти! */
        printf("Ошибка выделения памяти в WriteMemoryCallback!\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

char *get_chatgpt_response(const char *message, const char *api_key)
{
    // Инициализация библиотеки libcurl
    curl_global_init(CURL_GLOBAL_ALL);

    // Создание объекта CURL для отправки запроса на сервер
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        printf("Ошибка при инициализации объекта CURL!\n");
        return NULL;
    }

    // Создание заголовков для запроса
    struct curl_slist *headers = NULL;
    char authorization[128];
    snprintf(authorization, sizeof(authorization), "Authorization: Bearer %s", api_key);
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, authorization);

    // Создание объекта для хранения ответа от сервера
    struct MemoryStruct chunk;
    chunk.memory = malloc(1); // Начальное выделение памяти
    chunk.size = 0;

    // Создание строки с запросом
    char *post_fields = NULL;
    asprintf(&post_fields, "{ \
        \"model\": \"text-davinci-003\", \
        \"prompt\": \"%s\", \
        \"temperature\": 0.9, \
        \"max_tokens\": 150, \
        \"top_p\": 1, \
        \"frequency_penalty\": 0, \
        \"presence_penalty\": 0.6, \
        \"stop\": [\" Human:\", \" AI:\"] \
        }",
             message);

    // Настройка параметров запроса
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/completions");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    // Отправка запроса на сервер
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        printf("Ошибка при отправке запроса: %s\n", curl_easy_strerror(res));
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        free(post_fields);
        free(chunk.memory);
        return NULL;
    }

    // Освобождение ресурсов
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(post_fields);

    // Возвращение ответа
    return chunk.memory;
}

char *extract_text_from_response(char *response)
{

    char *text_start = strstr(response, "\"text\":");

    if (text_start == NULL)
    {
        return NULL;
    }

    text_start += strlen("\"text\":") + 5; // учитываем символ кавычки после :

    char *text_end = strchr(text_start, '"');

    if (text_end == NULL)
    {
        return NULL;
    }

    int text_len = text_end - text_start;
    char *text = (char *)malloc(text_len + 1);
    strncpy(text, text_start, text_len);
    text[text_len] = '\0';

    return text;
}

static void user_data_to_chat_gpt(gpointer chat_gpt_entry)
{

    GtkEntryBuffer *user_data = gtk_entry_get_buffer(GTK_ENTRY(chat_gpt_entry));

    GtkWidget *q_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);

    const char *question = gtk_entry_buffer_get_text(user_data);

    GtkWidget *user_avatar = gtk_image_new_from_pixbuf(current_scaled_avatar.scaled_avatar);

    GtkWidget *question_label = gtk_label_new(question);
    
    gtk_widget_set_halign(question_label, GTK_ALIGN_START);

    gtk_widget_set_margin_start(q_box, 10);

    gtk_widget_set_margin_top(q_box, 10);

    gtk_widget_set_size_request(user_avatar, 50, 50);

    gtk_box_append(GTK_BOX(q_box), user_avatar);

    gtk_box_append(GTK_BOX(q_box), question_label);    


    GtkWidget *a_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);

    GtkWidget *open_ai_avatar = gtk_image_new_from_file("resources/images/openai.svg");

    const char *api_key = "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";

    char *response = get_chatgpt_response(question, api_key);
    char *text = extract_text_from_response(response);

    GtkWidget *answer = gtk_label_new(text);

    gtk_widget_set_hexpand(answer, TRUE);

    gtk_widget_set_halign(answer, GTK_ALIGN_START);
    
    gtk_label_set_wrap(GTK_LABEL(answer), TRUE);
    gtk_label_set_max_width_chars(GTK_LABEL(answer), 60);
    gtk_label_set_wrap_mode(GTK_LABEL(answer), PANGO_WRAP_WORD_CHAR);
    //gtk_label_set_selectable(GTK_LABEL(answer), FALSE);

    gtk_widget_set_size_request(open_ai_avatar, 50, 50);

    gtk_widget_set_margin_start(a_box, 10);
    
    gtk_widget_set_margin_top(a_box, 10);

    gtk_box_append(GTK_BOX(a_box), open_ai_avatar);

    gtk_box_append(GTK_BOX(a_box), answer); 

    
    
    gtk_grid_attach(GTK_GRID(current_home.chat_grid), q_box, 0, pos, 1, 1);

    gtk_grid_attach(GTK_GRID(current_home.chat_grid), a_box, 0, pos + 1, 1, 1);

    gtk_editable_set_text(GTK_EDITABLE(chat_gpt_entry), "");

    pos += 2;
    mx_printstr(response);

    free(response);

    widget_styling(answer, current_screen, "chat_gpt_message");

    widget_styling(question_label, current_screen, "chat_gpt_message");
}

void show_home_grid()
{

    current_home.chat_gpt_box_for_label = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    current_home.chat_gpt_label = gtk_label_new("ChatGPT");

    current_home.chat_gpt_desc_label = gtk_label_new("This thing needs no introduction.");

    current_home.chat_grid = create_grid(700, 472, "chat_gpt_scrolled");

    GtkWidget *chat_scrolled = gtk_scrolled_window_new();

    gtk_widget_set_size_request(chat_scrolled, 700, 472);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(chat_scrolled), current_home.chat_grid);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(chat_scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    current_home.chat_gpt_response = gtk_label_new("ChatGPT response:");

    // GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);

    // gtk_text_buffer_set_text(buffer, "ChatGPT response:", -1);

    // gtk_text_view_set_buffer(GTK_TEXT_VIEW(current_home.chat_gpt_response), buffer);

    current_home.chat_gpt_entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    current_home.chat_gpt_entry = gtk_entry_new();

    gtk_entry_set_placeholder_text(GTK_ENTRY(current_home.chat_gpt_entry), "Write your question...");

    current_home.chat_gpt_send_btn = gtk_button_new();

    gtk_widget_set_halign(current_grid.home, GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(current_grid.home, TRUE);
    gtk_widget_set_vexpand(current_grid.home, TRUE);

    gtk_grid_attach(GTK_GRID(current_grid.home), current_home.chat_gpt_box_for_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.home), current_home.chat_gpt_desc_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.home), chat_scrolled, 0, 2, 1, 1);
    // gtk_grid_attach(GTK_GRID(current_home.chat_grid), current_home.chat_gpt_response, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.home), current_home.chat_gpt_entry_box, 0, 4, 1, 1);

    gtk_box_append(GTK_BOX(current_home.chat_gpt_box_for_label), current_home.chat_gpt_label);

    gtk_widget_set_size_request(current_home.chat_gpt_box_for_label, 250, 60);

    // gtk_widget_set_margin_start(current_home.chat_gpt_box_for_label, 380);
    // gtk_widget_set_margin_end(current_home.chat_gpt_box_for_label, 380);
    gtk_widget_set_halign(current_home.chat_gpt_box_for_label, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(current_home.chat_gpt_box_for_label, 17);

    gtk_widget_set_halign(current_home.chat_gpt_label, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_top(current_home.chat_gpt_label, 15);
    gtk_widget_set_margin_bottom(current_home.chat_gpt_label, 16);

    gtk_widget_set_size_request(current_home.chat_gpt_label, 104, 29);

    gtk_widget_set_halign(current_home.chat_gpt_desc_label, GTK_ALIGN_CENTER);

    gtk_widget_set_margin_top(current_home.chat_gpt_desc_label, 10);
    // gtk_widget_set_margin_bottom(current_home.chat_gpt_desc_label, 24);

    gtk_widget_set_halign(current_home.chat_gpt_box_for_label, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(current_home.chat_gpt_box_for_label, 17);

    gtk_widget_set_halign(chat_scrolled, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(chat_scrolled, 10);
    gtk_widget_set_margin_bottom(chat_scrolled, 17);

    gtk_widget_set_halign(current_home.chat_gpt_response, GTK_ALIGN_CENTER);

    // gtk_widget_set_margin_start(current_home.chat_gpt_response, 155);
    // gtk_widget_set_margin_end(current_home.chat_gpt_response, 365);
    gtk_widget_set_margin_top(current_home.chat_gpt_response, 13);
    // gtk_widget_set_margin_bottom(current_home.chat_gpt_response, 460);

    // gtk_text_view_set_editable(GTK_TEXT_VIEW(current_home.chat_gpt_response), false);

    // gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(current_home.chat_gpt_response), false);

    // gtk_label_set_max_width_chars(GTK_LABEL(current_home.chat_gpt_response), 76);
    // gtk_label_set_justify(GTK_LABEL(current_home.chat_gpt_response), GTK_JUSTIFY_CENTER);

    gtk_box_append(GTK_BOX(current_home.chat_gpt_entry_box), current_home.chat_gpt_entry);
    gtk_box_append(GTK_BOX(current_home.chat_gpt_entry_box), current_home.chat_gpt_send_btn);

    gtk_widget_set_size_request(current_home.chat_gpt_entry_box, 700, 40);

    gtk_widget_set_margin_start(current_home.chat_gpt_entry_box, 155);
    gtk_widget_set_margin_end(current_home.chat_gpt_entry_box, 155);
    gtk_widget_set_margin_bottom(current_home.chat_gpt_entry_box, 15);

    gtk_widget_set_margin_start(current_home.chat_gpt_entry, 29);
    gtk_widget_set_valign(current_home.chat_gpt_entry, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(current_home.chat_gpt_entry, 627, 15);

    gtk_widget_set_margin_start(current_home.chat_gpt_send_btn, 6);
    gtk_widget_set_valign(current_home.chat_gpt_send_btn, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(current_home.chat_gpt_send_btn, 24, 24);

    widget_styling(current_home.chat_gpt_box_for_label, current_screen, "chat_gpt_box_for_label");
    widget_styling(current_home.chat_gpt_label, current_screen, "chat_gpt_label");
    widget_styling(current_home.chat_gpt_desc_label, current_screen, "chat_gpt_desc_label");
    widget_styling(current_home.chat_gpt_response, current_screen, "chat_gpt_response");
    widget_styling(current_home.chat_gpt_entry_box, current_screen, "chat_gpt_entry_box");
    widget_styling(current_home.chat_gpt_entry, current_screen, "entry_for_users_text");

    widget_styling(current_home.chat_gpt_send_btn, current_screen, "chat_gpt_send_btn");

    //widget_styling(chat_scrolled, current_screen, "chat_gpt_scrolled");

    g_signal_connect(G_OBJECT(current_home.chat_gpt_entry), "activate", G_CALLBACK(user_data_to_chat_gpt), current_home.chat_gpt_entry);
}
