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
        game->get_board().set_card(i, Card(NONE_SUIT, NONE_RANK));
    }
    game->get_board().set_num_cards(0);

    // Устанавливаем фазу игры
    game->set_phase("preflop");

    // Устанавливаем количество игроков
    game->set_num_players(num_players); // Используем сеттер для количества игроков
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



