#define _CRT_SECURE_NO_WARNINGS
#include "Functions.h"

void print_editBoardMenu(Game* game, bool used_cards[15][4]) {
    int choice;
    bool exit_editor = false;
    int num_cards;
    bool deal_cards = true;
    bool debugging_mode = false;

    while (!exit_editor) {
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

        if (strcmp(game->get_phase(), "preflop") == 0) {
            printf("Карты на столе: отсутствуют\n");
        }
        else {
            game->get_board().print_board_cards();
        }

        printf("Текущая стадия игры (улица): %s\n", game->get_phase());
        printf("================================================\n");
        printf("               Редактор стола                   \n");
        printf("================================================\n");
        printf("1. Добавить/изменить Flop (3 карты)\n");
        printf("2. Добавить/изменить Turn (4-ая карта)\n");
        printf("3. Добавить/изменить River (5-ая карта)\n");
        printf("4. Заполнить Flop случайными картами\n");
        printf("5. Заполнить Turn случайной картой\n");
        printf("6. Заполнить River случайной картой\n");
        printf("7. Очистить Turn\n");
        printf("8. Очистить River\n");
        printf("9. Очистить стол\n");
        if (debugging_mode == false) {
            printf("10. Показать элементы отладки\n");
        }
        else {
            printf("10. Скрыть элементы отладки\n");
        }
        if (debugging_mode == true) {
            printf("11. Получить количество карт на столе (отладка)\n");
            printf("12. Вывести массив учтённых карт (отладка)\n");
        }

        printf("-----------------------------------------------\n");
        printf("0. Назад\n");
        printf("================================================\n");
        printf("Выберите действие: ");
        choice = (int)scanf_secure("int");

        handle_editBoardMenu_choice(choice, game, &exit_editor, used_cards, &deal_cards, &debugging_mode);
    }
}

