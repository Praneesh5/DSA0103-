#include <iostream>
#include <string>
#include <iomanip>

class Auditorium {
    std::string eventName;
    int rows;
    int cols;
    char** seatMap; // 2D dynamically allocated array

public:
    // Dynamic Constructor
    Auditorium(const std::string& event, int r, int c)
        : eventName(event), rows(r), cols(c), seatMap(nullptr) {
        std::cout << "[CONSTRUCTOR] Allocating Auditorium for: " << eventName 
                  << " (" << rows << "x" << cols << ")
";
        if (rows > 0 && cols > 0) {
            seatMap = new char*[rows];
            for (int i = 0; i < rows; ++i) {
                seatMap[i] = new char[cols];
                for (int j = 0; j < cols; ++j) {
                    seatMap[i][j] = '.'; // Empty seat
                }
            }
        }
    }

    // Destructor (RAII)
    ~Auditorium() {
        std::cout << "[DESTRUCTOR] Closing Event: " << eventName << ". Summary: ";
        int booked = 0;
        if (seatMap) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if (seatMap[i][j] == 'X') booked++;
                }
                delete[] seatMap[i]; // Free row buffer
            }
            delete[] seatMap; // Free row pointer array
            seatMap = nullptr;
        }
        std::cout << booked << " seats booked out of " << (rows * cols) << ". Memory freed safely.
";
    }

    bool bookSeat(int r, int c) {
        if (r >= 0 && r < rows && c >= 0 && c < cols) {
            if (seatMap[r][c] == '.') {
                seatMap[r][c] = 'X';
                return true;
            }
        }
        return false;
    }

    bool cancelSeat(int r, int c) {
        if (r >= 0 && r < rows && c >= 0 && c < cols) {
            if (seatMap[r][c] == 'X') {
                seatMap[r][c] = '.';
                return true;
            }
        }
        return false;
    }
};

int main() {
    std::cout << "--- 1. Stack Allocation ---
";
    {
        Auditorium stackEvent("Tech Symposium", 3, 5);
        stackEvent.bookSeat(0, 0);
        stackEvent.bookSeat(1, 2);
    } // Auto-destroyed here

    std::cout << "
--- 2. Block Scope Allocation ---
";
    {
        Auditorium blockEvent("Annual Concert", 2, 4);
        blockEvent.bookSeat(0, 1);
        blockEvent.cancelSeat(0, 1);
    } // Auto-destroyed here

    std::cout << "
--- 3. Heap Allocation (new/delete) ---
";
    Auditorium* heapEvent = new Auditorium("Convocation Ceremony", 4, 6);
    heapEvent->bookSeat(0, 0);
    heapEvent->bookSeat(3, 5);
    delete heapEvent; // Explicit destruction

    return 0;
}
