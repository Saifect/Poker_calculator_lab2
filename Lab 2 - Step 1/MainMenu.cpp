#define _CRT_SECURE_NO_WARNINGS
#include "Functions.h"

//Начальное меню//
void print_mainMenu(Game* game, PokerCombination* result_player, int* choice) {
    printf("\n");
    printf("================================================\n");
    printf("               Добро пожаловать!                \n");
    printf("================================================\n");
    printf("Выберите действие:\n");
    printf("1. Покерный калькулятор\n");
    printf("2. Режим практики\n");
    printf("3. Настройки\n");
    printf("4. Прочее\n");
    printf("------------------------------------------------\n");
    printf("0. Выход из программы\n");
    printf("================================================\n");
    printf("Ваш выбор: ");
    get_user_choice(choice);
    handle_mainMenu_choice(game, result_player, *choice);
}

//Получение выбора пользователя//
void get_user_choice(int* choice) {
    *choice = (int)scanf_secure("int");
}

//Обработка выбора пользователя в основном меню//
void handle_mainMenu_choice(Game* game, PokerCombination* result_player, int choice) {
    switch (choice) {
    case 0:
        printf("Выход из программы.\n");
        exit(0);
        break;
    case -1:
        clearConsole(); //Обработка неправильного ввода функции scanf_secure (фича)
        break;

    case 1:
        clearConsole();
        print_calculatorMenu(game, result_player);
        break;
    case 2:
        printf("Не реализовано\n");
        press_any_key_to_continue();
        clearConsole();

        break;
    case 3:
        printf("Не реализовано\n");
        press_any_key_to_continue();
        clearConsole();
        break;
    case 4:
        printf("Status: EditBoardMenu - Done; Probability - In development\n");
        printf("Version program: 2.4x\n");
        printf("Author: Saifect@mail.ru\n");
        press_any_key_to_continue();
        break;

    default:
        break;
    }
}