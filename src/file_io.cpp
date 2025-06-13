
#include "editor.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <locale>
/**
 * @brief Создает новый файл (очищает текущее содержимое)
 */
void TextEditor::createNewFile() {
    saveState();
    lines.clear();
    currentFilePath.clear();
    clearPassword();
    unsavedChanges = true;
    std::cout << "New file created\n";
}

/**
 * @brief Загружает файл из указанного пути
 * @param filePath Путь к файлу
 * @return true при успешной загрузке, false при ошибке
 */
bool TextEditor::loadFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file\n";
        return false;
    }

    saveState();
    lines.clear();
    std::string line;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    currentFilePath = filePath;
    clearPassword();
    unsavedChanges = false;
    std::cout << "File loaded: " << filePath << "\n";
    return true;
}

/**
 * @brief Сохраняет файл по текущему пути
 * @return true при успешном сохранении, false при ошибке
 */
bool TextEditor::saveToFile() {
    if (currentFilePath.empty()) {
        std::cerr << "Error: No file selected\n";
        return false;
    }
    return saveToFile(currentFilePath);
}

/**
 * @brief Сохраняет файл по указанному пути
 * @param filePath Путь для сохранения
 * @return true при успешном сохранении, false при ошибке
 */
bool TextEditor::saveToFile(const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to save file\n";
        return false;
    }

    for (const auto& line : lines) {
        file << line << "\n";
    }

    currentFilePath = filePath;
    unsavedChanges = false;
    std::cout << "File saved: " << filePath << "\n";
    return true;
}

/**
 * @brief Очищает текущий текст
 */
void TextEditor::clearText() {
    saveState();
    lines.clear();
    clearPassword();
    unsavedChanges = true;
    std::cout << "Text cleared\n";
}

/**
 * @brief Отображает текущий текст с нумерацией строк
 */
void TextEditor::displayText() const {
    if (lines.empty()) {
        std::cout << "(File is empty)\n";
        return;
    }
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << i + 1 << ": " << lines[i] << "\n";
    }
}

/**
 * @brief Проверяет наличие несохраненных изменений
 * @return true если есть несохраненные изменения
 */
bool TextEditor::hasUnsavedChanges() const {
    return unsavedChanges;
}

/**
 * @brief Возвращает ссылку на текущие строки текста
 * @return Константная ссылка на вектор строк
 */
const std::vector<std::string>& TextEditor::getLines() const {
    return lines;
}

