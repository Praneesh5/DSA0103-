# DSA01-192571006-Assignment4
## Object Oriented Programming with C++ (DSA01) — Course Outcome 4 (CO4)
**Student Name:** Praneesh  
**Register Number:** 192571006  
**Compiler:** g++ (GCC) 15.2.0 (MinGW-w64)  
**Compilation Flags:** `g++ -std=c++17 -Wall -Wextra -fsanitize=address`  

---

### Folder & Task Structure:
- `Task01/`: Class Design Review (Diagnosis, Refactored Payroll Hierarchy)
  - `Task1_payroll_hierarchy.cpp`
- `Task02/`: University People Management (Virtual Base Classes, Diamond Resolution)
  - `Task2_university_diamond.cpp`
- `Task03/`: Constructors in Derived & Member Classes (Order of Construction/Destruction)
  - `Task3_construction_order.cpp`
  - `Task3_complete_the_code.cpp`
- `Task04/`: Pointers to Derived Classes Forensics (Memory Leaks, Slicing, Virtual Dtor)
  - `Task4_account_forensics.cpp`
  - `Task4_predict_output.cpp`
- `Task05/`: Pointers to Objects & the this Pointer (Chaining, Array of Pointers)
  - `Task5_cart_this_pointer.cpp`
- `Task06/`: Vehicle Rental Fleet Management System (Design Challenge, OCP Extension)
  - `Task6_vehicle_rental_fleet.cpp`

---

### Compilation & Execution Commands:
```bash
# Compile and run Task 1
g++ -std=c++17 -Wall -Wextra Task01/Task1_payroll_hierarchy.cpp -o Task01/Task1.exe
./Task01/Task1.exe

# Compile and run Task 4 under Address Sanitizer
g++ -std=c++17 -Wall -Wextra -fsanitize=address Task04/Task4_account_forensics.cpp -o Task04/Task4.exe
./Task04/Task4.exe

# Compile and run Task 6 Design Challenge
g++ -std=c++17 -Wall -Wextra Task06/Task6_vehicle_rental_fleet.cpp -o Task06/Task6.exe
./Task06/Task6.exe
```
