#include <iostream>
#include <fstream>
#include <string>
#include <clocale>
#include "GameFunctions.h"

// Для Windows: добавляем поддержку русской кодировки
#ifdef _WIN32
#include <windows.h>
#endif

void displayMenu(Language lang) {
	std::cout << "\n";
	if (lang == Language::RUSSIAN) {
		std::cout << "Выберите действие:\n";
		std::cout << "1. Поменять координаты игрока\n";
		std::cout << "2. Изменить время в мире\n";
		std::cout << "3. Изменить режим игрока (полёт/ходьба)\n";
		std::cout << "4. Изменить направление взгляда\n";
		std::cout << "0. Выход\n";
		std::cout << "Введите номер пункта: ";
	}
	else {
		std::cout << "Select an action:\n";
		std::cout << "1. Change player coordinates\n";
		std::cout << "2. Change world time\n";
		std::cout << "3. Change player mode (flying/walking)\n";
		std::cout << "4. Change view direction\n";
		std::cout << "0. Exit\n";
		std::cout << "Enter option number: ";
	}
}

int main() {
	// Устанавливаем русскую кодировку
#ifdef _WIN32
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
#endif
	setlocale(LC_ALL, "Russian");

	// Выбор языка
	Language lang;
	int langChoice;
	std::cout << "Select language (number) / Выберите язык (номер):\n";
	std::cout << "1. English\n";
	std::cout << "2. Русский\n";
	std::cout << "Enter choice / Введите выбор: ";
	std::cin >> langChoice;
	std::cin.ignore();

	if (langChoice == 1) {
		lang = Language::ENGLISH;
	}
	else {
		lang = Language::RUSSIAN;
	}

	std::string filename;
	do {
		if (lang == Language::RUSSIAN) {
			std::cout << "Введите имя файла для проверки (например, save00.dat): ";
		}
		else {
			std::cout << "Enter the file name to check (e.g., save00.dat): ";
		}
		std::getline(std::cin, filename);
		if (filename.empty()) {
			if (lang == Language::RUSSIAN) {
				std::cout << "Ошибка: имя файла не может быть пустым! Попробуйте снова.\n";
			}
			else {
				std::cout << "Error: file name cannot be empty! Try again.\n";
			}
		}
	} while (filename.empty());

	std::ifstream checkFile(filename, std::ios::binary);
	if (!checkFile.is_open()) {
		if (lang == Language::RUSSIAN) {
			std::cout << "Ошибка: не удалось открыть файл " << filename << "! Проверьте путь и повторите.\n";
			std::cout << "Нажмите Enter для выхода.\n";
		}
		else {
			std::cout << "Error: could not open file " << filename << "! Check the path and try again.\n";
			std::cout << "Press Enter to exit.\n";
		}
		std::cin.get();
		return 1;
	}
	checkFile.close();

	int choice;
	do {
		displayMenu(lang);
		std::cin >> choice;
		std::cin.ignore();

		switch (choice) {
		case 1:
			changePlayerCoordinates(filename, lang);
			break;
		case 2:
			changeWorldTime(filename, lang);
			break;
		case 3:
			changePlayerMode(filename, lang);
			break;
		case 4:
			changePlayerDirection(filename, lang);
			break;
		case 0:
			if (lang == Language::RUSSIAN) {
				std::cout << "Выход из программы.\n";
			}
			else {
				std::cout << "Exiting the program.\n";
			}
			break;
		default:
			if (lang == Language::RUSSIAN) {
				std::cout << "Неверный выбор! Введите число от 0 до 4.\n";
			}
			else {
				std::cout << "Invalid choice! Enter a number from 0 to 4.\n";
			}
		}
	} while (choice != 0);

	if (lang == Language::RUSSIAN) {
		std::cout << "Нажмите Enter для выхода.\n";
	}
	else {
		std::cout << "Press Enter to exit.\n";
	}
	std::cin.get();
	return 0;
}