#define _CRT_SECURE_NO_WARNINGS
#include "Functions.h"

void print_hand(PokerCombination hand) {
    switch (hand.hand_rank) {
    case STRAIGHT_FLUSH:
        printf("����������: �����-����\n");
        printf("������� �����: %d\n", hand.high_card + 2);
        break;
    case FOUR_OF_A_KIND:
        printf("����������: ���� %d\n", hand.high_card + 2);
        printf("�����: %d\n", hand.kicker[0] + 2);
        break;
    case FULL_HOUSE:
        printf("����������: ����-����\n");
        printf("������: %d\n", hand.high_card + 2);
        printf("����: %d\n", hand.kicker[0] + 2);
        break;
    case FLUSH:
        printf("����������: ����\n");
        printf("������� �����: %d\n", hand.high_card + 2);
        break;
    case STRAIGHT:
        printf("����������: �����\n");
        printf("������� �����: %d\n", hand.high_card + 2);
        break;
    case THREE_OF_A_KIND:
        if (hand.kicker[0] == NONE_RANK) {
            printf("����������: ��� %d\n", hand.high_card + 2);
        }
        else {
            printf("����������: ����� %d\n", hand.high_card + 2);
            printf("�����: %d\n", hand.kicker[0] + 2);
        }
        break;
    case TWO_PAIR:
        printf("����������: ��� ����\n");
        printf("������� ����: %d\n", hand.high_card + 2);
        printf("������� ����: %d\n", hand.kicker[0] + 2);
        break;
    case ONE_PAIR:
        if (hand.kicker[0] == NONE_RANK && hand.kicker[1] == NONE_RANK) {
            printf("����������: ��������� ���� %d\n", hand.high_card + 2);
        }
        else {
            printf("����������: ���� %d\n", hand.high_card + 2);
            printf("�����: %d\n", hand.kicker[0] + 2);
        }
        break;
    case HIGH_CARD:
        printf("����������: ������� ����� %d\n", hand.high_card + 2);
        printf("�����: %d\n", hand.kicker[0] + 2);
        break;
    default:
        printf("����������� ����������\n");
        break;
    }
}

void print_player_hand(Game* game, int player_index) {
    if (player_index < 0 || player_index >= game->get_current_players()) {
        printf("�������� ������ ������.\n");
        return;
    }

    Hand hand = game->get_player(player_index).get_hand();
    Board board = game->get_board();
    PokerCombination combination = determine_hand(hand, board);
    print_hand(combination);
}

// ������� ��� ������������� ������� ����
void shuffle_deck(Card* deck, int size) {
    for (int i = 0; i < size; i++) {
        int randomIndex = rand() % size;
        Card temp = deck[i];
        deck[i] = deck[randomIndex];
        deck[randomIndex] = temp;
    }
}

// ������� ��� �������� ������ ������ ����
void create_deck(Card* deck, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    int index = 0;
    for (int suit = HEARTS; suit <= SPADES; suit++) {
        for (int rank = TWO; rank <= ACE; rank++) {
            deck[index].init_card((Suit)suit, (Rank)rank, used_cards);
            index++;
        }
    }
}

// ������� ��� ��������� ��������� �����
Card generate_random_card(bool used_cards[NUM_RANKS][NUM_SUITS]) {
    Card card;
    Rank rank_choice;
    Suit suit_choice;

    do {
        rank_choice = (Rank)(rand() % NUM_RANKS);  // ����� �� 0 �� 12 (2-14: 2-10, J, Q, K, A)
        suit_choice = (Suit)(rand() % NUM_SUITS);  // ����� �� 0 �� 3
    } while (used_cards[rank_choice][suit_choice]);

    used_cards[rank_choice][suit_choice] = true;

    // ���������� ������� ��� ��������� ����� � �����
    card.set_rank(rank_choice);
    card.set_suit(suit_choice);

    return card;
}

