#define _CRT_SECURE_NO_WARNINGS
#include "Classes.h"
#include <cstdlib>
#include <ctime>

// Реализация методов классов
Card::Card() : suit(NONE_SUIT), rank(NONE_RANK), is_used(false) {}

Card::Card(Suit suit, Rank rank) : suit(suit), rank(rank), is_used(false) {}

Suit Card::get_suit() const { return suit; }
Rank Card::get_rank() const { return rank; }

void Card::set_suit(Suit suit) { this->suit = suit; }
void Card::set_rank(Rank rank) { this->rank = rank; }

const char* Card::get_rank_name() const {
    switch (rank) {
    case TWO: return "Двойка";
    case THREE: return "Тройка";
    case FOUR: return "Четверка";
    case FIVE: return "Пятерка";
    case SIX: return "Шестерка";
    case SEVEN: return "Семерка";
    case EIGHT: return "Восьмерка";
    case NINE: return "Девятка";
    case TEN: return "Десятка";
    case JACK: return "Валет";
    case QUEEN: return "Дама";
    case KING: return "Король";
    case ACE: return "Туз";
    default: return "Неизвестно";
    }
}

const char* Card::get_suit_name() const {
    switch (suit) {
    case HEARTS: return "Черви";
    case DIAMONDS: return "Бубны";
    case CLUBS: return "Трефы";
    case SPADES: return "Пики";
    default: return "Неизвестно";
    }
}

void Card::print_card() const {
    printf("%s %s\n", get_rank_name(), get_suit_name());
}

void Card::release_card(bool used_cards[NUM_RANKS][NUM_SUITS]) {
    if (is_used && suit != NONE_SUIT && rank != NONE_RANK) {
        used_cards[rank][suit] = false;
        is_used = false;
    }
    suit = NONE_SUIT;
    rank = NONE_RANK;
}

void Card::init_card(Suit suit, Rank rank, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    release_card(used_cards);

    if (used_cards[rank][suit]) {
        printf("Ошибка: Карта %s %s уже используется!\n", get_rank_name(), get_suit_name());
        return;
    }

    this->suit = suit;
    this->rank = rank;
    used_cards[rank][suit] = true;
    is_used = true;
}

void Card::init_random_card(bool used_cards[NUM_RANKS][NUM_SUITS]) {
    release_card(used_cards);

    int rank, suit;
    do {
        suit = rand() % NUM_SUITS;
        rank = rand() % NUM_RANKS;
    } while (used_cards[rank][suit]);

    this->suit = static_cast<Suit>(suit);
    this->rank = static_cast<Rank>(rank);
    used_cards[rank][suit] = true;
    is_used = true;
}

Hand::Hand() {
    card[0] = Card();
    card[1] = Card();
}

Hand::Hand(const Card& card1, const Card& card2) {
    card[0] = card1;
    card[1] = card2;
}

void Hand::set_card(int index, const Card& card) {
    if (index < 0 || index >= 2) {
        printf("Ошибка: индекс карты вне допустимого диапазона (0 или 1).\n");
        return;
    }
    this->card[index] = card;
}

void Hand::init_hand(const Card& card1, const Card& card2) {
    card[0] = card1;
    card[1] = card2;
}

void Hand::clear_hand(bool used_cards[NUM_RANKS][NUM_SUITS]) {
    card[0].release_card(used_cards);
    card[1].release_card(used_cards);
}

const Card& Hand::get_const_card(int index) const {
    return card[index];
}

Card& Hand::get_card(int index) {
    return card[index];
}

Player::Player() : equity(0.0), wins(0), ties(0), losses(0) {}

void Player::set_equity(double e) { equity = e; }
void Player::set_wins(int w) { wins = w; }
void Player::set_ties(int t) { ties = t; }
void Player::set_losses(int l) { losses = l; }

Hand& Player::get_hand() {
    return hand;
}

void Player::set_hand(const Hand& h, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    clear_player_cards(used_cards);
    hand = h;

    used_cards[hand.get_card(0).get_rank()][hand.get_card(0).get_suit()] = true;
    used_cards[hand.get_card(1).get_rank()][hand.get_card(1).get_suit()] = true;
}

void Player::init_player(const Hand& new_hand, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    set_hand(new_hand, used_cards);
    equity = 0.0;
}

