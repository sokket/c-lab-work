#include <malloc.h>
#include <string.h>
#include <stdio.h>

typedef struct node {
    char* str;
    struct node* next;
} node;

node* add(node* list, char* str) {
    if (!list) {
        list = malloc(sizeof(node));
        list->next = 0;
        list->str = str;
    } else {
        node *iter = list;
        for (; iter->next; iter = iter->next) {}
        iter->next = malloc(sizeof(node));
        iter = iter->next;
        iter->next = 0;
        iter->str = str;
    }
    return list;
}

char *get_line_impl() {
    char *line = malloc(10 * sizeof(char));
    int last_index = 0;
    for (;;) {
        scanf("%1[^\r]", &line[last_index]);
        if (line[last_index] == '\n') {
            line[last_index] = 0;
            break;
        }
        line = realloc(line, (++last_index + 10) * sizeof(char));
    }
    return line;
}

int contains(char *str, char **array, int array_len) {
    for (int i = 0; i < array_len; i++) {
        if (!strcmp(str, array[i])) {
            return 1;
        }
    }
    return 0;
}

// Words from str1 that str2 contains
char *process(char *str1, char *str2) {
    char **words_from_str2 = 0;
    int str2_words_index = 0;
    int start = 0;
    size_t str2_len = strlen(str2);
    for (int i = 0; i <= str2_len; i++) {
        if (i == str2_len || (str2[i] == ' ' || str2[i] == '\t')) {
            int end = i - 1;
            if (!words_from_str2) {
                words_from_str2 = malloc(sizeof(char *));
            } else {
                int new_size = ++str2_words_index + 1;
                words_from_str2 = realloc(words_from_str2, new_size);
            }
            //printf("%d %d\n", start, end);
            int len = end - start + 1;
            words_from_str2[str2_words_index] = malloc((len + 1) * sizeof(char));
            strncpy(words_from_str2[str2_words_index], str2 + start, len);
            start = -1;
        }
        if (i != str2_len && str2[i] != ' ' && str2[i] != '\t' && start == -1) {
            start = i;
        }
    }

    char *result = 0;
    int result_len = 0;

    size_t str1_len = strlen(str1);
    for (int i = 0; i <= str1_len; i++) {
        if (i == str1_len || (str1[i] == ' ' || str1[i] == '\t')) {
            int end = i - 1;
            int word_len = end - start + 1;
            char tmp[word_len + 1];
            strncpy(tmp, &str1[start], word_len);
            tmp[word_len] = 0;
            if (contains(tmp, words_from_str2, str2_words_index + 1)) {
                if (!result) {
                    result = malloc((word_len + 2) * sizeof(char));
                    result_len = word_len + 1;
                } else {
                    result_len += word_len + 1;
                    result = realloc(result, result_len);
                }
                strncat(result, tmp, word_len);
                strncat(result, " ", 1);
            }
            start = -1;
        }
        if (i != str1_len && str1[i] != ' ' && str1[i] != '\t' && start == -1) {
            start = i;
        }
    }

    return result;
}

int main() {
    for (;;) {
        char *str1 = get_line_impl();
        if (strlen(str1) == 0)
            break;
        char *str2 = get_line_impl();
        char *result = process(str1, str2);
        free(str1);
        free(str2);
        if (result) {
            printf("%s\n", result);
            free(result);
        }
    }
    return 0;
}
