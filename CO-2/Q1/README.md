# Task 01 (Q1) – Call by Value, by Reference and by Pointer

## Problem Context
The hostel office applies concessions (Merit 10%, Sports 15%, Single Parent 25%) to students' hostel fees. When written using call-by-value, the function calculates the discount locally, but the caller's stored fee remains unchanged upon function return.

## Source Files
- `Q1_predict.cpp`: Predict-the-output verification code testing call-by-value, call-by-reference, call-by-pointer, return-by-reference, and default arguments.
- `Q1_complete.cpp`: Completed code for `TODO 1` implementing integer concession calculation via call by reference.
- `Q1_three_ways.cpp`: Comprehensive implementation showing pass-by-value, pass-by-reference, pass-by-pointer, sequential multi-concession chaining, and pass-by-const-reference for large `Student` structures.

---

## 1. Predict the Output Analysis

### Program Code:
```cpp
#include <iostream>

void change(int a, int& b, int* c)
{
    a += 10;
    b += 10;
    *c += 10;
}

int& bigger(int& p, int& q) { return p > q ? p : q; }

void show(int x, int y = 5, int z = 10) { std::cout << x + y + z << ' '; }

int main()
{
    int a = 1, b = 2, c = 3;
    change(a, b, &c);
    std::cout << a << ' ' << b << ' ' << c << '\n';

    bigger(a, c) = 100;
    std::cout << a << ' ' << c << '\n';

    show(1);
    show(1, 2);
    show(1, 2, 3);
    std::cout << '\n';
}
```

### Predicted vs Observed Output:
| Line | Predicted Output | Actual Output | Status |
|------|------------------|---------------|--------|
| Call 1 (`change`) | `1 12 13` | `1 12 13` | Exact Match |
| Call 2 (`bigger`) | `1 100` | `1 100` | Exact Match |
| Call 3 (`show`) | `16 13 6 ` | `16 13 6 ` | Exact Match |

### Detailed Explanation:
1. **`change(a, b, &c)`**:
   - `a` is passed by value: parameter `a` is a local copy created on `change`'s stack frame. Incrementing `a += 10` makes local `a = 11`, but caller's variable `a` in `main` remains `1`.
   - `b` is passed by reference (`int& b`): `b` is an alias/lvalue reference to caller's `b`. Incrementing `b += 10` modifies the memory location of caller's `b` from `2` to `12`.
   - `c` is passed by pointer (`int* c`): `c` receives the address of caller's `c` (`&c`). Dereferencing `*c += 10` modifies the memory content of caller's `c` from `3` to `13`.
   - Output: `1 12 13`.

