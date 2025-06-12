#include <iostream>
#include "editor.h"
#include <windows.h>
#include <sstream>
#include <vector>

void showHelp() {
    std::cout << "Команды:\n"
              << "  new             - Создать новый файл\n"
              << "  load <path>     - Загрузить файл\n"
              << "  save            - Сохранить в текущий файл\n"
              << "  saveas <path>   - Сохранить как...\n"
              << "  clear           - Очистить текст\n"
              << "  show            - Показать текст\n"
              << "  add             - Добавить строку\n"
              << "  delete <num>    - Удалить строку по номеру\n"
              << "  edit <num>      - Изменить конкретную строку\n"
              << "  replace <num> <text> - Заменить строку\n"
              << "  search <text>   - Поиск текста\n"
              << "  filter <text>   - Оставить строки, содержащие текст\n"
              << "  undo            - Отменить последнее действие\n"
              << "  redo            - Вернуть отменённое действие\n"
              << "  stats           - Показать статистику текста\n"
              << "  exit            - Выход\n"
              << "  help            - Показать эту справку\n";
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
        if (command.empty()) continue;

        std::istringstream iss(command);
        std::string cmd;
        iss >> cmd;

        if (cmd == "new") {
            editor.createNewFile();
        }
        else if (cmd == "add") {
            std::cout << "Введите строку для добавления: ";
            std::string newLine;
            std::getline(std::cin, newLine);
            if (!newLine.empty()) {
                editor.addLine(newLine);
            }
        }
        else if (cmd == "load") {
            std::string path;
            if (iss >> std::ws && std::getline(iss, path)) {
                if (!editor.loadFile(path)) {
                    std::cout << "Не удалось загрузить файл.\n";
                }
            }
            else {
                std::cout << "Ошибка: укажите путь к файлу.\n";
            }
        }
        else if (cmd == "save") {
            if (!editor.saveToFile()) {
                std::cout << "Не удалось сохранить файл.\n";
            }
        }
        else if (cmd == "saveas") {
            std::string path;
            if (iss >> std::ws && std::getline(iss, path)) {
                if (!editor.saveToFile(path)) {
                    std::cout << "Не удалось сохранить файл.\n";
                }
            }
            else {
                std::cout << "Ошибка: укажите путь к файлу.\n";
            }
        }
        else if (cmd == "clear") {
            editor.clearText();
        }
        else if (cmd == "show") {
            editor.displayText();
        }
        else if (cmd == "delete") {
            size_t lineNum;
            if (iss >> lineNum) {
                if (!editor.deleteLine(lineNum)) {
                    std::cout << "Ошибка: неверный номер строки.\n";
                }
            }
            else {
                std::cout << "Ошибка: укажите номер строки.\n";
            }
        }
        else if (cmd == "edit") {
            size_t lineNum;
            if (iss >> lineNum) {
                if (lineNum >= 1 && lineNum <= editor.getLines().size()) {
                    std::cout << "Текущий текст строки " << lineNum << ": " << editor.getLines()[lineNum-1] << "\n";
                    std::cout << "Введите новый текст: ";
                    std::string newText;
                    std::getline(std::cin, newText);
                    if (!newText.empty()) {
                        editor.replaceLine(lineNum, newText);
                    }
                } else {
                    std::cout << "Ошибка: неверный номер строки.\n";
                }
            }
            else {
                std::cout << "Ошибка: укажите номер строки.\n";
            }
        }
        else if (cmd == "replace") {
            size_t lineNum;
            std::string newText;
            if (iss >> lineNum && iss >> std::ws && std::getline(iss, newText)) {
                if (!editor.replaceLine(lineNum, newText)) {
                    std::cout << "Ошибка: неверный номер строки.\n";
                }
            }
            else {
                std::cout << "Ошибка: укажите номер строки и новый текст.\n";
            }
        }
        else if (cmd == "search") {
            std::string keyword;
            if (iss >> std::ws && std::getline(iss, keyword)) {
                auto results = editor.searchText(keyword);
                if (results.empty()) {
                    std::cout << "Текст не найден.\n";
                }
                else {
                    std::cout << "Найдено в строках: ";
                    for (auto line : results) {
                        std::cout << line << " ";
                    }
                    std::cout << "\n";
                }
            }
            else {
                std::cout << "Ошибка: укажите текст для поиска.\n";
            }
        }
        else if (cmd == "filter") {
            std::string keyword;
            if (iss >> std::ws && std::getline(iss, keyword)) {
                editor.filterLines(keyword);
            }
            else {
                std::cout << "Ошибка: укажите ключевое слово для фильтрации.\n";
            }
        }
        else if (cmd == "undo") {
            editor.undo();
        }
        else if (cmd == "redo") {
            editor.redo();
        }
        else if (cmd == "stats") {
            editor.showStats();
        }
        else if (cmd == "exit") {
            if (editor.hasUnsavedChanges()) {
                std::cout << "Есть несохранённые изменения. Выйти без сохранения? (y/n): ";
                char choice;
                std::cin >> choice;
                std::cin.ignore();
                if (choice != 'y' && choice != 'Y') continue;
            }
            break;
        }
        else if (cmd == "help") {
            showHelp();
        }
        else {
            std::cout << "Неизвестная команда. Введите 'help' для списка команд.\n";
        }
    }

    return 0;
}
