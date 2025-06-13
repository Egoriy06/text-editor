
#include <iostream>
#include "editor.h"
#include <windows.h>
#include <sstream>
#include <vector>
#include <locale>
/**
 * @brief Отображает справочную информацию по командам
 */
void showHelp() {
    std::cout << "Commands:\n"
              << "  new             - Create new file\n"
              << "  load <path>     - Load file\n"
              << "  save            - Save to current file\n"
              << "  saveas <path>   - Save as...\n"
              << "  encrypt         - Encrypt file\n"
              << "  decrypt         - Decrypt file\n"
              << "  clear           - Clear text\n"
              << "  show            - Show text\n"
              << "  add             - Add line\n"
              << "  delete <num>    - Delete line by number\n"
              << "  edit <num>      - Edit specific line\n"
              << "  replace <num> <text> - Replace line\n"
              << "  search <text>   - Search text\n"
              << "  filter <text>   - Keep lines containing text\n"
              << "  upper <num>     - Convert line to uppercase\n"
              << "  lower <num>     - Convert line to lowercase\n"
              << "  title <num>     - Convert line to title case\n"
              << "  allupper        - Convert all lines to uppercase\n"
              << "  alllower        - Convert all lines to lowercase\n"
              << "  alltitle        - Convert all lines to title case\n"
              << "  undo            - Undo last action\n"
              << "  redo            - Redo undone action\n"
              << "  stats           - Show text statistics\n"
              << "  exit            - Exit\n"
              << "  help            - Show this help\n";
}

/**
 * @brief Главная функция текстового редактора
 * @return Код завершения программы
 */
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    TextEditor editor;
    std::string command;

    std::cout << "Text Editor (C++) with Case Conversion\n";
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
            std::cout << "Enter line to add: ";
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
                    std::cout << "Failed to load file.\n";
                }
            }
            else {
                std::cout << "Error: Specify file path.\n";
            }
        }
        else if (cmd == "save") {
            if (!editor.saveToFile()) {
                std::cout << "Failed to save file.\n";
            }
        }
        else if (cmd == "saveas") {
            std::string path;
            if (iss >> std::ws && std::getline(iss, path)) {
                if (!editor.saveToFile(path)) {
                    std::cout << "Failed to save file.\n";
                }
            }
            else {
                std::cout << "Error: Specify file path.\n";
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
                    std::cout << "Error: Invalid line number.\n";
                }
            }
            else {
                std::cout << "Error: Specify line number.\n";
            }
        }
        else if (cmd == "edit") {
            size_t lineNum;
            if (iss >> lineNum) {
                if (lineNum >= 1 && lineNum <= editor.getLines().size()) {
                    std::cout << "Current text of line " << lineNum << ": " << editor.getLines()[lineNum-1] << "\n";
                    std::cout << "Enter new text: ";
                    std::string newText;
                    std::getline(std::cin, newText);
                    if (!newText.empty()) {
                        editor.replaceLine(lineNum, newText);
                    }
                } else {
                    std::cout << "Error: Invalid line number.\n";
                }
            }
            else {
                std::cout << "Error: Specify line number.\n";
            }
        }
        else if (cmd == "replace") {
            size_t lineNum;
            std::string newText;
            if (iss >> lineNum && iss >> std::ws && std::getline(iss, newText)) {
                if (!editor.replaceLine(lineNum, newText)) {
                    std::cout << "Error: Invalid line number.\n";
                }
            }
            else {
                std::cout << "Error: Specify line number and new text.\n";
            }
        }
        else if (cmd == "search") {
            std::string keyword;
            if (iss >> std::ws && std::getline(iss, keyword)) {
                auto results = editor.searchText(keyword);
                if (results.empty()) {
                    std::cout << "Text not found.\n";
                }
                else {
                    std::cout << "Found in lines: ";
                    for (auto line : results) {
                        std::cout << line << " ";
                    }
                    std::cout << "\n";
                }
            }
            else {
                std::cout << "Error: Specify search text.\n";
            }
        }
        else if (cmd == "filter") {
            std::string keyword;
            if (iss >> std::ws && std::getline(iss, keyword)) {
                editor.filterLines(keyword);
            }
            else {
                std::cout << "Error: Specify filter keyword.\n";
            }
        }
        else if (cmd == "encrypt") {
            std::cout << "Enter password: ";
            std::string password;
            std::getline(std::cin, password);
            if (editor.encryptFile(password)) {
                std::cout << "File encrypted. Remember to save changes!\n";
            }
        }
        else if (cmd == "decrypt") {
            std::cout << "Enter password: ";
            std::string password;
            std::getline(std::cin, password);
            if (editor.decryptFile(password)) {
                std::cout << "File decrypted.\n";
            } else {
                std::cout << "Failed to decrypt (wrong password?)\n";
            }
        }
        else if (cmd == "upper") {
            size_t lineNum;
            if (iss >> lineNum) {
                if (!editor.toUpperCase(lineNum)) {
                    std::cout << "Error: Invalid line number.\n";
                }
            } else {
                std::cout << "Error: Specify line number.\n";
            }
        }
        else if (cmd == "lower") {
            size_t lineNum;
            if (iss >> lineNum) {
                if (!editor.toLowerCase(lineNum)) {
                    std::cout << "Error: Invalid line number.\n";
                }
            } else {
                std::cout << "Error: Specify line number.\n";
            }
        }
        else if (cmd == "title") {
            size_t lineNum;
            if (iss >> lineNum) {
                if (!editor.toTitleCase(lineNum)) {
                    std::cout << "Error: Invalid line number.\n";
                }
            } else {
                std::cout << "Error: Specify line number.\n";
            }
        }
        else if (cmd == "allupper") {
            editor.changeAllLinesCase(1);
            std::cout << "All lines converted to uppercase.\n";
        }
        else if (cmd == "alllower") {
            editor.changeAllLinesCase(2);
            std::cout << "All lines converted to lowercase.\n";
        }
        else if (cmd == "alltitle") {
            editor.changeAllLinesCase(3);
            std::cout << "All lines converted to title case.\n";
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
                std::cout << "You have unsaved changes. Exit without saving? (y/n): ";
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
            std::cout << "Unknown command. Type 'help' for command list.\n";
        }
    }

    return 0;
}

