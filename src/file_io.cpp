#include "editor.h"
#include <fstream>
#include <iostream>

TextEditor::TextEditor() : unsavedChanges(false) {}

// Создание нового файла
void TextEditor::createNewFile() {
    lines.clear();
    currentFilePath.clear();
    unsavedChanges = true;
    std::cout << "Новый файл создан. Введите текст.\n";
}

// Загрузка файла
bool TextEditor::loadFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Ошибка: файл не найден или недоступен.\n";
        return false;
    }

    lines.clear();
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    currentFilePath = filePath;
    unsavedChanges = false;
    std::cout << "Файл загружен: " << filePath << "\n";
    return true;
}

// Сохранение в текущий файл
bool TextEditor::saveToFile() {
    if (currentFilePath.empty()) {
        std::cerr << "Ошибка: файл не выбран. Используйте 'save as'.\n";
        return false;
    }
    return saveToFile(currentFilePath);
}

// Сохранение в указанный файл
bool TextEditor::saveToFile(const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Ошибка: невозможно сохранить файл.\n";
        return false;
    }

    for (const auto& line : lines) {
        file << line << "\n";
    }

    currentFilePath = filePath;
    unsavedChanges = false;
    std::cout << "Файл сохранён: " << filePath << "\n";
    return true;
}

// Очистка текста
void TextEditor::clearText() {
    lines.clear();
    unsavedChanges = true;
    std::cout << "Текст очищен.\n";
}

// Вывод текста на экран
void TextEditor::displayText() const {
    if (lines.empty()) {
        std::cout << "(Файл пуст)\n";
        return;
    }
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << i + 1 << ": " << lines[i] << "\n";
    }
}

// Проверка на несохранённые изменения
bool TextEditor::hasUnsavedChanges() const {
    return unsavedChanges;
}

// Получение строк (для редактирования)
const std::vector<std::string>& TextEditor::getLines() const {
    return lines;
}