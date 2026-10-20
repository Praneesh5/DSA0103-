# TASK 10: Compare Expectations with Execution
**Bloom's Taxonomy Level:** K5 – Evaluate  
**Course Outcome:** CO1 (Empirical Measurement, Overflow Analysis, Compiler Warnings, Precision Drift)

---

## 1. Scale Simulation Results: 20,000 vs. 500,000 vs. 3,000,000 Journeys

The simulation models the progressive scaling of the Namma Metro Smart-Card Fare System from a single station to a single line and finally the entire urban network:

| Scale Tier | Daily Volume ($N$) | Exact `long long` (Paise) | Exact `long long` (Rs) | `double` Accumulator (Rs) | Observed Drift (Paise) | 32-bit `int` Overflow Status |
| :--- | :---: | :---: | :---: | :---: | :---: | :--- |
| **1 Station** | $20,000$ | $39,960,000$ | Rs $399,600.00$ | Rs $399,600.0000$ | $+0.0000$ paise | Fits safely ($39.9\text{M} < \text{INT\_MAX}$) |
| **1 Metro Line** | $500,000$ | $999,000,000$ | Rs $9,990,000.00$ | Rs $9,990,000.0000$ | $+0.0000$ paise | Fits safely ($999\text{M} < \text{INT\_MAX}$) |
| **Entire Network**| $\mathbf{3,000,000}$ | $\mathbf{5,994,000,000}$ | $\mathbf{Rs\ 59,940,000.00}$ | $\mathbf{Rs\ 59,940,000.0000}$ | Fractional bit drift | **OVERFLOWED! Wrapped into negative integer!** |

---

## 2. Integer Overflow Analysis for the Whole Network

- **32-Bit Signed Integer Maximum Range:**
  $$\text{INT\_MAX} = 2^{31} - 1 = 2,147,483,647\text{ paise (Rs 21,474,836.47)}$$
- **Total Network Revenue at 3,000,000 Journeys:**
  $$\text{Revenue} = 5,994,000,000\text{ paise (Rs 59,940,000.00)}$$
- **Overflow Mechanism:**
  $$5,994,000,000 > 2,147,483,647 \implies \text{Revenue} > \text{INT\_MAX}$$
  In two's complement 32-bit arithmetic, the 31st sign bit is inverted:
  $$\text{static\_cast<int>}(5,994,000,000) = 5,994,000,000 - 2^{32} = 5,994,000,000 - 4,294,967,296 = 1,699,032,704$$
  With signed overflow undefined behavior in C++, this causes severe revenue misreporting, tax audit failure, and financial discrepancies.
- **Remedy:** Accumulation must be stored in `long long` (or `int64_t`), which supports up to $9.22 \times 10^{18}$ paise ($\approx 1.5$ million years of continuous network revenue).

---

## 3. Analysis of Compiler Warnings with `-Wall -Wextra`

When compiled with `g++ -std=c++17 -Wall -Wextra -Wconversion -Wsign-compare`, the compiler emits crucial diagnostics:

1. **Warning: `-Wsign-compare`:**
   - *Trigger:* `int s = -1; unsigned int u = 1; if (s < u)`
   - *Meaning:* Comparing signed and unsigned integers triggers implicit conversion of the signed operand to unsigned, transforming negative values into massive positive numbers (`UINT_MAX`).
   - *Remedy:* Explicit casting after asserting non-negativity or using C++20 `std::cmp_less`.
2. **Warning: `-Wconversion`:**
   - *Trigger:* `int base = 1000; int result = base * 1.2;`
   - *Meaning:* Implicit conversion from `double` to `int` alters value and discards fractional parts.
   - *Remedy:* Explicit `static_cast<int>(...)` or pure integer arithmetic.
3. **Warning: `-Wswitch`:**
   - *Trigger:* Missing an enum variant in a switch statement without a `default:` label.
   - *Meaning:* An unhandled enum value leads to undefined control flow.
   - *Remedy:* Explicitly handle every `enum class` variant.

---

## 4. Alternative Concept & Complexity Analysis: `int` vs. `long long` vs. `double` Accumulators

| Evaluation Criterion | 32-Bit `int` | 64-Bit `long long` (Mandatory Choice) | 64-Bit `double` |
| :--- | :--- | :--- | :--- |
| **Failure Point** | **Fails at $N \approx 1,074,000$ journeys** (Signed 32-bit overflow) | **Never fails** (Exceeds capacity only after $4.6 \times 10^{15}$ journeys) | **Fails gradually via precision drift** when mantissa bits run out ($> 53$ bits) |
| **Addition Time** | $O(1)$ (1 clock cycle) | $O(1)$ (1 clock cycle on 64-bit CPU) | $O(1)$ (1 to 3 cycles in FPU) |
| **Memory per Variable** | 4 bytes | 8 bytes | 8 bytes |
| **Accuracy** | Truncates on overflow | $100\%$ Bit-Exact across all 3M journeys | Prone to IEEE 754 epsilon rounding errors |
