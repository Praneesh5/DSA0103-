#include <iostream>
#include <cstring>
#include <stdexcept>

class Text {
    char* buf;
    size_t len;

public:
    // Constructor
    Text(const char* s = "") {
        len = std::strlen(s);
        buf = new char[len + 1];
        std::strcpy(buf, s);
    }

    // Copy Constructor (Deep Copy)
    Text(const Text& o) : len(o.len), buf(new char[o.len + 1]) {
        std::strcpy(buf, o.buf);
    }

    // Copy Assignment Operator (Self-Assignment Guarded)
    Text& operator=(const Text& o) {
        if (this != &o) {
            char* new_buf = new char[o.len + 1];
            std::strcpy(new_buf, o.buf);
            delete[] buf;
            buf = new_buf;
            len = o.len;
        }
        return *this;
    }

    // Destructor
    ~Text() {
        delete[] buf;
    }

    // Overload + for Concatenation
    Text operator+(const Text& o) const {
        Text res;
        delete[] res.buf;
        res.len = len + o.len;
        res.buf = new char[res.len + 1];
        std::strcpy(res.buf, buf);
        std::strcat(res.buf, o.buf);
        return res;
    }

    bool operator==(const Text& o) const { return std::strcmp(buf, o.buf) == 0; }
    bool operator!=(const Text& o) const { return !(*this == o); }

    friend std::ostream& operator<<(std::ostream& os, const Text& t) {
        os << t.buf;
        return os;
    }

    size_t length() const { return len; }
};

int main() {
    Text t1("Hello, ");
    Text t2("World!");
    Text t3 = t1 + t2;
    std::cout << "Concatenated Text: " << t3 << "\n";

    // Self-assignment safety check
    t3 = t3;
    std::cout << "After Self-Assignment: " << t3 << "\n";

    // Empty and Long string check
    Text empty;
    std::cout << "Empty text length: " << empty.length() << "\n";

    return 0;
}
