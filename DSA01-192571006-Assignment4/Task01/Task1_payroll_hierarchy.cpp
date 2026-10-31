#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

// Base Class: Common Abstraction
class Employee {
protected:
    std::string name;
    int id;
    static constexpr double DA_RATE = 0.42; // Constant replaces magic number

public:
    Employee(const std::string& n, int i) : name(n), id(i) {}
    virtual ~Employee() = default;

    virtual double pay() const = 0; // Pure virtual function

    virtual void printPaySlip() const {
        std::cout << "ID: " << std::setw(4) << id 
                  << " | Name: " << std::setw(15) << std::left << name 
                  << " | Net Pay: Rs " << std::fixed << std::setprecision(2) << pay() << "
";
    }
};

class Professor : public Employee {
    double basic;
    double research;

public:
    Professor(const std::string& n, int i, double b, double r)
        : Employee(n, i), basic(b), research(r) {}

    double pay() const override {
        return basic + (basic * DA_RATE) + research;
    }
};

class LabAssistant : public Employee {
    double basic;

public:
    LabAssistant(const std::string& n, int i, double b)
        : Employee(n, i), basic(b) {}

    double pay() const override {
        return basic + (basic * DA_RATE);
    }
};

class GuestFaculty : public Employee {
    double hours;
    double rate;

public:
    GuestFaculty(const std::string& n, int i, double h, double r)
        : Employee(n, i), hours(h), rate(r) {}

    double pay() const override {
        return hours * rate;
    }
};

int main() {
    std::cout << "=====================================================
";
    std::cout << "TASK 1: COLLEGE PAYROLL REFACTORED HIERARCHY DEMO
";
    std::cout << "=====================================================
";

    std::vector<Employee*> staff = {
        new Professor("Dr. Aris", 101, 80000, 15000),
        new LabAssistant("Suresh", 202, 35000),
        new GuestFaculty("Anita", 303, 40, 1200)
    };

    double totalPayroll = 0;
    for (const auto* emp : staff) {
        emp->printPaySlip();
        totalPayroll += emp->pay();
    }

    std::cout << "-----------------------------------------------------
";
    std::cout << "Total Institutional Payroll: Rs " << std::fixed << std::setprecision(2) << totalPayroll << "
";

    for (auto* emp : staff) delete emp;

    return 0;
}
