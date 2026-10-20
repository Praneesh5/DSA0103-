/**
 * @file Task11_FinalDesign.cpp
 * @brief Task 11: Recommended Production Architecture for Namma Metro Fare System
 * @course DSA01 - Assignment 1 (CO1: Bloom's Level K5 - Evaluate)
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <array>

// 1. Strongly Typed Scoped Enumerations
enum class Zone : unsigned char {
    Inner  = 0,
    Middle = 1,
    Outer  = 2
};

enum class PassengerCategory : unsigned char {
    Regular,
    Student,
    SeniorCitizen
};

// 2. High-Performance Configuration Engine (Configurable / Extensible)
class FarePolicy {
public:
    static constexpr long long BASE_FARES[]       = {1000LL, 2000LL, 3000LL};
    static constexpr int       PEAK_SURCHARGE_PCT = 20;
    static constexpr long long MIN_BALANCE_PAISE  = 5000LL;

    static constexpr int STUDENT_CONCESSION_PCT = 25; // 25% discount
    static constexpr int SENIOR_CONCESSION_PCT  = 30; // 30% discount

    static bool isPeak(int hour) noexcept {
        return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    }

    static long long computeFare(Zone z, int hour, PassengerCategory cat) noexcept {
        size_t idx = static_cast<size_t>(z);
        if (idx >= 3) return 0LL;
        long long fare = BASE_FARES[idx];

        // Apply peak surcharge if applicable
        if (isPeak(hour)) {
            fare += (fare * PEAK_SURCHARGE_PCT) / 100LL;
        }

        // Apply passenger concession if applicable
        if (cat == PassengerCategory::Student) {
            fare -= (fare * STUDENT_CONCESSION_PCT) / 100LL;
        } else if (cat == PassengerCategory::SeniorCitizen) {
            fare -= (fare * SENIOR_CONCESSION_PCT) / 100LL;
        }

        return fare;
    }
};

// 3. Encapsulated SmartCard Class
class SmartCard {
private:
    unsigned long long cardId_;
    long long balancePaise_;
    PassengerCategory category_;
    unsigned long long journeyCount_;

public:
    SmartCard(unsigned long long id, long long initialPaise, PassengerCategory cat = PassengerCategory::Regular)
        : cardId_(id), balancePaise_(initialPaise), category_(cat), journeyCount_(0) {}

    unsigned long long getCardId() const noexcept { return cardId_; }
    long long getBalancePaise() const noexcept { return balancePaise_; }
    PassengerCategory getCategory() const noexcept { return category_; }
    unsigned long long getJourneyCount() const noexcept { return journeyCount_; }

    bool canTravel(long long farePaise) const noexcept {
        return (balancePaise_ - farePaise) >= FarePolicy::MIN_BALANCE_PAISE;
    }

    bool tapTurnstile(Zone z, int hour, long long& outDeductedFare) noexcept {
        long long fare = FarePolicy::computeFare(z, hour, category_);
        if (canTravel(fare)) {
            balancePaise_ -= fare;
            outDeductedFare = fare;
            ++journeyCount_;
            return true;
        }
        outDeductedFare = fare;
        return false;
    }

    bool recharge(long long topUpPaise) noexcept {
        if (topUpPaise <= 0 || (balancePaise_ > INT64_MAX - topUpPaise)) return false;
        balancePaise_ += topUpPaise;
        return true;
    }
};

int main() {
    std::cout << "=========================================================================\n";
    std::cout << "  TASK 11: RECOMMENDED FINAL SYSTEM ARCHITECTURE                         \n";
    std::cout << "=========================================================================\n\n";

    SmartCard regularCard(101, 10000, PassengerCategory::Regular);
    SmartCard studentCard(102, 10000, PassengerCategory::Student);
    SmartCard seniorCard(103, 10000, PassengerCategory::SeniorCitizen);

    std::cout << "Demonstrating Concession & Peak Processing across Commuter Classes:\n";
    std::cout << "Journey: Outer Zone @ 08:30 AM (Peak Period - Base Rs 30 + 20% = Rs 36.00)\n";
    std::cout << "-------------------------------------------------------------------------\n";

    long long fare = 0;
    regularCard.tapTurnstile(Zone::Outer, 8, fare);
    std::cout << "Regular Card: Deducted Rs " << fare / 100 << '.' << std::setw(2) << std::setfill('0') << fare % 100
              << " | Remaining: Rs " << regularCard.getBalancePaise() / 100 << '.' << regularCard.getBalancePaise() % 100 << '\n';

    studentCard.tapTurnstile(Zone::Outer, 8, fare);
    std::cout << "Student Card: Deducted Rs " << fare / 100 << '.' << std::setw(2) << std::setfill('0') << fare % 100
              << " (25% off) | Remaining: Rs " << studentCard.getBalancePaise() / 100 << '.' << studentCard.getBalancePaise() % 100 << '\n';

    seniorCard.tapTurnstile(Zone::Outer, 8, fare);
    std::cout << "Senior Card:  Deducted Rs " << fare / 100 << '.' << std::setw(2) << std::setfill('0') << fare % 100
              << " (30% off) | Remaining: Rs " << seniorCard.getBalancePaise() / 100 << '.' << seniorCard.getBalancePaise() % 100 << '\n';

    std::cout << "-------------------------------------------------------------------------\n";
    std::cout << "All cards preserved balance integrity >= Rs 50.00.\n\n";

    std::cout << "=========================================================================\n";
    std::cout << "  TASK 11 COMPLETED SUCCESSFULLY                                         \n";
    std::cout << "=========================================================================\n";
    return 0;
}
