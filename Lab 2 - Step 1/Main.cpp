#define _CRT_SECURE_NO_WARNINGS
#include "Globals.h"
#include "Classes.h"
#include "Functions.h"

bool used_cards[NUM_RANKS][NUM_SUITS] = { false }; // Определение глобальной переменной

int main() {

    /// Инициализация игры ///
    Game game;
    initialize_game(&game, 12);

    // Используем геттер для доступа к количеству игроков
    PokerCombination* result_player = (PokerCombination*)malloc(game.get_num_players() * sizeof(PokerCombination));

    /// Установка кодировок 
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Для православного языка
    setlocale(LC_ALL, "Rus");
    clearConsole();


    while (true) {
        int choice;
        print_mainMenu(&game, result_player, &choice);
        clearConsole();
    }

    free(result_player);
    return 0;
}

void initialize_game(Game* game, int num_players) {
    if (num_players > 12) {
        num_players = 12; // Ограничение на 12 игроков
    }

    // Инициализируем игроков
    for (int i = 0; i < num_players; i++) {
        Card card1(NONE_SUIT, NONE_RANK);
        Card card2(NONE_SUIT, NONE_RANK);
        Hand hand(card1, card2);

        Player player;
        player.set_hand(hand, used_cards);
        player.set_equity(0.0);
        player.set_wins(0);
        player.set_ties(0);
        player.set_losses(0);

        game->set_player(i, player); // Используем сеттер для игрока
    }

    // Инициализируем поле
    for (int i = 0; i < 5; i++) {
        game->get_board().set_card(i, Card(NONE_SUIT, NONE_RANK), used_cards);
    }
    game->get_board().set_num_cards(0);

    // Устанавливаем фазу игры
    game->set_phase("preflop");

    // Устанавливаем количество игроков
    game->set_num_players(num_players); // Используем сеттер для количества игроков
}

void print_hand(PokerCombination hand) {
    switch (hand.hand_rank) {
    case STRAIGHT_FLUSH:
        printf("Комбинация: Стрит-флеш\n");
        printf("Старшая карта: %d\n", hand.high_card);
        break;
    case FOUR_OF_A_KIND:
        printf("Комбинация: Каре %d\n", hand.high_card);
        printf("Кикер: %d\n", hand.kicker[0]);
        break;
    case FULL_HOUSE:
        printf("Комбинация: Фулл-хаус\n");
        printf("Тройка: %d\n", hand.high_card);
        printf("Пара: %d\n", hand.kicker[0]);
        break;
    case FLUSH:
        printf("Комбинация: Флеш\n");
        printf("Старшая карта: %d\n", hand.high_card);
        break;
    case STRAIGHT:
        printf("Комбинация: Стрит\n");
        printf("Старшая карта: %d\n", hand.high_card);
        break;
    case THREE_OF_A_KIND:
        if (hand.kicker[0] == NONE_RANK) {
            printf("Комбинация: Сет %d\n", hand.high_card);
        }
        else {
            printf("Комбинация: Трипс %d\n", hand.high_card);
            printf("Кикер: %d\n", hand.kicker[0]);
        }
        break;
    case TWO_PAIR:
        printf("Комбинация: Две пары\n");
        printf("Старшая пара: %d\n", hand.high_card);
        printf("Младшая пара: %d\n", hand.kicker[0]);
        break;
    case ONE_PAIR:
        if (hand.kicker[0] == NONE_RANK && hand.kicker[1] == NONE_RANK) {
            printf("Комбинация: Карманная пара %d\n", hand.high_card);
        }
        else {
            printf("Комбинация: Пара %d\n", hand.high_card);
            printf("Кикер: %d\n", hand.kicker[0]);
        }
        break;
    case HIGH_CARD:
        printf("Комбинация: Старшая карта %d\n", hand.high_card);
        printf("Кикер: %d\n", hand.kicker[0]);
        break;
    default:
        printf("Неизвестная комбинация\n");
        break;
    }
}

