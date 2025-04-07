#include "GameFunctions.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

// Функция для получения текста на нужном языке
std::string getText(const std::string& key, Language lang) {
	if (lang == Language::RUSSIAN) {
		if (key == "error_file_open") return "Ошибка: не удалось открыть файл ";
		if (key == "error_file_read_write") return "Ошибка: не удалось открыть файл для чтения/записи!";
		if (key == "current_coordinates") return "Текущие координаты игрока:\n";
		if (key == "enter_new_coordinates") return "Введите новые координаты:\n";
		if (key == "coordinates_changed") return "Координаты успешно изменены!\n";
		if (key == "current_time") return "Текущее время в мире: ";
		if (key == "ticks_info") return " тиков, 0-24000)";
		if (key == "time_reference") return "0 (00:00) - полночь, 6000 (06:00) - утро, 12000 (12:00) - день, 18000 (18:00) - вечер\n";
		if (key == "enter_new_time") return "Введите новое время (0-24000 или в формате HH:MM, например 14:30): ";
		if (key == "invalid_time_format") return "Неверный формат времени! Должно быть HH:MM (например, 14:30). Устанавливаем 0 тиков.\n";
		if (key == "invalid_time_range") return "Время должно быть в диапазоне от 0 до 24000! Устанавливаем 0.\n";
		if (key == "time_changed") return "Время в мире успешно изменено!\n";
		if (key == "current_modes") return "Текущие режимы игрока:\n";
		if (key == "mode_walk_on") return "Ходьба: включено\n";
		if (key == "mode_fly_on") return "Полёт: включено\n";
		if (key == "select_mode") return "Выберите режим игры Ходьба (1) или Полёт (2): ";
		if (key == "invalid_mode_choice") return "Неверный выбор! Доступны только 1 (Ходьба) или 2 (Полёт). Оставляем текущий режим.\n";
		if (key == "mode_walk_set") return "Успешно поставлена Ходьба\n";
		if (key == "mode_fly_set") return "Успешно поставлен Полёт\n";
		if (key == "current_direction") return "Текущее направление взгляда: yaw=";
		if (key == "enter_new_direction") return "Введите новое направление:\n";
		if (key == "yaw_range_error") return "Yaw должен быть в диапазоне 0-360! Устанавливаем 0.\n";
		if (key == "pitch_range_error") return "Pitch должен быть в диапазоне -90 до 90! Устанавливаем 0.\n";
		if (key == "direction_changed") return "Направление взгляда успешно изменено!\n";
	}
	else { // Language::ENGLISH
		if (key == "error_file_open") return "Error: could not open file ";
		if (key == "error_file_read_write") return "Error: could not open file for reading/writing!";
		if (key == "current_coordinates") return "Current player coordinates:\n";
		if (key == "enter_new_coordinates") return "Enter new coordinates:\n";
		if (key == "coordinates_changed") return "Coordinates successfully changed!\n";
		if (key == "current_time") return "Current world time: ";
		if (key == "ticks_info") return " ticks, 0-24000)";
		if (key == "time_reference") return "0 (00:00) - midnight, 6000 (06:00) - morning, 12000 (12:00) - day, 18000 (18:00) - evening\n";
		if (key == "enter_new_time") return "Enter new time (0-24000 or in HH:MM format, e.g., 14:30): ";
		if (key == "invalid_time_format") return "Invalid time format! Must be HH:MM (e.g., 14:30). Setting to 0 ticks.\n";
		if (key == "invalid_time_range") return "Time must be in range 0 to 24000! Setting to 0.\n";
		if (key == "time_changed") return "World time successfully changed!\n";
		if (key == "current_modes") return "Current player modes:\n";
		if (key == "mode_walk_on") return "Walking: enabled\n";
		if (key == "mode_fly_on") return "Flying: enabled\n";
		if (key == "select_mode") return "Select game mode Walking (1) or Flying (2): ";
		if (key == "invalid_mode_choice") return "Invalid choice! Only 1 (Walking) or 2 (Flying) are available. Keeping current mode.\n";
		if (key == "mode_walk_set") return "Walking mode successfully set\n";
		if (key == "mode_fly_set") return "Flying mode successfully set\n";
		if (key == "current_direction") return "Current view direction: yaw=";
		if (key == "enter_new_direction") return "Enter new direction:\n";
		if (key == "yaw_range_error") return "Yaw must be in range 0-360! Setting to 0.\n";
		if (key == "pitch_range_error") return "Pitch must be in range -90 to 90! Setting to 0.\n";
		if (key == "direction_changed") return "View direction successfully changed!\n";
	}
	return key; // Если ключ не найден, возвращаем его как есть
}

