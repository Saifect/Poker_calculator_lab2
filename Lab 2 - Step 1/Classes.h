#pragma once
#include "Globals.h"

// Масть карты
typedef enum {
    NONE_SUIT = -1,
    HEARTS,   // Черви
    DIAMONDS, // Бубны
    CLUBS,    // Трефы
    SPADES,   // Пики
} Suit;

// Ранк карты
typedef enum {
    NONE_RANK = -1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
    JACK, QUEEN, KING, ACE
} Rank;

class Card {
private:
    Suit suit;
    Rank rank;
    bool is_used; // Флаг использования карты

public:
    // Конструктор по умолчанию
    Card();

    // Конструктор с параметрами
    Card(Suit suit, Rank rank);

    // Геттеры
    Suit get_suit() const;
    Rank get_rank() const;

    // Добавление сеттеров
    void set_suit(Suit suit);
    void set_rank(Rank rank);

    // Установка флага использования карты
    void set_used(bool used) {
        is_used = used;
    }

    // Возвращает название ранга
    const char* get_rank_name() const;

    // Возвращает название масти
    const char* get_suit_name() const;

    // Функция для отображения карты
    void print_card() const;

    // Функция освобождения карты
    void release_card(bool used_cards[NUM_RANKS][NUM_SUITS]);

    // Инициализация карты
    void init_card(Suit suit, Rank rank);

    // Инициализация случайной карты
    void init_random_card(bool used_cards[NUM_RANKS][NUM_SUITS]);
};

class Hand {
private:
    Card card[2]; // Две карты руки

public:
    // Конструктор по умолчанию
    Hand();

    // Конструктор с параметрами
    Hand(const Card& card1, const Card& card2);

    // Установка карты
    void set_card(int index, const Card& card);

    // Инициализация руки
    void init_hand(const Card& card1, const Card& card2);

    // Очистка руки
    void clear_hand(bool used_cards[NUM_RANKS][NUM_SUITS]);

    // Геттер для карты
    const Card& get_const_card(int index) const;
    Card& get_card(int index);
};

class Player {
private:
    Hand hand;         // Рука игрока
    double equity;     // Вероятность победы
    int wins;          // Количество побед
    int ties;          // Количество ничьих
    int losses;        // Количество поражений

public:
    Player();

    // Сеттеры для статистики
    void set_equity(double e);
    void set_wins(int w);
    void set_ties(int t);
    void set_losses(int l);

    // Геттер для доступа к руке
    Hand& get_hand();

    // Сеттер для установки руки игрока
    void set_hand(const Hand& h, bool used_cards[NUM_RANKS][NUM_SUITS]);

    // Инициализация игрока
    void init_player(const Hand& new_hand, bool used_cards[NUM_RANKS][NUM_SUITS]);

    // Очистка карт игрока
    void clear_player_cards(bool used_cards[NUM_RANKS][NUM_SUITS]);

    // Геттеры для получения статистики игрока
    double get_equity() const;
    int get_wins() const;
    int get_ties() const;
    int get_losses() const;
};

class Board {
private:
    Card cards[5]; // До 5 карт на столе
    int num_cards;

public:
    Board();
    Board(bool initial_used_cards[NUM_RANKS][NUM_SUITS]); // Новый конструктор
    void set_used_cards(bool new_used_cards[NUM_RANKS][NUM_SUITS]);
    int get_num_cards() const;
    const Card& get_card(int index) const;
    void init_board();
    void set_num_cards(int num);
    void set_card(int index, const Card& card);
    void print_board_cards() const;
    void add_card(const Card& card, bool used_cards[NUM_RANKS][NUM_SUITS]);
    void clear_board(bool used_cards[NUM_RANKS][NUM_SUITS]);
    void debug_print_cards() const;
};

class Game {
public:
    Game();

    void press_any_key_to_continue();
    int get_current_players() const;
    void set_current_players(int num);
    Board& get_board();
    char* get_phase();
    void set_phase(const char* ph);
    int get_num_players() const;
    void set_player(int index, const Player& player);
    Player& get_player(int index);
    void set_num_players(int num);
    void clear_all_players_cards(bool used_cards[14][4]);
    void init_game(int num_players);
    void add_player(const Player& player, int index);

private:
    Player players[12];   // Игроки
    int num_players;      // Общее количество игроков
    int current_players;  // Текущие игроки в игре
    Board board;          // Игровое поле
    char phase[10];       // Текущая фаза игры
};

// Покерные комбинации
typedef enum {
    HIGH_CARD,        // Старшая картаа
    ONE_PAIR,         // Пара
    TWO_PAIR,         // Две пары
    THREE_OF_A_KIND,  // Сет
    STRAIGHT,         // Стрит
    FLUSH,            // Флеш
    FULL_HOUSE,       // Фулл хаусее
    FOUR_OF_A_KIND,   // Каре
    STRAIGHT_FLUSH,   // Стрит флеш
    ROYAL_FLUSH
} PokerHandRank;

// Структура для покерной комбинации
typedef struct {
    PokerHandRank hand_rank;
    Rank high_card;
    Rank kicker[5];
} PokerCombination;

typedef struct {
    bool ties_mode;
    bool wins_mode;
    bool show;
    int current_winner;
    int debug_player_index; // Индекс игрока для отладки
} Settings_debugging_mode;