void print_probabilityMenu(Game* game, bool used_cards[15][4]) {
    int num_simulations = 250000;
    int probabilityMenu_choice = 0;
    bool exit = false;

    Settings_debugging_mode settings_debugging_mode;
    settings_debugging_mode.show = false;
    settings_debugging_mode.current_winner = -1;
    settings_debugging_mode.ties_mode = false;
    settings_debugging_mode.wins_mode = false;

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
        if (settings_debugging_mode.wins_mode == false && settings_debugging_mode.ties_mode == false) {
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
        if (settings_debugging_mode.show == true) {
            printf("3. Скрыть элементы отладки\n");
            if (settings_debugging_mode.wins_mode == false) {
                printf("4. Вкючить отображение побед для игрока (отладка)\n");
            }
            if (settings_debugging_mode.wins_mode == true) {
                printf("4. Выключить отоброжение побед для игрока (отладка)\n");
            }
            if (settings_debugging_mode.ties_mode == false) {
                printf("5. Включить отоброжение ничей (отладка)\n");
            }
            if (settings_debugging_mode.ties_mode == true) {
                printf("5. Выключить отоброжение ничей (отладка)\n");
            }
        }
        else if (settings_debugging_mode.show == false) {
            printf("3. Показать элементы отладки\n");
        }
        printf("-----------------------------------------------\n");
        printf("0. Назад\n");
        printf("================================================\n");
        printf("Ваш выбор: ");

        get_user_choice(&probabilityMenu_choice);
        handle_probabilityMenu_choice(probabilityMenu_choice, game, &exit, used_cards, &num_simulations, &settings_debugging_mode);
    }
}

void handle_probabilityMenu_choice(int choice, Game* game, bool* exit, bool used_cards[15][4], int* num_simulations, Settings_debugging_mode* settings_debugging_mode) {
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


            if (settings_debugging_mode->ties_mode == true || settings_debugging_mode->wins_mode == true) {
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
            calculate_probabilities(game, used_cards, *num_simulations, settings_debugging_mode);

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

        settings_debugging_mode->show = !(settings_debugging_mode->show);
        clearConsole();

        break;

    case 4:
        if (settings_debugging_mode->wins_mode == true) {
            settings_debugging_mode->wins_mode = !(settings_debugging_mode->wins_mode);
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
                settings_debugging_mode->current_winner = choice_user;
                settings_debugging_mode->wins_mode = !(settings_debugging_mode->wins_mode);
                if (settings_debugging_mode->ties_mode == true) {
                    settings_debugging_mode->ties_mode = !(settings_debugging_mode->ties_mode);
                }
                clearConsole();
            }
        }
        break;

    case 5:

        settings_debugging_mode->ties_mode = !(settings_debugging_mode->ties_mode);
        if (settings_debugging_mode->wins_mode == true) {
            settings_debugging_mode->wins_mode = !(settings_debugging_mode->wins_mode);
        }

        clearConsole();

        break;
    default:
        clearConsole();
        break;
    }
}

// Инициализация руки
void init_hand(Hand* hand, const Card& card1, const Card& card2) {
    hand->init_hand(card1, card2);
}

// Инициализация игрока
void init_player(Player* players, const Hand& hand) {
    players->init_player(hand, used_cards);
}

// Инициализация игрового поля
void init_board(Board* board) {
    board->init_board();
}

// Функция для перемешивания массива карт//
void shuffle_deck(Card* deck, int size) {
    for (int i = 0; i < size; i++) {
        int randomIndex = rand() % size;
        Card temp = deck[i];
        deck[i] = deck[randomIndex];
        deck[randomIndex] = temp;
    }
}

// Функция для создания полной колоды карт
void create_deck(Card* deck, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    int index = 0;
    for (int suit = HEARTS; suit <= SPADES; suit++) {
        for (int rank = TWO; rank <= ACE; rank++) {
            deck[index].init_card((Suit)suit, (Rank)rank, used_cards);
            index++;
        }
    }
}

// Функция для случайной генерации карты
Card generate_random_card(bool used_cards[15][4]) {
    Card card;
    Rank rank_choice;
    Suit suit_choice;

    do {
        rank_choice = (Rank)(rand() % NUM_RANKS + 2);  // Ранги от 2 до 14 (2-10, J, Q, K, A)
        suit_choice = (Suit)(rand() % NUM_SUITS + 1);  // Масти от 1 до 4
    } while (used_cards[rank_choice][suit_choice]);

    used_cards[rank_choice][suit_choice] = true;

    // Используем сеттеры для установки ранга и масти
    card.set_rank(rank_choice);
    card.set_suit(suit_choice);

    return card;
}


