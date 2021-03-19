#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

    size_t real_len = strlen(line);
    line = realloc(line, real_len + 1);
    line[real_len] = 0;

    return line;
}

void iterate_string_words(char *str, void (*process_word)(int, int, char *, void *, int *), void *args) {
    int start = 0;
    size_t str_len = strlen(str);
    for (int i = 0; i <= str_len; i++) {
        if (start != -1 && (i == str_len || (str[i] == ' ' || str[i] == '\t'))) {
            int end = i - 1;
            int len = end - start + 1;
            char word[len + 1];
            strncpy(word, &str[start], len);
            word[len] = 0;
            int stop = 0;
            process_word(len, start, word, args, &stop);
            if (stop) {
                break;
            }
            start = -1;
        }
        if (i != str_len && str[i] != ' ' && str[i] != '\t' && start == -1) {
            start = i;
        }
    }
}

typedef struct process_str2_args {
    char *word;
    int *found;
} process_str2_args;

void process_str2(int len, int word_index, char *word, void *args, int *stop) {
    process_str2_args *args_ptr = args;
    if (!strcmp(word, args_ptr->word)) {
        *(args_ptr->found) = 1;
        *stop = 1;
    }
}

typedef struct process_str1_args {
    char *result;
    int result_len;
    char *str2;
    int found;
} process_str1_args;

void process_str1(int len, int word_index, char *word, void *args, int *stop) {
    process_str1_args *args_ptr = args;
    process_str2_args str2_args;
    str2_args.word = word;
    str2_args.found = &args_ptr->found;
    iterate_string_words(args_ptr->str2, process_str2, &str2_args);
    if (args_ptr->found) {
        if (!args_ptr->result) {
            // terminator + space => 2
            args_ptr->result = malloc((len + 2) * sizeof(char));
            args_ptr->result_len = len + 1;
            strncpy(args_ptr->result, word, len);
            args_ptr->result[len] = 0;
        } else {
            args_ptr->result_len += len + 1;
            args_ptr->result = realloc(args_ptr->result, args_ptr->result_len + 1);
            strncat(args_ptr->result, word, len);
        }
        strcat(args_ptr->result, " ");
    }
}

// Words from str1 that str2 contains
char *process(char *str1, char *str2) {
    process_str1_args args;
    args.result = 0;
    args.result_len = 0;
    args.str2 = str2;
    args.found = 0;
    iterate_string_words(str1, process_str1, &args);

    // Remove last space character
    if (args.result && args.result_len > 0) {
        args.result[args.result_len - 1] = 0;
    }

    return args.result;
}

void process_get_first(int len, int word_index, char *word, void *args, int *stop) {
    char **first_word = args;
    size_t full_len = strlen(word) + 1;
    *first_word = malloc(full_len);
    strncpy(*first_word, word, full_len);
    *stop = 1;
}

typedef struct add_symbol {
    size_t result_len;
    int offset;
    char *result_copy;
    char *word;
} add_symbol;

void process_add_symbol(int len, int word_index, char *word, void *args, int *stop) {
    add_symbol *args_ptr = args;
    if (!strcmp(word, args_ptr->word)) {
        args_ptr->result_copy = realloc(args_ptr->result_copy, args_ptr->result_len + args_ptr->offset + 2);
        char *word_start_addr = args_ptr->result_copy + args_ptr->offset + word_index;
        memmove(word_start_addr + 1, word_start_addr, ( args_ptr->result_len) - word_index + 1);
        *word_start_addr = '*';
        args_ptr->offset++;
    }
}

int main() {
    for (;;) {
        printf("Введите строку № 1: ");
        char *str1 = get_line_impl();
        printf("Введите строку № 2: ");
        char *str2 = get_line_impl();

        if (strlen(str1) == 0 && strlen(str2) == 0) {
            printf("Пустой ввод, завершение программы\n");
            free(str1);
            free(str2);
            break;
        }

        char *result = process(str1, str2);
        free(str1);
        free(str2);
        if (result) {
            printf("%s\n", result);
            char *first_word = 0;
            iterate_string_words(result, process_get_first, &first_word);
            printf("%s\n", first_word);
            add_symbol args;
            args.result_len = strlen(result);
            args.result_copy = malloc(args.result_len + 1);
            strncpy(args.result_copy, result, args.result_len + 1);
            args.offset = 0;
            args.word = first_word;
            iterate_string_words(result, process_add_symbol, &args);
            free(first_word);
            free(result);
            printf("%s\n", args.result_copy);
            free(args.result_copy);
        }
    }
    return 0;
}