bool is_unique_cards(Card* cards, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (cards[i].get_rank() == cards[j].get_rank() &&
                cards[i].get_suit() == cards[j].get_suit()) {
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

    // ������� �������� ��������� ������� ��� ������
    if (hand1.hand_rank == STRAIGHT || hand1.hand_rank == STRAIGHT_FLUSH) {
        return 0; // ��� ������ ����� ������������ ������ �� ������� �����
    }

    // ���������� ������
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
        // � ������ ������ ���� �����
        if (hand1.kicker[0] > hand2.kicker[0]) return 1;
        if (hand1.kicker[0] < hand2.kicker[0]) return -1;
        break;

    case ONE_PAIR:
        // ���������� ������� ����
        if (hand1.high_card > hand2.high_card) return 1;
        if (hand1.high_card < hand2.high_card) return -1;

        // ���������� ������
        for (int i = 0; i < 3; i++) {
            if (hand1.kicker[i] > hand2.kicker[i]) return 1;
            if (hand1.kicker[i] < hand2.kicker[i]) return -1;
        }
        break;


    case TWO_PAIR:
        // ��� ���� � ���� ��� ������������ ��� ������
        for (int i = 0; i < 3; i++) {
            if (hand1.kicker[i] > hand2.kicker[i]) return 1;
            if (hand1.kicker[i] < hand2.kicker[i]) return -1;
        }
        break;

    case FLUSH:
        // � ����� ��� ���� ���� ������������ �� ��������
        for (int i = 0; i < 5; i++) {
            if (hand1.kicker[i] > hand2.kicker[i]) return 1;
            if (hand1.kicker[i] < hand2.kicker[i]) return -1;
        }
        break;

    case HIGH_CARD:
        // � ������� ����� ��� ���� ���� ������������
        for (int i = 0; i < 5; i++) {
            if (hand1.kicker[i] > hand2.kicker[i]) return 1;
            if (hand1.kicker[i] < hand2.kicker[i]) return -1;
        }
        break;

    default:
        break;
    }

    return 0; // �����
}