// Проверка на уникальность всех карт в массиве
bool is_unique_cards(Card* cards, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (cards[i].get_rank() == cards[j].get_rank() && cards[i].get_suit() == cards[j].get_suit()) {
                return false;
            }
        }
    }
    return true;
}



int compare_hands(PokerCombination hand1, PokerCombination hand2) {

    if (hand1.hand_rank > hand2.hand_rank) return 1;
    if (hand1.hand_rank < hand2.hand_rank) return -1;


    if (hand1.high_card > hand2.high_card) return 1;
    if (hand1.high_card < hand2.high_card) return -1;

    // сравниваем кикеры
    switch (hand1.hand_rank) {
    case STRAIGHT:
    case STRAIGHT_FLUSH:
        if (hand1.high_card == hand2.high_card) {
            // Compare the entire straight
            for (int i = 0; i < 5; i++) {
                if (hand1.kicker[i] > hand2.kicker[i]) return 1;
                if (hand1.kicker[i] < hand2.kicker[i]) return -1;
            }
            return 0;
        }
        break;

    case FOUR_OF_A_KIND:
    case FULL_HOUSE:
        if (hand1.kicker[0] > hand2.kicker[0]) return 1;
        if (hand1.kicker[0] < hand2.kicker[0]) return -1;
        break;

    case THREE_OF_A_KIND:
        // у трипса только один кикер //
        for (int i = 0; i < 2; i++) {
            if (hand1.kicker[i] > hand2.kicker[i]) return 1;
            if (hand1.kicker[i] < hand2.kicker[i]) return -1;
        }
        break;

    case ONE_PAIR:
    case TWO_PAIR:
        // для пары и двух пар используются три кикера //
        for (int i = 0; i < 3; i++) {
            if (hand1.kicker[i] > hand2.kicker[i]) return 1;
            if (hand1.kicker[i] < hand2.kicker[i]) return -1;
        }
        break;

    case FLUSH:
        // у флеша все пять карт сравниваются по убыванию //
        for (int i = 0; i < 5; i++) {
            if (hand1.kicker[i] > hand2.kicker[i]) return 1;
            if (hand1.kicker[i] < hand2.kicker[i]) return -1;
        }
        break;

    case HIGH_CARD:
        // у старшей карты все пять карт сравниваются //
        for (int i = 0; i < 5; i++) {
            if (hand1.kicker[i] > hand2.kicker[i]) return 1;
            if (hand1.kicker[i] < hand2.kicker[i]) return -1;
        }
        break;

    default:
        break;
    }

    return 0; // Ничья
}

