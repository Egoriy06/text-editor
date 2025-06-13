
#include "editor.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstring>
#include <locale>
/**
 * @brief Конструктор по умолчанию
 *
 * Инициализирует редактор без несохраненных изменений.
 */
TextEditor::TextEditor() : unsavedChanges(false) {}

/**
 * @brief Деструктор
 *
 * Очищает временный пароль из памяти при уничтожении объекта.
 */
TextEditor::~TextEditor() {
    clearPassword();
}

/**
 * @brief Безопасно очищает строку (заполняет нулями)
 * @param str Ссылка на строку для очистки
 */
void TextEditor::secureClear(std::string& str) {
    if (!str.empty()) {
        memset(&str[0], 0, str.size());
        str.clear();
    }
}

/**
 * @brief Очищает временный пароль из памяти
 */
void TextEditor::clearPassword() {
    secureClear(tempPassword);
}

/**
 * @brief Генерирует ключ шифрования на основе пароля
 * @param password Пароль для шифрования
 * @param length Требуемая длина ключа
 * @return Сгенерированный ключ
 */
std::string TextEditor::deriveKey(const std::string& password, size_t length) const {
    std::string key;
    size_t pwdLen = password.length();
    if (pwdLen == 0) return key;

    for (size_t i = 0; key.size() < length; ++i) {
        key += std::to_string(pwdLen * (i + 1)) + password;
    }
    key.resize(length);
    return key;
}

/**
 * @brief Выполняет операцию XOR-шифрования
 * @param data Данные для шифрования/расшифровки
 * @param key Ключ шифрования
 * @return Результат шифрования
 */
std::string TextEditor::xorCrypt(const std::string& data, const std::string& key) const {
    if (key.empty()) return data;

    std::string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ key[i % key.size()];
    }
    return result;
}

/**
 * @brief Сохраняет текущее состояние текста в стек отмены
 */
void TextEditor::saveState() {
    undoStack.push(lines);
    redoStack = std::stack<std::vector<std::string>>();
}

/**
 * @brief Шифрует текущий текст с использованием пароля
 * @param password Пароль для шифрования
 * @return true при успешном шифровании, false при ошибке
 */
bool TextEditor::encryptFile(const std::string& password) {
    if (password.empty()) {
        std::cerr << "Error: Password cannot be empty\n";
        return false;
    }

    saveState();
    tempPassword = password;

    try {
        for (auto& line : lines) {
            std::string key = deriveKey(password, line.size());
            line = xorCrypt(line, key);
        }
        unsavedChanges = true;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Encryption error: " << e.what() << "\n";
        undo();
        return false;
    }
}

/**
 * @brief Расшифровывает текущий текст с использованием пароля
 * @param password Пароль для расшифровки
 * @return true при успешной расшифровке, false при ошибке
 */
bool TextEditor::decryptFile(const std::string& password) {
    if (password.empty()) {
        std::cerr << "Error: Password cannot be empty\n";
        return false;
    }

    std::vector<std::string> backup = lines;
    saveState();
    tempPassword = password;

    try {
        // First pass - attempt decryption
        for (auto& line : lines) {
            std::string key = deriveKey(password, line.size());
            line = xorCrypt(line, key);
        }

        // Second pass - verify result
        bool allPrintable = true;
        for (const auto& line : lines) {
            for (char c : line) {
                if (!std::isprint(static_cast<unsigned char>(c))) {
                    allPrintable = false;
                    break;
                }
            }
            if (!allPrintable) break;
        }

        if (allPrintable) {
            unsavedChanges = true;
            return true;
        }

        // Restore backup if decryption failed
        lines = backup;
        return false;
    } catch (...) {
        lines = backup;
        return false;
    }
}


/**
 * @brief Добавляет новую строку в конец текста
 * @param line Текст добавляемой строки
 */
void TextEditor::addLine(const std::string& line) {
    saveState();
    lines.push_back(line);
    unsavedChanges = true;
}