// Функция для преобразования тиков в формат HH:MM
std::string ticksToTime(float ticks, Language lang) {
	// Нормализуем тики в диапазон 0-24000
	while (ticks >= 24000) ticks -= 24000;
	while (ticks < 0) ticks += 24000;

	// 1 час = 1000 тиков, 1 минута = 1000/60 ≈ 16.67 тиков
	int totalMinutes = static_cast<int>(ticks * 60 / 1000);
	int hours = totalMinutes / 60;
	int minutes = totalMinutes % 60;

	// Форматируем строку в формате HH:MM
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(2) << hours << ":" << std::setfill('0') << std::setw(2) << minutes;
	return ss.str();
}

// Функция для чтения данных игрока из файла
void readPlayerData(const std::string& filename, Player& player) {
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		std::cout << getText("error_file_open", Language::RUSSIAN) << filename << "!\n";
		return;
	}

	// Читаем время в мире (адрес 0x005C, 3 байта)
	unsigned char timeBytes[3];
	file.seekg(0x005C, std::ios::beg);
	file.read(reinterpret_cast<char*>(timeBytes), 3);

	// Преобразуем 3 байта в целое число (little-endian)
	int rawValue = (timeBytes[2] << 16) | (timeBytes[1] << 8) | timeBytes[0];
	// Преобразуем в тики (0-24000), инвертируя направление
	const int MIN_VALUE = 454067;  // Значение для 14:00 (14000 тиков)
	const float TICKS_PER_UNIT = 14000.0f / (1159916 - 454067); // 50.4178
	player.worldTime = 23000 - (rawValue - MIN_VALUE) * TICKS_PER_UNIT;
	// Добавляем смещение +6000 тиков
	player.worldTime += 6000;
	// Нормализуем время в диапазон 0-24000
	while (player.worldTime >= 24000) player.worldTime -= 24000;
	while (player.worldTime < 0) player.worldTime += 24000;

	// Читаем координаты (X, Y, Z)
	file.seekg(0x0068, std::ios::beg);
	file.read(reinterpret_cast<char*>(&player.x), sizeof(float));
	file.seekg(0x006C, std::ios::beg);
	file.read(reinterpret_cast<char*>(&player.y), sizeof(float));
	file.seekg(0x0070, std::ios::beg);
	file.read(reinterpret_cast<char*>(&player.z), sizeof(float));

	// Читаем направление взгляда (yaw, pitch)
	file.seekg(YAW_OFFSET, std::ios::beg);
	file.read(reinterpret_cast<char*>(&player.yaw), sizeof(float));
	file.seekg(PITCH_OFFSET, std::ios::beg);
	file.read(reinterpret_cast<char*>(&player.pitch), sizeof(float));

	// Читаем режим игры (адрес 0x0050): 0 - Ходьба, 1 - Полёт
	file.seekg(0x0050, std::ios::beg);
	file.read(&player.gameMode, sizeof(char));

	file.close();
}

// Функция для изменения координат игрока
void changePlayerCoordinates(const std::string& filename, Language lang) {
	// Читаем текущие данные игрока
	Player player;
	readPlayerData(filename, player);

	// Показываем текущие координаты с округлением до 2 знаков
	std::cout << getText("current_coordinates", lang);
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "X = " << player.x << "\n";
	std::cout << "Y = " << player.y << "\n";
	std::cout << "Z = " << player.z << "\n";

	// Спрашиваем новые координаты
	std::cout << getText("enter_new_coordinates", lang);
	std::cout << "X: ";
	std::cin >> player.x;
	std::cout << "Y: ";
	std::cin >> player.y;
	std::cout << "Z: ";
	std::cin >> player.z;

	// Открываем файл для записи
	std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
	if (!file.is_open()) {
		std::cout << getText("error_file_read_write", lang) << "\n";
		return;
	}

	// Записываем новые координаты
	file.seekp(0x0068, std::ios::beg);
	file.write(reinterpret_cast<const char*>(&player.x), sizeof(float));
	file.seekp(0x006C, std::ios::beg);
	file.write(reinterpret_cast<const char*>(&player.y), sizeof(float));
	file.seekp(0x0070, std::ios::beg);
	file.write(reinterpret_cast<const char*>(&player.z), sizeof(float));

	file.close();
	std::cout << getText("coordinates_changed", lang);
}

