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

// Words from str1 that str2 contains
char *process(char *str1, char *str2) {
    char *result = 0;
    int result_len = 0;

    size_t str1_len = strlen(str1);
    size_t str2_len = strlen(str2);


    int start2 = 0;
    for (int j = 0; j <= str1_len; j++) {
        if (start2 != -1 && (j == str1_len || (str1[j] == ' ' || str1[j] == '\t'))) {
            int end2 = j - 1;
            // Hello 4 - 0 + 1 = 5
            int word_len = end2 - start2 + 1;
            char tmp[word_len + 1];
            strncpy(tmp, &str1[start2], word_len);
            tmp[word_len] = 0;

            int start = 0;
            for (int i = 0; i <= str2_len; i++) {
                if (start != -1 && (i == str2_len || (str2[i] == ' ' || str2[i] == '\t'))) {
                    int end = i - 1;
                    int len = end - start + 1;
                    char word[len + 1];
                    strncpy(word, &str2[start], len);
                    word[len] = 0;
                    //words_from_str2 = add(words_from_str2, word);

                    if (!strcmp(word, tmp)) {
                        if (!result) {
                            // terminator + space => 2
                            result = malloc((word_len + 2) * sizeof(char));
                            result_len = word_len + 1;
                            strncpy(result, tmp, word_len);
                            result[word_len] = 0;
                        } else {
                            result_len += word_len + 1;
                            result = realloc(result, result_len + 1);
                            strncat(result, tmp, word_len);
                        }
                        strcat(result, " ");
                        break;
                    }

                    // END
                    start = -1;
                }
                if (i != str2_len && str2[i] != ' ' && str2[i] != '\t' && start == -1) {
                    start = i;
                }
            }
            start2 = -1;
        }
        if (j != str1_len && str1[j] != ' ' && str1[j] != '\t' && start2 == -1) {
            start2 = j;
        }
    }

    // Remove last space character
    if (result && result_len > 0) {
        result[result_len - 1] = 0;
    }

    return result;
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
            free(result);
        }
    }
    return 0;
}