PokerCombination determine_hand(Hand hand, Board board) {
    PokerCombination result;
    result.hand_rank = HIGH_CARD;
    result.high_card = NONE_RANK;
    for (int i = 0; i < 5; i++) {
        result.kicker[i] = NONE_RANK;
    }

    int card_count[15] = { 0 };
    int suit_count[5] = { 0 };
    Card all_cards[7];            // Массив всех карт (рука + борд) //

    // Собираем все карты (2 карты руки + карты на борде) //
    all_cards[0] = hand.get_card(0);
    all_cards[1] = hand.get_card(1);
    for (int i = 0; i < board.get_num_cards(); i++) {
        // Используем геттер для получения карты с доски
        all_cards[2 + i] = board.get_card(i);

    }

    // Подсчет количества карт по рангу и мастям //
    for (int i = 0; i < 2 + board.get_num_cards(); i++) {
        card_count[all_cards[i].get_rank()]++;
        suit_count[all_cards[i].get_suit()]++;
    }

    Rank top_pair = NONE_RANK, second_pair = NONE_RANK;
    Rank highest_rank = NONE_RANK;
    int flush_suit_index = -1;
    int three_of_a_kind = 0, second_three_of_a_kind = 0;
    int four_of_a_kind = 0;

    // Определение пар троек каре //
    for (int rank = ACE; rank >= TWO; rank--) {
        if (card_count[rank] == 4) {
            four_of_a_kind = rank;
        }
        else if (card_count[rank] == 3) {
            if (three_of_a_kind == 0) {
                three_of_a_kind = rank;
            }
            else {
                second_three_of_a_kind = rank;
            }
        }
        else if (card_count[rank] == 2) {
            if (top_pair == NONE_RANK) {
                top_pair = static_cast<Rank>(rank);
            }
            else if (second_pair == NONE_RANK) {
                second_pair = static_cast<Rank>(rank);
            }
        }
        else if (card_count[rank] == 1 && rank > highest_rank) {
            highest_rank = static_cast<Rank>(rank);
        }
    }

    // Опредееление наличия флеша //
    for (int suit = 0; suit < 5; suit++) {
        if (suit_count[suit] >= 5) {
            flush_suit_index = suit;
            break;
        }
    }

    // Проверка на фулл-хаус //
    if (three_of_a_kind > 0 && (top_pair > 0 || second_three_of_a_kind > 0)) {
        result.hand_rank = FULL_HOUSE;
        result.high_card = static_cast<Rank>(three_of_a_kind); // Старший сет
        result.kicker[0] = static_cast<Rank>((top_pair > 0) ? top_pair : second_three_of_a_kind); // Пара

        return result;
    }

    // Проверка на стрит //
    int consecutive = 0;
    for (int rank = ACE; rank >= TWO; rank--) {
        if (card_count[rank] > 0) {
            consecutive++;
            if (consecutive == 5) {
                result.hand_rank = STRAIGHT;
                result.high_card = static_cast<Rank>(rank + 4);
                return result;
            }
        }
        else {
            consecutive = 0;
        }
    }

    // Проверка на стрит A-2-3-4-5 //
    if (card_count[ACE] > 0 && card_count[TWO] > 0 && card_count[THREE] > 0 &&
        card_count[FOUR] > 0 && card_count[FIVE] > 0) {
        result.hand_rank = STRAIGHT;
        result.high_card = FIVE;
        return result;
    }

    // Проверка на стрит-флеш //
    if (flush_suit_index != -1) {
        Rank flush_cards[7];
        int flush_card_count = 0;

        // Собираем карты одной масти//
        for (int i = 0; i < 2 + board.get_num_cards(); i++) {
            if (all_cards[i].get_suit() == flush_suit_index) {
                flush_cards[flush_card_count++] = all_cards[i].get_rank();
            }
        }

        qsort(flush_cards, flush_card_count, sizeof(Rank), [](const void* a, const void* b) {
            return (*(Rank*)b - *(Rank*)a);
            });

        // Проверка на стрит-флеш //
        for (int i = 0; i < flush_card_count - 4; i++) {
            if (flush_cards[i] - flush_cards[i + 1] == 1 &&
                flush_cards[i + 1] - flush_cards[i + 2] == 1 &&
                flush_cards[i + 2] - flush_cards[i + 3] == 1 &&
                flush_cards[i + 3] - flush_cards[i + 4] == 1) {
                result.hand_rank = STRAIGHT_FLUSH;
                result.high_card = flush_cards[i];
                return result;
            }
        }

        // Определение флеша //
        result.hand_rank = FLUSH;
        result.high_card = flush_cards[0];
        for (int i = 1; i < 5; i++) {
            result.kicker[i - 1] = flush_cards[i];
        }
        return result;
    }



    // Проверка на каре //
    if (four_of_a_kind > 0) {
        result.hand_rank = FOUR_OF_A_KIND;
        result.high_card = static_cast<Rank>(four_of_a_kind);
        result.kicker[0] = highest_rank;
        return result;
    }

    // Проверка на тройку//
    if (three_of_a_kind > 0) {
        result.hand_rank = THREE_OF_A_KIND;
        result.high_card = static_cast<Rank>(three_of_a_kind);
        int kicker_index = 0;
        for (int rank = ACE; rank >= TWO && kicker_index < 2; rank--) {
            if (card_count[rank] == 1 && rank != three_of_a_kind) {
                result.kicker[kicker_index++] = static_cast<Rank>(rank);
            }
        }
        return result;
    }

    // Проверка на две пары//
    if (top_pair > 0 && second_pair > 0) {
        result.hand_rank = TWO_PAIR;
        result.high_card = top_pair;
        result.kicker[0] = second_pair;
        result.kicker[1] = highest_rank;
        return result;
    }

    // Проверка на пару//
    if (top_pair > 0) {
        result.hand_rank = ONE_PAIR;
        result.high_card = top_pair;
        int kicker_index = 0;
        for (int rank = ACE; rank >= TWO && kicker_index < 3; rank--) {
            if (card_count[rank] == 1 && rank != top_pair) {
                result.kicker[kicker_index++] = static_cast<Rank>(rank);
            }
        }
        return result;
    }

    // Если нет комбинаций то хай карта //
    result.hand_rank = HIGH_CARD;
    result.high_card = highest_rank;
    int kicker_index = 0;
    for (int rank = ACE; rank >= TWO && kicker_index < 4; rank--) {
        if (card_count[rank] == 1) {
            result.kicker[kicker_index++] = static_cast<Rank>(rank);
        }
    }

    return result;
}




