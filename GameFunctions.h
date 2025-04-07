#pragma once
#include <string>

#define YAW_OFFSET 0x0074
#define PITCH_OFFSET 0x0078

// Перечисление для языков
enum class Language {
	RUSSIAN,
	ENGLISH
};

struct Player {
	float x, y, z;      // Координаты игрока (X, Y, Z)
	float yaw, pitch;   // Углы поворота (направление взгляда)
	char gameMode;      // Режим игры: 0 - Ходьба, 1 - Полёт (адрес 0x0050)
	float worldTime;    // Время в мире (0-24000) — хранится как 3 байта по адресу 0x005C
};

// Функции для работы с файлом
void readPlayerData(const std::string& filename, Player& player);
void changePlayerCoordinates(const std::string& filename, Language lang);
void changeWorldTime(const std::string& filename, Language lang);
void changePlayerMode(const std::string& filename, Language lang);
void changePlayerDirection(const std::string& filename, Language lang);

// Функция для получения текста на нужном языке
std::string getText(const std::string& key, Language lang);
std::string ticksToTime(float ticks, Language lang);