#define _CRT_SECURE_NO_WARNINGS
#include "Functions.h"


void print_probabilityMenu(Game* game, bool used_cards[NUM_RANKS][NUM_SUITS]) {
    int num_simulations = 250000;
    int probabilityMenu_choice = 0;
    bool exit = false;

    Settings_debugging_mode settings;
    settings.show = false;
    settings.current_winner = -1;
    settings.ties_visible_mode = false;
    settings.wins_visible_mode = false;
    settings.simulations_visible_mode = false;

    while (exit == false) {

        printf("\n");
        printf("================================================\n");
        printf("             ������� ����������                 \n");
        printf("================================================\n");

        for (int i = 0; i < game->get_current_players(); i++) {
            // ���������, ���� �� ������� ����� ������
            const Card& card1 = game->get_player(i).get_hand().get_const_card(0);
            const Card& card2 = game->get_player(i).get_hand().get_const_card(1);

            if (card1.get_rank() != NONE_RANK && card2.get_rank() != NONE_RANK) {
                printf("����� ������ %d: %s %s � %s %s\n", i + 1,
                    card1.get_rank_name(),
                    card1.get_suit_name(),
                    card2.get_rank_name(),
                    card2.get_suit_name());
            }
            else {
                printf("����� ������ %d: �� ������\n", i + 1);
            }
        }


        // ����� �� �����
        if (strcmp(game->get_phase(), "preflop") == 0) {
            printf("����� �� �����: �����������\n");
        }
        else {
            print_board_cards(&game->get_board());
        }

        printf("������� ������ ���� (�����): %s\n", game->get_phase());
        printf("������� ���������� ���������: %d\n", num_simulations);
        if (settings.wins_visible_mode == false && settings.ties_visible_mode == false) {
            printf("����� �������: ��������\n");
        }
        else {
            printf("����� �������: �������\n");
        }
        printf("================================================\n");
        printf("       �������� ���������� ������������         \n");
        printf("================================================\n");
        printf("1. ������ �� ������ ��������� �����-�����\n");
        printf("2. �������� ���������� ���������\n");
        if (settings.show == true) {
            printf("3. ������ �������� �������\n");
            if (settings.wins_visible_mode == false) {
                printf("4. ������� ����������� ����� ��� ������ (�������)\n");
            }
            if (settings.wins_visible_mode == true) {
                printf("4. ��������� ����������� ����� ��� ������ (�������)\n");
            }
            if (settings.ties_visible_mode == false) {
                printf("5. �������� ����������� ����� (�������)\n");
            }
            if (settings.ties_visible_mode == true) {
                printf("5. ��������� ����������� ����� (�������)\n");
            }

            if (settings.simulations_visible_mode == false) {
                printf("6. �������� ����������� ���� ���������� (�������)\n");
            }
            if (settings.simulations_visible_mode == true) {
                printf("6. ��������� ����������� ���� ���������� (�������)\n");
            }

       
            printf("7. ������� ������ ������� ���� (�������)\n");
            




        }
        else if (settings.show == false) {
            printf("3. �������� �������� �������\n");
        }
        printf("-----------------------------------------------\n");
        printf("0. �����\n");
        printf("================================================\n");
        printf("��� �����: ");

        get_user_choice(&probabilityMenu_choice);
        handle_probabilityMenu_choice(probabilityMenu_choice, game, &exit, used_cards, &num_simulations, &settings);
    }
}

