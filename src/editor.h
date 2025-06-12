#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <vector>
#include <string>
#include <stack>

class TextEditor {
private:
    std::vector<std::string> lines;
    std::string currentFilePath;
    bool unsavedChanges;
    std::stack<std::vector<std::string>> undoStack;
    std::stack<std::vector<std::string>> redoStack;

    void saveState();

public:
    TextEditor();

    // Управление файлами
    void createNewFile();
    bool loadFile(const std::string& filePath);
    bool saveToFile();
    bool saveToFile(const std::string& filePath);

    // Работа с текстом
    void clearText();
    void displayText() const;
    bool hasUnsavedChanges() const;

    // Вспомогательные функции
    const std::vector<std::string>& getLines() const;

    // Редактирование текста
    void addLine(const std::string& line);
    bool deleteLine(size_t lineNumber);
    bool replaceLine(size_t lineNumber, const std::string& newLine);
    std::vector<size_t> searchText(const std::string& keyword) const;
    void highlightSyntax();

    // Undo/Redo
    bool undo();
    bool redo();

    // Статистика
    size_t getWordCount() const;
    size_t getCharCount() const;
    size_t getLineCount() const;
    void showStats() const;

    // Фильтрация
    void filterLines(const std::string& keyword);
};

#endif // TEXT_EDITOR_H
