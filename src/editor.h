
#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <vector>
#include <string>
#include <stack>
#include <cstring>
#include <locale>
/**
 * @class TextEditor
 * @brief Основной класс текстового редактора с поддержкой шифрования и работы с файлами
 *
 * Реализует функционал для работы с текстовыми файлами: создание, загрузку, сохранение,
 * редактирование, поиск, шифрование, изменение регистра текста и отмену действий.
 */
class TextEditor {
private:
    std::vector<std::string> lines; ///< Содержимое файла (каждый элемент - строка)
    std::string currentFilePath;    ///< Путь к текущему открытому файлу
    bool unsavedChanges;            ///< Флаг наличия несохраненных изменений
    std::stack<std::vector<std::string>> undoStack; ///< Стек состояний для отмены действий
    std::stack<std::vector<std::string>> redoStack; ///< Стек состояний для повтора действий
    std::string tempPassword;       ///< Временное хранение пароля для шифрования

    /**
     * @brief Сохраняет текущее состояние текста в стек отмены
     */
    void saveState();

    /**
     * @brief Генерирует ключ шифрования на основе пароля
     * @param password Пароль для шифрования
     * @param length Требуемая длина ключа
     * @return Сгенерированный ключ
     */
    std::string deriveKey(const std::string& password, size_t length) const;

    /**
     * @brief Выполняет операцию XOR-шифрования
     * @param data Данные для шифрования/расшифровки
     * @param key Ключ шифрования
     * @return Результат шифрования
     */
    std::string xorCrypt(const std::string& data, const std::string& key) const;

    /**
     * @brief Безопасно очищает строку (заполняет нулями)
     * @param str Ссылка на строку для очистки
     */
    void secureClear(std::string& str);

    /**
     * @brief Преобразует строку в верхний регистр
     * @param str Исходная строка
     * @return Строка в верхнем регистре
     */
    std::string toUpper(const std::string& str) const;

    /**
     * @brief Преобразует строку в нижний регистр
     * @param str Исходная строка
     * @return Строка в нижнем регистре
     */
    std::string toLower(const std::string& str) const;

    /**
     * @brief Преобразует строку в регистр заголовка
     * @param str Исходная строка
     * @return Строка в регистре заголовка
     */
    std::string toTitle(const std::string& str) const;

public:
    /**
     * @brief Конструктор по умолчанию
     */
    TextEditor();

    /**
     * @brief Деструктор
     */
    ~TextEditor();

    /**
     * @brief Создает новый файл (очищает текущее содержимое)
     */
    void createNewFile();

    /**
     * @brief Загружает файл из указанного пути
     * @param filePath Путь к файлу
     * @return true при успешной загрузке, false при ошибке
     */
    bool loadFile(const std::string& filePath);

    /**
     * @brief Сохраняет файл по текущему пути
     * @return true при успешном сохранении, false при ошибке
     */
    bool saveToFile();

    /**
     * @brief Сохраняет файл по указанному пути
     * @param filePath Путь для сохранения
     * @return true при успешном сохранении, false при ошибке
     */
    bool saveToFile(const std::string& filePath);

    /**
     * @brief Очищает текущий текст
     */
    void clearText();

    /**
     * @brief Отображает текущий текст с нумерацией строк
     */
    void displayText() const;

    /**
     * @brief Проверяет наличие несохраненных изменений
     * @return true если есть несохраненные изменения
     */
    bool hasUnsavedChanges() const;

    /**
     * @brief Возвращает ссылку на текущие строки текста
     * @return Константная ссылка на вектор строк
     */
    const std::vector<std::string>& getLines() const;

    /**
     * @brief Шифрует текущий текст с использованием пароля
     * @param password Пароль для шифрования
     * @return true при успешном шифровании, false при ошибке
     */
    bool encryptFile(const std::string& password);

    /**
     * @brief Расшифровывает текущий текст с использованием пароля
     * @param password Пароль для расшифровки
     * @return true при успешной расшифровке, false при ошибке
     */
    bool decryptFile(const std::string& password);

    /**
     * @brief Очищает временный пароль из памяти
     */
    void clearPassword();

    /**
     * @brief Добавляет новую строку в конец текста
     * @param line Текст добавляемой строки
     */
    void addLine(const std::string& line);

    /**
     * @brief Удаляет строку по номеру
     * @param lineNumber Номер строки (начиная с 1)
     * @return true при успешном удалении, false при неверном номере
     */
    bool deleteLine(size_t lineNumber);

    /**
     * @brief Заменяет содержимое строки
     * @param lineNumber Номер строки (начиная с 1)
     * @param newLine Новое содержимое строки
     * @return true при успешной замене, false при неверном номере
     */
    bool replaceLine(size_t lineNumber, const std::string& newLine);

    /**
     * @brief Ищет текст в строках
     * @param keyword Искомый текст
     * @return Вектор номеров строк, содержащих искомый текст
     */
    std::vector<size_t> searchText(const std::string& keyword) const;

    /**
     * @brief Подсвечивает синтаксис в тексте (заглушка)
     */
    void highlightSyntax();

    /**
     * @brief Конвертирует строку в верхний регистр
     * @param lineNumber Номер строки (начиная с 1)
     * @return true при успешном изменении, false при неверном номере
     */
    bool toUpperCase(size_t lineNumber);

    /**
     * @brief Конвертирует строку в нижний регистр
     * @param lineNumber Номер строки (начиная с 1)
     * @return true при успешном изменении, false при неверном номере
     */
    bool toLowerCase(size_t lineNumber);

    /**
     * @brief Конвертирует строку в регистр заголовка
     * @param lineNumber Номер строки (начиная с 1)
     * @return true при успешном изменении, false при неверном номере
     */
    bool toTitleCase(size_t lineNumber);

    /**
     * @brief Конвертирует все строки в указанный регистр
     * @param caseType Тип регистра (1 - верхний, 2 - нижний, 3 - заголовочный)
     */
    void changeAllLinesCase(int caseType);

    /**
     * @brief Отменяет последнее действие
     * @return true при успешной отмене, false если нечего отменять
     */
    bool undo();

    /**
     * @brief Повторяет отмененное действие
     * @return true при успешном повторе, false если нечего повторять
     */
    bool redo();

    /**
     * @brief Подсчитывает количество слов в тексте
     * @return Общее количество слов
     */
    size_t getWordCount() const;

    /**
     * @brief Подсчитывает количество символов в тексте
     * @return Общее количество символов
     */
    size_t getCharCount() const;

    /**
     * @brief Подсчитывает количество строк
     * @return Количество строк
     */
    size_t getLineCount() const;

    /**
     * @brief Выводит статистику по тексту (строки, слова, символы)
     */
    void showStats() const;

    /**
     * @brief Фильтрует строки, оставляя только содержащие указанный текст
     * @param keyword Текст для фильтрации
     */
    void filterLines(const std::string& keyword);
};

#endif // TEXT_EDITOR_H
