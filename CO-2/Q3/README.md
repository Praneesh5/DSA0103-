# Task 03 (Q3) – Default Arguments and Function Overloading

## Problem Context
A courier company charges for parcels based on weight, with optional express delivery and insurance add-ons. The initial developer implemented:
1. `charge(double weight)`
2. `charge(double weight, bool express)`
3. `charge(double weight, bool express = false, bool insured = false)`

This design failed to compile due to ambiguity errors.

## Source Files
- `Q3_ambiguity.cpp`: Demonstrates and reproduces the compiler ambiguity errors.
- `Q3_courier_charge.cpp`: Redesigned overload system handling weight in grams (`int`), weight in kilograms (`double`), and parcel structures (`Parcel`), utilizing non-ambiguous right-aligned default arguments.
- `output_ambiguity_error.txt`: Exact GCC compiler error log.
- `output_courier.txt`: Execution output validating predicted vs actual overload selections.

---

## 1. Explanation of the Compiler Error (Ambiguity)

### What Caused the Ambiguity?
When calling `charge(5.0)`:
- Candidate 1: `double charge(double weight)` requires exactly 1 argument of type `double`.
- Candidate 3: `double charge(double weight, bool express = false, bool insured = false)` can accept 1 argument because default arguments are provided for `express` and `insured`.

Both functions are viable candidates with identical parameter conversion ranks for the first argument (`double` $\rightarrow$ `double` exact match). The C++ compiler has no rule to prefer one over the other, causing:
```text
error: call of overloaded 'charge(double)' is ambiguous
note: candidate 1: 'double charge(double)'
note: candidate 2: 'double charge(double, bool, bool)'
```
Similarly, calling `charge(5.0, true)` creates an identical ambiguity between `charge(double, bool)` and `charge(double, bool, bool = false)`.

---

## 2. Redesigned Overload Architecture

To eliminate ambiguity while keeping common calls concise and supporting various inputs:
1. **Weight in Grams (`int`)**:
   ```cpp
   double charge(int weightGrams, bool express = false, bool insured = false);
   ```
2. **Weight in Kilograms (`double`)**:
   ```cpp
   double charge(double weightKg, bool express = false, bool insured = false);
   ```
3. **Parcel Object (`const Parcel&`)**:
   ```cpp
   double charge(const Parcel& p);
   ```

---

## 3. Predictions vs Actual Overload Resolution

| Call Invocation | Argument Types Provided | Predicted Function Selected | Actual Function Selected | Computed Cost | Rationale |
|-----------------|-------------------------|------------------------------|--------------------------|---------------|-----------|
| `charge(500)` | `(int)` | `charge(int, bool, bool)` | `charge(int, ...)` | 25.0 | Exact `int` match; defaults `express=false, insured=false` |
| `charge(2)` | `(int)` | `charge(int, bool, bool)` | `charge(int, ...)` | 0.1 | Exact `int` match (2 grams); defaults applied |
| `charge(2.0f)` | `(float)` | `charge(double, bool, bool)` | `charge(double, ...)` | 100.0 | `float` converts to `double` via standard floating-point conversion |
| `charge(2.5, true)` | `(double, bool)` | `charge(double, bool, bool)` | `charge(double, ...)` | 225.0 | Exact `(double, bool)` match; default `insured=false` applied |
| `charge(1500, false, true)` | `(int, bool, bool)` | `charge(int, bool, bool)` | `charge(int, ...)` | 125.0 | Exact `(int, bool, bool)` match with all 3 arguments |
| `charge(p1)` | `(Parcel)` | `charge(const Parcel&)` | `charge(const Parcel&)` | 600.0 | Exact match for user-defined `Parcel` entity |

---

## 4. Inquiry: Overload Resolution Rules and Default Argument Alignment

### C++ Overload Resolution Precedence:
When resolving an overloaded function call, the C++ compiler performs candidate ranking in strict priority order:
1. **Exact Match**: The argument matches parameter type exactly, or differs only by trivial qualifiers (`const`, `&`).
2. **Promotion**:
   - Integral promotion: `char`, `short`, `bool` $\rightarrow$ `int`.
   - Floating-point promotion: `float` $\rightarrow$ `double`.
3. **Standard Conversion**:
   - Floating-to-integral (e.g., `double` $\rightarrow$ `int`) or integral-to-floating (`int` $\rightarrow$ `double`).
   - Pointer conversions (e.g., derived to base pointer, `T*` $\rightarrow$ `void*`).
4. **User-Defined Conversions**: Conversion constructors or conversion operators.
5. **Ellipsis (`...`)**: Variadic catch-all.

*Why `charge(2.0f)` selected `charge(double, ...)`*: Converting `float` to `double` is a standard floating-point conversion, which ranks strictly higher than converting `float` to `int` (a truncating floating-integral conversion).

### Why Default Arguments Must be Specified from Right to Left:
In C++, function arguments are matched positionally from **left to right** at the call site.
- If a function were declared as `void test(int a = 10, int b);` and called as `test(5);`, the compiler has no way to determine whether `5` was intended for `a` (leaving `b` uninitialized) or for `b` (using the default for `a`).
- To prevent positional parsing ambiguity, the C++ language standard mandates that if any parameter has a default argument, **all subsequent parameters to its right must also have default arguments**.

---

## 5. Alternative Concept and Complexity Analysis

### Alternative: Single `Parcel` Configuration Struct vs Overloaded Signatures
Instead of multiple overloaded functions, we could use a single configuration structure:
```cpp
struct ParcelOptions {
    double weightKg = 0.0;
    bool express = false;
    bool insured = false;
    bool fragile = false;
};
double calculateCharge(const ParcelOptions& opt);
```

### Complexity and Extensibility Comparison:
| Metric | Overloaded Functions with Default Args | Single `ParcelOptions` Struct |
|--------|----------------------------------------|-------------------------------|
| **Compile-Time Cost** | Overload resolution during compilation | Minimal overload resolution |
| **Run-Time Time Complexity** | **$O(1)$** (Zero runtime dispatch overhead) | **$O(1)$** |
| **Stack Memory Copied** | Primitive values passed via registers ($0-8$ bytes) | 8-byte pointer passed via register (`const&`) |
| **Adding a New Option** (e.g., `fragile = true`) | Must modify function signatures, breaking existing call sites or adding yet another overload | **Seamless**: Add new field `bool fragile = false` to struct without breaking any existing caller |

### Conclusion:
For simple calls, overloaded functions with default arguments provide the most natural and concise syntax. However, as enterprise options multiply (express, insured, fragile, weekend delivery, customs declarations), transitioning to a **Parameter Object / Builder Pattern** (`ParcelOptions`) avoids combinatorial parameter explosion and preserves backward binary compatibility.
