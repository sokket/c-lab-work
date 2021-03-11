#include <stdio.h>

// Ввод строк должен быть организован с
// помощью функции scanf() со спецификациями
// для ввода строк. Функцию getchar() или спецификацию %c в scanf() не использовать!

int main() {
    char line[10000];
    scanf("%10000[^\n]", line);
    printf("%s\n", line);
    return 0;
}
