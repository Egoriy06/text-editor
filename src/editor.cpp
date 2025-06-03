#include "editor.h"
#include <algorithm>
#include <cctype>
#include <iostream>

// Добавление строки в текст
void TextEditor::addLine(const std::string& line) {
    lines.push_back(line);
    unsavedChanges = true;
}

// Удаление строки по номеру (индексация с 1)
bool TextEditor::deleteLine(size_t lineNumber) {
    if (lineNumber < 1 || lineNumber > lines.size()) {
        std::cerr << "Ошибка: неверный номер строки.\n";
        return false;
    }
    lines.erase(lines.begin() + lineNumber - 1);
    unsavedChanges = true;
    return true;
}

// Замена строки
bool TextEditor::replaceLine(size_t lineNumber, const std::string& newLine) {
    if (lineNumber < 1 || lineNumber > lines.size()) {
        std::cerr << "Ошибка: неверный номер строки.\n";
        return false;
    }
    lines[lineNumber - 1] = newLine;
    unsavedChanges = true;
    return true;
}

// Поиск по ключевому слову (возвращает номера строк)
std::vector<size_t> TextEditor::searchText(const std::string& keyword) const {
    std::vector<size_t> matches;
    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].find(keyword) != std::string::npos) {
            matches.push_back(i + 1); // Нумерация с 1 для пользователя
        }
    }
    return matches;
}

// (Опционально) Подсветка синтаксиса для C++ (упрощённая версия)
void TextEditor::highlightSyntax() {
    for (auto& line : lines) {
        // Простая замена ключевых слов
        size_t pos = line.find("for");
        if (pos != std::string::npos) {
            line.replace(pos, 3, "\033[1;32mfor\033[0m"); // Зелёный
        }
        // Аналогично для других ключевых слов...
    }
}