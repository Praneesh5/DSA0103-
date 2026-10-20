# TASK 6: Design the Control Structures
**Bloom's Taxonomy Level:** K5 – Evaluate  
**Course Outcome:** CO1 (Control Structures, Decisions, Looping Constructs, Switch Statements)

---

## 1. Evaluation of Decision Constructs

| Control Structure | Optimal Use-Case in Fare System | Technical Evaluation |
| :--- | :--- | :--- |
| **`switch (z)`** | Mapping discrete `enum class Zone` values to base fares. | The compiler optimizes contiguous enum values into an $O(1)$ branch table (jump table), eliminating linear comparison cascades. |
| **`if-else`** | Multi-range conditions such as compound peak-hour intervals: `(hour >= 8 && hour < 10) \|\| ...` | Indispensable for relational ranges and boundary expressions that cannot be represented as single discrete constants in a `switch`. |
| **Ternary Operator `?:`** | Inline, expression-oriented value selection, e.g. `(s < 0) ? -s : s` or UI label rendering. | Yields an expression with a return type, avoiding statement boilerplate. |

---

## 2. Looping Construct Comparison: Processing 3,000,000 Journeys

| Loop Construct | Syntax Form | Evaluation in High-Volume Transit Processing |
| :--- | :--- | :--- |
| **Range-based `for`** | `for (const Trip& t : trips)` | **Most idiomatic and safest in Modern C++.** Zero index out-of-bounds risk, eliminates off-by-one errors, and const-reference avoids copying large structures. |
| **Traditional Index `for`** | `for (size_t i = 0; i < N; ++i)` | Preferred when the numerical transaction index is required for logging or accessing parallel lookup buffers. |
| **`while` loop** | `while (gateStream >> record)` | Optimal for streaming input from hardware turnstile sensors or TCP socket streams until EOF or disconnect. |

---

## 3. Semantics of `break`, `continue`, and `default`

- **`break` in `switch`:** Prevents disastrous fall-through. If `case Zone::Inner:` lacked a `break`, the execution would fall through into `Zone::Middle:`, charging the passenger Rs 20 instead of Rs 10!
- **`continue` in processing loop:** Used to skip invalid journey packets (e.g. malformed turnstile sensor telemetry) without terminating the server loop.
- **`default:` handling:** Catches unknown or corrupted zone IDs. Returns base fare of 0 and sets an error state, preventing uninitialized memory reads.

---

## 4. Boundary Analysis: 10:00 AM and 8:00 PM (20:00)

- **Requirement:** Add journeys at exactly 10 AM and 8 PM (20:00) and verify that peak rules treat them properly.
- **Evaluation:**
  - Standard metro rules define morning peak as 8 AM to 10 AM. Commuters arriving at 10:00:00 are travelling in the off-peak period.
  - Standard evening peak is 5 PM to 8 PM (17:00 to 20:00). Commuters arriving at 20:00:00 are travelling in the off-peak period.
  - Formula: `(hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)`
  - At `hour = 10`, `hour < 10` is **false**. Peak surcharge is **not** applied. Result: OFF-PEAK.
  - At `hour = 20`, `hour < 20` is **false**. Peak surcharge is **not** applied. Result: OFF-PEAK.
  - Both boundary checks pass with 100% mathematical precision.

---

## 5. Alternative Concept & Complexity Analysis: Switch vs. Direct Array Lookup

```cpp
// Array lookup alternative:
constexpr long long FARE_TABLE[] = {1000LL, 2000LL, 3000LL};
long long f = FARE_TABLE[static_cast<size_t>(z)];
```

| Metric | `switch` Statement | Direct Array Lookup `FARE_TABLE[z]` |
| :--- | :--- | :--- |
| **Time Complexity** | $O(1)$ (compiler emits direct jump table) | $O(1)$ (direct offset address dereference) |
| **Space Overhead** | Code section jump table | 24 bytes in static constant memory |
| **Adding a 4th Zone (Airport Rs 60)** | Requires adding `case Zone::Airport: base = 6000; break;`. Compiler `-Wswitch` warns if any case is missed! | Requires appending `6000LL` to array. If enum order doesn't match array order, silent bug! |
| **Safety Verdict** | **Superior compile-time safety.** `-Wswitch` forces developer to handle all enum variants. | Fast, but fragile to reordering or out-of-bound indices. |
