#include <iostream>

// DANGEROUS FUNCTION: Returns reference to local stack variable
int& getLocalDanglingReference()
{
    int localMark = 95;
    std::cout << "[Inside getLocalDanglingReference] localMark created at stack address: " << &localMark << '\n';
    return localMark; // WARNING: Reference to local variable returned
}

void dummyStackOverwrite()
{
    // Clobbers stack memory where localMark previously resided
    volatile int garbage[16];
    for (int i = 0; i < 16; ++i) {
        garbage[i] = 0xDEADBEEF;
    }
}

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Q2: DANGLING REFERENCE & UNDEFINED BEHAVIOR TEST\n";
    std::cout << "=====================================================\n";

    std::cout << "Calling getLocalDanglingReference()...\n";
    int& danglingRef = getLocalDanglingReference();

    std::cout << "Dangling reference points to expired address: " << &danglingRef << '\n';
    std::cout << "Immediate read (may seemingly show 95 before stack overwrite): " << danglingRef << '\n';

    std::cout << "\nOverwriting stack frame via dummyStackOverwrite()...\n";
    dummyStackOverwrite();

    std::cout << "Value read after stack overwrite: " << danglingRef << '\n';
    std::cout << "Notice: Memory was corrupted/overwritten! This is UNDEFINED BEHAVIOR.\n";

    return 0;
}
