#pragma once
#include "Globals.h"

// �������� �������
double scanf_secure(const char* type);          // ������� ��� ����������� ����� ��������� ����� ������
void clearConsole();                            // ������� ��� ������� �������
void buffer_clear();                            // ������� ��� ������� ������ �����
void line_remove(char* str);                    // ������� ��� �������� ������� ����� ������ �� ������
void string_get_secure(char* buffer, int size); // ������� ��� ����������� ��������� ������ � ������� �� ������������ ������
void press_any_key_to_continue();               // ��� ���������� �������

