#define _CRT_SECURE_NO_WARNINGS
#include "Functions.h"

void print_hand(PokerCombination hand) {
    switch (hand.hand_rank) {
    case STRAIGHT_FLUSH:
        printf("����������: �����-����\n");
        printf("������� �����: %d\n", hand.high_card);
        break;
    case FOUR_OF_A_KIND:
        printf("����������: ���� %d\n", hand.high_card);
        printf("�����: %d\n", hand.kicker[0]);
        break;
    case FULL_HOUSE:
        printf("����������: ����-����\n");
        printf("������: %d\n", hand.high_card);
        printf("����: %d\n", hand.kicker[0]);
        break;
    case FLUSH:
        printf("����������: ����\n");
        printf("������� �����: %d\n", hand.high_card);
        break;
    case STRAIGHT:
        printf("����������: �����\n");
        printf("������� �����: %d\n", hand.high_card);
        break;
    case THREE_OF_A_KIND:
        if (hand.kicker[0] == NONE_RANK) {
            printf("����������: ��� %d\n", hand.high_card);
        }
        else {
            printf("����������: ����� %d\n", hand.high_card);
            printf("�����: %d\n", hand.kicker[0]);
        }
        break;
    case TWO_PAIR:
        printf("����������: ��� ����\n");
        printf("������� ����: %d\n", hand.high_card);
        printf("������� ����: %d\n", hand.kicker[0]);
        break;
    case ONE_PAIR:
        if (hand.kicker[0] == NONE_RANK && hand.kicker[1] == NONE_RANK) {
            printf("����������: ��������� ���� %d\n", hand.high_card);
        }
        else {
            printf("����������: ���� %d\n", hand.high_card);
            printf("�����: %d\n", hand.kicker[0]);
        }
        break;
    case HIGH_CARD:
        printf("����������: ������� ����� %d\n", hand.high_card);
        printf("�����: %d\n", hand.kicker[0]);
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
            if (used_cards[rank][suit] == false) {  // ��������� ������ �� �������������� �����
                deck[index].init_card((Suit)suit, (Rank)rank, used_cards);
                index++;
            }
        }
    }
}

// �������� �� ������������ ���� ���� � �������
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
        for (int i = 0; i < 2; i++) {
            if (hand1.kicker[i] > hand2.kicker[i]) return 1;
            if (hand1.kicker[i] < hand2.kicker[i]) return -1;
        }
        break;

    case ONE_PAIR:
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
    for (int i = 0; i < 5; i++) {
        result.kicker[i] = NONE_RANK;
    }

    int card_count[NUM_RANKS] = { 0 };
    int suit_count[NUM_SUITS] = { 0 };
    Card all_cards[7];            // ������ ���� ���� (���� + ����)

    // �������� ��� ����� (2 ����� ���� + ����� �� �����)
    all_cards[0] = hand.get_card(0);
    all_cards[1] = hand.get_card(1);
    for (int i = 0; i < board.get_num_cards(); i++) {
        // ���������� ������ ��� ��������� ����� � �����
        all_cards[2 + i] = board.get_card(i);
    }

    // ������� ���������� ���� �� ����� � ������
    for (int i = 0; i < 2 + board.get_num_cards(); i++) {
        card_count[all_cards[i].get_rank()]++;
        suit_count[all_cards[i].get_suit()]++;
    }

    Rank top_pair = NONE_RANK, second_pair = NONE_RANK;
    Rank highest_rank = NONE_RANK;
    int flush_suit_index = -1;
    int three_of_a_kind = 0, second_three_of_a_kind = 0;
    int four_of_a_kind = 0;

    // ����������� ���, �����, ����
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

    // ����������� ������� �����
    for (int suit = 0; suit < NUM_SUITS; suit++) {
        if (suit_count[suit] >= 5) {
            flush_suit_index = suit;
            break;
        }
    }

    // �������� �� ����-����
    if (three_of_a_kind > 0 && (top_pair > 0 || second_three_of_a_kind > 0)) {
        result.hand_rank = FULL_HOUSE;
        result.high_card = static_cast<Rank>(three_of_a_kind); // ������� ���
        result.kicker[0] = static_cast<Rank>((top_pair > 0) ? top_pair : second_three_of_a_kind); // ����
        return result;
    }

    // �������� �� �����
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

    // �������� �� ����� A-2-3-4-5
    if (card_count[ACE] > 0 && card_count[TWO] > 0 && card_count[THREE] > 0 &&
        card_count[FOUR] > 0 && card_count[FIVE] > 0) {
        result.hand_rank = STRAIGHT;
        result.high_card = FIVE;
        return result;
    }

    // �������� �� �����-����
    if (flush_suit_index != -1) {
        Rank flush_cards[7];
        int flush_card_count = 0;

        // �������� ����� ����� �����
        for (int i = 0; i < 2 + board.get_num_cards(); i++) {
            if (all_cards[i].get_suit() == flush_suit_index) {
                flush_cards[flush_card_count++] = all_cards[i].get_rank();
            }
        }

        qsort(flush_cards, flush_card_count, sizeof(Rank), [](const void* a, const void* b) {
            return (*(Rank*)b - *(Rank*)a);
            });

        // �������� �� �����-����
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

        // ����������� �����
        result.hand_rank = FLUSH;
        result.high_card = flush_cards[0];
        for (int i = 1; i < 5; i++) {
            result.kicker[i - 1] = flush_cards[i];
        }
        return result;
    }

    // �������� �� ����
    if (four_of_a_kind > 0) {
        result.hand_rank = FOUR_OF_A_KIND;
        result.high_card = static_cast<Rank>(four_of_a_kind);
        result.kicker[0] = highest_rank;
        return result;
    }

    // �������� �� ������
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

    // �������� �� ��� ����
    if (top_pair > 0 && second_pair > 0) {
        result.hand_rank = TWO_PAIR;
        result.high_card = top_pair;
        result.kicker[0] = second_pair;
        result.kicker[1] = highest_rank;
        return result;
    }

    // �������� �� ����
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

    // ���� ��� ����������, �� ��� �����
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

