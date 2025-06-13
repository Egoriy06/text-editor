#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <vector>
#include <string>
#include <stack>
#include <cstring>

class TextEditor {
private:
    std::vector<std::string> lines;
    std::string currentFilePath;
    bool unsavedChanges;
    std::stack<std::vector<std::string>> undoStack;
    std::stack<std::vector<std::string>> redoStack;
    std::string tempPassword;

    void saveState();
    std::string deriveKey(const std::string& password, size_t length) const;
    std::string xorCrypt(const std::string& data, const std::string& key) const;
    void secureClear(std::string& str);
    std::string toUpper(const std::string& str) const;
    std::string toLower(const std::string& str) const;
    std::string toTitle(const std::string& str) const;

public:
    TextEditor();
    ~TextEditor();

    void createNewFile();
    bool loadFile(const std::string& filePath);
    bool saveToFile();
    bool saveToFile(const std::string& filePath);
    void clearText();
    void displayText() const;
    bool hasUnsavedChanges() const;
    const std::vector<std::string>& getLines() const;

    bool encryptFile(const std::string& password);
    bool decryptFile(const std::string& password);
    void clearPassword();

    void addLine(const std::string& line);
    bool deleteLine(size_t lineNumber);
    bool replaceLine(size_t lineNumber, const std::string& newLine);
    std::vector<size_t> searchText(const std::string& keyword) const;
    void highlightSyntax();

    bool toUpperCase(size_t lineNumber);
    bool toLowerCase(size_t lineNumber);
    bool toTitleCase(size_t lineNumber);
    void changeAllLinesCase(int caseType);

    bool undo();
    bool redo();

    size_t getWordCount() const;
    size_t getCharCount() const;
    size_t getLineCount() const;
    void showStats() const;

    void filterLines(const std::string& keyword);
};

#endif // TEXT_EDITOR_H
