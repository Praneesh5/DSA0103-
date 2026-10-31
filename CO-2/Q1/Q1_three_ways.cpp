#include <iostream>
#include <string>
#include <vector>
#include <chrono>

// 1. Pass by Value
void applyByValue(long long fee, int percent)
{
    fee -= (fee * percent) / 100;
    std::cout << "  [Inside applyByValue] Calculated local fee: " << fee 
              << " (stored at local stack address: " << &fee << ")\n";
}

// 2. Pass by Reference
void applyByReference(long long& fee, int percent)
{
    fee -= (fee * percent) / 100;
    std::cout << "  [Inside applyByReference] Modified caller's fee: " << fee 
              << " (aliased to caller's address: " << &fee << ")\n";
}

// 3. Pass by Pointer
void applyByPointer(long long* fee, int percent)
{
    if (fee != nullptr) {
        *fee -= (*fee * percent) / 100;
        std::cout << "  [Inside applyByPointer] Modified dereferenced fee: " << *fee 
                  << " (pointer value: " << fee << ", pointer's own stack address: " << &fee << ")\n";
    }
}

// Alternative Functional approach: Return new fee (Immutable style)
long long applyFunctional(long long fee, int percent)
{
    return fee - (fee * percent) / 100;
}

// Large Student record for complexity and pass-by-const-ref analysis
struct Student {
    int id;
    std::string name;
    std::string department;
    long long tuitionFee;
    long long hostelFee;
    std::vector<std::string> enrolledCourses;
    char buffer[1024]; // 1 KB simulated records
};

// Pass large object by value (expensive copy)
void printStudentByValue(Student s) {
    std::cout << "Student " << s.name << " Hostel Fee: " << s.hostelFee 
              << " (Copied to stack: " << &s << ")\n";
}

// Pass large object by const reference (zero copy, read-only safety)
void printStudentByConstRef(const Student& s) {
    std::cout << "Student " << s.name << " Hostel Fee: " << s.hostelFee 
              << " (Direct reference: " << &s << ")\n";
}

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Q1: HOSTEL FEE CONCESSION UPDATER (THREE WAYS)\n";
    std::cout << "=====================================================\n";

    long long originalFee = 50000;
    std::cout << "Initial Base Hostel Fee: " << originalFee 
              << " (Address: " << &originalFee << ")\n\n";

    // Call by Value
    std::cout << "--- 1. Testing Call by Value (10% Merit Concession) ---\n";
    std::cout << "Predicted fee after call: " << originalFee << " (Unchanged)\n";
    applyByValue(originalFee, 10);
    std::cout << "Observed fee after call : " << originalFee << '\n';
    std::cout << "Verification: Fee remained unchanged because parameter was copied onto callee stack frame.\n\n";

    // Call by Reference
    std::cout << "--- 2. Testing Call by Reference (10% Merit Concession) ---\n";
    std::cout << "Predicted fee after call: " << (originalFee - (originalFee * 10 / 100)) << " (Changed)\n";
    applyByReference(originalFee, 10);
    std::cout << "Observed fee after call : " << originalFee << '\n';
    std::cout << "Verification: Fee modified in-place via reference alias.\n\n";

    // Call by Pointer
    std::cout << "--- 3. Testing Call by Pointer (15% Sports Concession) ---\n";
    long long expectedAfterSports = originalFee - (originalFee * 15 / 100);
    std::cout << "Predicted fee after call: " << expectedAfterSports << " (Changed)\n";
    applyByPointer(&originalFee, 15);
    std::cout << "Observed fee after call : " << originalFee << '\n';
    std::cout << "Verification: Fee modified in-place via pointer dereferencing.\n\n";

    // Multi-concession chaining with Functional approach
    std::cout << "--- 4. Multi-Concession Application (Functional vs In-Place) ---\n";
    long long studentFee = 50000;
    std::cout << "Student starting fee: " << studentFee << '\n';
    // Applying Merit (10%), Sports (15%), and Single Parent (25%) sequentially
    studentFee = applyFunctional(studentFee, 10);
    std::cout << "After 10% Merit Concession        : " << studentFee << '\n';
    studentFee = applyFunctional(studentFee, 15);
    std::cout << "After 15% Sports Concession       : " << studentFee << '\n';
    studentFee = applyFunctional(studentFee, 25);
    std::cout << "After 25% Single Parent Concession: " << studentFee << '\n';

    std::cout << "\n--- 5. Pass-by-Const-Reference vs Pass-by-Value for Large Objects ---\n";
    Student st{101, "Aarav", "CSE", 85000, studentFee, {"CS201", "CS202", "CS203"}, {}};
    std::cout << "Original Student address: " << &st << '\n';
    printStudentByConstRef(st);
    printStudentByValue(st);

    return 0;
}