void calculate_probabilities(Game* game, bool used_cards[NUM_RANKS][NUM_SUITS], int choice_numSimulations, Settings_debugging_mode* settings) {
    // ������, ��������� � ����� ��� ������� ������
    for (int i = 0; i < game->get_current_players(); i++) {
        game->get_player(i).set_wins(0);
        game->get_player(i).set_ties(0);
        game->get_player(i).set_losses(0);
    }

    for (int i = 0; i < choice_numSimulations; i++) {
        Card deck[52];
        int deck_index = 0;

        // �������� �������� used_cards ��� ������ ���������
        bool sim_used_cards[NUM_RANKS][NUM_SUITS];
        memcpy(sim_used_cards, used_cards, sizeof(sim_used_cards));

        // ��������� ������, �������� ����� �� ������� used_cards
        for (int rank = 0; rank < NUM_RANKS; rank++) {
            for (int suit = 0; suit < NUM_SUITS; suit++) {
                if (!sim_used_cards[rank][suit]) {
                    deck[deck_index].init_card((Suit)suit, (Rank)rank, used_cards);
                    deck_index++;
                }
            }
        }

        // ������������ ������
        shuffle_deck(deck, deck_index);

        Card simulation_board[5];

        // �������� ��� ��������� ����� �� ����
        for (int j = 0; j < game->get_board().get_num_cards(); j++) {
            simulation_board[j] = game->get_board().get_card(j);
        }

        for (int j = game->get_board().get_num_cards(); j < 5; j++) {
            bool card_found = false;
            while (!card_found) {
                simulation_board[j] = deck[--deck_index]; // ����� ����� �� ������
                // ���������, ��� ����� �� ������������ �����
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

        // ������ ��� �������� ���������� ���� �������
        PokerCombination* player_hands = (PokerCombination*)malloc(game->get_current_players() * sizeof(PokerCombination));

        // ������ ���������� ��� ������� ������
        for (int j = 0; j < game->get_current_players(); j++) {
            player_hands[j] = determine_hand(game->get_player(j).get_hand(), simulated_board);
        }

        // ������� ������ ���������� � ��������� �� �����
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

        // ��������� ����������
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

    // ������� �����������
    double total_simulations = (double)choice_numSimulations;

    for (int i = 0; i < game->get_current_players(); i++) {
        printf("------------------\n   ����� %d\n------------------\n", i + 1);
        printf("������: %.2f%%\n", (game->get_player(i).get_wins() / total_simulations) * 100);
        printf("���������: %.2f%%\n", (game->get_player(i).get_losses() / total_simulations) * 100);
        printf("�����: %.2f%%\n", (game->get_player(i).get_ties() / total_simulations) * 100);
    }
}

void calculate_probabilities_debugging(Game* game, Settings_debugging_mode* settings, Board simulated_board, PokerCombination* player_hands, int current_simulation, bool tie, int best_player) {
    if (settings->wins_mode == true) {
        if (best_player + 1 == settings->current_winner) {
            printf("��������� %d:\n", current_simulation + 1);
            printf("����� �� �����: ");
            for (int j = 0; j < 5; j++) {
                printf("%d%c ", simulated_board.get_card(j).get_rank(), simulated_board.get_card(j).get_suit() == HEARTS ? '�' :
                    simulated_board.get_card(j).get_suit() == DIAMONDS ? '�' :
                    simulated_board.get_card(j).get_suit() == CLUBS ? '�' :
                    simulated_board.get_card(j).get_suit() == SPADES ? '�' : '?');
            }
            printf("\n");

            for (int j = 0; j < game->get_current_players(); j++) {
                printf("����� %d: ���������� %d, ������� �����: %d\n", j + 1, player_hands[j].hand_rank, player_hands[j].high_card);
            }

            printf("����� %d �������.\n", best_player + 1);
            printf("\n");
        }
    }
    else {
        if (best_player + 1 == settings->current_winner && settings->current_winner != -1) {
            printf("��������� %d:\n", current_simulation + 1);
            printf("����� �� �����: ");
            for (int j = 0; j < 5; j++) {
                printf("%d%c ", simulated_board.get_card(j).get_rank(), simulated_board.get_card(j).get_suit() == HEARTS ? '�' :
                    simulated_board.get_card(j).get_suit() == DIAMONDS ? '�' :
                    simulated_board.get_card(j).get_suit() == CLUBS ? '�' :
                    simulated_board.get_card(j).get_suit() == SPADES ? '�' : '?');
            }
            printf("\n");

            if (tie) {
                printf("����� ����� ��������.\n");
            }

            printf("\n");
        }
    }

    // ����� ���������� ��� ������������� ������
    if (settings->debug_player_index >= 0 && settings->debug_player_index < game->get_current_players()) {
        printf("���������� ������ %d: ", settings->debug_player_index + 1);
        print_hand(player_hands[settings->debug_player_index]);
    }
}

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
