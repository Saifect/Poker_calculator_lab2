#define _CRT_SECURE_NO_WARNINGS
#include "Functions.h"


void print_probabilityMenu(Game* game, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    int num_simulations = 250000;
    int probabilityMenu_choice = 0;
    bool exit = false;

    Settings_debugging_mode settings;
    settings.show = false;
    settings.current_winner = -1;
    settings.ties_visible_mode = false;
    settings.wins_visible_mode = false;
    settings.simulations_visible_mode = false;

    while (exit == false) {

        printf("\n");
        printf("================================================\n");
        printf("             Текущая информация                 \n");
        printf("================================================\n");

        for (int i = 0; i < game->get_current_players(); i++) {
            // Проверяем, были ли введены карты игрока
            const Card& card1 = game->get_player(i).get_hand().get_const_card(0);
            const Card& card2 = game->get_player(i).get_hand().get_const_card(1);

            if (card1.get_rank() != NONE_RANK && card2.get_rank() != NONE_RANK) {
                printf("Карты игрока %d: %s %s и %s %s\n", i + 1,
                    card1.get_rank_name(),
                    card1.get_suit_name(),
                    card2.get_rank_name(),
                    card2.get_suit_name());
            }
            else {
                printf("Карты игрока %d: не заданы\n", i + 1);
            }
        }


        // Карты на столе
        if (strcmp(game->get_phase(), "preflop") == 0) {
            printf("Карты на столе: отсутствуют\n");
        }
        else {
            print_board_cards(&game->get_board());
        }

        printf("Текущая стадия игры (улица): %s\n", game->get_phase());
        printf("Текущее количество симуляций: %d\n", num_simulations);
        if (settings.wins_visible_mode == false && settings.ties_visible_mode == false) {
            printf("Режим отладки: Выключен\n");
        }
        else {
            printf("Режим отладки: Включен\n");
        }
        printf("================================================\n");
        printf("       Редактор вычисления вероятностей         \n");
        printf("================================================\n");
        printf("1. Расчёт по методу симуляций Монте-Карло\n");
        printf("2. Изменить количество симуляций\n");
        if (settings.show == true) {
            printf("3. Скрыть элементы отладки\n");
            if (settings.wins_visible_mode == false) {
                printf("4. Вкючить отображение побед для игрока (отладка)\n");
            }
            if (settings.wins_visible_mode == true) {
                printf("4. Выключить отоброжение побед для игрока (отладка)\n");
            }
            if (settings.ties_visible_mode == false) {
                printf("5. Включить отоброжение ничей (отладка)\n");
            }
            if (settings.ties_visible_mode == true) {
                printf("5. Выключить отоброжение ничей (отладка)\n");
            }

            if (settings.simulations_visible_mode == false) {
                printf("6. Включить отоброжение всех симмуляций (отладка)\n");
            }
            if (settings.simulations_visible_mode == true) {
                printf("6. Выключить отоброжение всех симмуляций (отладка)\n");
            }

       
            printf("7. Вывести массив учтённых карт (отладка)\n");
            




        }
        else if (settings.show == false) {
            printf("3. Показать элементы отладки\n");
        }
        printf("-----------------------------------------------\n");
        printf("0. Назад\n");
        printf("================================================\n");
        printf("Ваш выбор: ");

        get_user_choice(&probabilityMenu_choice);
        handle_probabilityMenu_choice(probabilityMenu_choice, game, &exit, used_cards, &num_simulations, &settings);
    }
}

