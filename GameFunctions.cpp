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
		if (key == "time_reference") return "20000 (00:00) - полночь, 2000 (06:00) - утро, 8000 (12:00) - день, 14000 (18:00) - вечер\n";
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
		if (key == "time_reference") return "20000 (00:00) - midnight, 2000 (06:00) - morning, 8000 (12:00) - day, 14000 (18:00) - evening\n";
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

// Массив с точными байтами для часов 06:00–13:00
const unsigned char predefinedTimeBytes[8][3] = {
	{0xE7, 0x12, 0x17}, // 06:00
	{0xF3, 0xD8, 0x17}, // 07:00
	{0xD9, 0x9C, 0x18}, // 08:00
	{0xD7, 0x13, 0x17}, // 09:00
	{0x56, 0x26, 0x1A}, // 10:00
	{0x68, 0xEB, 0x1A}, // 11:00
	{0x6A, 0xB0, 0x1B}, // 12:00
	{0x28, 0x75, 0x1C}  // 13:00
};

// Функция для преобразования тиков в формат HH:MM
std::string ticksToTime(int ticks, Language lang) {
	while (ticks >= 24000) ticks -= 24000;
	while (ticks < 0) ticks += 24000;

	// Adjust offset: 10000 ticks = 14:00
	ticks = (ticks + 4000) % 24000;

	// 1 день = 24000 тиков = 1440 минут
	int totalMinutes = static_cast<int>(ticks * (1440.0f / 24000.0f));
	int hours = (totalMinutes / 60) % 24;
	int minutes = totalMinutes % 60;

	std::stringstream ss;
	ss << std::setfill('0') << std::setw(2) << hours << ":" << std::setfill('0') << std::setw(2) << minutes;
	return ss.str();
}

// Функция для конвертации HH:MM в тики
int convertTimeToTicks(int hours, int minutes) {
	// Конвертируем время в минуты
	int totalMinutes = hours * 60 + minutes;
	// Учитываем цикличность: 24 часа = 1 день
	int minutesInDay = totalMinutes % (24 * 60);
	// 1 минута = 1000 / 60 тиков
	int ticks = static_cast<int>(minutesInDay * (1000.0f / 60));
	// Учитываем смещение: 14:00 в игре = 10000 тиков
	ticks = (ticks - 4000 + 24000) % 24000;
	return ticks;
}

// Функция для чтения данных игрока из файла
void readPlayerData(const std::string& filename, Player& player) {
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		std::cout << getText("error_file_open", Language::RUSSIAN) << filename << "!\n";
		return;
	}

	// Читаем время в мире (адрес 0x0058, смещение 88, 3 байта)
	unsigned char timeBytes[3];
	file.seekg(88, std::ios::beg); // Смещение 88
	std::streampos pos = file.tellg();
	std::cout << "Позиция курсора перед чтением (должно быть 88): " << pos << "\n";
	file.read(reinterpret_cast<char*>(timeBytes), 3);

	// Отладочный вывод: какие байты мы прочитали
	std::cout << "Прочитанные байты времени (по адресу 0x0058, смещение 88): "
		<< std::hex << std::setw(2) << std::setfill('0') << (int)timeBytes[0] << " "
		<< std::hex << std::setw(2) << std::setfill('0') << (int)timeBytes[1] << " "
		<< std::hex << std::setw(2) << std::setfill('0') << (int)timeBytes[2] << std::dec << "\n";

	// Преобразуем 3 байта в целое число (little-endian)
	int rawValue = (timeBytes[2] << 16) | (timeBytes[1] << 8) | timeBytes[0];

	// Преобразуем rawValue в тики (0-24000)
	if (rawValue >= 705305 && rawValue <= 1157159) {
		// От 10000 до 19000 тиков
		player.worldTime = 10000 + static_cast<int>((rawValue - 705305) * (9000.0f / 451854.0f));
	}
	else if (rawValue >= 1157159 && rawValue <= 1462273) {
		// От 19000 до 1000 тиков
		player.worldTime = 19000 + static_cast<int>((rawValue - 1157159) * (6000.0f / 305114.0f));
	}
	else {
		// Если rawValue вне диапазона, установим 0
		player.worldTime = 0;
	}

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
	int worldTime;
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
			// Преобразуем HH:MM в тики
			worldTime = convertTimeToTicks(hours, minutes);
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

	// Проверяем, можно ли использовать предопределённые байты (для 06:00–13:00 с минутами 00)
	unsigned char timeBytes[3];
	bool usePredefined = false;
	int hours = (worldTime + 4000) % 24000 / 1000; // Примерное преобразование тиков в часы
	int minutes = static_cast<int>((worldTime + 4000) % 24000 * (1440.0f / 24000.0f)) % 60;

	if (minutes == 0 && hours >= 6 && hours <= 13) {
		// Используем предопределённые байты для 06:00–13:00
		int index = hours - 6; // Смещение: 6 → 0, 7 → 1, ..., 13 → 7
		timeBytes[0] = predefinedTimeBytes[index][0];
		timeBytes[1] = predefinedTimeBytes[index][1];
		timeBytes[2] = predefinedTimeBytes[index][2];
		usePredefined = true;
	}
	else {
		// Преобразуем тики в rawValue
		int rawValue;
		if (worldTime >= 10000 && worldTime <= 19000) {
			// От 10000 до 19000 тиков
			rawValue = 705305 + static_cast<int>((worldTime - 10000) * (451854.0f / 9000.0f));
		}
		else {
			// От 19000 до 24000 или от 0 до 1000
			int adjustedTicks = (worldTime < 10000) ? (worldTime + 24000) : worldTime;
			rawValue = 1157159 + static_cast<int>((adjustedTicks - 19000) * (305114.0f / 6000.0f));
		}

		// Нормализуем rawValue
		while (rawValue > 1462273) rawValue -= (1462273 - 705305);
		while (rawValue < 705305) rawValue += (1462273 - 705305);

		// Разбиваем rawValue на 3 байта (little-endian)
		timeBytes[0] = rawValue & 0xFF;         // Младший байт
		timeBytes[1] = (rawValue >> 8) & 0xFF;  // Средний байт
		timeBytes[2] = (rawValue >> 16) & 0xFF; // Старший байт
	}

	// Отладочный вывод: какие байты мы записываем
	std::cout << "Записываемые байты времени (по адресу 0x0058, смещение 88): "
		<< std::hex << std::setw(2) << std::setfill('0') << (int)timeBytes[0] << " "
		<< std::hex << std::setw(2) << std::setfill('0') << (int)timeBytes[1] << " "
		<< std::hex << std::setw(2) << std::setfill('0') << (int)timeBytes[2] << std::dec << "\n";

	// Открываем файл для записи
	std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
	if (!file.is_open()) {
		std::cout << getText("error_file_read_write", lang) << "\n";
		return;
	}

	// Записываем новое время по адресу 0x0058 (смещение 88, 3 байта)
	file.seekp(88, std::ios::beg);
	std::streampos pos = file.tellp();
	std::cout << "Позиция курсора перед записью (должно быть 88): " << pos << "\n";
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