void handle_editBoardMenu_choice(int choice, Game* game, bool* exit_editor, bool used_cards[15][4], bool* deal_cards, bool* debugging_mode) {
    int num_cards;

    switch (choice) {
    case 1:
        printf("Редактирование Flop (первая, вторая и третья карты на столе)\n");
        *deal_cards = deal_board_cards(game, used_cards, 0, 3);  // Ввод трёх карт на флоп

        if (*deal_cards == true) {
            game->set_phase("flop");
        }

        break;

    case 2:
        if (game->get_board().get_num_cards() < 3) {
            printf("Сначала необходимо добавить Flop!\n");
            press_any_key_to_continue();
        }
        else {
            printf("Редактирование Turn (четвёртая карта)\n");
            *deal_cards = deal_board_cards(game, used_cards, 3, 1);
            /*if (*deal_cards == true) {
                game->set_phase("turn");
            }*/
        }
        break;

    case 3:
        if (game->get_board().get_num_cards() < 4) {
            printf("Сначала необходимо добавить Turn!\n");
            press_any_key_to_continue();
        }
        else {
            printf("Редактирование River (пятая карта)\n");
            *deal_cards = deal_board_cards(game, used_cards, 4, 1);
            if (*deal_cards == true) {
                game->set_phase("river");
            }
        }
        break;

    case 4:
        if (game->get_board().get_num_cards() < 3) {
            printf("Редактирование Flop (случайные 3 карты)\n");
            deal_board_random_cards(game, used_cards, 0, 3);
        }
        else {
            printf("Флоп уже задан! Сначала очистите стол.\n");
            press_any_key_to_continue();
        }
        break;

    case 5:
        if (game->get_board().get_num_cards() < 3) {
            printf("Сначала необходимо добавить Flop!\n");
            press_any_key_to_continue();
        }
        else if (game->get_board().get_num_cards() == 4 || game->get_board().get_num_cards() == 5) {
            printf("Терн уже задан! Сначала очистите стол.\n");
            press_any_key_to_continue();
        }
        else {
            printf("Редактирование Turn (случайная 4-ая карта)\n");
            deal_board_random_cards(game, used_cards, 3, 1);
        }
        break;

    case 6:
        if (game->get_board().get_num_cards() < 4) {
            printf("Сначала необходимо добавить Turn!\n");
            press_any_key_to_continue();
        }
        else if (game->get_board().get_num_cards() == 5) {
            printf("Ривер уже задан! Сначала очистите стол.\n");
            press_any_key_to_continue();
        }
        else {
            printf("Редактирование River (случайная 5-ая карта)\n");
            deal_board_random_cards(game, used_cards, 4, 1);
        }
        break;

    case 7:
        if (game->get_board().get_num_cards() < 3) {
            printf("Сначала необходимо добавить flop и turn!\n");
            press_any_key_to_continue();
        }
        else if (game->get_board().get_num_cards() == 3) {
            printf("Сначала необходимо добавить turn!\n");
            press_any_key_to_continue();
        }
        else if (game->get_board().get_num_cards() == 5) {
            printf("Сначала необходимо очистить River\n");
            press_any_key_to_continue();
        }
        else if (game->get_board().get_num_cards() == 4) {
            clear_board_turn(game, used_cards);
            printf("Turn очищен!\n");
            press_any_key_to_continue();
        }
        break;

    case 8:
        if (game->get_board().get_num_cards() < 3) {
            printf("Сначала необходимо добавить flop и turn и river!\n");
            press_any_key_to_continue();
        }
        else if (game->get_board().get_num_cards() == 3) {
            printf("Сначала необходимо добавить turn и river!\n");
            press_any_key_to_continue();
        }
        else if (game->get_board().get_num_cards() == 4) {
            printf("Сначала необходимо добавить river!\n");
            press_any_key_to_continue();
        }
        else if (game->get_board().get_num_cards() == 5) {
            clear_board_river(game, used_cards);
            printf("River очищен!\n");
            press_any_key_to_continue();
        }
        break;

    case 9:
        clear_board(game, used_cards);
        printf("Стол очищен!\n");
        press_any_key_to_continue();
        break;

    case 10:
        *debugging_mode = !(*debugging_mode);
        break;

    case 11:
        num_cards = game->get_board().get_num_cards();
        if (num_cards > 4) {
            printf("На столе сейчас: %d карт\n", num_cards);
        }
        else {
            printf("На столе сейчас: %d карты\n", num_cards);
        }
        press_any_key_to_continue();
        break;

    case 12:
        print_used_cards(used_cards);
        press_any_key_to_continue();
        clearConsole();
        break;

    case 0:
        *exit_editor = true;
        break;

    default:
        break;
    }
    clearConsole();
}

bool deal_board_cards(Game* game, bool used_cards[NUM_RANKS][NUM_SUITS], int start_index, int num_cards) {
    int rank_choice, suit_choice;
    int initial_num_cards = game->get_board().get_num_cards(); // Текущее количество карт на доске
    int current_num_cards = initial_num_cards;
    bool rewrite = false;

    if (start_index + num_cards > 5) {
        printf("Ошибка: превышено максимальное количество карт на столе.\n");
        return false;
    }

    for (int i = 0; i < num_cards; i++) {
        do {
            printf("Введите ранг карты %d (2-14) или 0 для выхода: ", start_index + i + 1);
            rank_choice = (int)scanf_secure("int");
            if (rank_choice == 0) {
                clear_board(game, used_cards); // Отмена изменений
                return false;
            }
            if (rank_choice < 2 || rank_choice > 14) {
                printf("Неверный ранг карты! Попробуйте снова.\n");
                continue;
            }

            printf("Введите масть карты %d (1 - Черви, 2 - Бубны, 3 - Трефы, 4 - Пики) или 0 для выхода: ", start_index + i + 1);
            suit_choice = (int)scanf_secure("int");
            if (suit_choice == 0) {
                clear_board(game, used_cards); // Отмена изменений
                return false;
            }
            if (suit_choice < 1 || suit_choice > 4) {
                printf("Неверная масть карты! Попробуйте снова.\n");
                continue;
            }

            if (used_cards[rank_choice - 2][suit_choice - 1]) {
                printf("Эта карта уже используется! Попробуйте другую карту.\n");
            }
            else {
                Card new_card((Suit)(suit_choice - 1), (Rank)(rank_choice - 2));
                if (start_index + i < current_num_cards) {
                    // Перезапись существующей карты
                    game->get_board().set_card(start_index + i, new_card);
                    rewrite = true;
                }
                else {
                    // Добавление новой карты
                    game->get_board().add_card(new_card, used_cards);
                    current_num_cards++;
                }
                used_cards[rank_choice - 2][suit_choice - 1] = true;
                break;
            }
        } while (true);
    }

    // Обновление фазы игры
    if (!rewrite) {
        if (current_num_cards > initial_num_cards) {
            if (current_num_cards == 3) {
                game->set_phase("flop");
            }
            else if (current_num_cards == 4) {
                game->set_phase("turn");
            }
            else if (current_num_cards == 5) {
                game->set_phase("river");
            }
        }
    }

    return true;
}