void calculate_probabilities(Game* game, bool used_cards[15][4], int choice_numSimulations, Settings_debugging_mode* settings) {
    // победы, поражения и ничьи для каждого игрока
    for (int i = 0; i < game->get_current_players(); i++) {
        game->get_player(i).set_wins(0);
        game->get_player(i).set_ties(0);
        game->get_player(i).set_losses(0);
    }

    for (int i = 0; i < choice_numSimulations; i++) {
        Card deck[52];
        int deck_index = 0;

        // Временно копируем used_cards для каждой simulation
        bool sim_used_cards[15][4];
        memcpy(sim_used_cards, used_cards, sizeof(sim_used_cards));

        // Заполняем колоду, исключая карты из массива used_cards
        for (int rank = 2; rank <= 14; rank++) {
            for (int suit = 1; suit <= 4; suit++) {
                if (!sim_used_cards[rank][suit]) {
                    deck[deck_index].init_card((Suit)suit, (Rank)rank, used_cards);
                    deck_index++;
                }
            }
        }

        // Перемешиваем колоду
        for (int j = deck_index - 1; j > 0; j--) {
            int k = rand() % (j + 1);
            Card temp = deck[j];
            deck[j] = deck[k];
            deck[k] = temp;
        }

        Card simulation_board[5];

        // Копируем уже известные карты на борд
        for (int j = 0; j < game->get_board().get_num_cards(); j++) {
            simulation_board[j] = game->get_board().get_card(j);
        }

        for (int j = game->get_board().get_num_cards(); j < 5; j++) {
            bool card_found = false;
            while (!card_found) {
                simulation_board[j] = deck[--deck_index]; // Берём карту здесь
                // Проверяем, что карта не использована нигде
                if (!sim_used_cards[simulation_board[j].get_rank()][simulation_board[j].get_suit()]) {
                    sim_used_cards[simulation_board[j].get_rank()][simulation_board[j].get_suit()] = true;
                    card_found = true;
                }
            }
        }

        Board simulated_board;
        simulated_board.set_num_cards(5);
        for (int j = 0; j < 5; j++) {
            simulated_board.set_card(j, simulation_board[j], used_cards);
        }

        // Массив для хранения комбинаций всех игроков
        PokerCombination* player_hands = (PokerCombination*)malloc(game->get_current_players() * sizeof(PokerCombination));

        // Оценка комбинаций для каждого игрока
        for (int j = 0; j < game->get_current_players(); j++) {
            player_hands[j] = determine_hand(game->get_player(j).get_hand(), simulated_board);
        }

        // Находим лучшую комбинацию и проверяем на ничью
        int best_player = 0;
        bool tie = false;
        int tie_count = 1;

        for (int j = 1; j < game->get_current_players(); j++) {
            int comparison = compare_hands(player_hands[j], player_hands[best_player]);
            if (comparison > 0) {
                best_player = j;
                tie = false;
                tie_count = 1;
            }
            else if (comparison == 0) {
                tie = true;
                tie_count++;
            }
        }

        // Обновляем статистику
        for (int j = 0; j < game->get_current_players(); j++) {
            if (tie && compare_hands(player_hands[j], player_hands[best_player]) == 0) {
                game->get_player(j).set_ties(game->get_player(j).get_ties() + 1);
            }
            else if (j == best_player) {
                game->get_player(j).set_wins(game->get_player(j).get_wins() + 1);
            }
            else {
                game->get_player(j).set_losses(game->get_player(j).get_losses() + 1);
            }
        }

        // TEST_MODE
        calculate_probabilities_debugging(game, settings, simulated_board, player_hands, i, tie, best_player);

        free(player_hands);
    }

    // Выводим вероятности
    double total_simulations = (double)choice_numSimulations;

    for (int i = 0; i < game->get_current_players(); i++) {
        printf("------------------\n   Игрок %d\n------------------\n", i + 1);
        printf("Победы: %.2f%%\n", (game->get_player(i).get_wins() / total_simulations) * 100);
        printf("Поражения: %.2f%%\n", (game->get_player(i).get_losses() / total_simulations) * 100);
        printf("Ничьи: %.2f%%\n", (game->get_player(i).get_ties() / total_simulations) * 100);
    }
}


