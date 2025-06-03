#include <iostream>
#include "editor.h"
#include <windows.h>

void showHelp() {
    std::cout << "Команды:\n"
              << "  new      - Создать новый файл\n"
              << "  load <path> - Загрузить файл\n"
              << "  save     - Сохранить в текущий файл\n"
              << "  saveas <path> - Сохранить как...\n"
              << "  clear    - Очистить текст\n"
              << "  show     - Показать текст\n"
              << "  exit     - Выход\n"
              << "  add      - Добавить строку\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    TextEditor editor;
    std::string command;

    std::cout << "Текстовый редактор (C++)\n";
    showHelp();

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "new") {
            editor.createNewFile();
        } else if (command == "add") {
            std::cout << "Введите строку для добавления: ";
            std::string newLine;
            std::getline(std::cin, newLine);
            editor.addLine(newLine);        
        } else if (command.substr(0, 4) == "load") {
            std::string path = command.substr(5);
            editor.loadFile(path);
        } else if (command == "save") {
            editor.saveToFile();
        } else if (command.substr(0, 6) == "saveas") {
            std::string path = command.substr(7);
            editor.saveToFile(path);
        } else if (command == "clear") {
            editor.clearText();
        } else if (command == "show") {
            editor.displayText();
        } else if (command == "exit") {
            if (editor.hasUnsavedChanges()) {
                std::cout << "Есть несохранённые изменения. Выйти без сохранения? (y/n): ";
                char choice;
                std::cin >> choice;
                if (choice != 'y') continue;
            }
            break;
        } else {
            std::cout << "Неизвестная команда. Введите 'help' для списка команд.\n";
        }
    }

    return 0;
}