void deal_board_random_cards(Game* game, bool used_cards[NUM_RANKS][NUM_SUITS], int start_index, int num_cards) {
    int rank_choice, suit_choice;

    Card temp_cards[MAX_BOARD_CARDS];
    int temp_card_count = 0;

    for (int i = 0; i < num_cards; i++) {
        do {
            // генерация случайного ранга (от 0 до 12)
            rank_choice = rand() % NUM_RANKS;
            // генерация случайной масти (от 0 до 3)
            suit_choice = rand() % NUM_SUITS;

            if (!used_cards[rank_choice][suit_choice]) {
                temp_cards[i].init_card((Suit)suit_choice, (Rank)rank_choice);
                temp_card_count++;
                break;
            }
        } while (true);
    }

    // Отладочный вывод
    printf("Сгенерированные карты:\n");
    for (int i = 0; i < num_cards; i++) {
        temp_cards[i].print_card();
    }
    press_any_key_to_continue();

    // Сохраняем карты на доске
    if (temp_card_count == num_cards) {
        for (int i = 0; i < num_cards; i++) {

            game->get_board().set_card(start_index + i, temp_cards[i]);

        }

        // Обновляем количество карт на доске
        game->get_board().set_num_cards(start_index + num_cards);

        // Изменяем фазу игры
        if (game->get_board().get_num_cards() == 3) {
            game->set_phase("flop");
        }
        else if (game->get_board().get_num_cards() == 4) {
            game->set_phase("turn");
        }
        else if (game->get_board().get_num_cards() == 5) {
            game->set_phase("river");
        }
    }
    else {
        printf("Не удалось ввести все карты, изменения отменены.\n");
    }
}

void clear_board(Game* game, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    for (int i = 0; i < game->get_board().get_num_cards(); i++) {
        Card card = game->get_board().get_card(i);
        if (card.get_rank() != NONE_RANK && card.get_suit() != NONE_SUIT) {
            used_cards[card.get_rank()][card.get_suit()] = false;
        }
        card.init_card(NONE_SUIT, NONE_RANK);
    }

    game->set_phase("preflop");
    game->get_board().set_num_cards(0);  // Обнуляем количество карт на столе
}

void clear_board_turn(Game* game, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    // Очистка карты терна (четвертая карта на столе)
    Card card = game->get_board().get_card(3); // Индекс 3 соответствует четвертой карте (0-based index)
    if (card.get_rank() != NONE_RANK && card.get_suit() != NONE_SUIT) {
        used_cards[card.get_rank()][card.get_suit()] = false;
    }
    card.init_card(NONE_SUIT, NONE_RANK);

    game->get_board().set_num_cards(3); // Устанавливаем количество карт на столе обратно к 3 (флоп)
    game->set_phase("flop");
}


void clear_board_river(Game* game, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    // Очистка карты ривера (пятая карта на столе)
    Card card = game->get_board().get_card(4); // Индекс 4 соответствует пятой карте (0-based index)
    if (card.get_rank() != NONE_RANK && card.get_suit() != NONE_SUIT) {
        used_cards[card.get_rank()][card.get_suit()] = false;
    }
    card.init_card(NONE_SUIT, NONE_RANK);

    game->get_board().set_num_cards(4); // Устанавливаем количество карт на столе обратно к 4 (после терна)
    game->set_phase("turn");
}

void print_board_cards(const Board* board) {
    board->print_board_cards();
}