void handle_probabilityMenu_choice(int choice, Game* game, bool* exit, bool used_cards[NUM_RANKS][NUM_SUITS], int* num_simulations, Settings_debugging_mode* settings) {
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
            // ��������� ���� �� ����� � ������
            if (game->get_player(i).get_hand().get_const_card(0).get_rank() != NONE_RANK && game->get_player(i).get_hand().get_const_card(1).get_rank() != NONE_RANK) {
                players_with_cards++;
            }

            if (players_with_cards >= 2) {
                break;
            }
        }

        // ��������� ���������
        if (players_with_cards < 2) {
            printf("����� ���� �� 2 �������� ���� ��� ������� �����������.\n");
            press_any_key_to_continue();
            clearConsole();
        }
        else if (*num_simulations >= 10 && *num_simulations <= 20000000) {


            if (settings->ties_visible_mode == true || settings->wins_visible_mode == true) {
                debugging_mode = true;
            }
            else {
                debugging_mode = false;
            }
            if (debugging_mode == true && *num_simulations > 5000) {
                if (*num_simulations > 3000 && *num_simulations < 10000) {
                    printf("������������ ����� ������� � ����������� ��������� < 3000 �� �������������\n�� �������?\n");
                }
                if (*num_simulations > 10000 && *num_simulations < 50000) {
                    printf("��, ��� ����� ���������.\n��� ������ ������� ������������� < 3000 ���������, �� �������?\n");
                }
                if (*num_simulations > 50000 && *num_simulations < 100000) {
                    printf("��� ����� ���� ����� �����.\n��� ������ ������� ������������� < 3000 ���������, �� �������?\n");
                }
                else {
                    printf("��� ����� � ������ ������� �����.\n��� ������ ������� ������������� < 3000 ���������, �� �������?\n");
                }
                printf("Y/yes - ��\n");
                printf("N/no - ���\n");

                char str_choice[3];
                string_get_secure(str_choice, 3);

                // �������� ���� � ������� �������� //
                for (int i = 0; str_choice[i]; i++) {
                    str_choice[i] = tolower(str_choice[i]);
                }

                if (strcmp(str_choice, "y") == 0 || strcmp(str_choice, "yes") == 0) {

                    printf("���������� ���������� � ������ ������� � %d �����������.\n", *num_simulations);

                }
                else {

                    printf("�������� ��������.\n");
                    press_any_key_to_continue();
                    clearConsole();
                    break;
                }

                press_any_key_to_continue();
                clearConsole();
            }
            printf("�� ��������� ������ ���� �� ���������� ��������� \n");
            printf("��������...\n");
            calculate_probabilities(game, used_cards, *num_simulations, settings);

            press_any_key_to_continue();
            clearConsole();
        }
        break;

    case 2: {

        printf("-----------------------------------------------\n");
        printf("������� ���������� ���������\n�������� �������� �� 100 �� 5.000.000\n");
        printf("������������� 100.000 - 300.000 ���������\n�������� >1.000.000 ����� ����� ��������\n");
        printf("��� ������� 0 ��� ������\n");
        printf("-----------------------------------------------\n");
        printf("��� �����: ");
        int num_simulations_new = scanf_secure("int");

        if (num_simulations_new == 0) {
            printf("�������� ��������.\n");
            press_any_key_to_continue();
            clearConsole();
        }
        else if (num_simulations_new >= 100 && num_simulations_new <= 5000000) {
            *num_simulations = num_simulations_new; // ��������� ���������� ����������
            printf("�� ���������� %d ��������� ��� ������ �����-�����\n", *num_simulations);
            press_any_key_to_continue();
            clearConsole();
        }
        else {
            printf("�� ����� �������� ��� ���������! ���������� 250000 �� ���������.\n");
            *num_simulations = 250000; // �������� �� ���������
            press_any_key_to_continue();
            clearConsole();
        }
        break;
    }

    case 3:

        settings->show = !(settings->show);
        clearConsole();

        break;

    case 4:
        if (settings->wins_visible_mode == true) {
            settings->wins_visible_mode = false;
            clearConsole();
        }
        else {
            printf("������� ����� ������ ��� 0 ��� ������: ");
            choice_user = scanf_secure("int");
            if (choice_user == 0) {
                printf("������ ��������.\n");
                press_any_key_to_continue();
                clearConsole();
                break;
            }
            else if (choice_user < 1 || choice_user > game->get_num_players()) {
                printf("�� ����� ����� ��� ���������!\n");
                press_any_key_to_continue();
                clearConsole();
                break;
            }
            else {
                settings->current_winner = choice_user;
                settings->wins_visible_mode = !(settings->wins_visible_mode);
                if (settings->ties_visible_mode == true) {
                    settings->ties_visible_mode = false;
                }
                clearConsole();
            }
        }
        break;

    case 5:

        settings->ties_visible_mode = !(settings->ties_visible_mode);
        if (settings->wins_visible_mode == true) {
            settings->wins_visible_mode = false;
        }
        if (settings->simulations_visible_mode == true) {
            settings->simulations_visible_mode = false;
        }

        clearConsole();
        break;

    case 6:

        settings->simulations_visible_mode = !(settings->simulations_visible_mode);
        if (settings->wins_visible_mode == true) {
            settings->wins_visible_mode = false;
        }
        if (settings->ties_visible_mode == true) {
            settings->ties_visible_mode = false;
        }

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