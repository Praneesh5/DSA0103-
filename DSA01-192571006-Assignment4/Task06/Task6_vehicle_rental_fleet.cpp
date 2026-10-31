#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

// Member Class (Composition)
class ServiceRecord {
    std::string lastService;
    std::string nextDue;

public:
    ServiceRecord(const std::string& last = "None", const std::string& next = "2026-12-31")
        : lastService(last), nextDue(next) {}

    std::string getDue() const { return nextDue; }
};

// Abstract Base Class
class Vehicle {
protected:
    std::string regNo;
    double dailyRate;
    bool isRented;

public:
    Vehicle(const std::string& reg, double rate)
        : regNo(reg), dailyRate(rate), isRented(false) {}
    virtual ~Vehicle() = default;

    virtual double computeCharge(int days, int lateDays) const = 0; // Pure Virtual Function
    virtual std::string getCategory() const = 0;

    bool rent() {
        if (!isRented) { isRented = true; return true; }
        return false;
    }

    bool returnVehicle() {
        if (isRented) { isRented = false; return true; }
        return false;
    }

    void display(int days, int lateDays) const {
        std::cout << std::left << std::setw(15) << getCategory()
                  << " | Reg: " << std::setw(12) << regNo
                  << " | Total Fee (" << days << "d + " << lateDays << "d late): Rs "
                  << std::fixed << std::setprecision(2) << computeCharge(days, lateDays) << "\n";
    }
};

// Derived 1: Bicycle (Hierarchical)
class Bicycle : public Vehicle {
public:
    Bicycle(const std::string& reg, double rate = 150.0)
        : Vehicle(reg, rate) {}

    std::string getCategory() const override { return "Bicycle"; }

    double computeCharge(int days, int lateDays) const override {
        double lateFee = lateDays * (dailyRate * 1.25);
        return (days * dailyRate) + lateFee;
    }
};

// Intermediate Base: MotorVehicle (Multilevel)
class MotorVehicle : public Vehicle {
protected:
    double fuelOrChargeLevel;
    ServiceRecord service;

public:
    MotorVehicle(const std::string& reg, double rate, double fuel, const ServiceRecord& s)
        : Vehicle(reg, rate), fuelOrChargeLevel(fuel), service(s) {}
};

// Derived 2: TwoWheeler (Multilevel Specialization)
class TwoWheeler : public MotorVehicle {
public:
    TwoWheeler(const std::string& reg, double rate, double fuel, const ServiceRecord& s)
        : MotorVehicle(reg, rate, fuel, s) {}

    std::string getCategory() const override { return "Two-Wheeler"; }

    double computeCharge(int days, int lateDays) const override {
        double lateFee = lateDays * (dailyRate * 1.50);
        return (days * dailyRate) + lateFee;
    }
};

// Derived 3: Car (Multilevel Specialization with Seating Capacity)
class Car : public MotorVehicle {
protected:
    int seats;

public:
    Car(const std::string& reg, double rate, double fuel, const ServiceRecord& s, int seatCount)
        : MotorVehicle(reg, rate, fuel, s), seats(seatCount) {}

    std::string getCategory() const override { return "Sedan Car"; }

    double computeCharge(int days, int lateDays) const override {
        double lateFee = lateDays * (dailyRate * 1.75);
        return (days * dailyRate) + lateFee;
    }
};

// Derived 4: ElectricCar (Specialized Charging Rate)
class ElectricCar : public Car {
    double batteryKWh;
    double chargePerKWh;

public:
    ElectricCar(const std::string& reg, double rate, double charge, const ServiceRecord& s, int seatCount, double kwh, double costPerKWh)
        : Car(reg, rate, charge, s, seatCount), batteryKWh(kwh), chargePerKWh(costPerKWh) {}

    std::string getCategory() const override { return "Electric Car"; }

    double computeCharge(int days, int lateDays) const override {
        double rechargeFee = (100.0 - fuelOrChargeLevel) / 100.0 * batteryKWh * chargePerKWh;
        double lateFee = lateDays * (dailyRate * 2.0);
        return (days * dailyRate) + rechargeFee + lateFee;
    }
};

// Extension Class: ElectricBus (Demonstrates Open-Closed Principle)
class ElectricBus : public Vehicle {
    int passengerCapacity;
    double batteryKWh;

public:
    ElectricBus(const std::string& reg, double rate, int passCap, double kwh)
        : Vehicle(reg, rate), passengerCapacity(passCap), batteryKWh(kwh) {}

    std::string getCategory() const override { return "Electric Bus"; }

    double computeCharge(int days, int lateDays) const override {
        double lateFee = lateDays * (dailyRate * 2.5);
        return (days * dailyRate) + lateFee;
    }
};

int main() {
    std::cout << "=====================================================
";
    std::cout << "TASK 6: VEHICLE RENTAL FLEET MANAGEMENT SYSTEM
";
    std::cout << "=====================================================
";

    ServiceRecord s1("2026-09-01", "2026-12-01");
    std::vector<Vehicle*> fleet = {
        new Bicycle("BIKE-001", 200.0),
        new TwoWheeler("TN-02-TW-101", 600.0, 80.0, s1),
        new Car("TN-09-CR-555", 2500.0, 60.0, s1, 5),
        new ElectricCar("TN-07-EV-999", 3500.0, 40.0, s1, 5, 60.0, 15.0),
        new ElectricBus("TN-01-EB-777", 8000.0, 45, 250.0) // Seamless Extension
    };

    for (auto* v : fleet) {
        v->rent();
        v->display(3, 1); // 3 rental days + 1 day late return
    }

    for (auto* v : fleet) delete v;

    return 0;
}
