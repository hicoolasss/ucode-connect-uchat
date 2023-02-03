#include "../inc/libmx.h"

typedef struct
{
    const char *start;
    size_t len;
} token;

char **mx_strsplit(const char *s, char c) {
    if (s == NULL) return NULL;
    char **array;
    unsigned int start = 0, stop, toks = 0, t;
    token *tokens = malloc((mx_strlen(s) + 1) * sizeof(token));
    for (stop = 0; s[stop]; stop++) {
        if (s[stop] == c) {
            tokens[toks].start = s + start;
            tokens[toks].len = stop - start;
                toks++;
            start = stop + 1;
        }
    }
    tokens[toks].start = s + start;
    tokens[toks].len = stop - start;
    toks++;
    array = malloc((toks + 1) * sizeof(char *));
    for (t = 0; t < toks; t++) {
            char *token = mx_strnew(tokens[t].len);
            mx_strncpy(token, tokens[t].start, tokens[t].len);
            array[t] = token;
    }
    array[t] = NULL;
    free(tokens);
    return array;
}
