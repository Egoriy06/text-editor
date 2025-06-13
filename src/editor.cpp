#include "editor.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>

TextEditor::TextEditor() : unsavedChanges(false) {}

TextEditor::~TextEditor() {
    clearPassword();
}

void TextEditor::secureClear(std::string& str) {
    if (!str.empty()) {
        memset(&str[0], 0, str.size());
        str.clear();
    }
}

void TextEditor::clearPassword() {
    secureClear(tempPassword);
}

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

std::string TextEditor::xorCrypt(const std::string& data, const std::string& key) const {
    if (key.empty()) return data;

    std::string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ key[i % key.size()];
    }
    return result;
}

void TextEditor::saveState() {
    undoStack.push(lines);
    redoStack = std::stack<std::vector<std::string>>();
}

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

bool TextEditor::decryptFile(const std::string& password) {
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
        std::cerr << "Decryption error: " << e.what() << "\n";
        undo();
        return false;
    }
}

void TextEditor::addLine(const std::string& line) {
    saveState();
    lines.push_back(line);
    unsavedChanges = true;
}

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

std::string TextEditor::toUpper(const std::string& str) const {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c){ return std::toupper(c); });
    return result;
}

std::string TextEditor::toLower(const std::string& str) const {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return result;
}

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
    std::cout << "Statistics:\n"
              << "  Lines: " << getLineCount() << "\n"
              << "  Words: " << getWordCount() << "\n"
              << "  Characters: " << getCharCount() << "\n";
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
}