PokerCombination determine_hand(Hand hand, Board board) {
    PokerCombination result;
    result.hand_rank = HIGH_CARD;
    result.high_card = NONE_RANK;
    std::fill(std::begin(result.kicker), std::end(result.kicker), NONE_RANK);

    int card_count[NUM_RANKS] = { 0 };
    int suit_count[NUM_SUITS] = { 0 };
    Card all_cards[7];

    // �������� ��� �����
    int total_cards = 2 + board.get_num_cards();
    if (total_cards > 7) return result; // ������������ ���������� ����
    all_cards[0] = hand.get_card(0);
    all_cards[1] = hand.get_card(1);
    for (int i = 0; i < board.get_num_cards(); i++) {
        all_cards[2 + i] = board.get_card(i);
    }

    // ������������ ����� � �����
    for (int i = 0; i < total_cards; i++) {
        Rank rank = all_cards[i].get_rank();
        Suit suit = all_cards[i].get_suit();
        if (rank != NONE_RANK && suit != NONE_SUIT) {
            card_count[rank]++;
            suit_count[suit]++;
        }
    }

    // ����������� ������
    int consecutive = 0;
    Rank straight_high_card = NONE_RANK;
    for (int rank = ACE; rank >= TWO; rank--) {
        if (card_count[rank] > 0) {
            consecutive++;
            if (consecutive >= 5) {
                straight_high_card = static_cast<Rank>(rank + 4); // ������ ����� ������
                break;
            }
        }
        else {
            consecutive = 0;
        }
    }
    // �������� �� "��������" ����� (A, 2, 3, 4, 5)
    if (card_count[ACE] && card_count[TWO] && card_count[THREE] && card_count[FOUR] && card_count[FIVE]) {
        straight_high_card = FIVE;
    }

    // ����������� �����
    int flush_suit = -1;
    for (int suit = 0; suit < NUM_SUITS; suit++) {
        if (suit_count[suit] >= 5) {
            flush_suit = suit;
            break;
        }
    }

    if (flush_suit != -1 && straight_high_card != NONE_RANK) {
        // �������� �� �����-����
        int flush_cards_count = 0;
        Rank flush_ranks[5] = { NONE_RANK };
        for (int i = 0; i < total_cards; i++) {
            if (all_cards[i].get_suit() == flush_suit) {
                flush_ranks[flush_cards_count++] = all_cards[i].get_rank();
                if (flush_cards_count == 5) break;
            }
        }
        std::sort(flush_ranks, flush_ranks + flush_cards_count, std::greater<Rank>());

        consecutive = 0;
        for (int i = 0; i < flush_cards_count; i++) {
            if (i > 0 && flush_ranks[i] == flush_ranks[i - 1] - 1) {
                consecutive++;
            }
            else {
                consecutive = 1;
            }
            if (consecutive == 5) {
                result.hand_rank = STRAIGHT_FLUSH;
                result.high_card = flush_ranks[i - 4];
                return result;
            }
        }
    }

    if (flush_suit != -1) {
        // ����
        result.hand_rank = FLUSH;
        int flush_cards_count = 0;
        for (int i = 0; i < total_cards; i++) {
            if (all_cards[i].get_suit() == flush_suit) {
                result.kicker[flush_cards_count++] = all_cards[i].get_rank();
                if (flush_cards_count == 5) break;
            }
        }
        std::sort(result.kicker, result.kicker + 5, std::greater<Rank>());
        result.high_card = result.kicker[0];
        return result;
    }

    if (straight_high_card != NONE_RANK) {
        // �����
        result.hand_rank = STRAIGHT;
        result.high_card = straight_high_card;
        return result;
    }

    // ����
    Rank four_of_a_kind = NONE_RANK;
    for (int rank = ACE; rank >= TWO; rank--) {
        if (card_count[rank] == 4) {
            four_of_a_kind = static_cast<Rank>(rank);
            break;
        }
    }
    if (four_of_a_kind != NONE_RANK) {
        result.hand_rank = FOUR_OF_A_KIND;
        result.high_card = four_of_a_kind;
        for (int rank = ACE; rank >= TWO; rank--) {
            if (card_count[rank] > 0 && rank != four_of_a_kind) {
                result.kicker[0] = static_cast<Rank>(rank);
                break;
            }
        }
        return result;
    }

    // ����-����
    Rank three_of_a_kind = NONE_RANK, pair = NONE_RANK;
    for (int rank = ACE; rank >= TWO; rank--) {
        if (card_count[rank] >= 3 && three_of_a_kind == NONE_RANK) {
            three_of_a_kind = static_cast<Rank>(rank);
        }
        else if (card_count[rank] >= 2) {
            if (pair == NONE_RANK || pair < rank) {
                pair = static_cast<Rank>(rank);
            }
        }
    }
    if (three_of_a_kind != NONE_RANK && pair != NONE_RANK) {
        result.hand_rank = FULL_HOUSE;
        result.high_card = three_of_a_kind;
        result.kicker[0] = pair;
        return result;
    }

    // �����
    if (three_of_a_kind != NONE_RANK) {
        result.hand_rank = THREE_OF_A_KIND;
        result.high_card = three_of_a_kind;
        int kicker_count = 0;
        for (int rank = ACE; rank >= TWO && kicker_count < 2; rank--) {
            if (card_count[rank] > 0 && rank != three_of_a_kind) {
                result.kicker[kicker_count++] = static_cast<Rank>(rank);
            }
        }
        return result;
    }

    // ��� ����
    Rank top_pair = NONE_RANK, second_pair = NONE_RANK;
    for (int rank = ACE; rank >= TWO; rank--) {
        if (card_count[rank] == 2) {
            if (top_pair == NONE_RANK) {
                top_pair = static_cast<Rank>(rank);
            }
            else if (second_pair == NONE_RANK) {
                second_pair = static_cast<Rank>(rank);
            }
        }
    }
    if (top_pair != NONE_RANK && second_pair != NONE_RANK) {
        result.hand_rank = TWO_PAIR;
        result.high_card = top_pair;
        result.kicker[0] = second_pair;
        for (int rank = ACE; rank >= TWO; rank--) {
            if (card_count[rank] > 0 && rank != top_pair && rank != second_pair) {
                result.kicker[1] = static_cast<Rank>(rank);
                break;
            }
        }
        return result;
    }

    // ���� ����
    if (top_pair != NONE_RANK) {
        result.hand_rank = ONE_PAIR;
        result.high_card = top_pair;
        int kicker_count = 0;
        for (int rank = ACE; rank >= TWO && kicker_count < 3; rank--) {
            if (card_count[rank] > 0 && rank != top_pair) {
                result.kicker[kicker_count++] = static_cast<Rank>(rank);
            }
        }
        return result;
    }

    // ������� �����
    int kicker_count = 0;
    for (int rank = ACE; rank >= TWO && kicker_count < 5; rank--) {
        if (card_count[rank] > 0) {
            result.kicker[kicker_count++] = static_cast<Rank>(rank);
        }
    }
    result.high_card = result.kicker[0];
    return result;
}


