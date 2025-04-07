#pragma once
#include <string>

#define YAW_OFFSET 0x0074
#define PITCH_OFFSET 0x0078

// ������������ ��� ������
enum class Language {
	RUSSIAN,
	ENGLISH
};

struct Player {
	float x, y, z;      // ���������� ������ (X, Y, Z)
	float yaw, pitch;   // ���� �������� (����������� �������)
	char gameMode;      // ����� ����: 0 - ������, 1 - ���� (����� 0x0050)
	float worldTime;    // ����� � ���� (0-24000) � �������� ��� 3 ����� �� ������ 0x005C
};

// ������� ��� ������ � ������
void readPlayerData(const std::string& filename, Player& player);
void changePlayerCoordinates(const std::string& filename, Language lang);
void changeWorldTime(const std::string& filename, Language lang);
void changePlayerMode(const std::string& filename, Language lang);
void changePlayerDirection(const std::string& filename, Language lang);

// ������� ��� ��������� ������ �� ������ �����
std::string getText(const std::string& key, Language lang);
std::string ticksToTime(float ticks, Language lang);