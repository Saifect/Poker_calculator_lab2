#define _CRT_SECURE_NO_WARNINGS
#include "Functions.h"

void print_editPlayerMenu(Game* game, bool used_cards[15][4]) {
    int choice;
    bool back = false;
    int choice_player;

    while (!back) {
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

        // Вывод карт на столе
        if (strcmp(game->get_phase(), "preflop") == 0) {
            printf("Карты на столе: отсутствуют\n");
        }
        else {
            game->get_board().print_board_cards();
        }

        printf("Текущая стадия игры (улица): %s\n", game->get_phase());

        printf("================================================\n");
        printf("               Редактор игроков                 \n");
        printf("================================================\n");
        printf("1. Добавить/изменить карты игроку\n");
        printf("2. Сгенерировать случайные карты игроку\n");
        printf("3. Сгенерировать случайные карты всем игрокам\n");
        printf("4. Очистить карты игроку\n");
        printf("5. Очистить карты всех игроков\n");
        printf("6. Задать количество игроков\n");
        printf("7. Вывести массив учтённых карт (отладка)\n");
        printf("-----------------------------------------------\n");
        printf("0. Назад\n");
        printf("================================================\n");
        printf("Ваш выбор: ");

        // Получение выбора пользователя
        choice = (int)scanf_secure("int");

        handle_editPlayerMenu_choice(choice, game, &back, used_cards);
    }
}

void handle_editPlayerMenu_choice(int choice, Game* game, bool* back, bool used_cards[15][4]) {
    int choice_player;

    switch (choice) {
    case 0:
        *back = true;
        clearConsole();
        break;
    case 1:
        printf("Введите номер игрока или 0 для отмены: ");
        choice_player = scanf_secure("int");
        if (choice_player == 0) {
            printf("Отмена операции.\n");
            press_any_key_to_continue();
            clearConsole();
            break;
        }

        if (choice_player < 0 || choice_player > game->get_current_players()) {
            printf("Игрока под таким номером не существует!\n");
            press_any_key_to_continue();
            clearConsole();
            return;
        }

        deal_cards(&game->get_player(choice_player - 1), used_cards);

        clearConsole();
        break;
    case 2:
        printf("Введите номер игрока или 0 для отмены: ");
        choice_player = scanf_secure("int");
        if (choice_player == 0) {
            printf("Отмена операции.\n");
            press_any_key_to_continue();
            clearConsole();
            break;
        }
        deal_random_cards(&game->get_player(choice_player - 1), used_cards, choice_player);
        printf("Рука игрока %d сгенерирована случайно!\n", choice_player);
        press_any_key_to_continue();
        clearConsole();
        break;

    case 3:
        for (int i = 0; game->get_current_players() > i; i++) {
            deal_random_cards(&game->get_player(i), used_cards, i);
        }

        printf("Карты для игроков успешно сгенерированы!\n");
        press_any_key_to_continue();
        clearConsole();
        break;

    case 4:
        printf("Введите номер игрока или 0 для отмены: ");
        choice_player = scanf_secure("int");
        if (choice_player == 0) {
            printf("Отмена операции.\n");
            press_any_key_to_continue();
            clearConsole();
            break;
        }
        game->get_player(choice_player - 1).clear_player_cards(used_cards);
        printf("Рука игрока %d успешно очищена!\n", choice_player);
        press_any_key_to_continue();
        clearConsole();
        break;
    case 5:
        game->clear_all_players_cards(used_cards);
        clearConsole();
        break;
    case 6:
        printf("Введите количество игроков (2-%d) или 0 для отмены: ", game->get_num_players());
        choice_player = scanf_secure("int");

        if (choice_player == 0) {
            printf("Отмена операции.\n");
            press_any_key_to_continue();
            clearConsole();
            break;
        }

        if (choice_player == game->get_current_players()) {
            printf("У вас столько же игроков!\n");
            press_any_key_to_continue();
            clearConsole();
            break;
        }

        if (choice_player < 2 || choice_player > game->get_num_players()) {
            printf("Вы ввели число вне диапазона!\n");
            press_any_key_to_continue();
            clearConsole();
            break;
        }

        if (choice_player < game->get_current_players()) {
            for (int i = choice_player; i < game->get_current_players(); i++) {
                game->get_player(i).clear_player_cards(used_cards);
            }
        }

        game->set_current_players(choice_player);
        printf("Вы задали %d игроков для калькулятора.\n", choice_player);
        press_any_key_to_continue();
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

void deal_cards(Player* player, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    printf("------------------------------------------------\n");
    printf("          Ввод карт для игрока\n");
    printf("------------------------------------------------\n");

    // Сначала очищаем текущие карты игрока
    player->clear_player_cards(used_cards);

    int rank_choice_card[2], suit_choice_card[2];

    for (int current_card = 0; current_card < 2; current_card++) {
        do {
            printf("Введите ранг %d-ой карты (2-14) или 0 для выхода: ", current_card + 1);
            rank_choice_card[current_card] = (int)scanf_secure("int");
            if (rank_choice_card[current_card] == 0) {
                // Отмена ввода
                for (int j = 0; j < current_card; j++) {
                    player->get_hand().get_card(j).init_card(NONE_SUIT, NONE_RANK);
                    used_cards[rank_choice_card[j] - 2][suit_choice_card[j] - 1] = false;
                }
                return;
            }
            if (rank_choice_card[current_card] < 2 || rank_choice_card[current_card] > 14) {
                printf("Неверный ранг карты! Попробуйте снова.\n");
                continue;
            }

            printf("Введите масть %d-ой карты (1 - Черви, 2 - Бубны, 3 - Трефы, 4 - Пики) или 0 для выхода: ", current_card + 1);
            suit_choice_card[current_card] = (int)scanf_secure("int");
            if (suit_choice_card[current_card] == 0) {
                // Отмена ввода
                for (int j = 0; j < current_card; j++) {
                    player->get_hand().get_card(j).init_card(NONE_SUIT, NONE_RANK);
                    used_cards[rank_choice_card[j] - 2][suit_choice_card[j] - 1] = false;
                }
                return;
            }
            if (suit_choice_card[current_card] < 1 || suit_choice_card[current_card] > 4) {
                printf("Неверная масть карты! Попробуйте снова.\n");
                continue;
            }

            // Проверка на уникальность карты
            if (used_cards[rank_choice_card[current_card] - 2][suit_choice_card[current_card] - 1]) {
                printf("Эта карта уже используется! Попробуйте другую карту.\n");
            }
            else {
                // Создаем карту
                Card new_card((Suit)(suit_choice_card[current_card] - 1), (Rank)(rank_choice_card[current_card] - 2));

                // Назначаем карту игроку
                player->get_hand().set_card(current_card, new_card);

                // Отмечаем карту как использованную
                used_cards[rank_choice_card[current_card] - 2][suit_choice_card[current_card] - 1] = true;

                // Отладочный вывод массива used_cards
                printf("Карта добавлена: %s %s\n",
                    new_card.get_rank_name(), new_card.get_suit_name());

                break;
            }
        } while (true);
    }
}


void deal_random_cards_all(Player* player, bool used_cards[15][4]) {
    // Инициализируем колоду
    Card full_deck[52];
    create_deck(full_deck, used_cards);

    // Массив для доступных карт
    Card* available_deck = (Card*)malloc(52 * sizeof(Card));
    int available_count = 0;

    // Собираем доступные карты
    for (int i = 0; i < 52; i++) {
        int rank = full_deck[i].get_rank();
        int suit = full_deck[i].get_suit() - 1;  // Масть уменьшаем на 1 для корректной индексации
        if (rank >= 2 && rank <= 14 && suit >= 0 && suit < 4) {
            if (!used_cards[rank][suit]) {
                available_deck[available_count++] = full_deck[i];
            }
        }
        else {
            printf("Ошибка: недопустимые значения rank = %d, suit = %d\n", rank, suit + 1);
        }
    }

    // Перемешиваем доступные карты
    shuffle_deck(available_deck, available_count);

    // Устанавливаем новые карты для игрока
    int old_rank1 = player->get_hand().get_card(0).get_rank();
    int old_suit1 = player->get_hand().get_card(0).get_suit() - 1;
    int old_rank2 = player->get_hand().get_card(1).get_rank();
    int old_suit2 = player->get_hand().get_card(1).get_suit() - 1;

    // Освобождаем старые карты
    if (old_rank1 >= 2 && old_rank1 <= 14 && old_suit1 >= 0 && old_suit1 < 4) {
        used_cards[old_rank1][old_suit1] = false;
    }
    if (old_rank2 >= 2 && old_rank2 <= 14 && old_suit2 >= 0 && old_suit2 < 4) {
        used_cards[old_rank2][old_suit2] = false;
    }

    // Задаем новые карты
    player->get_hand().get_card(0) = available_deck[0];
    player->get_hand().get_card(1) = available_deck[1];

    int new_rank1 = player->get_hand().get_card(0).get_rank();
    int new_suit1 = player->get_hand().get_card(0).get_suit() - 1;
    int new_rank2 = player->get_hand().get_card(1).get_rank();
    int new_suit2 = player->get_hand().get_card(1).get_suit() - 1;

    // Помечаем новые карты как занятые
    if (new_rank1 >= 2 && new_rank1 <= 14 && new_suit1 >= 0 && new_suit1 < 4) {
        used_cards[new_rank1][new_suit1] = true;
    }
    if (new_rank2 >= 2 && new_rank2 <= 14 && new_suit2 >= 0 && new_suit2 < 4) {
        used_cards[new_rank2][new_suit2] = true;
    }

    // Освобождаем память
    free(available_deck);
}

void clear_player_cards(Player* players, int current_players, bool used_cards[15][4], int choice_player, bool mute_mode) {
    players[choice_player - 1].clear_player_cards(used_cards);
}

void clear_all_players_cards(Player* players, int current_players, bool used_cards[15][4], bool mute_mode) {
    for (int i = 0; i < current_players; i++) {
        players[i].clear_player_cards(used_cards);
        if (!mute_mode) {
            printf("Рука игрока %d успешно очищена!\n", i + 1);
        }
    }
    printf("Операция завершена!\n");
    press_any_key_to_continue();
}

void clear_player_hand(Player* player, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    for (int i = 0; i < 2; i++) {
        if (player->get_hand().get_card(i).get_rank() != NONE_RANK && player->get_hand().get_card(i).get_suit() != NONE_SUIT) {
            used_cards[player->get_hand().get_card(i).get_rank()][player->get_hand().get_card(i).get_suit()] = false;
        }
        player->get_hand().get_card(i).set_rank(NONE_RANK);
        player->get_hand().get_card(i).set_suit(NONE_SUIT);
    }
}

void assign_random_card(Card* card, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    int rank, suit;
    do {
        rank = rand() % NUM_RANKS;
        suit = rand() % NUM_SUITS;
    } while (used_cards[rank][suit]);

    card->set_rank((Rank)rank);
    card->set_suit((Suit)suit);
    used_cards[rank][suit] = true;
}

void assign_random_hand(Player* player, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    clear_player_hand(player, used_cards);
    for (int i = 0; i < 2; i++) {
        assign_random_card(&player->get_hand().get_card(i), used_cards);
    }
}

void deal_random_cards(Player* player, bool used_cards[NUM_RANKS][NUM_SUITS], int choice_player) {
    assign_random_hand(player, used_cards);
}