void calculate_probabilities(Game* game, bool used_cards[NUM_RANKS][NUM_SUITS], int choice_numSimulations, Settings_debugging_mode* settings) {
    // ����� ����������
    for (int i = 0; i < game->get_current_players(); i++) {
        game->get_player(i).set_wins(0);
        game->get_player(i).set_ties(0);
        game->get_player(i).set_losses(0);
    }



    // �������� ���� ���������
    for (int sim = 0; sim < choice_numSimulations; sim++) {
        Card deck[52];
        int deck_index = 0;

        // ���������� ������, �������� ��� �������������� �����
        for (int rank = 0; rank < NUM_RANKS; rank++) {
            for (int suit = 0; suit < NUM_SUITS; suit++) {
                if (!used_cards[rank][suit]) {
                    deck[deck_index].init_card((Suit)suit, (Rank)rank, used_cards);
                    deck_index++;
                }
            }
        }

        // ������������� ������
        for (int j = deck_index - 1; j > 0; j--) {
            int k = rand() % (j + 1);
            std::swap(deck[j], deck[k]);
        }

        // ������������ ������������� �����
        Card simulation_board[5];

        int num_existing_board_cards = game->get_board().get_num_cards();

        // ��������� ������������ ���������� ���� �����
        if (num_existing_board_cards > 5) {
            fprintf(stderr, "������: ������������ ���������� ���� �� ����� (%d)\n", num_existing_board_cards);
            continue;
        }

        // ����������� ������������ ���� �����
        for (int j = 0; j < num_existing_board_cards; j++) {
            simulation_board[j] = game->get_board().get_card(j);
        }

        // ���������� ����������� ���� �� ������
        int sim_board_index = num_existing_board_cards;
        while (sim_board_index < 5 && deck_index > 0) {
            simulation_board[sim_board_index++] = deck[--deck_index];
        }

        // ���������, ��� ������������� ����� ��������� ���������
        if (sim_board_index != 5) {
            fprintf(stderr, "������: ����� ��������� �����������. sim_board_index = %d\n", sim_board_index);
            continue;
        }

        // �������� ������� ����� ��� ���������
        Board simulated_board;
        simulated_board.set_num_cards(5);

        for (int j = 0; j < 5; j++) {
            simulated_board.set_card(j, simulation_board[j]);
        }

        // ������ ��� �������� ���������� ���� �������
        PokerCombination* player_hands = (PokerCombination*)malloc(game->get_current_players() * sizeof(PokerCombination));

        // ��������� ���������� ������� � ������� ������
        for (int j = 0; j < game->get_current_players(); j++) {
            player_hands[j] = determine_hand(game->get_player(j).get_hand(), simulation_board);
        }

        // ����� ������ ���������� � ��������� ������
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

        // ���������� ����������
        for (int j = 0; j < game->get_current_players(); j++) {
            if (tie && compare_hands(player_hands[j], player_hands[best_player]) == 0) {
                game->get_player(j).set_ties(game->get_player(j).get_ties() + 1);
                settings->tie = true;
            }
            else if (j == best_player) {
                game->get_player(j).set_wins(game->get_player(j).get_wins() + 1);
            }
            else {
                game->get_player(j).set_losses(game->get_player(j).get_losses() + 1);
            }
        }
         
        calculate_probabilities_debugging(game, settings, simulation_board, player_hands, sim, tie, best_player);

        // ������������ ������
        free(player_hands);
    }

    // ����� �����������
    double total_simulations = (double)choice_numSimulations;




    for (int i = 0; i < game->get_current_players(); i++) {
        printf("------------------\n   ����� %d\n------------------\n", i + 1);
        printf("������: %.2f%%\n", (game->get_player(i).get_wins() / total_simulations) * 100);
        printf("���������: %.2f%%\n", (game->get_player(i).get_losses() / total_simulations) * 100);
        printf("�����: %.2f%%\n", (game->get_player(i).get_ties() / total_simulations) * 100);
    }
}

