#pragma once
#define GLOBALS_H

#include <windows.h>
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm> // Подключаем заголовочный файл для std::sort


#define NUM_RANKS 13
#define NUM_SUITS 4
#define MAX_BOARD_CARDS 5

extern bool used_cards[NUM_RANKS][NUM_SUITS]; // Объявление глобальной переменной
const int MIN_SIMULATIONS = 100;
const int MAX_SIMULATIONS = 5000000;
const int DEFAULT_SIMULATIONS = 250000;