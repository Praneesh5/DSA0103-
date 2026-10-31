#include <iostream>
#include <string>
#include <stdexcept>
#include <iomanip>

class Time {
    int total_seconds;

public:
    Time(int h = 0, int m = 0, int s = 0) {
        total_seconds = h * 3600 + m * 60 + s;
    }

    // Prefix increment (++t): mutates in-place, returns reference (zero copy)
    Time& operator++() {
        ++total_seconds;
        return *this;
    }

    // Postfix increment (t++): dummy int distinguishes syntax, returns old copy
    Time operator++(int) {
        Time old = *this;
        ++total_seconds;
        return old;
    }

    bool operator==(const Time& o) const { return total_seconds == o.total_seconds; }
    bool operator<(const Time& o) const { return total_seconds < o.total_seconds; }

    void print() const {
        int h = total_seconds / 3600;
        int m = (total_seconds % 3600) / 60;
        int s = total_seconds % 60;
        std::cout << std::setfill('0') << std::setw(2) << h << ":"
                  << std::setfill('0') << std::setw(2) << m << ":"
                  << std::setfill('0') << std::setw(2) << s;
    }
};

class Playlist {
    std::string tracks[5];
    int count = 0;

public:
    void add(const std::string& t) {
        if (count < 5) tracks[count++] = t;
    }

    // Non-const operator[]: allows modification
    std::string& operator[](int idx) {
        if (idx < 0 || idx >= count) {
            throw std::out_of_range("Playlist index out of bounds: " + std::to_string(idx));
        }
        return tracks[idx];
    }

    // Const operator[]: read-only access for const Playlist
    const std::string& operator[](int idx) const {
        if (idx < 0 || idx >= count) {
            throw std::out_of_range("Playlist index out of bounds: " + std::to_string(idx));
        }
        return tracks[idx];
    }

    int size() const { return count; }
};

int main() {
    Playlist pl;
    pl.add("Track 01 - Cyberpunk Symphony");
    pl.add("Track 02 - Classical Overture");
    pl.add("Track 03 - Ambient Synthwave");

    Time timer(0, 59, 58);
    std::cout << "Initial Timer: "; timer.print(); std::cout << "\n";

    ++timer; // 00:59:59
    std::cout << "After prefix ++: "; timer.print(); std::cout << "\n";

    Time snap = timer++; // Postfix: snap is 00:59:59, timer becomes 01:00:00
    std::cout << "Snapshot from postfix: "; snap.print(); std::cout << "\n";
    std::cout << "Timer after roll-over: "; timer.print(); std::cout << "\n";

    std::cout << "\nNow Playing: " << pl[0] << "\n";

    try {
        std::cout << pl[5] << "\n"; // Out of range test
    } catch (const std::out_of_range& e) {
        std::cout << "[EXCEPTION CAUGHT]: " << e.what() << "\n";
    }

    return 0;
}
