#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

#include <string>

enum class Language { ENGLISH, RUSSIAN };

const int YAW_OFFSET = 0x0074;
const int PITCH_OFFSET = 0x0078;

struct Player {
	float x, y, z;      // Координаты
	float yaw, pitch;   // Направление взгляда
	char gameMode;      // Режим игры (0 - ходьба, 1 - полёт)
	int worldTime;      // Время в мире (в тиках: 0-24000)
};

std::string getText(const std::string& key, Language lang);
std::string ticksToTime(int ticks, Language lang);
int convertTimeToTicks(int hours, int minutes);
void readPlayerData(const std::string& filename, Player& player);
void changePlayerCoordinates(const std::string& filename, Language lang);
void changeWorldTime(const std::string& filename, Language lang);
void changePlayerMode(const std::string& filename, Language lang);
void changePlayerDirection(const std::string& filename, Language lang);

#endif