2. **`bigger(a, c) = 100`**:
   - `bigger(int& p, int& q)` accepts references to `a` (1) and `c` (13).
   - Ternary check `p > q ? p : q` evaluates `1 > 13`, which is `false`. Hence, it returns a reference to `q` (which aliases caller's `c`).
   - Because the return type is an lvalue reference (`int&`), the function call expression `bigger(a, c)` is a modifiable lvalue that can appear on the left side of the assignment operator `=`.
   - `c` is assigned `100`. Variable `a` remains `1`.
   - Output: `1 100`.

3. **Calls to `show(x, y = 5, z = 10)`**:
   - Default arguments are applied right-to-left when arguments are omitted.
   - `show(1)`: uses default `y = 5` and default `z = 10`. Evaluates $1 + 5 + 10 = 16$.
   - `show(1, 2)`: passes `x = 1, y = 2`, uses default `z = 10`. Evaluates $1 + 2 + 10 = 13$.
   - `show(1, 2, 3)`: overrides all defaults: $1 + 2 + 3 = 6$.
   - Output: `16 13 6 `.

---

## 2. Complete the Code Analysis (`Q1_complete.cpp`)
```cpp
void applyConcession(long long& fee, int percent)
{
    // TODO 1: Reduce the caller's fee by percent using integer arithmetic (call by reference)
    fee -= fee * percent / 100;
}
```
### Execution Result:
```text
after call by value : 45000
after call by reference: 40500
```
### Why Call by Value Leaves the Fee at 45000:
In C++, passing by value creates a brand new copy of `long long fee` (8 bytes) on the callee's activation record (stack frame). The arithmetic `fee -= fee * percent / 100` alters only this temporary stack variable. Upon returning, the callee's activation frame is popped, destroying the local variable. The caller's `hostelFee` stored in `main`'s stack frame was never touched.

---

## 3. Inquiry: Memory Locations, Parameter Storage & Const References
1. **Memory Storage**:
   - **Pass by Value**: The argument's value is bitwise copied onto the callee's stack frame. It occupies a distinct memory address. Changes are isolated.
   - **Pass by Reference**: Under the hood (in assembly/machine code), references are implemented as dereferenced pointers. The reference holds the address of caller's variable. It requires no copy of the underlying data and directly mutates the caller's memory.
   - **Pass by Pointer**: A pointer variable (8 bytes on 64-bit architecture) is pushed onto the callee's stack frame storing the memory address passed via `&variable`. Explicit dereferencing `*ptr` accesses and mutates the caller's memory.
2. **When to use `const` Reference**:
   - A `const` reference (`const T&`) should be used whenever passing non-primitive, large objects (e.g., `std::string`, `std::vector`, large `struct Student`) that must **not** be modified by the function.
   - **Benefits**:
     1. Avoids expensive copying overhead (deep copies, memory allocations).
     2. Provides compile-time immutability guarantee: caller is guaranteed the function cannot alter the object.
     3. Can bind to temporary rvalues (e.g., `printStudent(Student{...})`).

---

## 4. Design Decision: In-Place Modification vs Functional Return
### Multi-Concession Context:
A student may qualify for multiple concessions (e.g., Merit 10%, Sports 15%, Single Parent 25%).
- **Option A (In-Place via Reference: `void applyConcession(long long& fee, int percent)`)**:
  - Direct mutation of student record.
  - Efficient, zero intermediate variables.
  - Drawback: Modifies original state, making audit trails, rollbacks, and before/after comparisons difficult.
- **Option B (Functional Return: `long long applyConcession(long long fee, int percent)`)**:
  - Returns `fee - (fee * percent / 100)`.
  - Enables clean functional pipelining:
    `fee = applyConcession(applyConcession(fee, 10), 15);`
  - Preserves immutability and intermediate computation stages for receipts and auditing.
- **Defense**:
  For financial fee transactions, the **Functional style (returning new fee)** or **Chained Reference style (`long long& applyConcession(long long& fee, int percent)`)** is superior because it enables chaining concessions while maintaining transparency. For updating a persistent `Student` entity in a database or register, in-place reference passing is optimal to avoid copying.

---

## 5. Alternative Concept and Complexity Analysis
### Complexity Comparison:
| Mechanism | Time Complexity | Auxiliary Space Copied (`long long`) | Auxiliary Space Copied (`Student` ~1.2 KB) |
|-----------|-----------------|--------------------------------------|---------------------------------------------|
| **Pass by Value** | $O(1)$ | 8 bytes (stack copy) | ~1200 bytes + dynamic memory copies |
| **Pass by Reference** | $O(1)$ | 0 bytes copied (address alias) | 0 bytes copied (8-byte pointer in machine code) |
| **Pass by Pointer** | $O(1)$ | 8 bytes (pointer address) | 8 bytes (pointer address) |
| **Pass by Const Reference**| $O(1)$ | 0 bytes copied | 0 bytes copied (8-byte pointer internally) |
| **Functional Return** | $O(1)$ | 8 bytes return register | Same as above |

### Key Takeaway:
For primitive types like `long long`, passing by value costs only 8 bytes (often passed directly in CPU registers like `%rcx` / `%rdi`), making value vs reference negligible in time and space. However, for compound structures (`Student`), pass-by-value causes $O(\text{size})$ deep copy overhead, whereas pass-by-const-reference achieves $O(1)$ space and instant transfer without safety risks.