/**
 * @brief Удаляет строку по номеру
 * @param lineNumber Номер строки (начиная с 1)
 * @return true при успешном удалении, false при неверном номере
 */
bool TextEditor::deleteLine(size_t lineNumber) {
    if (lineNumber < 1 || lineNumber > lines.size()) {
        std::cerr << "Error: Invalid line number\n";
        return false;
    }
    saveState();
    lines.erase(lines.begin() + lineNumber - 1);
    unsavedChanges = true;
    return true;
}

/**
 * @brief Заменяет содержимое строки
 * @param lineNumber Номер строки (начиная с 1)
 * @param newLine Новое содержимое строки
 * @return true при успешной замене, false при неверном номере
 */
bool TextEditor::replaceLine(size_t lineNumber, const std::string& newLine) {
    if (lineNumber < 1 || lineNumber > lines.size()) {
        std::cerr << "Error: Invalid line number\n";
        return false;
    }
    saveState();
    lines[lineNumber - 1] = newLine;
    unsavedChanges = true;
    return true;
}

/**
 * @brief Ищет текст в строках
 * @param keyword Искомый текст
 * @return Вектор номеров строк, содержащих искомый текст
 */
std::vector<size_t> TextEditor::searchText(const std::string& keyword) const {
    std::vector<size_t> matches;
    if (keyword.empty()) return matches;

    for (size_t i = 0; i < lines.size(); ++i) {
        size_t pos = 0;
        while ((pos = lines[i].find(keyword, pos)) != std::string::npos) {
            // Check word boundaries
            bool startBoundary = (pos == 0) || !std::isalnum(lines[i][pos-1]);
            bool endBoundary = (pos + keyword.length() == lines[i].length()) ||
                              !std::isalnum(lines[i][pos + keyword.length()]);

            if (startBoundary && endBoundary) {
                matches.push_back(i + 1);
                break; // Only count one match per line
            }
            pos += keyword.length();
        }
    }
    return matches;
}

/**
 * @brief Подсвечивает синтаксис в тексте (экспериментальная функция)
 */
void TextEditor::highlightSyntax() {
    for (auto& line : lines) {
        size_t pos = line.find("for");
        if (pos != std::string::npos) {
            // Check if it's a whole word
            bool startOk = (pos == 0) || !std::isalnum(line[pos-1]);
            bool endOk = (pos + 3 == line.length()) || !std::isalnum(line[pos + 3]);
            if (startOk && endOk) {
                line.replace(pos, 3, "\033[1;32mfor\033[0m");
            }
        }
    }
}

/**
 * @brief Преобразует строку в верхний регистр
 * @param str Исходная строка
 * @return Строка в верхнем регистре
 */
std::string TextEditor::toUpper(const std::string& str) const {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c){ return std::toupper(c); });
    return result;
}

/**
 * @brief Преобразует строку в нижний регистр
 * @param str Исходная строка
 * @return Строка в нижнем регистре
 */
std::string TextEditor::toLower(const std::string& str) const {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return result;
}

/**
 * @brief Преобразует строку в регистр заголовка
 * @param str Исходная строка
 * @return Строка в регистре заголовка
 */
std::string TextEditor::toTitle(const std::string& str) const {
    std::string result = str;
    bool newWord = true;
    for (char& c : result) {
        if (newWord && std::isalpha(c)) {
            c = std::toupper(c);
            newWord = false;
        } else if (std::isspace(c)) {
            newWord = true;
        } else {
            c = std::tolower(c);
        }
    }
    return result;
}

/**
 * @brief Конвертирует строку в верхний регистр
 * @param lineNumber Номер строки (начиная с 1)
 * @return true при успешном изменении, false при неверном номере
 */
bool TextEditor::toUpperCase(size_t lineNumber) {
    if (lineNumber < 1 || lineNumber > lines.size()) {
        std::cerr << "Error: Invalid line number\n";
        return false;
    }
    saveState();
    lines[lineNumber - 1] = toUpper(lines[lineNumber - 1]);
    unsavedChanges = true;
    return true;
}

