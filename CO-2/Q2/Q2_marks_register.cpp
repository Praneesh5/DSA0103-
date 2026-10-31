#include <iostream>
#include <iomanip>

// 1. Return by Reference (Modifiable Lvalue)
int& findHighest(int marks[], int n)
{
    int maxIdx = 0;
    for (int i = 1; i < n; ++i) {
        if (marks[i] > marks[maxIdx]) {
            maxIdx = i;
        }
    }
    return marks[maxIdx]; // Returns reference to the actual array element
}

// 2. Return by Const Reference (Read-Only Lvalue)
const int& findHighestConst(const int marks[], int n)
{
    int maxIdx = 0;
    for (int i = 1; i < n; ++i) {
        if (marks[i] > marks[maxIdx]) {
            maxIdx = i;
        }
    }
    return marks[maxIdx]; // Cannot be modified by caller
}

// 3. Return by Copy (Rvalue)
int findHighestCopy(const int marks[], int n)
{
    int maxVal = marks[0];
    for (int i = 1; i < n; ++i) {
        if (marks[i] > maxVal) {
            maxVal = marks[i];
        }
    }
    return maxVal; // Returns temporary copy
}

// Alternative Concept A: Return by Index
int findHighestIndex(const int marks[], int n)
{
    int maxIdx = 0;
    for (int i = 1; i < n; ++i) {
        if (marks[i] > marks[maxIdx]) {
            maxIdx = i;
        }
    }
    return maxIdx;
}

// Alternative Concept B: Return by Pointer
int* findHighestPointer(int marks[], int n)
{
    int maxIdx = 0;
    for (int i = 1; i < n; ++i) {
        if (marks[i] > marks[maxIdx]) {
            maxIdx = i;
        }
    }
    return &marks[maxIdx];
}

void printMarks(const int marks[], int n, const std::string& label)
{
    std::cout << std::left << std::setw(35) << label << ": [ ";
    for (int i = 0; i < n; ++i) {
        std::cout << marks[i] << (i + 1 < n ? ", " : " ");
    }
    std::cout << "]\n";
}

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Q2: EXAMINATION MARKS REGISTER - RETURN BY REFERENCE\n";
    std::cout << "=====================================================\n";

    int marks[] = {78, 85, 92, 64, 89};
    int n = sizeof(marks) / sizeof(marks[0]);

    printMarks(marks, n, "Initial Marks Register");

    // 1. Direct Moderation using Return by Reference
    std::cout << "\n--- 1. Moderation (+2 marks) via Return by Reference ---\n";
    std::cout << "Highest mark before moderation: " << findHighest(marks, n) << '\n';
    
    // Using function call on the left-hand side of += and =
    findHighest(marks, n) += 2;
    printMarks(marks, n, "After +2 Moderation");

    // Direct assignment to function call
    std::cout << "\n--- 2. Direct Assignment via Left-hand-side Reference ---\n";
    findHighest(marks, n) = 99; // Sets the highest mark directly to 99
    printMarks(marks, n, "After findHighest(...) = 99");

    // 3. Const Reference Demonstration
    std::cout << "\n--- 3. Return by Const Reference (Safety Guarantee) ---\n";
    const int& readOnlyTop = findHighestConst(marks, n);
    std::cout << "Read-only top mark: " << readOnlyTop << '\n';
    // The line below would trigger compile error: error: assignment of read-only reference
    // findHighestConst(marks, n) += 5; 
    std::cout << "Compile-time protection: findHighestConst(marks, n) cannot appear on LHS of =\n";

    // 4. Return by Copy Demonstration
    std::cout << "\n--- 4. Return by Copy (Rvalue behavior) ---\n";
    int copiedVal = findHighestCopy(marks, n);
    std::cout << "Copied top mark: " << copiedVal << " (Independent copy in stack memory)\n";
    // findHighestCopy(marks, n) = 100; // Compile error: lvalue required as left operand of assignment

    // 5. Alternative Concept A: Return by Index
    std::cout << "\n--- 5. Alternative Concept A: Return by Index ---\n";
    int topIdx = findHighestIndex(marks, n);
    std::cout << "Found highest at index: " << topIdx << " (Value: " << marks[topIdx] << ")\n";
    marks[topIdx] -= 5; // Moderation rollback
    printMarks(marks, n, "After index-based reduction (-5)");

    // 6. Alternative Concept B: Return by Pointer
    std::cout << "\n--- 6. Alternative Concept B: Return by Pointer ---\n";
    int* topPtr = findHighestPointer(marks, n);
    std::cout << "Found highest at address: " << topPtr << " (Value: " << *topPtr << ")\n";
    *topPtr += 1; // Direct pointer modification
    printMarks(marks, n, "After pointer-based increment (+1)");

    return 0;
}
