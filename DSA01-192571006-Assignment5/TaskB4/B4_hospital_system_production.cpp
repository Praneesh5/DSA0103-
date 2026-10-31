#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iomanip>

// Base Abstract Classes
class Bill {
protected:
    std::string patient;
public:
    explicit Bill(const std::string& p) : patient(p) {}
    virtual ~Bill() = default;
    virtual double total() const = 0;
    virtual std::string type() const = 0;
    std::string getPatient() const { return patient; }
};

class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& b) const = 0;
    virtual std::string name() const = 0;
};

// Existing Classes
class OutPatient : public Bill {
    double consultation, tests;
public:
    OutPatient(const std::string& p, double c, double t)
        : Bill(p), consultation(c), tests(t) {}
    double total() const override { return consultation + tests; }
    std::string type() const override { return "OutPatient"; }
};

class InPatient : public Bill {
    int days; double perDay, surgery;
public:
    InPatient(const std::string& p, int d, double r, double s)
        : Bill(p), days(d), perDay(r), surgery(s) {}
    double total() const override { return (days * perDay) + surgery; }
    std::string type() const override { return "InPatient"; }
};

// NEW EXTENSION BILL TYPE: DayCareSurgery (Zero modifications to existing code)
class DayCareSurgery : public Bill {
    double packagePrice;
public:
    DayCareSurgery(const std::string& p, double pkg)
        : Bill(p), packagePrice(pkg) {}
    double total() const override { return packagePrice; }
    std::string type() const override { return "DayCareSurgery"; }
};

// Existing Payers
class Insurance : public Payer {
    double limit, copay;
public:
    Insurance(double l, double cp) : limit(l), copay(cp) {}
    double amountCovered(const Bill& b) const override {
        double payable = b.total() * (1.0 - copay);
        return payable > limit ? limit : payable;
    }
    std::string name() const override { return "Commercial Insurance"; }
};

// NEW EXTENSION PAYER: CorporateTieUp (Zero modifications to existing code)
class CorporateTieUp : public Payer {
    double corporateDiscount;
public:
    explicit CorporateTieUp(double disc = 0.15) : corporateDiscount(disc) {}
    double amountCovered(const Bill& b) const override {
        return b.total() * (1.0 - corporateDiscount); // 85% settled directly
    }
    std::string name() const override { return "Corporate Corporate-Pass"; }
};

int main() {
    std::cout << "=====================================================
";
    std::cout << "TASK B4: PRODUCTION-SCALE MULTI-HOSPITAL BILLING
";
    std::cout << "=====================================================
";

    std::vector<std::unique_ptr<Bill>> bills;
    bills.push_back(std::make_unique<OutPatient>("Aarav", 600, 1400));
    bills.push_back(std::make_unique<InPatient>("Divya", 4, 3000, 65000));
    bills.push_back(std::make_unique<DayCareSurgery>("Karthik", 28000)); // Extension

    Insurance ins(50000, 0.10); // 10% copay, 50k cap
    CorporateTieUp corp(0.15);  // 15% corporate discount

    for (const auto& b : bills) {
        std::cout << "[" << b->type() << "] Patient: " << std::setw(8) << b->getPatient()
                  << " | Total: Rs " << std::fixed << std::setprecision(2) << std::setw(9) << b->total()
                  << " | Ins Covered: Rs " << std::setw(9) << ins.amountCovered(*b)
                  << " | Corp Covered: Rs " << std::setw(9) << corp.amountCovered(*b) << "\n";
    }

    return 0;
}
