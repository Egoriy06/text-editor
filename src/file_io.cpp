#include "editor.h"
#include <fstream>
#include <iostream>

void TextEditor::createNewFile() {
    lines.clear();
    currentFilePath.clear();
    unsavedChanges = true;
    std::cout << "Новый файл создан. Введите текст.\n";
}

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

bool TextEditor::saveToFile() {
    if (currentFilePath.empty()) {
        std::cerr << "Ошибка: файл не выбран. Используйте 'save as'.\n";
        return false;
    }
    return saveToFile(currentFilePath);
}

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

void TextEditor::clearText() {
    lines.clear();
    unsavedChanges = true;
    std::cout << "Текст очищен.\n";
}

void TextEditor::displayText() const {
    if (lines.empty()) {
        std::cout << "(Файл пуст)\n";
        return;
    }
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << i + 1 << ": " << lines[i] << "\n";
    }
}

bool TextEditor::hasUnsavedChanges() const {
    return unsavedChanges;
}

const std::vector<std::string>& TextEditor::getLines() const {
    return lines;
}
