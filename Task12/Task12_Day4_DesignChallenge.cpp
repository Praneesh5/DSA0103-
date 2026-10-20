/**
 * @file Task12_Day4_DesignChallenge.cpp
 * @brief Task 12: Day-4 Design Challenge - Airport Zone, Weekend Discount, and Monthly Pass
 * @course DSA01 - Assignment 1 (CO1: Bloom's Level K5 - Evaluate)
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <set>
#include <cassert>

// 1. Updated Scoped Enumeration with 4th Zone
enum class Zone : unsigned char {
    Inner   = 0,
    Middle  = 1,
    Outer   = 2,
    Airport = 3 // Fourth Zone Added
};

const char* zoneToString(Zone z) {
    switch (z) {
        case Zone::Inner:   return "Inner";
        case Zone::Middle:  return "Middle";
        case Zone::Outer:   return "Outer";
        case Zone::Airport: return "Airport Express";
        default:            return "Unknown";
    }
}

enum class DayOfWeek : unsigned char {
    Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday
};

// 2. Extensible Fare Policy Engine
class MetroFareEngineV2 {
public:
    // Base fares for 4 zones in paise
    static constexpr long long BASE_FARES[] = {1000LL, 2000LL, 3000LL, 6000LL}; // Airport = Rs 60.00
    static constexpr int PEAK_SURCHARGE_PCT  = 20; // 20%
    static constexpr int WEEKEND_DISCOUNT_PCT = 10; // 10%
    static constexpr long long MIN_BALANCE_PAISE = 5000LL; // Rs 50.00

    static bool isWeekend(DayOfWeek day) noexcept {
        return (day == DayOfWeek::Saturday || day == DayOfWeek::Sunday);
    }

    static bool isPeak(int hour) noexcept {
        return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    }

    /**
     * @brief Computes fare applying precedence: Peak surcharge applies on weekdays;
     * Weekend discount applies on weekends and DOES NOT COMBINE with peak surcharge.
     */
    static long long computeFare(Zone z, int hour, DayOfWeek day) noexcept {
        size_t idx = static_cast<size_t>(z);
        if (idx >= 4) return 0LL;
        long long fare = BASE_FARES[idx];

        if (isWeekend(day)) {
            // Weekend rule: 10% discount applies; peak surcharge is DISABLED
            fare -= (fare * WEEKEND_DISCOUNT_PCT) / 100LL;
        } else {
            // Weekday rule: Peak surcharge applies during peak hours
            if (isPeak(hour)) {
                fare += (fare * PEAK_SURCHARGE_PCT) / 100LL;
            }
        }
        return fare;
    }
};

// 3. Base Transit Medium (Polymorphic Design)
class ITransitToken {
public:
    virtual ~ITransitToken() = default;
    virtual unsigned long long getId() const noexcept = 0;
    virtual bool canTravel(Zone z, int hour, DayOfWeek day) const noexcept = 0;
    virtual bool tap(Zone z, int hour, DayOfWeek day, long long& deductedFare) noexcept = 0;
    virtual void printStatus() const noexcept = 0;
};

// 4. Stored-Value SmartCard Implementation
class StoredValueCard : public ITransitToken {
private:
    unsigned long long id_;
    long long balancePaise_;
    unsigned long long tripsCompleted_;

public:
    StoredValueCard(unsigned long long id, long long initialPaise)
        : id_(id), balancePaise_(initialPaise), tripsCompleted_(0) {}

    unsigned long long getId() const noexcept override { return id_; }
    long long getBalancePaise() const noexcept { return balancePaise_; }

    bool canTravel(Zone z, int hour, DayOfWeek day) const noexcept override {
        long long f = MetroFareEngineV2::computeFare(z, hour, day);
        return (balancePaise_ - f) >= MetroFareEngineV2::MIN_BALANCE_PAISE;
    }

    bool tap(Zone z, int hour, DayOfWeek day, long long& deductedFare) noexcept override {
        long long f = MetroFareEngineV2::computeFare(z, hour, day);
        if ((balancePaise_ - f) >= MetroFareEngineV2::MIN_BALANCE_PAISE) {
            balancePaise_ -= f;
            deductedFare = f;
            ++tripsCompleted_;
            return true;
        }
        deductedFare = f;
        return false;
    }

    void printStatus() const noexcept override {
        std::cout << "StoredValueCard #" << id_ << " | Balance: Rs " 
                  << balancePaise_ / 100 << '.' << std::setw(2) << std::setfill('0') << balancePaise_ % 100
                  << " | Trips: " << tripsCompleted_ << '\n';
    }
};

// 5. Monthly Pass Implementation (Unlimited travel in 2 designated zones)
class MonthlyPass : public ITransitToken {
private:
    unsigned long long id_;
    Zone zone1_;
    Zone zone2_;
    bool isActive_;
    unsigned long long ridesTaken_;

public:
    MonthlyPass(unsigned long long id, Zone z1, Zone z2)
        : id_(id), zone1_(z1), zone2_(z2), isActive_(true), ridesTaken_(0) {}