// Функция для изменения времени в мире
void changeWorldTime(const std::string& filename, Language lang) {
	// Читаем текущие данные
	Player player;
	readPlayerData(filename, player);

	// Показываем текущее время
	std::cout << getText("current_time", lang) << ticksToTime(player.worldTime, lang) << " (" << player.worldTime << getText("ticks_info", lang) << "\n";
	std::cout << getText("time_reference", lang);

	// Спрашиваем новое время
	std::string input;
	float worldTime;
	std::cout << getText("enter_new_time", lang);
	std::cin >> input;

	// Проверяем, введено ли время в формате HH:MM
	if (input.find(':') != std::string::npos) {
		int hours, minutes;
		char colon;
		std::stringstream ss(input);
		ss >> hours >> colon >> minutes;

		// Проверяем корректность ввода
		if (ss.fail() || colon != ':' || hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
			std::cout << getText("invalid_time_format", lang);
			worldTime = 0;
		}
		else {
			// Преобразуем HH:MM в тики (1 час = 1000 тиков, 1 минута = 1000/60 ≈ 16.67 тиков)
			worldTime = static_cast<float>(hours * 1000 + (minutes * 1000) / 60);
		}
	}
	else {
		// Пробуем интерпретировать ввод как тики
		std::stringstream ss(input);
		ss >> worldTime;
		if (ss.fail() || worldTime < 0 || worldTime > 24000) {
			std::cout << getText("invalid_time_range", lang);
			worldTime = 0;
		}
	}

	// Применяем смещение -6000 тиков
	worldTime -= 6000;
	// Нормализуем время в диапазон 0-24000
	while (worldTime >= 24000) worldTime -= 24000;
	while (worldTime < 0) worldTime += 24000;

	// Преобразуем тики в 3-байтовое значение, инвертируя направление
	const int MIN_VALUE = 454067;  // Значение для 14:00 (14000 тиков)
	const float TICKS_PER_UNIT = 14000.0f / (1159916 - 454067); // 50.4178
	int rawValue = MIN_VALUE + static_cast<int>((23000 - worldTime) / TICKS_PER_UNIT);
	// Нормализуем rawValue
	while (rawValue > 1159916) rawValue -= (1159916 - 454067);
	while (rawValue < 454067) rawValue += (1159916 - 454067);

	// Разбиваем rawValue на 3 байта (little-endian)
	unsigned char timeBytes[3];
	timeBytes[0] = rawValue & 0xFF;         // Младший байт
	timeBytes[1] = (rawValue >> 8) & 0xFF;  // Средний байт
	timeBytes[2] = (rawValue >> 16) & 0xFF; // Старший байт

	// Открываем файл для записи
	std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
	if (!file.is_open()) {
		std::cout << getText("error_file_read_write", lang) << "\n";
		return;
	}

	// Записываем новое время по адресу 0x005C (3 байта)
	file.seekp(0x005C, std::ios::beg);
	file.write(reinterpret_cast<const char*>(timeBytes), 3);

	file.close();
	std::cout << getText("time_changed", lang);
}

// Функция для изменения режима игры (Ходьба/Полёт)
void changePlayerMode(const std::string& filename, Language lang) {
	// Читаем текущие данные
	Player player;
	readPlayerData(filename, player);

	// Показываем текущий режим
	std::cout << getText("current_modes", lang);
	if (player.gameMode == 0) {
		std::cout << getText("mode_walk_on", lang);
	}
	else {
		std::cout << getText("mode_fly_on", lang);
	}

	// Спрашиваем новый режим
	int newMode;
	std::cout << getText("select_mode", lang);
	std::cin >> newMode;

	// Проверяем ввод
	if (newMode != 1 && newMode != 2) {
		std::cout << getText("invalid_mode_choice", lang);
		return;
	}

	// Преобразуем выбор в значение для gameMode: 0 - Ходьба, 1 - Полёт
	char gameModeValue = (newMode == 1) ? 0 : 1;

	// Открываем файл для записи
	std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
	if (!file.is_open()) {
		std::cout << getText("error_file_read_write", lang) << "\n";
		return;
	}

	// Записываем новый режим по адресу 0x0050
	file.seekp(0x0050, std::ios::beg);
	file.write(&gameModeValue, sizeof(char));

	file.close();

	// Показываем результат
	if (newMode == 1) {
		std::cout << getText("mode_walk_set", lang);
	}
	else {
		std::cout << getText("mode_fly_set", lang);
	}
}

// Функция для изменения направления взгляда
void changePlayerDirection(const std::string& filename, Language lang) {
	// Читаем текущие данные
	Player player;
	readPlayerData(filename, player);

	// Показываем текущее направление
	std::cout << getText("current_direction", lang) << player.yaw << ", pitch=" << player.pitch << "\n";

	// Спрашиваем новое направление
	std::cout << getText("enter_new_direction", lang);
	std::cout << "yaw (degrees, 0-360): ";
	std::cin >> player.yaw;
	std::cout << "pitch (degrees, -90 to 90): ";
	std::cin >> player.pitch;

	if (player.yaw < 0 || player.yaw > 360) {
		std::cout << getText("yaw_range_error", lang);
		player.yaw = 0;
	}
	if (player.pitch < -90 || player.pitch > 90) {
		std::cout << getText("pitch_range_error", lang);
		player.pitch = 0;
	}

	// Открываем файл для записи
	std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
	if (!file.is_open()) {
		std::cout << getText("error_file_read_write", lang) << "\n";
		return;
	}

	// Записываем новое направление
	file.seekp(YAW_OFFSET, std::ios::beg);
	file.write(reinterpret_cast<const char*>(&player.yaw), sizeof(float));
	file.seekp(PITCH_OFFSET, std::ios::beg);
	file.write(reinterpret_cast<const char*>(&player.pitch), sizeof(float));

	file.close();
	std::cout << getText("direction_changed", lang);
}