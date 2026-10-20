/**
 * @file Task08_SafeIO.cpp
 * @brief Task 8: Robust Input Stream Processing, Error Recovery, and Neatly Formatted Output
 * @course DSA01 - Assignment 1 (CO1: Bloom's Level K5 - Evaluate)
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <sstream>

enum class Zone : unsigned char { Inner = 0, Middle = 1, Outer = 2 };

const char* zoneToString(Zone z) {
    switch (z) {
        case Zone::Inner:  return "Inner";
        case Zone::Middle: return "Middle";
        case Zone::Outer:  return "Outer";
        default:           return "Unknown";
    }
}

long long calculateFare(Zone z, bool isPeak) {
    constexpr long long BASE[] = {1000LL, 2000LL, 3000LL};
    size_t idx = static_cast<size_t>(z);
    if (idx >= 3) return 0LL;
    long long f = BASE[idx];
    if (isPeak) {
        f += (f * 20LL) / 100LL;
    }
    return f;
}

// Method 1: Robust cin extraction with failbit check and buffer flushing
long long readLongLongSafe(const std::string& prompt) {
    long long value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            // Check for trailing trash on same line
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        } else {
            if (std::cin.eof()) {
                std::cout << "\n[EOF reached]\n";
                return 0;
            }
            std::cout << "  [ERROR]: Non-numeric input detected! Resetting stream...\n";
            std::cin.clear(); // Clear failbit
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard bad line
        }
    }
}

// Method 2: Alternative Concept - std::getline + std::stoll inside try/catch
bool readLongLongViaGetline(std::istream& is, long long& outValue) {
    std::string line;
    if (!std::getline(is, line)) return false;
    try {
        size_t pos = 0;
        outValue = std::stoll(line, &pos);
        // Verify entire line was consumed
        while (pos < line.size() && isspace(static_cast<unsigned char>(line[pos]))) ++pos;
        return pos == line.size();
    } catch (...) {
        return false;
    }
}

void printFormattedReportRow(int tripId, Zone z, int hour, bool isPeak, long long fare, long long balance, bool approved) {
    std::cout << "| " << std::left  << std::setw(6)  << tripId
              << "| " << std::left  << std::setw(8)  << zoneToString(z)
              << "| " << std::right << std::setw(4)  << hour << ":00"
              << "| " << std::left  << std::setw(9)  << (isPeak ? "PEAK" : "OFF-PEAK")
              << "| Rs " << std::right << std::setw(4) << fare / 100 << '.' 
              << std::setw(2) << std::setfill('0') << fare % 100 << std::setfill(' ')
              << "| Rs " << std::right << std::setw(5) << balance / 100 << '.' 
              << std::setw(2) << std::setfill('0') << balance % 100 << std::setfill(' ')
              << "| " << std::left << std::setw(10) << (approved ? "APPROVED" : "DENIED")
              << " |\n";
}

int main() {
    std::cout << "=========================================================================\n";
    std::cout << "  TASK 8: SAFE STREAM I/O & FORMATTED AUDIT REPORT                       \n";
    std::cout << "=========================================================================\n\n";

    long long balance = 10000LL; // Rs 100.00
    constexpr long long MIN_BALANCE = 5000LL; // Rs 50.00

    std::cout << "+-------+---------+-------+----------+----------+-----------+------------+\n";
    std::cout << "| Trip# | Zone    | Time  | Window   | Fare     | Balance   | Status     |\n";
    std::cout << "+-------+---------+-------+----------+----------+-----------+------------+\n";

    struct SampleTrip { Zone z; int hour; };
    SampleTrip batch[] = {
        {Zone::Inner,  9},  // Peak 9 AM: Rs 12.00
        {Zone::Outer,  14}, // Off-peak 2 PM: Rs 30.00
        {Zone::Middle, 18}, // Peak 6 PM: Rs 24.00 (Leaves Rs 34 -> DENIED)
        {Zone::Inner,  10}  // Off-peak 10 AM (leaves Rs 24 -> DENIED)
    };

    int tripIdx = 1;
    for (const auto& t : batch) {
        bool peak = (t.hour >= 8 && t.hour < 10) || (t.hour >= 17 && t.hour < 20);
        long long f = calculateFare(t.z, peak);
        bool approved = false;
        if ((balance - f) >= MIN_BALANCE) {
            balance -= f;
            approved = true;
        }
        printFormattedReportRow(tripIdx++, t.z, t.hour, peak, f, balance, approved);
    }
    std::cout << "+-------+---------+-------+----------+----------+-----------+------------+\n\n";

    // Demonstration of Safe Recovery using simulated invalid stream
    std::cout << "Demonstrating Stream Error Recovery on Malformed Input Stream:\n";
    std::istringstream badStream("abc\n-25\n500\n");
    std::string line;
    long long val = 0;
    while (std::getline(badStream, line)) {
        try {
            val = std::stoll(line);
            std::cout << "  -> Parsed valid numeric: " << val << '\n';
        } catch (const std::exception& e) {
            std::cout << "  -> Successfully caught invalid token: \"" << line << "\" (" << e.what() << ") - Safely skipped.\n";
        }
    }

    std::cout << "\n=========================================================================\n";
    std::cout << "  TASK 8 COMPLETED SUCCESSFULLY                                          \n";
    std::cout << "=========================================================================\n";
    return 0;
}
