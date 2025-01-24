#define _CRT_SECURE_NO_WARNINGS
#include "Functions.h"

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

void print_player_hand(Game* game, int player_index) {
    if (player_index < 0 || player_index >= game->get_current_players()) {
        printf("Неверный индекс игрока.\n");
        return;
    }

    Hand hand = game->get_player(player_index).get_hand();
    Board board = game->get_board();
    PokerCombination combination = determine_hand(hand, board);
    print_hand(combination);
}

// Функция для перемешивания массива карт
void shuffle_deck(Card* deck, int size) {
    for (int i = 0; i < size; i++) {
        int randomIndex = rand() % size;
        Card temp = deck[i];
        deck[i] = deck[randomIndex];
        deck[randomIndex] = temp;
    }
}

bool is_deck_valid(Card deck[], int size) {
    for (int i = 0; i < size; i++) {
        if (!deck[i].is_valid()) {
            fprintf(stderr, "Ошибка: Карта с индексом %d невалидна.\n", i);
            return false;
        }
    }
    return true;
}



void create_deck(Card deck[], bool used_cards[NUM_RANKS][NUM_SUITS]) {
    int index = 0;

    // Проходим по всем комбинациям рангов и мастей
    for (int rank = 0; rank < NUM_RANKS; rank++) {
        for (int suit = 0; suit < NUM_SUITS; suit++) {
            // Пропускаем карты, которые уже используются
            if (used_cards[rank][suit]) continue;

            // Создаем новую карту
            Card new_card(static_cast<Suit>(suit), static_cast<Rank>(rank));

            // Проверяем валидность карты
            if (!new_card.is_valid()) {
                fprintf(stderr, "Ошибка: Не удалось создать карту %d %d.\n", rank, suit);
                continue;
            }

            // Добавляем карту в колоду
            deck[index++] = new_card;
        }
    }

    // Проверяем, что сформированная колода имеет правильное количество карт
    if (index != 52) {
    //Отладочный код
    }
}



