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

#define NUM_RANKS 13
#define NUM_SUITS 4
#define MAX_BOARD_CARDS 5

extern bool used_cards[NUM_RANKS][NUM_SUITS]; // ќбъ€вление глобальной переменной
