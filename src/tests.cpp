
// test_editor.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "editor.h"
#include <fstream>
#include <filesystem>
#include <locale>
/**
 * @file tests.cpp
 * @brief Модульные тесты для класса TextEditor
 *
 * Содержит набор тестов для проверки функциональности текстового редактора,
 * включая операции с файлами, манипуляции с текстом и шифрование.
 */

TEST_SUITE("TextEditor Tests") {
    TEST_CASE("File Operations") {
        TextEditor editor;
        const std::string testFile = "test_file.txt";
        const std::string testContent = "Line 1\nLine 2\nLine 3";

        // Create test file
        {
            std::ofstream out(testFile);
            out << testContent;
        }

        SUBCASE("Load existing file") {
            CHECK(editor.loadFile(testFile));
            CHECK(editor.getLines().size() == 3);
            CHECK(editor.getLines()[0] == "Line 1");
        }

        SUBCASE("Load non-existent file") {
            CHECK_FALSE(editor.loadFile("nonexistent.txt"));
        }

        SUBCASE("Save to file") {
            editor.loadFile(testFile);
            CHECK(editor.saveToFile());
            CHECK(editor.hasUnsavedChanges() == false);
        }

        SUBCASE("Save as new file") {
            const std::string newFile = "new_file.txt";
            editor.loadFile(testFile);
            CHECK(editor.saveToFile(newFile));
            CHECK(std::filesystem::exists(newFile));
            std::filesystem::remove(newFile);
        }

        SUBCASE("Create new file") {
            editor.createNewFile();
            CHECK(editor.getLines().empty());
            CHECK(editor.hasUnsavedChanges());
        }

        SUBCASE("Clear text") {
            editor.loadFile(testFile);
            editor.clearText();
            CHECK(editor.getLines().empty());
            CHECK(editor.hasUnsavedChanges());
        }

        std::filesystem::remove(testFile);
    }

    TEST_CASE("Text Manipulation") {
        TextEditor editor;

        SUBCASE("Add line") {
            editor.addLine("Test line");
            CHECK(editor.getLines().size() == 1);
            CHECK(editor.getLines()[0] == "Test line");
            CHECK(editor.hasUnsavedChanges());
        }

        SUBCASE("Delete line - valid") {
            editor.addLine("Line 1");
            editor.addLine("Line 2");
            CHECK(editor.deleteLine(1));
            CHECK(editor.getLines().size() == 1);
            CHECK(editor.getLines()[0] == "Line 2");
        }

        SUBCASE("Delete line - invalid") {
            editor.addLine("Line 1");
            CHECK_FALSE(editor.deleteLine(0));
            CHECK_FALSE(editor.deleteLine(2));
            CHECK(editor.getLines().size() == 1);
        }

        SUBCASE("Replace line - valid") {
            editor.addLine("Old line");
            CHECK(editor.replaceLine(1, "New line"));
            CHECK(editor.getLines()[0] == "New line");
        }

        SUBCASE("Replace line - invalid") {
            CHECK_FALSE(editor.replaceLine(1, "Test"));
        }

        SUBCASE("Search text") {
            editor.addLine("This is a test");
            editor.addLine("Another test line");
            editor.addLine("No match here");

            auto results = editor.searchText("test");
            CHECK(results.size() == 2);
            CHECK(results[0] == 1);
            CHECK(results[1] == 2);

            auto noResults = editor.searchText("missing");
            CHECK(noResults.empty());
        }

        SUBCASE("Filter lines") {
            editor.addLine("apple banana");
            editor.addLine("orange");
            editor.addLine("banana cherry");

            editor.filterLines("banana");
            CHECK(editor.getLines().size() == 2);
            CHECK(editor.getLines()[0] == "apple banana");
            CHECK(editor.getLines()[1] == "banana cherry");
        }
    }

    TEST_CASE("Case Conversion") {
        TextEditor editor;
        editor.addLine("test line");
        editor.addLine("ANOTHER TEST");
        editor.addLine("Title Case Test");

        SUBCASE("To upper case - single line") {
            CHECK(editor.toUpperCase(1));
            CHECK(editor.getLines()[0] == "TEST LINE");
        }

        SUBCASE("To lower case - single line") {
            CHECK(editor.toLowerCase(2));
            CHECK(editor.getLines()[1] == "another test");
        }

        SUBCASE("To title case - single line") {
            CHECK(editor.toTitleCase(3));
            CHECK(editor.getLines()[2] == "Title Case Test");
        }

        SUBCASE("Invalid line number") {
            CHECK_FALSE(editor.toUpperCase(0));
            CHECK_FALSE(editor.toLowerCase(4));
        }

        SUBCASE("Convert all lines") {
            editor.changeAllLinesCase(1); // Upper
            CHECK(editor.getLines()[0] == "TEST LINE");
            CHECK(editor.getLines()[1] == "ANOTHER TEST");

            editor.changeAllLinesCase(2); // Lower
            CHECK(editor.getLines()[0] == "test line");
            CHECK(editor.getLines()[1] == "another test");

            editor.changeAllLinesCase(3); // Title
            CHECK(editor.getLines()[0] == "Test Line");
            CHECK(editor.getLines()[1] == "Another Test");
        }
    }

    TEST_CASE("Undo/Redo") {
        TextEditor editor;
        editor.addLine("First line");

        SUBCASE("Undo add") {
            CHECK(editor.undo());
            CHECK(editor.getLines().empty());
        }

        SUBCASE("Redo add") {
            editor.undo();
            CHECK(editor.redo());
            CHECK(editor.getLines().size() == 1);
        }

        SUBCASE("Undo with empty stack") {
            TextEditor newEditor;
            CHECK_FALSE(newEditor.undo());
        }

        SUBCASE("Redo with empty stack") {
            TextEditor newEditor;
            CHECK_FALSE(newEditor.redo());
        }
    }

    TEST_CASE("Statistics") {
        TextEditor editor;
        editor.addLine("First line");
        editor.addLine("Second line with more words");
        editor.addLine("Third");

        SUBCASE("Line count") {
            CHECK(editor.getLineCount() == 3);
        }

        SUBCASE("Word count") {
            CHECK(editor.getWordCount() == 8); // First, line, Second, line, with, more, words, Third
        }

        SUBCASE("Character count") {
            // Count includes all characters (including spaces)
            size_t expected = strlen("First line") + strlen("Second line with more words") + strlen("Third");
            CHECK(editor.getCharCount() == expected);
        }
    }

    TEST_CASE("Encryption/Decryption") {
        TextEditor editor;
        editor.addLine("This is a secret message");
        editor.addLine("Another line to encrypt");
        const std::string password = "strongPassword123";

        SUBCASE("Encrypt with empty password") {
            CHECK_FALSE(editor.encryptFile(""));
        }

        SUBCASE("Successful encryption/decryption") {
            CHECK(editor.encryptFile(password));
            auto encryptedLines = editor.getLines();

            // Encrypted lines shouldn't match original
            CHECK(encryptedLines[0] != "This is a secret message");

            // Test decryption
            CHECK(editor.decryptFile(password));
            CHECK(editor.getLines()[0] == "This is a secret message");
        }

        SUBCASE("Decryption with wrong password") {
            editor.encryptFile(password);
            CHECK_FALSE(editor.decryptFile("wrongPassword"));
        }

        SUBCASE("Password clearing") {
            editor.encryptFile(password);
            editor.clearPassword();
            // Can't directly check if password was cleared, but shouldn't crash
        }
    }
}

