# DSA01-192571006-Assignment5
## Object Oriented Programming with C++ (DSA01) — Course Outcome 5 (CO5)
**Student Name:** Praneesh  
**Register Number:** 192571006  
**Compiler:** g++ (GCC) 15.2.0 (MinGW-w64)  
**Compilation Flags:** `g++ -std=c++17 -Wall -Wextra -fsanitize=address`  

---

### Folder & Task Structure:
- `PartA_Design/`: Architectural Models & Predict Output Traces
  - `A2_predict_output.cpp`
  - `A5_predict_output.cpp`
- `TaskB1/`: Polymorphic Billing Implementation & Validation
  - `B1_polymorphic_billing.cpp`
- `TaskB2/`: Engineered Error Conditions & Exception Rethrowing
  - `B2_error_engineering.cpp`
  - `B2_predict_output.cpp`
- `TaskB3/`: Performance Microbenchmarks (Virtual vs Switch vs Exceptions)
  - `B3_virtual_vs_switch_benchmark.cpp`
- `TaskB4/`: Production Healthcare Conglomerate Extension & Defense
  - `B4_hospital_system_production.cpp`

---

### Compilation & Execution Commands:
```bash
# Compile and run Task B1 Complete the Code
g++ -std=c++17 -Wall -Wextra TaskB1/B1_polymorphic_billing.cpp -o TaskB1/B1.exe
./TaskB1/B1.exe

# Compile and run Task B2 under Address Sanitizer
g++ -std=c++17 -Wall -Wextra -fsanitize=address TaskB2/B2_error_engineering.cpp -o TaskB2/B2.exe
./TaskB2/B2.exe

# Compile and run Task B3 Benchmarks with -O2
g++ -std=c++17 -O2 TaskB3/B3_virtual_vs_switch_benchmark.cpp -o TaskB3/B3.exe
./TaskB3/B3.exe

# Compile and run Task B4 Production System
g++ -std=c++17 -Wall -Wextra -fsanitize=address TaskB4/B4_hospital_system_production.cpp -o TaskB4/B4.exe
./TaskB4/B4.exe
```
