#include <iostream>
#include <string>
#include <memory>
#include <fstream> // Для работы с файлами

using namespace std;

// Базовый класс "Document" (прототип)
class Document {
protected:
    string title;
    string author;
    string content;

public:
    // Конструктор
    Document(const string& title = "", const string& author = "", const string& content = "")
        : title(title), author(author), content(content) {}

    // Виртуальный деструктор
    virtual ~Document() {}

    // Метод для клонирования объекта (интерфейс прототипа)
    virtual unique_ptr<Document> clone() const = 0;

    // Метод для записи документа в файл
    virtual void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file '" << filename << "' for writing." << endl;
            return;
        }

        file << "Title: " << title << endl;
        file << "Author: " << author << endl;
        file << "Content: " << content << endl;

        file.close();
        cout << "Document saved to file: " << filename << endl;
    }

    // Методы для получения информации о документе
    void displayInfo() const {
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "Content: " << content << endl;
    }

    // Методы для установки данных
    void setTitle(const string& new_title) { title = new_title; }
    void setAuthor(const string& new_author) { author = new_author; }
    void setContent(const string& new_content) { content = new_content; }
};

// Производный класс "DocumentCopy"
class DocumentCopy : public Document {
private:
    string permissions;

public:
    // Конструктор
    DocumentCopy(const string& title = "", const string& author = "", const string& content = "", const string& permissions = "")
        : Document(title, author, content), permissions(permissions) {}

    // Реализация метода клонирования
    unique_ptr<Document> clone() const override {
        return make_unique<DocumentCopy>(*this); // Создаем глубокую копию объекта
    }

    // Метод для записи документа в файл (расширяем базовый метод)
    void saveToFile(const string& filename) const override {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file '" << filename << "' for writing." << endl;
            return;
        }

        file << "Title: " << title << endl;
        file << "Author: " << author << endl;
        file << "Content: " << content << endl;
        file << "Permissions: " << permissions << endl; // Добавляем права доступа

        file.close();
        cout << "Document with permissions saved to file: " << filename << endl;
    }

    // Метод для отображения прав доступа
    void displayPermissions() const {
        cout << "Permissions: " << permissions << endl;
    }

    // Метод для установки прав доступа
    void setPermissions(const string& new_permissions) {
        permissions = new_permissions;
    }
};

// Функция для демонстрации работы паттерна Prototype
void demonstratePrototype() {
    // Создаем оригинальный документ
    unique_ptr<Document> original = make_unique<DocumentCopy>("Report", "Ivanov", "This is an important document.", "Read/Write");

    cout << "Original Document:" << endl;
    original->displayInfo();
    dynamic_cast<DocumentCopy*>(original.get())->displayPermissions();

    // Сохраняем оригинальный документ в файл
    original->saveToFile("original_document.txt");

    // Создаем копию документа
    unique_ptr<Document> copy = dynamic_cast<DocumentCopy*>(original.get())->clone();

    cout << "\nCopied Document:" << endl;
    copy->displayInfo();
    dynamic_cast<DocumentCopy*>(copy.get())->displayPermissions();

    // Изменяем данные в копии
    dynamic_cast<DocumentCopy*>(copy.get())->setTitle("Copy of Report");
    dynamic_cast<DocumentCopy*>(copy.get())->setPermissions("Read Only");

    cout << "\nModified Copied Document:" << endl;
    copy->displayInfo();
    dynamic_cast<DocumentCopy*>(copy.get())->displayPermissions();

    // Сохраняем измененную копию в файл
    copy->saveToFile("copied_document.txt");
}

int main() {
    demonstratePrototype();
    return 0;
}