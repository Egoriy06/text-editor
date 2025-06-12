#include "editor.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

TextEditor::TextEditor() : unsavedChanges(false) {}

void TextEditor::saveState() {
    undoStack.push(lines);
    redoStack = std::stack<std::vector<std::string>>();
}

void TextEditor::addLine(const std::string& line) {
    saveState();
    lines.push_back(line);
    unsavedChanges = true;
}

bool TextEditor::deleteLine(size_t lineNumber) {
    if (lineNumber < 1 || lineNumber > lines.size()) {
        std::cerr << "Ошибка: неверный номер строки.\n";
        return false;
    }
    saveState();
    lines.erase(lines.begin() + lineNumber - 1);
    unsavedChanges = true;
    return true;
}

bool TextEditor::replaceLine(size_t lineNumber, const std::string& newLine) {
    if (lineNumber < 1 || lineNumber > lines.size()) {
        std::cerr << "Ошибка: неверный номер строки.\n";
        return false;
    }
    saveState();
    lines[lineNumber - 1] = newLine;
    unsavedChanges = true;
    return true;
}

std::vector<size_t> TextEditor::searchText(const std::string& keyword) const {
    std::vector<size_t> matches;
    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].find(keyword) != std::string::npos) {
            matches.push_back(i + 1);
        }
    }
    return matches;
}

void TextEditor::highlightSyntax() {
    for (auto& line : lines) {
        size_t pos = line.find("for");
        if (pos != std::string::npos) {
            line.replace(pos, 3, "\033[1;32mfor\033[0m");
        }
    }
}

bool TextEditor::undo() {
    if (undoStack.empty()) {
        std::cerr << "Нечего отменять.\n";
        return false;
    }
    redoStack.push(lines);
    lines = undoStack.top();
    undoStack.pop();
    unsavedChanges = true;
    return true;
}

bool TextEditor::redo() {
    if (redoStack.empty()) {
        std::cerr << "Нечего возвращать.\n";
        return false;
    }
    undoStack.push(lines);
    lines = redoStack.top();
    redoStack.pop();
    unsavedChanges = true;
    return true;
}

size_t TextEditor::getWordCount() const {
    size_t count = 0;
    for (const auto& line : lines) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) count++;
    }
    return count;
}

size_t TextEditor::getCharCount() const {
    size_t count = 0;
    for (const auto& line : lines) {
        count += line.size();
    }
    return count;
}

size_t TextEditor::getLineCount() const {
    return lines.size();
}

void TextEditor::showStats() const {
    std::cout << "Статистика:\n"
              << "  Строк: " << getLineCount() << "\n"
              << "  Слов: " << getWordCount() << "\n"
              << "  Символов: " << getCharCount() << "\n";
}

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
    std::cout << "Оставлено строк: " << lines.size() << "\n";
}