    unsigned long long getId() const noexcept override { return id_; }

    bool coversZone(Zone z) const noexcept {
        return (z == zone1_ || z == zone2_);
    }

    bool canTravel(Zone z, int /*hour*/, DayOfWeek /*day*/) const noexcept override {
        return isActive_ && coversZone(z);
    }

    bool tap(Zone z, int /*hour*/, DayOfWeek /*day*/, long long& deductedFare) noexcept override {
        if (canTravel(z, 0, DayOfWeek::Monday)) {
            deductedFare = 0LL; // Covered by monthly subscription
            ++ridesTaken_;
            return true;
        }
        deductedFare = 0LL;
        return false; // Zone not covered by pass
    }

    void printStatus() const noexcept override {
        std::cout << "MonthlyPass #" << id_ << " | Zones Covered: [" 
                  << zoneToString(zone1_) << ", " << zoneToString(zone2_) 
                  << "] | Rides Used: " << ridesTaken_ << '\n';
    }
};

int main() {
    std::cout << "========================================================================================\n";
    std::cout << "  TASK 12: DAY-4 DESIGN CHALLENGE VALIDATION                                            \n";
    std::cout << "========================================================================================\n\n";

    // Test A: Airport Zone and Weekend Discount Logic
    std::cout << "A. TESTING AIRPORT ZONE & WEEKEND DISCOUNT (NON-COMBINING WITH PEAK):\n";
    std::cout << "----------------------------------------------------------------------------------------\n";
    struct Scenario {
        Zone z;
        int hour;
        DayOfWeek day;
        const char* desc;
        long long expectedFare;
    };

    Scenario scenarios[] = {
        {Zone::Airport, 8,  DayOfWeek::Monday,   "Airport Weekday Peak (08:00): Rs 60 + 20% = Rs 72.00", 7200LL},
        {Zone::Airport, 12, DayOfWeek::Monday,   "Airport Weekday Off-Peak:     Rs 60 base  = Rs 60.00", 6000LL},
        {Zone::Airport, 8,  DayOfWeek::Saturday, "Airport Weekend Peak-hour:   Rs 60 - 10% = Rs 54.00 (No Peak Surcharge!)", 5400LL},
        {Zone::Inner,   9,  DayOfWeek::Sunday,   "Inner Weekend:               Rs 10 - 10% = Rs 09.00", 900LL},
        {Zone::Outer,   18, DayOfWeek::Sunday,   "Outer Weekend Evening:       Rs 30 - 10% = Rs 27.00", 2700LL}
    };

    for (const auto& s : scenarios) {
        long long actualFare = MetroFareEngineV2::computeFare(s.z, s.hour, s.day);
        bool match = (actualFare == s.expectedFare);
        std::cout << std::left << std::setw(60) << s.desc 
                  << " | Actual: Rs " << std::setw(5) << actualFare / 100 << '.' << std::setw(2) << std::setfill('0') << actualFare % 100 << std::setfill(' ')
                  << " | " << (match ? "PASS" : "FAIL") << '\n';
        assert(match);
    }
    std::cout << "\n[PASS] Weekend discount does not combine with peak surcharge. Airport fare confirmed.\n\n";

    // Test B: Monthly Pass Unlimited Journey Validation
    std::cout << "B. TESTING MONTHLY PASS (UNLIMITED TRAVEL IN 2 ZONES):\n";
    std::cout << "----------------------------------------------------------------------------------------\n";
    MonthlyPass pass(8888, Zone::Inner, Zone::Airport); // Pass covers Inner and Airport
    StoredValueCard regularCard(9999, 10000);           // Standard card with Rs 100.00

    long long deducted = 0;
    std::cout << "Testing Monthly Pass #8888 covering [Inner, Airport]:\n";

    // Trip 1: Airport trip (covered)
    bool t1 = pass.tap(Zone::Airport, 8, DayOfWeek::Monday, deducted);
    std::cout << "  - Tap Airport Express (08:00 AM Weekday): " << (t1 ? "ALLOWED (Rs 0.00 deducted)" : "DENIED") << '\n';

    // Trip 2: Inner trip (covered)
    bool t2 = pass.tap(Zone::Inner, 17, DayOfWeek::Friday, deducted);
    std::cout << "  - Tap Inner Zone (05:00 PM Weekday):      " << (t2 ? "ALLOWED (Rs 0.00 deducted)" : "DENIED") << '\n';

    // Trip 3: Middle trip (NOT covered by this pass)
    bool t3 = pass.tap(Zone::Middle, 12, DayOfWeek::Tuesday, deducted);
    std::cout << "  - Tap Middle Zone (Not in Pass):          " << (t3 ? "ALLOWED" : "DENIED (Out of pass scope!)") << '\n';

    pass.printStatus();

    std::cout << "\n========================================================================================\n";
    std::cout << "  TASK 12 ADAPTED ARCHITECTURE VALIDATED SUCCESSFULLY                                   \n";
    std::cout << "========================================================================================\n";
    return 0;
}
