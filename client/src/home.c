#include "../inc/client.h"

extern t_screen current_screen;
extern t_grid current_grid;
t_home current_home;


struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
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

char *get_chatgpt_response(const char *message, const char *api_key) {
    // Инициализация библиотеки libcurl
    curl_global_init(CURL_GLOBAL_ALL);

    // Создание объекта CURL для отправки запроса на сервер
    CURL *curl = curl_easy_init();
    if (!curl) {
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
    chunk.memory = malloc(1);  // Начальное выделение памяти
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
        }", message);

    // Настройка параметров запроса
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/completions");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    // Отправка запроса на сервер
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
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


char* extract_text_from_response(char* response) {
    
    char* text_start = strstr(response, "\"text\":");
    
    if (text_start == NULL) {
        return NULL;
    }
    
    text_start += strlen("\"text\":") + 5; // учитываем символ кавычки после :
    
    char* text_end = strchr(text_start, '"');
    
    if (text_end == NULL) {
        return NULL;
    }
    
    int text_len = text_end - text_start;
    char* text = (char*)malloc(text_len + 1);
    strncpy(text, text_start, text_len);
    text[text_len] = '\0';

    return text;

}

static void user_data_to_chat_gpt(gpointer chat_gpt_entry) {

    GtkEntryBuffer *user_data = gtk_entry_get_buffer(GTK_ENTRY(chat_gpt_entry));
    
    const char *question = gtk_entry_buffer_get_text(user_data);
    
    const char* api_key = "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";

    char* response = get_chatgpt_response(question, api_key);
    char* text = extract_text_from_response(response);

    gtk_label_set_label(GTK_LABEL(current_home.chat_gpt_response), text);
    gtk_label_set_max_width_chars(GTK_LABEL(current_home.chat_gpt_response), 76);
    gtk_label_set_justify(GTK_LABEL(current_home.chat_gpt_response), GTK_JUSTIFY_CENTER);

    mx_printstr(response);
    
    free(response);

}

void show_home_grid() {

    current_home.chat_gpt_box_for_label = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    current_home.chat_gpt_label = gtk_label_new("ChatGPT");

    current_home.chat_gpt_desc_label = gtk_label_new("This thing needs no introduction.");

    current_home.chat_gpt_response = gtk_label_new("ChatGPT response:");

    current_home.chat_gpt_entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    current_home.chat_gpt_entry = gtk_entry_new();

    gtk_entry_set_placeholder_text(GTK_ENTRY(current_home.chat_gpt_entry), "Write your question...");

    current_home.chat_gpt_send_btn = gtk_button_new();


    gtk_grid_attach(GTK_GRID(current_grid.home), current_home.chat_gpt_box_for_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.home), current_home.chat_gpt_desc_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.home), current_home.chat_gpt_response, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(current_grid.home), current_home.chat_gpt_entry_box, 0, 3, 1, 1);

    gtk_box_append(GTK_BOX(current_home.chat_gpt_box_for_label), current_home.chat_gpt_label);
    
    gtk_widget_set_size_request(current_home.chat_gpt_box_for_label, 250, 60);
    gtk_widget_set_margin_start(current_home.chat_gpt_box_for_label, 380);
    gtk_widget_set_margin_end(current_home.chat_gpt_box_for_label, 380);
    gtk_widget_set_margin_top(current_home.chat_gpt_box_for_label, 17);
    
    
    gtk_widget_set_margin_start(current_home.chat_gpt_label, 73);
    gtk_widget_set_margin_end(current_home.chat_gpt_label, 73);
    gtk_widget_set_margin_top(current_home.chat_gpt_label, 15);
    gtk_widget_set_margin_bottom(current_home.chat_gpt_label, 16);

    gtk_widget_set_margin_start(current_home.chat_gpt_desc_label, 350);
    gtk_widget_set_margin_end(current_home.chat_gpt_desc_label, 350);
    gtk_widget_set_margin_top(current_home.chat_gpt_desc_label, 23);
    gtk_widget_set_margin_bottom(current_home.chat_gpt_desc_label, 24);

    gtk_widget_set_margin_start(current_home.chat_gpt_response, 155);
    gtk_widget_set_margin_end(current_home.chat_gpt_response, 365);
    gtk_widget_set_margin_top(current_home.chat_gpt_response, 13);
    gtk_widget_set_margin_bottom(current_home.chat_gpt_response, 460);






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
    widget_styling(current_home.chat_gpt_entry, current_screen, "chat_gpt_entry");

    g_signal_connect(G_OBJECT(current_home.chat_gpt_entry), "activate", G_CALLBACK(user_data_to_chat_gpt), current_home.chat_gpt_entry);


}