void debug_board(const Board& board) {
    printf("������� ����� (������� debug_board):\n");
    for (int i = 0; i < 5; i++) {
        const Card& card = board.get_card(i);
        if (card.is_valid()) {
            printf("����� %d: %s %s\n", i + 1, card.get_rank_name(), card.get_suit_name());
        }
        else {
            printf("����� %d: ����������\n", i + 1);
        }
    }
}
void calculate_probabilities_debugging(Game* game, Settings_debugging_mode* settings, Board simulated_board, PokerCombination* player_hands, int current_simulation, bool tie, int best_player) {
    // ����������� ���������� ��� ������
    if (settings->wins_visible_mode == true) {
        if (best_player + 1 == settings->current_winner) {
            printf("��������� %d:\n", current_simulation + 1);
            printf("����� �� �����: ");
            for (int j = 0; j < 5; j++) {
                const Card& card = simulated_board.get_card(j);
                if (card.is_valid()) {
                    printf("%d%s ",
                        card.get_rank() + 2,
                        card.get_suit() == HEARTS ? " �����" :
                        card.get_suit() == DIAMONDS ? " �����" :
                        card.get_suit() == CLUBS ? " �����" :
                        card.get_suit() == SPADES ? " ����" : "?");
                }
                else {
                    printf("���������� ");
                }
            }
            printf("\n");

            // ����� ���������� � ����������� �������
            for (int j = 0; j < game->get_current_players(); j++) {
                const Player& player = game->get_player(j);
                printf("����� %d:\n", j + 1);
                print_hand(player_hands[j]); // ���������� ���� ������� print_hand ��� ����������� ����������
            }

            printf("����� %d �������.\n\n", best_player + 1);
        
        }
    }
    else if (settings->ties_visible_mode == true) {
        if (settings->tie = true) {
            settings->tie = false;
            printf("��������� %d:\n", current_simulation + 1);
            printf("����� �� �����: ");
            for (int j = 0; j < 5; j++) {
                printf("%d%s ",
                    simulated_board.get_card(j).get_rank() + 2,
                    simulated_board.get_card(j).get_suit() == HEARTS ? " �����" :
                    simulated_board.get_card(j).get_suit() == DIAMONDS ? " �����" :
                    simulated_board.get_card(j).get_suit() == CLUBS ? " �����" :
                    simulated_board.get_card(j).get_suit() == SPADES ? " ����" : "?");
            }
            printf("\n");

            // ����� ���������� � ����������� �������
            for (int j = 0; j < game->get_current_players(); j++) {
                const Player& player = game->get_player(j);
                printf("����� %d:\n", j + 1);
                print_hand(player_hands[j]); // ���������� ���� ������� print_hand ��� ����������� ����������
            }

            printf("\n");
        }
    }
    else if (settings->simulations_visible_mode = false) {
        printf("��������� %d:\n", current_simulation + 1);
        printf("����� �� �����: ");
        for (int j = 0; j < 5; j++) {
            printf("%d%s ",
                simulated_board.get_card(j).get_rank() + 2,
                simulated_board.get_card(j).get_suit() == HEARTS ? "�����" :
                simulated_board.get_card(j).get_suit() == DIAMONDS ? "�����" :
                simulated_board.get_card(j).get_suit() == CLUBS ? "�����" :
                simulated_board.get_card(j).get_suit() == SPADES ? "����" : "?");
        }
        printf("\n");

        if (tie) {
            printf("����� ����� ��������.\n");
        }
        else {
            printf("����� %d �������.\n", best_player + 1);
        }
        printf("\n");

        // ����� ���������� ��� ������������� ������
        for (int j = 0; j < game->get_current_players(); j++) {
            const Player& player = game->get_player(j);
            printf("����� %d:\n", j + 1);
            print_hand(player_hands[j]); // ���������� ���� ������� print_hand ��� ����������� ����������
        }

    }

  
}