/**
 * @brief Конвертирует строку в нижний регистр
 * @param lineNumber Номер строки (начиная с 1)
 * @return true при успешном изменении, false при неверном номере
 */
bool TextEditor::toLowerCase(size_t lineNumber) {
    if (lineNumber < 1 || lineNumber > lines.size()) {
        std::cerr << "Error: Invalid line number\n";
        return false;
    }
    saveState();
    lines[lineNumber - 1] = toLower(lines[lineNumber - 1]);
    unsavedChanges = true;
    return true;
}

/**
 * @brief Конвертирует строку в регистр заголовка
 * @param lineNumber Номер строки (начиная с 1)
 * @return true при успешном изменении, false при неверном номере
 */
bool TextEditor::toTitleCase(size_t lineNumber) {
    if (lineNumber < 1 || lineNumber > lines.size()) {
        std::cerr << "Error: Invalid line number\n";
        return false;
    }
    saveState();
    lines[lineNumber - 1] = toTitle(lines[lineNumber - 1]);
    unsavedChanges = true;
    return true;
}

/**
 * @brief Конвертирует все строки в указанный регистр
 * @param caseType Тип регистра (1 - верхний, 2 - нижний, 3 - заголовочный)
 */
void TextEditor::changeAllLinesCase(int caseType) {
    saveState();
    for (auto& line : lines) {
        switch (caseType) {
            case 1: line = toUpper(line); break;
            case 2: line = toLower(line); break;
            case 3: line = toTitle(line); break;
        }
    }
    unsavedChanges = true;
}

/**
 * @brief Отменяет последнее действие
 * @return true при успешной отмене, false если нечего отменять
 */
bool TextEditor::undo() {
    if (undoStack.empty()) {
        std::cerr << "Nothing to undo\n";
        return false;
    }
    redoStack.push(lines);
    lines = undoStack.top();
    undoStack.pop();
    unsavedChanges = true;
    return true;
}

/**
 * @brief Повторяет отмененное действие
 * @return true при успешном повторе, false если нечего повторять
 */
bool TextEditor::redo() {
    if (redoStack.empty()) {
        std::cerr << "Nothing to redo\n";
        return false;
    }
    undoStack.push(lines);
    lines = redoStack.top();
    redoStack.pop();
    unsavedChanges = true;
    return true;
}

/**
 * @brief Подсчитывает количество слов в тексте
 * @return Общее количество слов
 */
size_t TextEditor::getWordCount() const {
    size_t count = 0;
    for (const auto& line : lines) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            count++;
        }
    }
    return count;
}

/**
 * @brief Подсчитывает количество символов в тексте
 * @return Общее количество символов
 */
size_t TextEditor::getCharCount() const {
    size_t count = 0;
    for (const auto& line : lines) {
        count += line.length();
    }
    return count;
}

/**
 * @brief Подсчитывает количество строк
 * @return Количество строк
 */
size_t TextEditor::getLineCount() const {
    return lines.size();
}

/**
 * @brief Выводит статистику по тексту (строки, слова, символы)
 */
void TextEditor::showStats() const {
    std::cout << "Statistics:\n"
              << "  Lines: " << getLineCount() << "\n"
              << "  Words: " << getWordCount() << "\n"
              << "  Characters: " << getCharCount() << "\n";
}

/**
 * @brief Фильтрует строки, оставляя только содержащие указанный текст
 * @param keyword Текст для фильтрации
 */
void TextEditor::filterLines(const std::string& keyword) {
    saveState();
    std::vector<std::string> filteredLines;
    for (const auto& line : lines) {
        if (line.find(keyword) != std::string::npos) {
            filteredLines.push_back(line);
        }
    }
    lines = filteredLines;
    unsavedChanges = true;
}

