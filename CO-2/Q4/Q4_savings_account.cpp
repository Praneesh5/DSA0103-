#include <iostream>
#include <iomanip>
#include <string>

class SavingsAccount {
private:
    static const int CAPACITY = 10;
    long long accountNumber;
    double balance;
    double transactions[CAPACITY]; // Array within the class storing last 10 transactions
    int txCount;                   // Total number of transactions recorded so far

    // Private helper: Shift-based buffer update
    void recordTransaction(double amount);

public:
    // Constructor defined inside the class
    SavingsAccount(long long accNo, double initialBalance)
        : accountNumber(accNo), balance(initialBalance), txCount(0)
    {
        for (int i = 0; i < CAPACITY; ++i) {
            transactions[i] = 0.0;
        }
    }

    // Member function defined INSIDE the class
    bool deposit(double amount)
    {
        if (amount <= 0) {
            std::cout << "  [Error] Deposit amount must be positive.\n";
            return false;
        }
        balance += amount;
        recordTransaction(+amount);
        return true;
    }

    // Member functions defined OUTSIDE the class using scope resolution operator (::)
    bool withdraw(double amount);
    void miniStatement() const;
    double getBalance() const;
    long long getAccountNumber() const;
};

// Definition of member function outside class
void SavingsAccount::recordTransaction(double amount)
{
    if (txCount < CAPACITY) {
        // Buffer has empty slots: append at current count
        transactions[txCount] = amount;
        txCount++;
    } else {
        // Buffer is FULL (11th or subsequent transaction arrives):
        // Shift all elements left by 1 position (discarding transactions[0])
        for (int i = 0; i < CAPACITY - 1; ++i) {
            transactions[i] = transactions[i + 1];
        }
        // Place new transaction at the end (index CAPACITY - 1)
        transactions[CAPACITY - 1] = amount;
        txCount++;
    }
}

// Definition of withdraw outside class
bool SavingsAccount::withdraw(double amount)
{
    if (amount <= 0) {
        std::cout << "  [Error] Withdrawal amount must be positive.\n";
        return false;
    }
    if (amount > balance) {
        std::cout << "  [Declined] Insufficient funds! Current Balance: " << balance 
                  << ", Attempted Withdrawal: " << amount << '\n';
        return false;
    }
    balance -= amount;
    recordTransaction(-amount);
    return true;
}

// Definition of miniStatement outside class
void SavingsAccount::miniStatement() const
{
    std::cout << "\n=====================================================\n";
    std::cout << "             SAVINGS ACCOUNT MINI-STATEMENT          \n";
    std::cout << "=====================================================\n";
    std::cout << " Account Number: " << accountNumber << '\n';
    std::cout << " Current Balance: $" << std::fixed << std::setprecision(2) << balance << '\n';
    std::cout << " Total Transactions Executed: " << txCount << '\n';
    std::cout << " Showing Last " << (txCount < CAPACITY ? txCount : CAPACITY) << " Transactions:\n";
    std::cout << "-----------------------------------------------------\n";
    std::cout << "  No. | Type       | Amount       \n";
    std::cout << "-----------------------------------------------------\n";

    int displayedCount = (txCount < CAPACITY) ? txCount : CAPACITY;
    for (int i = 0; i < displayedCount; ++i) {
        double val = transactions[i];
        std::string type = (val >= 0) ? "DEPOSIT   " : "WITHDRAWAL";
        double absVal = (val >= 0) ? val : -val;
        std::cout << "  " << std::setw(3) << (i + 1) << " | " 
                  << type << " | $" << std::setw(10) << absVal << '\n';
    }
    std::cout << "=====================================================\n";
}

double SavingsAccount::getBalance() const { return balance; }
long long SavingsAccount::getAccountNumber() const { return accountNumber; }

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Q4: SAVINGS ACCOUNT MINI-STATEMENT (ARRAY IN CLASS)\n";
    std::cout << "=====================================================\n";

    SavingsAccount acc(192571006001LL, 5000.00);
    std::cout << "Created Account " << acc.getAccountNumber() 
              << " with Opening Balance: $" << acc.getBalance() << "\n\n";

    std::cout << "--- Executing 14 Transactions to Observe 11th+ Shift Behavior ---\n";
    acc.deposit(1200.00); // Tx 1
    acc.withdraw(300.00); // Tx 2
    acc.deposit(450.00);  // Tx 3
    acc.withdraw(150.00); // Tx 4
    acc.deposit(2000.00); // Tx 5
    acc.withdraw(500.00); // Tx 6
    acc.deposit(800.00);  // Tx 7
    acc.withdraw(250.00); // Tx 8
    acc.deposit(1100.00); // Tx 9
    acc.withdraw(400.00); // Tx 10 (Buffer now completely full)
    
    std::cout << "\n[Status after 10 transactions]: Buffer is now at maximum capacity (10).\n";
    std::cout << "Next 4 transactions will trigger array shifting, discarding oldest records:\n";
    
    acc.deposit(950.00);  // Tx 11 (Discards Tx 1: +1200)
    acc.withdraw(600.00); // Tx 12 (Discards Tx 2: -300)
    acc.deposit(1500.00); // Tx 13 (Discards Tx 3: +450)
    acc.withdraw(350.00); // Tx 14 (Discards Tx 4: -150)

    // Print Mini-Statement
    acc.miniStatement();

    return 0;
}
