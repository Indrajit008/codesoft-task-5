#include <iostream>
#include <vector>
#include <algorithm>

class Book {
public:
    std::string title;
    std::string author;
    std::string ISBN;
    bool available;

    Book(const std::string& title, const std::string& author, const std::string& ISBN)
        : title(title), author(author), ISBN(ISBN), available(true) {}
};

class Borrower {
public:
    std::string name;
    int borrowerID;

    Borrower(const std::string& name, int borrowerID) : name(name), borrowerID(borrowerID) {}
};

class Transaction {
public:
    Book* book;
    Borrower* borrower;
    bool checkedOut;
    bool returned;
    double fine;

    Transaction(Book* book, Borrower* borrower) : book(book), borrower(borrower), checkedOut(false), returned(false), fine(0.0) {}
};

class Library {
private:
    std::vector<Book> books;
    std::vector<Borrower> borrowers;
    std::vector<Transaction> transactions;

public:
    void addBook(const std::string& title, const std::string& author, const std::string& ISBN) {
        books.push_back(Book(title, author, ISBN));
    }

    void addBorrower(const std::string& name, int borrowerID) {
        borrowers.push_back(Borrower(name, borrowerID));
    }

    void bookSearch(const std::string& keyword) {
        std::cout << "Search results:" << std::endl;
        for (const Book& book : books) {
            if (book.title.find(keyword) != std::string::npos ||
                book.author.find(keyword) != std::string::npos ||
                book.ISBN.find(keyword) != std::string::npos) {
                std::cout << "Title: " << book.title << ", Author: " << book.author << ", ISBN: " << book.ISBN
                          << ", Status: " << (book.available ? "Available" : "Checked Out") << std::endl;
            }
        }
    }

    void checkoutBook(int bookIndex, int borrowerIndex) {
        if (bookIndex >= 0 && bookIndex < books.size() && borrowerIndex >= 0 && borrowerIndex < borrowers.size()) {
            if (books[bookIndex].available) {
                transactions.push_back(Transaction(&books[bookIndex], &borrowers[borrowerIndex]));
                books[bookIndex].available = false;
                transactions.back().checkedOut = true;
                std::cout << "Book checked out successfully." << std::endl;
            } else {
                std::cout << "Book is already checked out." << std::endl;
            }
        } else {
            std::cout << "Invalid book or borrower index." << std::endl;
        }
    }

    void returnBook(int bookIndex) {
        if (bookIndex >= 0 && bookIndex < books.size()) {
            if (!books[bookIndex].available) {
                auto transactionIter = std::find_if(transactions.begin(), transactions.end(),
                                                    [&books, &bookIndex](const Transaction &transaction) {
                                                        return transaction.book == &books[bookIndex] && !transaction.returned;
                                                    });

                if (transactionIter != transactions.end()) {
                    transactionIter->returned = true;
                    books[bookIndex].available = true;

                    // Calculate fine for overdue books (simplified for demonstration)
                    // For a real-world scenario, you'd need a more sophisticated approach to fine calculation
                    // such as using due dates and actual return dates.
                    if (transactionIter->checkedOut) {
                        // Assuming a fixed fine amount for simplicity
                        const double fixedFineRate = 0.5;  // Example fine rate per day
                        const int overdueDays = 7;          // Example overdue period
                        transactionIter->fine = fixedFineRate * overdueDays;
                    }

                    std::cout << "Book returned successfully." << std::endl;
                } else {
                    std::cout << "Book transaction not found." << std::endl;
                }
            } else {
                std::cout << "Invalid book index or book is not checked out." << std::endl;
            }
        }
    }
};

int main() 
{
    Library library;

    // Adding books
    library.addBook("The Catcher in the Rye", "J.D. Salinger", "978-0-316-76948-0");
    library.addBook("To Kill a Mockingbird", "Harper Lee", "0-06-112008-1");
    library.addBook("1984", "George Orwell", "978-0-452-28423-4");

    // Adding borrowers
    library.addBorrower("John Doe", 1);
    library.addBorrower("Jane Smith", 2);

    // Searching for books
    library.bookSearch("Catcher");

    // Checking out a book
    library.checkoutBook(0, 1);

    // Returning a book
    library.returnBook(0);

    // Displaying search results after book return
    library.bookSearch("Catcher");

    return 0;
}
