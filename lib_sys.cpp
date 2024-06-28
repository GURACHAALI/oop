#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Book class represents a book in the library
class Book {
public:
    int id;
    std::string title;
    std::string author;
    bool isBorrowed;

    // Constructor to initialize a book with id, title, and author
    Book(int id, const std::string& title, const std::string& author)
        : id(id), title(title), author(author), isBorrowed(false) {}

    // Method to display information about the book
    void displayBookInfo() const {
        std::cout << "Book ID: " << id << "\nTitle: " << title 
                  << "\nAuthor: " << author << "\nStatus: " 
                  << (isBorrowed ? "Borrowed" : "Available") << "\n";
    }
};

// User class represents a user of the library
class User {
public:
    int id;
    std::string name;
    std::vector<int> borrowedBookIds;

    // Constructor to initialize a user with id and name
    User(int id, const std::string& name) 
        : id(id), name(name) {}

    // Method to display information about the user
    void displayUserInfo() const {
        std::cout << "User ID: " << id << "\nName: " << name << "\nBorrowed Books: ";
        for (int bookId : borrowedBookIds) {
            std::cout << bookId << " ";
        }
        std::cout << "\n";
    }

    // Method to add a book to the user's borrowed books
    void borrowBook(int bookId) {
        borrowedBookIds.push_back(bookId);
    }

    // Method to remove a book from the user's borrowed books
    void returnBook(int bookId) {
        borrowedBookIds.erase(std::remove(borrowedBookIds.begin(), borrowedBookIds.end(), bookId), borrowedBookIds.end());
    }
};

// Library class represents the library system
class Library {
private:
    std::vector<Book> books;
    std::vector<User> users;
    User* loggedInUser = nullptr;
    const std::string libraryPassword = "securepassword"; // Single password for all users

public:
    // Method to add a book to the library
    void addBook(const Book& book) {
        books.push_back(book);
    }

    // Method to add a user to the library
    void addUser(const User& user) {
        users.push_back(user);
    }

    // Method to display all books in the library
    void displayAllBooks() const {
        for (const auto& book : books) {
            book.displayBookInfo();
        }
    }

    // Method to display all users in the library
    void displayAllUsers() const {
        for (const auto& user : users) {
            user.displayUserInfo();
        }
    }

    // Method to log in a user
    bool login(const std::string& username, const std::string& password) {
        if (password == libraryPassword) {
            // If the password is correct, create a new User with the provided username
            int newId = users.size() + 1; // Generate a new ID for the user
            loggedInUser = new User(newId, username); // Allocate memory for the new user
            users.push_back(*loggedInUser); // Add the user to the library
            std::cout << "Login successful.\n";
            return true;
        }
        std::cout << "Login failed. Incorrect password.\n";
        return false;
    }

    // Method to log out the current user
    void logout() {
        if (loggedInUser) {
            delete loggedInUser; // Deallocate memory
            loggedInUser = nullptr;
            std::cout << "Logout successful.\n";
        } else {
            std::cout << "No user is currently logged in.\n";
        }
    }

    // Method to search for a book by title
    void searchBookByTitle() const {
        std::string searchTerm;
        std::cout << "Enter the title of the book you want to search for: ";
        std::cin.ignore();
        std::getline(std::cin, searchTerm);

        bool found = false;
        for (const auto& book : books) {
            if (book.title.find(searchTerm) != std::string::npos) {
                book.displayBookInfo();
                found = true;
            }
        }
        if (!found) {
            std::cout << "Book not found.\n";  
        }
    }

    // Method to borrow a book by title
    void borrowBookByTitle() {
        if (!loggedInUser) {
            std::cout << "You must be logged in to borrow books.\n";
            return;
        }

        std::string borrowTerm;
        std::cout << "Enter the title of the book you want to borrow: ";
        std::cin.ignore();
        std::getline(std::cin, borrowTerm);

        for (auto& book : books) {
            if (book.title.find(borrowTerm) != std::string::npos && !book.isBorrowed) {
                book.isBorrowed = true;
                loggedInUser->borrowBook(book.id);
                std::cout << "Book borrowed successfully.\n";
                return;
            }
        }
        std::cout << "Book not available or already borrowed.\n";
    }

    // Method to return a borrowed book by title
    void returnBookByTitle() {
        if (!loggedInUser) {
            std::cout << "You must be logged in to return books.\n";
            return;
        }

        std::string returnTerm;
        std::cout << "Enter the title of the book you want to return: ";
        std::cin.ignore();
        std::getline(std::cin, returnTerm);

        for (auto& book : books) {
            if (book.title.find(returnTerm) != std::string::npos && book.isBorrowed) {
                book.isBorrowed = false;
                loggedInUser->returnBook(book.id);
                std::cout << "Book returned successfully.\n";
                return;
            }
        }
        std::cout << "Book not found in borrowed list.\n";
    }
};

int main() {
    Library library;

    // Adding sample books
    library.addBook(Book(1, "The Great Gatsby", "F. Scott Fitzgerald"));
    library.addBook(Book(2, "1984", "George Orwell"));
    library.addBook(Book(3, "To Kill a Mockingbird", "Harper Lee"));

    // Adding sample users
    library.addUser(User(1, "Alice"));
    library.addUser(User(2, "Bob"));

    // Attempt to login
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (library.login(username, password)) {
        // User actions after login
        library.searchBookByTitle();
        library.borrowBookByTitle();
        library.searchBookByTitle();
        library.returnBookByTitle();
        library.searchBookByTitle();

        // Logout
        library.logout();
    } else {
        std::cout << "Login failed. Please try again.\n";
    }

    return 0;
}