//void calculate_probabilities_debugging(Game* game, Settings_debugging_mode* settings, Board simulated_board, PokerCombination* player_hands, int current_simulation, bool tie, int best_player) {
//    // ����������� ���������� ��� ������
//    printf("��������� %d:\n", current_simulation + 1);
//
//    // ����� ���� �� �����
//    printf("����� �� �����: ");
//    for (int j = 0; j < 5; j++) {
//        const Card& card = simulated_board.get_card(j);
//        if (card.is_valid()) {
//            printf("%d%s ",
//                card.get_rank() + 2,
//                card.get_suit() == HEARTS ? " �����" :
//                card.get_suit() == DIAMONDS ? " �����" :
//                card.get_suit() == CLUBS ? " �����" :
//                card.get_suit() == SPADES ? " ����" : "?");
//        }
//        else {
//            printf("���������� ");
//        }
//    }
//    printf("\n");
//
//    // ����� ���������� � ����������� �������
//    for (int j = 0; j < game->get_current_players(); j++) {
//        const Player& player = game->get_player(j);
//        printf("����� %d:\n", j + 1);
//        print_hand(player_hands[j]); // ���������� ���� ������� print_hand ��� ����������� ����������
//    }
//
//    // ����������� ���������� ���������
//    if (settings->wins_visible_mode && best_player + 1 == settings->current_winner) {
//        printf("����� %d �������.\n", best_player + 1);
//    }
//    else if (settings->ties_visible_mode && tie) {
//        printf("����� ����� ��������.\n");
//        for (int j = 0; j < game->get_current_players(); j++) {
//            printf("����� %d:\n", j + 1);
//            print_hand(player_hands[j]); // ���������� ���������� ���� ���������� ������
//        }
//    }
//
//    // �������������� ���������� ����������
//    if (settings->debug_player_index >= 0 && settings->debug_player_index < game->get_current_players()) {
//        printf("��������� ���������� � ���������� ������ %d:\n", settings->debug_player_index + 1);
//        print_hand(player_hands[settings->debug_player_index]);
//    }
//
//    printf("\n");
//}


// ���������� ���������� ���� �������
void compare_all_hands(Game* game, PokerCombination hands[]) {
    for (int i = 0; i < game->get_current_players(); i++) {
        for (int j = i + 1; j < game->get_current_players(); j++) {
            int result = compare_hands(hands[i], hands[j]);

            if (result == 1) {
                // ����� i ��������� ������ j
                game->get_player(i).set_wins(game->get_player(i).get_wins() + 1);
                game->get_player(j).set_losses(game->get_player(j).get_losses() + 1);
            }
            else if (result == -1) {
                // ����� j ��������� ������ i
                game->get_player(j).set_wins(game->get_player(j).get_wins() + 1);
                game->get_player(i).set_losses(game->get_player(i).get_losses() + 1);
            }
            else {
                // �����
                game->get_player(i).set_ties(game->get_player(i).get_ties() + 1);
                game->get_player(j).set_ties(game->get_player(j).get_ties() + 1);
            }
        }
    }
}