void handle_probabilityMenu_choice(int choice, Game* game, bool* exit, bool used_cards[NUM_RANKS][NUM_SUITS], int* num_simulations, Settings_debugging_mode* settings) {
    bool debugging_mode = false;

    int choice_user;
    char str_choice[4];
    int players_with_cards = 0;
    switch (choice) {
    case 0:
        *exit = true;
        clearConsole();
        break;

    case 1:
        for (int i = 0; i < game->get_current_players(); i++) {
            // Проверяем есть ли карты у игрока
            if (game->get_player(i).get_hand().get_const_card(0).get_rank() != NONE_RANK && game->get_player(i).get_hand().get_const_card(1).get_rank() != NONE_RANK) {
                players_with_cards++;
            }

            if (players_with_cards >= 2) {
                break;
            }
        }

        // Проверяем результат
        if (players_with_cards < 2) {
            printf("Нужно хотя бы 2 заданные руки для анализа вероятности.\n");
            press_any_key_to_continue();
            clearConsole();
        }
        else if (*num_simulations >= 10 && *num_simulations <= 20000000) {


            if (settings->ties_visible_mode == true || settings->wins_visible_mode == true) {
                debugging_mode = true;
            }
            else {
                debugging_mode = false;
            }
            if (debugging_mode == true && *num_simulations > 5000) {
                if (*num_simulations > 3000 && *num_simulations < 10000) {
                    printf("Использовать режим отладки с количеством симуляций < 3000 не рекомендуется\nВы уверены?\n");
                }
                if (*num_simulations > 10000 && *num_simulations < 50000) {
                    printf("Оу, это будет долговато.\nДля режима отладки рекомендуется < 3000 симуляций, вы уверены?\n");
                }
                if (*num_simulations > 50000 && *num_simulations < 100000) {
                    printf("Это может быть очень долго.\nДля режима отладки рекомендуется < 3000 симуляций, вы уверены?\n");
                }
                else {
                    printf("Это будет в высшей степени долго.\nДля режима отладки рекомендуется < 3000 симуляций, вы уверены?\n");
                }
                printf("Y/yes - да\n");
                printf("N/no - нет\n");

                char str_choice[3];
                string_get_secure(str_choice, 3);

                // Приводим ввод к нижнему регистру //
                for (int i = 0; str_choice[i]; i++) {
                    str_choice[i] = tolower(str_choice[i]);
                }

                if (strcmp(str_choice, "y") == 0 || strcmp(str_choice, "yes") == 0) {

                    printf("Продолжаем выполнение в режиме отладки с %d симуляциями.\n", *num_simulations);

                }
                else {

                    printf("Операция отменена.\n");
                    press_any_key_to_continue();
                    clearConsole();
                    break;
                }

                press_any_key_to_continue();
                clearConsole();
            }
            printf("Не нажимайте ничего пока не загрузится результат \n");
            printf("Загрузка...\n");
            calculate_probabilities(game, used_cards, *num_simulations, settings);

            press_any_key_to_continue();
            clearConsole();
        }
        break;

    case 2: {

        printf("-----------------------------------------------\n");
        printf("Введите количество симуляций\nДиапазон значений от 100 до 5.000.000\n");
        printf("Рекомендуется 100.000 - 300.000 симуляций\nЗначения >1.000.000 могут долго работать\n");
        printf("Или введите 0 для отмены\n");
        printf("-----------------------------------------------\n");
        printf("Ваш выбор: ");
        int num_simulations_new = scanf_secure("int");

        if (num_simulations_new == 0) {
            printf("Операция отменена.\n");
            press_any_key_to_continue();
            clearConsole();
        }
        else if (num_simulations_new >= 100 && num_simulations_new <= 5000000) {
            *num_simulations = num_simulations_new; // обновляем количество симуляцийй
            printf("Вы установили %d симуляций для метода Монте-Карло\n", *num_simulations);
            press_any_key_to_continue();
            clearConsole();
        }
        else {
            printf("Вы ввели значение вне диапазона! Используем 250000 по умолчанию.\n");
            *num_simulations = 250000; // значение по умолчанию
            press_any_key_to_continue();
            clearConsole();
        }
        break;
    }

    case 3:

        settings->show = !(settings->show);
        clearConsole();

        break;

    case 4:
        if (settings->wins_visible_mode == true) {
            settings->wins_visible_mode = false;
            clearConsole();
        }
        else {
            printf("Введите номер игрока или 0 для отмены: ");
            choice_user = scanf_secure("int");
            if (choice_user == 0) {
                printf("Отмена операции.\n");
                press_any_key_to_continue();
                clearConsole();
                break;
            }
            else if (choice_user < 1 || choice_user > game->get_num_players()) {
                printf("Вы ввели число вне диапазона!\n");
                press_any_key_to_continue();
                clearConsole();
                break;
            }
            else {
                settings->current_winner = choice_user;
                settings->wins_visible_mode = !(settings->wins_visible_mode);
                if (settings->ties_visible_mode == true) {
                    settings->ties_visible_mode = false;
                }
                clearConsole();
            }
        }
        break;

    case 5:

        settings->ties_visible_mode = !(settings->ties_visible_mode);
        if (settings->wins_visible_mode == true) {
            settings->wins_visible_mode = false;
        }
        if (settings->simulations_visible_mode == true) {
            settings->simulations_visible_mode = false;
        }

        clearConsole();
        break;

    case 6:

        settings->simulations_visible_mode = !(settings->simulations_visible_mode);
        if (settings->wins_visible_mode == true) {
            settings->wins_visible_mode = false;
        }
        if (settings->ties_visible_mode == true) {
            settings->ties_visible_mode = false;
        }

        clearConsole();
        break;

    case 7:

        print_used_cards(used_cards);
        press_any_key_to_continue();
        clearConsole();
        break;

    default:
        clearConsole();
        break;
    }
}