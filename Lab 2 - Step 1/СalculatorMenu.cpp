#define _CRT_SECURE_NO_WARNINGS
#include "Functions.h"

void print_calculatorMenu(Game* game, PokerCombination* result_player) {
    srand(time(NULL));

    game->set_current_players(2);  // Установка текущих игроков через сеттер
    int choice = -1;
    bool edit_mode = false;
    int edit_current_player = 0;
    bool exit = false;

    while (!exit) {
        printf("\n");
        printf("================================================\n");
        printf("             Текущая информация                 \n");
        printf("================================================\n");

        for (int i = 0; i < game->get_current_players(); i++) {
            const Hand& hand = game->get_player(i).get_hand();  // Получаем руку через геттер
            const Card& card1 = hand.get_const_card(0);
            const Card& card2 = hand.get_const_card(1);

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

        if (strcmp(game->get_phase(), "preflop") == 0) {  // Используем геттер для фазы игры
            printf("Карты на столе: отсутствуют\n");
        }
        else {
            print_board_cards(&game->get_board());
        }

        printf("Текущая стадия игры (улица): %s\n", game->get_phase());  // Используем геттер для фазы

        // Меню
        printf("================================================\n");
        printf("           Функционал калькулятора              \n");
        printf("================================================\n");
        printf("1. Редактор игроков\n");
        printf("2. Редактор стола и стадий игры\n");
        printf("3. Редактор вычисления вероятностей\n");
        printf("4. Проанализировать комбинации игроков\n");
        printf("5. Вывести массив учтённых карт (отладка)\n");
        printf("-----------------------------------------------\n");
        printf("0. Выйти из покерного калькулятора\n");
        printf("================================================\n");
        printf("Ваш выбор: ");

        get_user_choice(&choice);
        handle_calculatorMenu_choice(choice, game, &exit, result_player);
    }
}

void handle_calculatorMenu_choice(int choice, Game* game, bool* exit, PokerCombination* result_player) {
    if (result_player == NULL) {
        printf("Ошибка: недостаточно памяти для вычисления комбинаций.\n");
        press_any_key_to_continue();
        return;
    }

    used_cards[NUM_RANKS][NUM_SUITS] = { false };

    const Hand& hand0 = game->get_player(0).get_hand();
    int rank = hand0.get_const_card(0).get_rank();
    int suit = hand0.get_const_card(0).get_suit(); // ВЫЧИТАНИЕ 1 ДЛЯ МАСТИ

    // Проверка корректности индексов
    if (rank >= 2 && rank <= 14 && suit > 0 && suit < 4) {
        used_cards[rank][suit - 1] = true;
    }

    rank = hand0.get_const_card(1).get_rank();
    suit = hand0.get_const_card(1).get_suit();  // ВЫЧИТАНИЕ 1 ДЛЯ МАСТИ
    if (rank >= 2 && rank <= 14 && suit > 0 && suit < 4) {
        used_cards[rank][suit - 1] = true;
    }

    // Помечаем карты игрока 2 как занятые
    const Hand& hand1 = game->get_player(1).get_hand();  // Получаем руку второго игрока
    rank = hand1.get_const_card(0).get_rank();
    suit = hand1.get_const_card(0).get_suit();  // ВЫЧИТАНИЕ 1 ДЛЯ МАСТИ
    if (rank >= 2 && rank <= 14 && suit > 0 && suit < 4) {
        used_cards[rank][suit - 1] = true;
    }

    rank = hand1.get_const_card(1).get_rank();
    suit = hand1.get_const_card(1).get_suit();  // ВЫЧИТАНИЕ 1 ДЛЯ МАСТИ
    if (rank >= 2 && rank <= 14 && suit > 0 && suit < 4) {
        used_cards[rank][suit - 1] = true;
    }

    // Помечаем карты на доске как занятые
    for (int i = 0; i < game->get_board().get_num_cards(); i++) {
        rank = game->get_board().get_card(i).get_rank();
        suit = game->get_board().get_card(i).get_suit() - 1;  // ВЫЧИТАНИЕ 1 ДЛЯ МАСТИ
        if (rank >= 2 && rank <= 14 && suit >= 0 && suit < 4) {
            used_cards[rank][suit] = true;
        }
    }

    // Обработка выбора пользователя
    switch (choice) {
    case -1:

        clearConsole();
        break;

    case 0:

        printf("Вы уверены? Введённые вами карты сбросятся\n");
        printf("Y/yes - да\n");
        printf("N/no - нет\n");

        char str_choice[3];
        string_get_secure(str_choice, 3);

        for (int i = 0; str_choice[i]; i++) {
            str_choice[i] = tolower(str_choice[i]);
        }

        if (strcmp(str_choice, "y") == 0 || strcmp(str_choice, "yes") == 0) {
            *exit = true;
        }
        else {
            printf("Операция отменена.\n");
            press_any_key_to_continue();
            clearConsole();
            break;
        }
        clearConsole();
        break;

    case 1:

        clearConsole();
        print_editPlayerMenu(game, used_cards);
        break;

    case 2:

        clearConsole();
        print_editBoardMenu(game, used_cards);
        break;

    case 3:

        clearConsole();
        print_probabilityMenu(game, used_cards);
        break;

    case 4:

        for (int i = 0; i < game->get_current_players() && i < game->get_num_players(); i++) {
            if (game->get_player(i).get_hand().get_const_card(0).get_rank() != NONE_RANK &&
                game->get_player(i).get_hand().get_const_card(1).get_rank() != NONE_RANK) {
                result_player[i] = determine_hand(game->get_player(i).get_hand(), game->get_board());
                print_hand(result_player[i]);
            }
            else {
                printf("Карты %d-го игрока не заданы, поэтому у него не может быть комбинации!\n", i + 1);
            }
        }

        press_any_key_to_continue();
        clearConsole();
        break;

    case 5:

        print_used_cards(used_cards);
        press_any_key_to_continue();
        clearConsole();
        break;

    default:

        clearConsole();
        break;
    }
}
