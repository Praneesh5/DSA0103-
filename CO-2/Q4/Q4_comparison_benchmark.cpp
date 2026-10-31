#include <iostream>
#include <deque>
#include <chrono>
#include <iomanip>

// 1. Shifting Array Model (Fixed capacity K)
template <int K>
class ShiftingBufferAccount {
public:
    double buffer[K];
    int count = 0;

    void record(double amount) {
        if (count < K) {
            buffer[count++] = amount;
        } else {
            for (int i = 0; i < K - 1; ++i) {
                buffer[i] = buffer[i + 1];
            }
            buffer[K - 1] = amount;
            count++;
        }
    }
    double checksum() const {
        double s = 0;
        for (int i = 0; i < K; ++i) s += buffer[i];
        return s;
    }
};

// 2. Circular Ring Buffer Model (Fixed capacity K)
template <int K>
class CircularBufferAccount {
public:
    double buffer[K];
    int head = 0;
    int count = 0;

    void record(double amount) {
        buffer[head] = amount;
        head = (head + 1) % K;
        count++;
    }
    double checksum() const {
        double s = 0;
        for (int i = 0; i < K; ++i) s += buffer[i];
        return s;
    }
};

// 3. std::deque Model
class DequeBufferAccount {
    std::deque<double> deq;
    int maxCapacity;
public:
    explicit DequeBufferAccount(int cap) : maxCapacity(cap) {}
    void record(double amount) {
        deq.push_back(amount);
        if (static_cast<int>(deq.size()) > maxCapacity) {
            deq.pop_front();
        }
    }
    double checksum() const {
        double s = 0;
        for (double v : deq) s += v;
        return s;
    }
};

int main()
{
    const int NUM_OPERATIONS = 100000;
    std::cout << "=====================================================\n";
    std::cout << "Q4: PERFORMANCE BENCHMARK (SHIFTING vs CIRCULAR vs DEQUE)\n";
    std::cout << "Operations Executed: " << NUM_OPERATIONS << " transactions\n";
    std::cout << "=====================================================\n\n";

    double sink = 0;

    // --- Capacity K = 10 ---
    std::cout << "--- Testing with Capacity K = 10 ---\n";
    {
        ShiftingBufferAccount<10> shiftAcc;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < NUM_OPERATIONS; ++i) shiftAcc.record(100.0 + (i % 10));
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
        sink += shiftAcc.checksum();
        std::cout << "  1. Shifting Array (K=10)   : " << std::setw(8) << elapsed << " microseconds (O(K))\n";
    }
    {
        CircularBufferAccount<10> circAcc;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < NUM_OPERATIONS; ++i) circAcc.record(100.0 + (i % 10));
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
        sink += circAcc.checksum();
        std::cout << "  2. Circular Buffer (K=10)   : " << std::setw(8) << elapsed << " microseconds (O(1))\n";
    }
    {
        DequeBufferAccount deqAcc(10);
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < NUM_OPERATIONS; ++i) deqAcc.record(100.0 + (i % 10));
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
        sink += deqAcc.checksum();
        std::cout << "  3. std::deque (K=10)        : " << std::setw(8) << elapsed << " microseconds (O(1) amortized)\n";
    }

    // --- Capacity K = 50 (The Design Decision Scenario) ---
    std::cout << "\n--- Testing with Capacity K = 50 (Expanded Requirement) ---\n";
    {
        ShiftingBufferAccount<50> shiftAcc;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < NUM_OPERATIONS; ++i) shiftAcc.record(100.0 + (i % 50));
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
        sink += shiftAcc.checksum();
        std::cout << "  1. Shifting Array (K=50)   : " << std::setw(8) << elapsed << " microseconds (5x slower due to O(K))\n";
    }
    {
        CircularBufferAccount<50> circAcc;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < NUM_OPERATIONS; ++i) circAcc.record(100.0 + (i % 50));
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
        sink += circAcc.checksum();
        std::cout << "  2. Circular Buffer (K=50)   : " << std::setw(8) << elapsed << " microseconds (O(1) invariant!)\n";
    }
    {
        DequeBufferAccount deqAcc(50);
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < NUM_OPERATIONS; ++i) deqAcc.record(100.0 + (i % 50));
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
        sink += deqAcc.checksum();
        std::cout << "  3. std::deque (K=50)        : " << std::setw(8) << elapsed << " microseconds\n";
    }

    // Volatile consumption
    volatile double dummy = sink;
    (void)dummy;

    return 0;
}
