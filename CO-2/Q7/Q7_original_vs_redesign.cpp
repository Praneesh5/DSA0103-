#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>

// =========================================================================
// ORIGINAL DESIGN: Fixed-Array Shifting Buffer (O(K) per insertion)
// =========================================================================
template <size_t CAPACITY>
class ShiftingSavingsAccount {
private:
    long long accountNumber;
    double balance;
    double transactions[CAPACITY];
    size_t count;

public:
    ShiftingSavingsAccount(long long accNo, double initBal)
        : accountNumber(accNo), balance(initBal), count(0) {}

    void deposit(double amount) {
        balance += amount;
        if (count < CAPACITY) {
            transactions[count++] = amount;
        } else {
            // Bottleneck: O(K) memory shift
            for (size_t i = 0; i < CAPACITY - 1; ++i) {
                transactions[i] = transactions[i + 1];
            }
            transactions[CAPACITY - 1] = amount;
            count++;
        }
    }

    double getBalance() const { return balance; }
    size_t getCount() const { return count; }
    double getLatest() const { 
        return (count < CAPACITY) ? transactions[count - 1] : transactions[CAPACITY - 1]; 
    }
};

// =========================================================================
// REDESIGNED SOLUTION: Zero-Shift Circular Ring Buffer (O(1) per insertion)
// =========================================================================
template <size_t CAPACITY>
class CircularSavingsAccount {
private:
    long long accountNumber;
    double balance;
    double transactions[CAPACITY];
    size_t head;        // Next write pointer
    size_t currentSize; // Elements currently stored in buffer (up to CAPACITY)
    size_t totalCount;  // Lifetime transactions executed

public:
    CircularSavingsAccount(long long accNo, double initBal)
        : accountNumber(accNo), balance(initBal), head(0), currentSize(0), totalCount(0) {}

    // O(1) constant-time record
    void deposit(double amount) {
        balance += amount;
        transactions[head] = amount;
        head = (head + 1) % CAPACITY;
        if (currentSize < CAPACITY) {
            currentSize++;
        }
        totalCount++;
    }

    double getBalance() const { return balance; }
    size_t getCount() const { return totalCount; }
    
    // Index translation for mini-statement retrieval
    double getTransaction(size_t logicalIndex) const {
        if (logicalIndex >= currentSize) return 0.0;
        size_t physicalIndex;
        if (currentSize < CAPACITY) {
            physicalIndex = logicalIndex;
        } else {
            physicalIndex = (head + logicalIndex) % CAPACITY;
        }
        return transactions[physicalIndex];
    }

    void printMiniStatement(size_t limit = 10) const {
        std::cout << "Mini-Statement (Account " << accountNumber << ", Balance: $" 
                  << std::fixed << std::setprecision(2) << balance << "):\n";
        size_t n = (currentSize < limit) ? currentSize : limit;
        for (size_t i = 0; i < n; ++i) {
            std::cout << "  [" << (i + 1) << "] $" << getTransaction(i) << '\n';
        }
    }
};

int main()
{
    std::cout << "=========================================================================\n";
    std::cout << "DAY-4 DESIGN CHALLENGE (Q7): DIAGNOSE, REDESIGN AND VALIDATE\n";
    std::cout << "Target: Savings Account Transaction History (Shifting Array vs Ring Buffer)\n";
    std::cout << "=========================================================================\n\n";

    const size_t NUM_OPERATIONS = 100000;
    std::cout << "Workload: " << NUM_OPERATIONS << " continuous transactions per benchmark\n\n";

    // Matrix of capacities to test
    std::cout << std::left << std::setw(15) << "Capacity (K)" 
              << std::setw(25) << "Original Shifting (us)" 
              << std::setw(25) << "Redesigned Ring (us)" 
              << std::setw(15) << "Speedup Factor\n";
    std::cout << "-------------------------------------------------------------------------\n";

    // Test K = 10
    {
        ShiftingSavingsAccount<10> orig(1001, 1000.0);
        auto t0 = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < NUM_OPERATIONS; ++i) orig.deposit(10.0 + (i % 5));
        auto origTime = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - t0).count();

        CircularSavingsAccount<10> red(1001, 1000.0);
        auto t1 = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < NUM_OPERATIONS; ++i) red.deposit(10.0 + (i % 5));
        auto redTime = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - t1).count();

        double speedup = (redTime > 0) ? static_cast<double>(origTime) / redTime : 1.0;
        std::cout << std::left << std::setw(15) << "K = 10" 
                  << std::setw(25) << origTime 
                  << std::setw(25) << redTime 
                  << std::fixed << std::setprecision(2) << speedup << "x\n";
    }

    // Test K = 50
    {
        ShiftingSavingsAccount<50> orig(1002, 1000.0);
        auto t0 = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < NUM_OPERATIONS; ++i) orig.deposit(10.0 + (i % 5));
        auto origTime = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - t0).count();

        CircularSavingsAccount<50> red(1002, 1000.0);
        auto t1 = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < NUM_OPERATIONS; ++i) red.deposit(10.0 + (i % 5));
        auto redTime = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - t1).count();

        double speedup = (redTime > 0) ? static_cast<double>(origTime) / redTime : 1.0;
        std::cout << std::left << std::setw(15) << "K = 50" 
                  << std::setw(25) << origTime 
                  << std::setw(25) << redTime 
                  << std::fixed << std::setprecision(2) << speedup << "x\n";
    }

    // Test K = 200
    {
        ShiftingSavingsAccount<200> orig(1003, 1000.0);
        auto t0 = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < NUM_OPERATIONS; ++i) orig.deposit(10.0 + (i % 5));
        auto origTime = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - t0).count();

        CircularSavingsAccount<200> red(1003, 1000.0);
        auto t1 = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < NUM_OPERATIONS; ++i) red.deposit(10.0 + (i % 5));
        auto redTime = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - t1).count();

        double speedup = (redTime > 0) ? static_cast<double>(origTime) / redTime : 1.0;
        std::cout << std::left << std::setw(15) << "K = 200" 
                  << std::setw(25) << origTime 
                  << std::setw(25) << redTime 
                  << std::fixed << std::setprecision(2) << speedup << "x\n";
    }

    // Test K = 500
    {
        ShiftingSavingsAccount<500> orig(1004, 1000.0);
        auto t0 = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < NUM_OPERATIONS; ++i) orig.deposit(10.0 + (i % 5));
        auto origTime = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - t0).count();

        CircularSavingsAccount<500> red(1004, 1000.0);
        auto t1 = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < NUM_OPERATIONS; ++i) red.deposit(10.0 + (i % 5));
        auto redTime = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - t1).count();

        double speedup = (redTime > 0) ? static_cast<double>(origTime) / redTime : 1.0;
        std::cout << std::left << std::setw(15) << "K = 500" 
                  << std::setw(25) << origTime 
                  << std::setw(25) << redTime 
                  << std::fixed << std::setprecision(2) << speedup << "x\n";
    }

    std::cout << "\n--- Correctness Validation of Mini-Statement Retrieval ---\n";
    CircularSavingsAccount<5> testAcc(192571006, 2500.0);
    testAcc.deposit(100.0);
    testAcc.deposit(200.0);
    testAcc.deposit(300.0);
    testAcc.deposit(400.0);
    testAcc.deposit(500.0);
    testAcc.deposit(600.0); // Overwrites 100.0
    testAcc.deposit(700.0); // Overwrites 200.0
    testAcc.printMiniStatement();

    return 0;
}
