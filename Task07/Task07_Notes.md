# TASK 7: From Procedural to Object-Oriented
**Bloom's Taxonomy Level:** K5 – Evaluate  
**Course Outcome:** CO1 (Procedural vs OOP Paradigms, Encapsulation, Class Invariants, Refactoring Resilience)

---

## 1. Architectural Comparison: Procedural vs. Object-Oriented

```
+--------------------------------------------------------------------------+
|                       PARADIGM VULNERABILITY COMPARISON                  |
+--------------------------------------------------------------------------+
|  PROCEDURAL DESIGN:                                                      |
|    [Gate Code] ------+                                                   |
|    [Audit Code] -----+---> struct RawCard { long long balance; }         |
|    [Rogue Module] ---+        ^ (Direct external read/write access)      |
|                               |                                          |
|    * Any component can execute: card.balance = -1000;                    |
|    * Zero encapsulation; balance invariant is completely bypassable.     |
+--------------------------------------------------------------------------+
|  OBJECT-ORIENTED DESIGN:                                                 |
|    [Gate Code] ---> SmartCard::deduct(fare)                              |
|    [Audit Code] --> SmartCard::getBalancePaise() [Read-Only]             |
|    [Rogue Code] -X- balancePaise_ [PRIVATE - COMPILE ERROR]              |
|                                                                          |
|    * Balance invariant (balance >= Rs 50) is mathematically enforced.    |
|    * State corruption is rejected at COMPILE TIME.                       |
+--------------------------------------------------------------------------+
```

---

## 2. Invariant Protection: How the Class Prevents Negative Balances

In the legacy procedural codebase, cards occasionally exhibited negative balances because:
1. Turnstiles subtracted fares before verifying whether balance was sufficient.
2. Concurrent or duplicate taps resulted in uncoordinated subtractions.
3. Raw data structures allowed external code to overwrite the balance directly.

In the Object-Oriented `SmartCard` class:
- `balancePaise_` is declared `private`. No outside function or operator can read or alter it except through public member methods.
- The mutator `deduct(long long farePaise)` incorporates a pre-condition guard:
  ```cpp
  bool deduct(long long farePaise) noexcept {
      if (canTravel(farePaise)) {
          balancePaise_ -= farePaise;
          return true;
      }
      return false;
  }
  ```
- Because `canTravel()` requires `(balancePaise_ - farePaise) >= 5000LL`, the balance can **never** drop below 5000 paise (Rs 50.00), making a negative balance mathematically impossible.

---

## 3. Maintenance Analysis: Changing the Minimum Balance (e.g., Rs 50 $\to$ Rs 60)

### Procedural Approach:
- The minimum balance literal (`5000`) or global constant is dispersed across multiple free functions (`proc_can_travel`, gate controllers, passenger top-up validators, reporting scripts).
- If a developer misses updating one location, the system develops split-brain behavior (e.g. gate allows entry, but exit turnstile traps the commuter).
- Cost of Change: **$O(M)$ lines altered across multiple files**, with high regression risk.

### Object-Oriented Approach:
- The threshold is centralized within `SmartCard`:
  ```cpp
  static constexpr long long MIN_BALANCE_PAISE = 6000LL; // Single point of truth!
  ```
- Because all gate controllers delegate validation to `card.canTravel(fare)` or `card.deduct(fare)`, changing this single line automatically propagates across every station, line, and network turnstile.
- Cost of Change: **$O(1)$ line altered in 1 file**, with zero regression risk.

---

## 4. Alternative Concept & Complexity Analysis: Public vs. Private Balance Member

| Analytical Dimension | Public Data Member (`public: long long balance;`) | Private Member with `deduct()` & `canTravel()` |
| :--- | :--- | :--- |
| **Execution Time** | $O(1)$ | $O(1)$ (Inlined method call has zero CPU overhead) |
| **Memory Footprint** | 8 bytes per card | 8 bytes per card (Identical data layout in memory) |
| **Rule 1 Violation Risk** | **Severe:** Any programmer or library can execute `card.balance -= 5000;` without checking if the remaining balance is $\ge$ Rs 50. | **Zero:** Enforced by class method. |
| **Rule 5 Violation Risk** | **Severe:** Accidental assignment of negative numbers causes card balance to become negative. | **Zero:** The compiler refuses compilation of `card.balancePaise_ = -100`. |
| **Conclusion** | Unacceptable for financial and ticketing software. | Industry gold standard for mission-critical software. |
