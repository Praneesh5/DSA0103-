#include <iostream>
#include <vector>

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Q2: VECTOR REALLOCATION & REFERENCE INVALIDATION\n";
    std::cout << "=====================================================\n";

    std::vector<int> marksRegister = {75, 82, 88};
    
    std::cout << "Initial Vector Details:\n";
    std::cout << "  Size    : " << marksRegister.size() << '\n';
    std::cout << "  Capacity: " << marksRegister.capacity() << '\n';
    std::cout << "  Address of underlying data: " << marksRegister.data() << '\n';

    // Obtain reference and pointer to the highest mark (element at index 2, value 88)
    int highestIndex = 2;
    int& markRef = marksRegister[highestIndex];
    int* markPtr = &marksRegister[highestIndex];

    std::cout << "\nObtained reference to highest mark:\n";
    std::cout << "  markRef value   : " << markRef << '\n';
    std::cout << "  markRef address : " << &markRef << '\n';
    std::cout << "  markPtr value   : " << *markPtr << '\n';

    std::cout << "\n--- Adding elements to force vector reallocation beyond capacity ---\n";
    // Adding multiple elements forces reallocation to a new heap block
    for (int i = 0; i < 20; ++i) {
        marksRegister.push_back(60 + i);
    }

    std::cout << "After Adding 20 Elements:\n";
    std::cout << "  New Size    : " << marksRegister.size() << '\n';
    std::cout << "  New Capacity: " << marksRegister.capacity() << '\n';
    std::cout << "  New Address of data buffer: " << marksRegister.data() << '\n';

    std::cout << "\n--- Safety Comparison: Reference vs Index ---\n";
    std::cout << "1. Evaluating Index-based access:\n";
    std::cout << "   marksRegister[highestIndex] is completely safe!\n";
    std::cout << "   Value at index " << highestIndex << ": " << marksRegister[highestIndex] 
              << " (Current address: " << &marksRegister[highestIndex] << ")\n";

    std::cout << "\n2. Evaluating Reference and Pointer access:\n";
    std::cout << "   The pre-existing markRef address is: " << &markRef << '\n';
    std::cout << "   The pre-existing markPtr points to : " << markPtr << '\n';
    std::cout << "   CRITICAL DANGER: The memory at " << markPtr 
              << " was DEALLOCATED when the vector grew!\n";
    std::cout << "   Any read or write through markRef / markPtr is now a DANGLING REFERENCE / HEAP USE AFTER FREE!\n";

    return 0;
}
