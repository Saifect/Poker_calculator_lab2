#define _CRT_SECURE_NO_WARNINGS
#include "CommonFunctions.h"           


double scanf_secure(const char* type) {
    while (1) {
        if (strcmp(type, "int") == 0) {
            int num;
            if (scanf("%d", &num) == 1) {
                while (getchar() != '\n');
                return num;
            }
            else {
                printf("Ошибка ввода. Пожалуйста, введите целое число.\n");
                press_any_key_to_continue();
                buffer_clear();
                return -1;
            }
        }
        else if (strcmp(type, "float") == 0) {
            float num;
            if (scanf("%f", &num) == 1) {
                while (getchar() != '\n');
                return num;
            }
            else {
                printf("Ошибка ввода. Пожалуйста, введите вещественное число: ");
                buffer_clear();
            }
        }
        else if (strcmp(type, "double") == 0) {
            double num;
            if (scanf("%lf", &num) == 1) {
                while (getchar() != '\n');
                return num;
            }
            else {
                printf("Ошибка ввода. Пожалуйста, введите вещественное число: ");
                buffer_clear();
            }
        }
        else if (strcmp(type, "bool") == 0) {
            char input[10];
            if (scanf("%9s", input) == 1) {
                if (strcmp(input, "true") == 0 || strcmp(input, "1") == 0) { //Оно подрчеркивает зеленым и я незнаю почему так
                    return 1;
                }
                else if (strcmp(input, "false") == 0 || strcmp(input, "0") == 0) {
                    return 0;
                }
                else {
                    printf("Ошибка ввода. Пожалуйста, введите 'true' или 'false': ");
                    buffer_clear();
                }
            }
            else {
                printf("Ошибка ввода. Пожалуйста, введите 'true' или 'false': ");
                buffer_clear();
            }
        }
        else {
            printf("Неизвестный тип данных.\n");
            return 0;
        }
    }
}

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


// Очистка буфераа
void buffer_clear() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Удаление строки
void line_remove(char* str) {

    str[strcspn(str, "\n")] = '\0';
}

// Получение строки с защитой от переполнения буфера
void string_get_secure(char* buffer, int size) {
    if (fgets(buffer, size, stdin)) {
        line_remove(buffer);
    }
    else {
        buffer_clear();
    }
}

void press_any_key_to_continue() {
    printf("Нажмите любую клавишу для продолжения...\n");
    _getch(); // Ожидание нажатия клавиши
}

void print_used_cards(bool used_cards[NUM_RANKS][NUM_SUITS]) {
    int count = 0;
    printf("Использованные карты:\n");
    printf("     Черви  Бубны  Трефы  Пики\n");

    // Проходим по рангам карт
    for (int rank = 0; rank < NUM_RANKS; rank++) { // Ранги от 0 до 12
        printf("%2d: ", rank + 2); // Отображаем ранг как 2..14

        // Проходим по мастям карт
        for (int suit = 0; suit < NUM_SUITS; suit++) { // Масти от 0 до 3
            if (used_cards[rank][suit]) {
                count++;
            }
            printf("%5s ", used_cards[rank][suit] ? "Да" : "Нет");
        }

        printf("\n"); // Переход на новую строку после каждой строки ранг/масть
    }

    printf("Количество учтённых карт: %d\n", count);
}