#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void* malloc_wr(size_t size) {
    void* result = malloc(size);
    if (result)
        return result;
    else {
        perror("malloc");
        exit(1);
    }
}

typedef struct node {
    char *str;
    struct node *next;
} node;

node *add(node *list, char *str) {
    if (!list) {
        list = malloc_wr(sizeof(node));
        list->next = 0;
        list->str = str;
    } else {
        node *iter = list;
        for (; iter->next; iter = iter->next) {}
        iter->next = malloc_wr(sizeof(node));
        iter = iter->next;
        iter->next = 0;
        iter->str = str;
    }
    return list;
}

void free_list(node *head) {
    node *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp->str);
        free(tmp);
    }
}

char *get_line_impl() {
    char *line = malloc_wr(10 * sizeof(char));
    int last_index = 0;
    for (;;) {
        scanf("%1[^\r]", &line[last_index]);
        if (line[last_index] == '\n') {
            line[last_index] = 0;
            break;
        }
        line = realloc(line, (++last_index + 10) * sizeof(char));
    }

    size_t real_len = strlen(line);
    line = realloc(line, real_len + 1);
    line[real_len] = 0;

    return line;
}

int contains(node *list, char *word) {
    for (node *iter = list; iter; iter = iter->next) {
        if (!strcmp(word, iter->str)) {
            return 1;
        }
    }
    return 0;
}

// Words from str1 that str2 contains
char *process(char *str1, char *str2) {
    node *words_from_str2 = 0;

    int start = 0;
    size_t str2_len = strlen(str2);
    for (int i = 0; i <= str2_len; i++) {
        if (start != -1 && (i == str2_len || (str2[i] == ' ' || str2[i] == '\t'))) {
            int end = i - 1;
            int len = end - start + 1;
            char *word = malloc_wr((len + 1) * sizeof(char));
            strncpy(word, &str2[start], len);
            word[len] = 0;
            words_from_str2 = add(words_from_str2, word);
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
        if (start != -1 && (i == str1_len || (str1[i] == ' ' || str1[i] == '\t'))) {
            int end = i - 1;
            // Hello 4 - 0 + 1 = 5
            int word_len = end - start + 1;
            char tmp[word_len + 1];
            strncpy(tmp, &str1[start], word_len);
            tmp[word_len] = 0;
            if (contains(words_from_str2, tmp)) {
                if (!result) {
                    // terminator + space => 2
                    result = malloc_wr((word_len + 2) * sizeof(char));
                    result_len = word_len + 1;
                    strncpy(result, tmp, word_len);
                    result[word_len] = 0;
                } else {
                    result_len += word_len + 1;
                    result = realloc(result, result_len + 1);
                    strncat(result, tmp, word_len);
                }
                strcat(result, " ");
            }
            start = -1;
        }
        if (i != str1_len && str1[i] != ' ' && str1[i] != '\t' && start == -1) {
            start = i;
        }
    }

    free_list(words_from_str2);

    return result;
}

int main() {
    for (;;) {
        char *str1 = get_line_impl();
        if (strlen(str1) == 0) {
            free(str1);
            break;
        }
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
