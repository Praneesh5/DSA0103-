#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

class Book {
private:
    std::string title;
    std::string author;
    std::string isbn;
    int copies;

    void validateCopies(int c) {
        if (c < 0) {
            throw std::invalid_argument("Invalid inventory: Number of copies cannot be negative (" + std::to_string(c) + ")");
        }
    }

public:
    // 1. Master Parameterized Constructor
    Book(const std::string& t, const std::string& a, const std::string& i, int c)
        : title(t), author(a), isbn(i), copies(c) {
        validateCopies(copies);
    }

    // 2. Overloaded Constructor for Donated Book (Delegates to Master)
    explicit Book(const std::string& t)
        : Book(t, "Unknown / Donor", "N/A-DONATED", 1) {}

    // 3. Default Constructor for Empty Shell Record (Delegates to Master)
    Book()
        : Book("Untitled", "Unassigned", "000-0-00-000000-0", 0) {}

    void print() const {
        std::cout << "[BOOK RECORD] Title: "" << title 
                  << "", Author: " << author 
                  << ", ISBN: " << isbn 
                  << ", Copies Available: " << copies << "
";
    }

    int getCopies() const { return copies; }
};

int main() {
    std::cout << "=====================================================
";
    std::cout << "TASK 01: LIBRARY BOOK RECORDS CONSTRUCTORS DEMO
";
    std::cout << "=====================================================
";

    // Situation 1: Full details
    Book b1("Structure and Interpretation of Computer Programs", "Abelson & Sussman", "978-0262510875", 12);
    b1.print();

    // Situation 2: Donated book with title only
    Book b2("Clean Code: Agile Software Craftsmanship");
    b2.print();

    // Situation 3: Empty record to be filled later
    Book b3;
    b3.print();

    // Defensive Verification: Negative copies validation
    std::cout << "
Testing Negative Copies Rejection:
";
    try {
        Book invalidBook("Corrupted Record", "Hacker", "111-222", -5);
        invalidBook.print();
    } catch (const std::invalid_argument& ex) {
        std::cout << "[ERROR CAUGHT SUCCESSFULLY]: " << ex.what() << "
";
    }

    return 0;
}
