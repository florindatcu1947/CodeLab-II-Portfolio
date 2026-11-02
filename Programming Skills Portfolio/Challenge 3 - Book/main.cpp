#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

class Book {
private:
    std::string title;
    std::string author;
    int pages;
    std::string id;
    bool isBorrowed;

public:
    Book(std::string t, std::string a, int p, std::string i, bool b)
        : title(t), author(a), pages(p), id(i), isBorrowed(b) {}

    std::string getId() const { return id; }
    bool getBorrowedStatus() const { return isBorrowed; }

    void borrowBook() {
        if (!isBorrowed) {
            isBorrowed = true;
            std::cout << "Book successfully borrowed.\n";
        } else {
            std::cout << "This book is already borrowed.\n";
        }
    }

    void returnBook() {
        if (isBorrowed) {
            isBorrowed = false;
            std::cout << "Book successfully returned.\n";
        } else {
            std::cout << "This book is not currently borrowed.\n";
        }
    }

    void displaySummary() const {
        std::cout << "[" << id << "] " << std::left << std::setw(30) << title 
                  << " | " << std::setw(20) << author 
                  << " | " << (isBorrowed ? "Borrowed" : "Available") << "\n";
    }

    void displayFullDetails() const {
        std::cout << "\n--- Book Details ---\n";
        std::cout << "ID:       " << id << "\n";
        std::cout << "Title:    " << title << "\n";
        std::cout << "Author:   " << author << "\n";
        std::cout << "Pages:    " << pages << "\n";
        std::cout << "Status:   " << (isBorrowed ? "Borrowed" : "Available") << "\n";
    }
};

std::vector<Book> loadBooks(const std::string& filename) {
    std::vector<Book> library;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening " << filename << "\n";
        return library;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string title, author, pagesStr, id, statusStr;

        std::getline(ss, title, ',');
        std::getline(ss, author, ',');
        std::getline(ss, pagesStr, ',');
        std::getline(ss, id, ',');
        std::getline(ss, statusStr, ',');

        int pages = std::stoi(pagesStr);
        bool status = (statusStr == "true" || statusStr == "1");

        library.emplace_back(title, author, pages, id, status);
    }
    file.close();
    return library;
}

Book* findBook(std::vector<Book>& library, const std::string& id) {
    for (auto& book : library) {
        if (book.getId() == id) {
            return &book;
        }
    }
    return nullptr;
}

int main() {
    std::vector<Book> library = loadBooks("bookData.txt");

    if (library.empty()) {
        std::cerr << "Library database empty or file not found.\n";
        return 1;
    }

    int choice = 0;
    while (choice != 5) {
        std::cout << "\n=== Library Management System ===\n";
        std::cout << "1. View All Books\n";
        std::cout << "2. View Individual Book\n";
        std::cout << "3. Borrow a Book\n";
        std::cout << "4. Return a Book\n";
        std::cout << "5. Exit\n";
        std::cout << "Choose an option: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 1) {
            std::cout << "\n--------------------------------------------------------------\n";
            for (const auto& book : library) {
                book.displaySummary();
            }
            std::cout << "--------------------------------------------------------------\n";
        } else if (choice == 2 || choice == 3 || choice == 4) {
            std::string bookId;
            std::cout << "Enter Book ID: ";
            std::cin >> bookId;

            Book* book = findBook(library, bookId);
            if (!book) {
                std::cout << "Book with ID " << bookId << " not found.\n";
            } else {
                if (choice == 2) book->displayFullDetails();
                else if (choice == 3) book->borrowBook();
                else if (choice == 4) book->returnBook();
            }
        }
    }

    std::cout << "Exiting system.\n";
    return 0;
}