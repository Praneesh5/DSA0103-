#include <iostream>
#include <vector>
#include <memory>

class Account {
public:
    virtual double rate() const { return 3.5; }
    virtual double vrate() const { return 3.5; }
    virtual ~Account() {
        std::cout << "~Account() executed cleanly\n";
    }
};

class SeniorAccount : public Account {
    double* history;

public:
    SeniorAccount() : history(new double[100]) {
        for (int i = 0; i < 100; ++i) history[i] = 0.0;
    }

    double rate() const override { return 4.0; }
    double vrate() const override { return 4.0; }

    ~SeniorAccount() override {
        delete[] history;
        std::cout << "~SeniorAccount() executed: 800 bytes dynamic history released\n";
    }
};

int main() {
    std::cout << "--- Polymorphic Banking Account Execution ---\n";
    std::vector<std::unique_ptr<Account>> accounts;
    accounts.push_back(std::make_unique<Account>());
    accounts.push_back(std::make_unique<SeniorAccount>());

    for (const auto& a : accounts) {
        std::cout << "Account Interest Rate: " << a->rate() << "%\n";
    }

    std::cout << "\nExiting scope (Automatic Virtual Destruction):\n";
    accounts.clear();

    return 0;
}