void Player::clear_player_cards(bool used_cards[NUM_RANKS][NUM_SUITS]) {
    Card& card1 = hand.get_card(0);
    Card& card2 = hand.get_card(1);

    if (card1.get_rank() != NONE_RANK && card1.get_suit() != NONE_SUIT) {
        used_cards[card1.get_rank()][card1.get_suit()] = false;
    }

    if (card2.get_rank() != NONE_RANK && card2.get_suit() != NONE_SUIT) {
        used_cards[card2.get_rank()][card2.get_suit()] = false;
    }

    hand.clear_hand(used_cards);
}

double Player::get_equity() const { return equity; }
int Player::get_wins() const { return wins; }
int Player::get_ties() const { return ties; }
int Player::get_losses() const { return losses; }

Board::Board() : num_cards(0) {
    for (int i = 0; i < NUM_RANKS; ++i)
        for (int j = 0; j < NUM_SUITS; ++j)
            used_cards[i][j] = false;
}

int Board::get_num_cards() const {
    return num_cards;
}

const Card& Board::get_card(int index) const {
    return cards[index];
}

void Board::init_board() {
    clear_board(used_cards);
}

void Board::set_num_cards(int num) {
    num_cards = num;
}

void Board::set_card(int index, const Card& card, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    if (index >= 0 && index < 5) {
        if (cards[index].get_rank() != NONE_RANK && cards[index].get_suit() != NONE_SUIT) {
            used_cards[cards[index].get_rank()][cards[index].get_suit()] = false;
        }

        if (used_cards[card.get_rank()][card.get_suit()]) {
            printf("Ошибка: Карта %s %s уже используется!\n",
                card.get_rank_name(), card.get_suit_name());
            return;
        }

        cards[index] = card;
        used_cards[card.get_rank()][card.get_suit()] = true;
    }
}

void Board::print_board_cards() const {
    printf("Карты на столе:\n");
    for (int i = 0; i < num_cards; i++) {
        cards[i].print_card();
    }
}

void Board::add_card(const Card& card, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    if (num_cards < 5) {
        if (used_cards[card.get_rank()][card.get_suit()]) {
            printf("Ошибка: Карта %s %s уже используется!\n",
                card.get_rank_name(), card.get_suit_name());
            return;
        }

        cards[num_cards] = card;
        used_cards[card.get_rank()][card.get_suit()] = true;
        num_cards++;
    }
}

void Board::clear_board(bool used_cards[NUM_RANKS][NUM_SUITS]) {
    for (int i = 0; i < num_cards; i++) {
        if (cards[i].get_rank() != NONE_RANK && cards[i].get_suit() != NONE_SUIT) {
            used_cards[cards[i].get_rank()][cards[i].get_suit()] = false;
        }
        cards[i] = Card(); // Обнуление карты
    }
    num_cards = 0; // Сброс количества карт
}


void Board::debug_print_cards() const {
    printf("Отладочный вывод карт на столе:\n");
    for (int i = 0; i < 5; i++) {
        cards[i].print_card();
    }
}

Game::Game() : num_players(0), current_players(0) {}

void Game::press_any_key_to_continue() {
    printf("Нажмите любую клавишу для продолжения...\n");
    _getch(); // Ожидание нажатия клавиши
}

int Game::get_current_players() const {
    return current_players;
}

void Game::set_current_players(int num) {
    current_players = num;
}

Board& Game::get_board() {
    return board;
}

char* Game::get_phase() {
    return phase;
}

void Game::set_phase(const char* ph) {
    strcpy(phase, ph);
}

int Game::get_num_players() const {
    return num_players;
}

void Game::set_player(int index, const Player& player) {
    if (index >= 0 && index < 12) {
        players[index] = player;
    }
}

Player& Game::get_player(int index) {
    return players[index];
}

void Game::set_num_players(int num) {
    num_players = num;
}

void Game::clear_all_players_cards(bool used_cards[14][4]) {
    for (int i = 0; i < current_players; i++) {
        players[i].clear_player_cards(used_cards);

        printf("Рука игрока %d успешно очищена!\n", i + 1);

    }
    printf("Операция завершена!\n");
    press_any_key_to_continue();
}

void Game::init_game(int num_players) {
    this->num_players = num_players;
    current_players = num_players;
    board.init_board();
}

void Game::add_player(const Player& player, int index) {
    if (index >= 0 && index < 12) {
        players[index] = player;
    }
}
