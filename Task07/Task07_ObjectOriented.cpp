/**
 * @file Task07_ObjectOriented.cpp
 * @brief Task 7: Object-Oriented Implementation with Strict Encapsulation and Invariant Guarantees
 * @course DSA01 - Assignment 1 (CO1: Bloom's Level K5 - Evaluate)
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

enum class Zone : unsigned char { Inner = 0, Middle = 1, Outer = 2 };

class SmartCard {
private:
    unsigned long long cardId_;
    long long balancePaise_; // Encapsulated: Strictly inaccessible to external tampering!
    unsigned long long journeyCount_;

    // Single source of truth for the minimum balance rule
    static constexpr long long MIN_BALANCE_PAISE = 5000LL; // Rs 50.00

public:
    // Constructor enforcing minimum initial balance invariant
    SmartCard(unsigned long long id, long long initialBalancePaise)
        : cardId_(id), balancePaise_(initialBalancePaise), journeyCount_(0) {
        if (initialBalancePaise < MIN_BALANCE_PAISE) {
            throw std::invalid_argument("Initial card balance cannot be lower than mandatory minimum Rs 50.00 (5000 paise).");
        }
    }

    // Read-only accessors
    unsigned long long getCardId() const noexcept { return cardId_; }
    long long getBalancePaise() const noexcept { return balancePaise_; }
    unsigned long long getJourneyCount() const noexcept { return journeyCount_; }

    // Evaluates whether journey can proceed while keeping balance >= Rs 50
    bool canTravel(long long farePaise) const noexcept {
        return (balancePaise_ - farePaise) >= MIN_BALANCE_PAISE;
    }

    // Transactional mutator: Deducts fare atomically IF AND ONLY IF invariants hold
    bool deduct(long long farePaise) noexcept {
        if (canTravel(farePaise)) {
            balancePaise_ -= farePaise;
            ++journeyCount_;
            return true;
        }
        return false; // Zero mutation on rejection
    }

    // Safe recharge preventing negative recharge or integer overflow
    bool recharge(long long creditPaise) noexcept {
        if (creditPaise <= 0) return false;
        if (balancePaise_ > INT64_MAX - creditPaise) return false; // Prevent overflow
        balancePaise_ += creditPaise;
        return true;
    }

    static constexpr long long getMinBalanceRule() noexcept {
        return MIN_BALANCE_PAISE;
    }
};

class FareEngine {
private:
    static constexpr long long BASE_FARES[] = {1000LL, 2000LL, 3000LL};
    static constexpr int PEAK_SURCHARGE_PCT = 20;

public:
    static long long computeFare(Zone z, bool isPeak) noexcept {
        size_t idx = static_cast<size_t>(z);
        if (idx >= 3) return 0LL;
        long long fare = BASE_FARES[idx];
        if (isPeak) {
            fare += (fare * PEAK_SURCHARGE_PCT) / 100LL;
        }
        return fare;
    }
};

int main() {
    std::cout << "====================================================\n";
    std::cout << "  TASK 7: OBJECT-ORIENTED SMARTCARD DEMONSTRATION   \n";
    std::cout << "====================================================\n\n";

    SmartCard myCard(1001ULL, 10000LL); // Rs 100.00
    std::cout << "Card #" << myCard.getCardId() << " Initial Balance: Rs " 
              << myCard.getBalancePaise() / 100 << ".00\n";
    std::cout << "Mandatory Minimum Solvency Threshold: Rs " 
              << SmartCard::getMinBalanceRule() / 100 << ".00\n\n";

    // Trip 1: Inner zone, off-peak (Rs 10.00)
    long long fare1 = FareEngine::computeFare(Zone::Inner, false);
    if (myCard.deduct(fare1)) {
        std::cout << "Trip 1 [Inner Off-Peak]: Approved. Fare Rs " << fare1 / 100 << ".00. "
                  << "New Balance: Rs " << myCard.getBalancePaise() / 100 << ".00\n";
    }

    // Trip 2: Outer zone, peak (Rs 36.00)
    long long fare2 = FareEngine::computeFare(Zone::Outer, true);
    if (myCard.deduct(fare2)) {
        std::cout << "Trip 2 [Outer Peak]:     Approved. Fare Rs " << fare2 / 100 << ".00. "
                  << "New Balance: Rs " << myCard.getBalancePaise() / 100 << ".00\n";
    }

    // Trip 3: Middle zone, off-peak (Rs 20.00) -> Remaining would be Rs 34.00 (< Rs 50)
    long long fare3 = FareEngine::computeFare(Zone::Middle, false);
    std::cout << "Trip 3 [Middle Off-Peak]: Requesting Fare Rs " << fare3 / 100 << ".00...\n";
    if (myCard.deduct(fare3)) {
        std::cout << "Trip 3 Approved.\n";
    } else {
        std::cout << "[GATE ACCESS DENIED]: Insufficient balance! Retained balance: Rs "
                  << myCard.getBalancePaise() / 100 << ".00 (Never drops below Rs 50.00)\n";
    }

    std::cout << "\n[ENCAPSULATION GUARANTEE]: Direct balance mutation (e.g. card.balancePaise_ = -100)\n";
    std::cout << "                           is strictly rejected by the C++ compiler as a compile-time error.\n";
    std::cout << "                           Negative balance is mathematically IMPOSSIBLE in this design.\n\n";

    std::cout << "====================================================\n";
    std::cout << "  TASK 7 OOP DEMONSTRATION COMPLETE                 \n";
    std::cout << "====================================================\n";
    return 0;
}