// Функция для случайной генерации карты
Card generate_random_card(bool used_cards[NUM_RANKS][NUM_SUITS]) {
    Card card;
    Rank rank_choice;
    Suit suit_choice;

    do {
        rank_choice = (Rank)(rand() % NUM_RANKS);  // Ранги от 0 до 12 (2-14: 2-10, J, Q, K, A)
        suit_choice = (Suit)(rand() % NUM_SUITS);  // Масти от 0 до 3
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

    // Сравниваем старшую карту комбинации
    if (hand1.high_card > hand2.high_card) return 1;
    if (hand1.high_card < hand2.high_card) return -1;

    // Сравнение кикеров (для всех комбинаций с кикерами)
    for (int i = 0; i < 5; i++) {
        if (hand1.kicker[i] > hand2.kicker[i]) return 1;
        if (hand1.kicker[i] < hand2.kicker[i]) return -1;
    }

    return 0; // Ничья
}


PokerCombination determine_hand(Hand hand, Board board) {
    PokerCombination result;
    result.hand_rank = HIGH_CARD;
    result.high_card = NONE_RANK;
    std::fill(std::begin(result.kicker), std::end(result.kicker), NONE_RANK);

    int card_count[NUM_RANKS] = { 0 };
    int suit_count[NUM_SUITS] = { 0 };
    Card all_cards[7];

    // Собираем все карты
    all_cards[0] = hand.get_card(0);
    all_cards[1] = hand.get_card(1);
    for (int i = 0; i < board.get_num_cards(); i++) {
        all_cards[2 + i] = board.get_card(i);
    }

    for (int i = 0; i < 2 + board.get_num_cards(); i++) {
        Rank rank = all_cards[i].get_rank();
        Suit suit = all_cards[i].get_suit();

        if (rank < 0 || rank >= NUM_RANKS || suit < 0 || suit >= NUM_SUITS) {
            continue;
        }

        card_count[rank]++;
        suit_count[suit]++;
    }

    Rank four_of_a_kind = NONE_RANK, three_of_a_kind = NONE_RANK;
    Rank top_pair = NONE_RANK, second_pair = NONE_RANK, highest_card = NONE_RANK;
    int flush_suit = -1;

    for (int rank = ACE; rank >= TWO; rank--) {
        if (card_count[rank] == 4) {
            four_of_a_kind = static_cast<Rank>(rank);
        }
        else if (card_count[rank] == 3) {
            three_of_a_kind = (three_of_a_kind == NONE_RANK) ? static_cast<Rank>(rank) : three_of_a_kind;
        }
        else if (card_count[rank] == 2) {
            if (top_pair == NONE_RANK) {
                top_pair = static_cast<Rank>(rank);
            }
            else if (second_pair == NONE_RANK) {
                second_pair = static_cast<Rank>(rank);
            }
        }
        if (card_count[rank] > 0 && highest_card == NONE_RANK) {
            highest_card = static_cast<Rank>(rank);
        }
    }

    for (int suit = 0; suit < NUM_SUITS; suit++) {
        if (suit_count[suit] >= 5) {
            flush_suit = suit;
            break;
        }
    }

    int consecutive = 0;
    Rank straight_high_card = NONE_RANK;
    for (int rank = ACE; rank >= TWO; rank--) {
        if (card_count[rank] > 0) {
            consecutive++;
            if (consecutive == 5) {
                straight_high_card = static_cast<Rank>(rank + 4);
                break;
            }
        }
        else {
            consecutive = 0;
        }
    }
    if (straight_high_card == NONE_RANK && card_count[ACE] && card_count[TWO] && card_count[THREE] && card_count[FOUR] && card_count[FIVE]) {
        straight_high_card = FIVE;
    }

    if (flush_suit != -1) {
        std::vector<Rank> flush_cards;
        for (int i = 0; i < 7; i++) {
            if (all_cards[i].get_suit() == flush_suit) {
                flush_cards.push_back(all_cards[i].get_rank());
            }
        }
        std::sort(flush_cards.begin(), flush_cards.end(), std::greater<Rank>());

        // Проверка на стрит-флеш
        consecutive = 0;
        for (size_t i = 0; i < flush_cards.size() - 1; i++) {
            if (flush_cards[i] - flush_cards[i + 1] == 1) {
                consecutive++;
                if (consecutive == 4) { // 5 карт подряд
                    result.hand_rank = STRAIGHT_FLUSH;
                    result.high_card = flush_cards[i - 3]; // Старшая карта в стрит-флеше
                    return result;
                }
            }
            else {
                consecutive = 0;
            }
        }
    }


    if (four_of_a_kind != NONE_RANK) {
        result.hand_rank = FOUR_OF_A_KIND;
        result.high_card = four_of_a_kind;
        result.kicker[0] = highest_card;
        return result;
    }

    if (three_of_a_kind != NONE_RANK && (top_pair != NONE_RANK || second_pair != NONE_RANK)) {
        result.hand_rank = FULL_HOUSE;
        result.high_card = three_of_a_kind;
        result.kicker[0] = (top_pair != NONE_RANK) ? top_pair : second_pair;
        return result;
    }

    if (flush_suit != -1) {
        result.hand_rank = FLUSH;
        int flush_index = 0;
        for (int i = 0; i < 7; i++) {
            if (all_cards[i].get_suit() == flush_suit) {
                result.kicker[flush_index++] = all_cards[i].get_rank();
                if (flush_index == 5) break;
            }
        }
        result.high_card = result.kicker[0];
        return result;
    }

    if (straight_high_card != NONE_RANK) {
        result.hand_rank = STRAIGHT;
        result.high_card = straight_high_card;
        return result;
    }

    if (three_of_a_kind != NONE_RANK) {
        result.hand_rank = THREE_OF_A_KIND;
        result.high_card = three_of_a_kind;
        return result;
    }

    if (top_pair != NONE_RANK && second_pair != NONE_RANK) {
        result.hand_rank = TWO_PAIR;
        result.high_card = top_pair;
        result.kicker[0] = second_pair;
        result.kicker[1] = highest_card;
        return result;
    }

    if (top_pair != NONE_RANK) {
        result.hand_rank = ONE_PAIR;
        result.high_card = top_pair;
        return result;
    }

    result.hand_rank = HIGH_CARD;
    result.high_card = highest_card;
    return result;
}


void calculate_probabilities(Game* game, const bool used_cards[NUM_RANKS][NUM_SUITS], int choice_numSimulations, Settings_debugging_mode* settings) {
    // Сброс статистики
    for (int i = 0; i < game->get_current_players(); i++) {
        game->get_player(i).set_wins(0);
        game->get_player(i).set_ties(0);
        game->get_player(i).set_losses(0);
    }

    // Основной цикл симуляции
    for (int sim = 0; sim < choice_numSimulations; sim++) {
        bool sim_used_cards[NUM_RANKS][NUM_SUITS];
        memcpy(sim_used_cards, used_cards, sizeof(bool) * NUM_RANKS * NUM_SUITS);

        Card deck[52];
        create_deck(deck, sim_used_cards);
        shuffle_deck(deck, 52);

        Card simulation_board[5];
        int num_existing_board_cards = game->get_board().get_num_cards();

        if (num_existing_board_cards > 5) {
            fprintf(stderr, "Ошибка: недопустимое количество карт на борде (%d)\n", num_existing_board_cards);
            continue;
        }

        for (int j = 0; j < num_existing_board_cards; j++) {
            simulation_board[j] = game->get_board().get_card(j);
        }

        int sim_board_index = num_existing_board_cards;
        int deck_index = 0;
        while (sim_board_index < 5 && deck_index < 52) {
            simulation_board[sim_board_index] = deck[deck_index++];
            Rank rank = simulation_board[sim_board_index].get_rank();
            Suit suit = simulation_board[sim_board_index].get_suit();
            sim_used_cards[rank][suit] = true;
            sim_board_index++;
        }

        if (sim_board_index != 5) {
            fprintf(stderr, "Ошибка: доска заполнена некорректно. sim_board_index = %d\n", sim_board_index);
            continue;
        }

        Board simulated_board;
        simulated_board.set_num_cards(5);
        for (int j = 0; j < 5; j++) {
            simulated_board.set_card(j, simulation_board[j]);
        }

        // Используем std::vector вместо динамического массива
        std::vector<PokerCombination> player_hands(game->get_current_players());

        for (int j = 0; j < game->get_current_players(); j++) {
            player_hands[j] = determine_hand(game->get_player(j).get_hand(), simulated_board);
        }

        int best_player = 0;
        bool tie = false;

        for (int j = 1; j < game->get_current_players(); j++) {
            int comparison = compare_hands(player_hands[j], player_hands[best_player]);
            if (comparison > 0) {
                best_player = j;
                tie = false;
            }
            else if (comparison == 0) {
                tie = true;
            }
        }

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
    }

    double total_simulations = (double)choice_numSimulations;
    for (int i = 0; i < game->get_current_players(); i++) {
        printf("------------------\n   Игрок %d\n------------------\n", i + 1);
        printf("Победы: %.2f%%\n", (game->get_player(i).get_wins() / total_simulations) * 100);
        printf("Поражения: %.2f%%\n", (game->get_player(i).get_losses() / total_simulations) * 100);
        printf("Ничьи: %.2f%%\n", (game->get_player(i).get_ties() / total_simulations) * 100);
    }
}


void initialize_used_cards(Game* game, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    // Очищаем массив
    memset(used_cards, false, sizeof(bool) * NUM_RANKS * NUM_SUITS);

    // Помечаем карты игроков как использованные
    for (int i = 0; i < game->get_current_players(); i++) {
        Card first = game->get_player(i).get_hand().get_card(0);
        Card second = game->get_player(i).get_hand().get_card(1);

        if (first.is_valid()) {
            used_cards[first.get_rank()][first.get_suit()] = true;
        }
        if (second.is_valid()) {
            used_cards[second.get_rank()][second.get_suit()] = true;
        }
    }

    // Помечаем карты на борде как использованные
    for (int i = 0; i < game->get_board().get_num_cards(); i++) {
        Card board_card = game->get_board().get_card(i);
        if (board_card.is_valid()) {
            used_cards[board_card.get_rank()][board_card.get_suit()] = true;
        }
    }
}


void calculate_probabilities_debugging(Game* game, Settings_debugging_mode* settings, Board simulated_board, PokerCombination* player_hands, int current_simulation, bool tie, int best_player) {
    if (settings->wins_mode == true) {
        if (best_player + 1 == settings->current_winner) {
            printf("Симуляция %d:\n", current_simulation + 1);
            printf("Карты на столе: ");
            for (int j = 0; j < 5; j++) {
                printf("%d%s ", simulated_board.get_card(j).get_rank() + 2, simulated_board.get_card(j).get_suit() == HEARTS ? "Чeрви" :
                    simulated_board.get_card(j).get_suit() == DIAMONDS ? "Бубны" :
                    simulated_board.get_card(j).get_suit() == CLUBS ? "Трефы" :
                    simulated_board.get_card(j).get_suit() == SPADES ? "Пики" : "?");
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
                printf("%d%s ", simulated_board.get_card(j).get_rank() + 2, simulated_board.get_card(j).get_suit() == HEARTS ? "Чeрви" :
                    simulated_board.get_card(j).get_suit() == DIAMONDS ? "Бубны" :
                    simulated_board.get_card(j).get_suit() == CLUBS ? "Трефы" :
                    simulated_board.get_card(j).get_suit() == SPADES ? "Пики" : "?");
            }
            printf("\n");

            if (tie) {
                printf("Ничья между игроками.\n");
            }

            printf("\n");
        }
    }

    // Вывод комбинаций для определенного игрока
    if (settings->debug_player_index >= 0 && settings->debug_player_index < game->get_current_players()) {
        printf("Комбинация игрока %d: ", settings->debug_player_index + 1);
        print_hand(player_hands[settings->debug_player_index]);
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