void calculate_probabilities_debugging(Game* game, Settings_debugging_mode* settings, Board simulated_board, PokerCombination* player_hands, int current_simulation, bool tie, int best_player) {
    if (settings->wins_mode == true) {
        if (best_player + 1 == settings->current_winner) {
            printf("Симуляция %d:\n", current_simulation + 1);
            printf("Карты на столе: ");
            for (int j = 0; j < 5; j++) {
                printf("%d%c ", simulated_board.get_card(j).get_rank(), simulated_board.get_card(j).get_suit() == HEARTS ? 'Ч' :
                    simulated_board.get_card(j).get_suit() == DIAMONDS ? 'Б' :
                    simulated_board.get_card(j).get_suit() == CLUBS ? 'Т' :
                    simulated_board.get_card(j).get_suit() == SPADES ? 'П' : '?');
            }
            printf("\n");

            for (int j = 0; j < game->get_current_players(); j++) {
                printf("Игрок %d: комбинация %d, старшая карта: %d\n", j + 1, player_hands[j].hand_rank, player_hands[j].high_card);
            }

            printf("Игрок %d победил.\n", best_player + 1);
            printf("\n");
        }
    }
    else {
        if (best_player + 1 == settings->current_winner && settings->current_winner != -1) {
            printf("Симуляция %d:\n", current_simulation + 1);
            printf("Карты на столе: ");
            for (int j = 0; j < 5; j++) {
                printf("%d%c ", simulated_board.get_card(j).get_rank(), simulated_board.get_card(j).get_suit() == HEARTS ? 'Ч' :
                    simulated_board.get_card(j).get_suit() == DIAMONDS ? 'Б' :
                    simulated_board.get_card(j).get_suit() == CLUBS ? 'Т' :
                    simulated_board.get_card(j).get_suit() == SPADES ? 'П' : '?');
            }
            printf("\n");

            if (tie) {
                printf("Ничья между игроками.\n");
            }

            printf("\n");
        }
    }
}

// Сравнивает комбинации всех игроков
void compare_all_hands(Game* game, PokerCombination hands[]) {
    for (int i = 0; i < game->get_current_players(); i++) {
        for (int j = i + 1; j < game->get_current_players(); j++) {
            int result = compare_hands(hands[i], hands[j]);

            if (result == 1) {
                // Игрок i побеждает игрока j
                game->get_player(i).set_wins(game->get_player(i).get_wins() + 1);
                game->get_player(j).set_losses(game->get_player(j).get_losses() + 1);
            }
            else if (result == -1) {
                // Игрок j побеждает игрока i
                game->get_player(j).set_wins(game->get_player(j).get_wins() + 1);
                game->get_player(i).set_losses(game->get_player(i).get_losses() + 1);
            }
            else {
                // Ничья
                game->get_player(i).set_ties(game->get_player(i).get_ties() + 1);
                game->get_player(j).set_ties(game->get_player(j).get_ties() + 1);
            }
        }
    }
}


