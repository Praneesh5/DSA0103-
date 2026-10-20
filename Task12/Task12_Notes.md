# TASK 12: Day-4 Design Challenge — Re-evaluate and Adapt the Design
**Bloom's Taxonomy Level:** K5 – Evaluate  
**Total Marks:** 8 Marks  
**Course Outcome:** CO1 (Architectural Evolution, Impact Analysis, Polymorphic Redesign, Evidence-Based Defence)

---

## Changed Requirements Summary
The transit authority introduces three strategic network enhancements:
1. **Fourth Zone (Airport Express):** Fare Rs 60.00 ($6,000$ paise).
2. **Weekend Discount:** $10\%$ off all base fares on Saturdays and Sundays. **Crucial Rule:** Does **not** combine with peak-hour surcharge (peak surcharge is disabled or overridden by weekend discount).
3. **Monthly Pass:** Allows unlimited transit in two chosen zones for a subscription term without per-trip stored-value deduction.

---

## Part A: Impact Analysis (2 Marks)

The impact of the new requirements propagates through the system components:

| Component in System | Impact Classification | Concrete Code Changes Required |
| :--- | :--- | :--- |
| **`enum class Zone`** | **Modified** | Add enumerator `Airport = 3`. Changes underlying range from 3 elements to 4 elements. |
| **`BASE_PAISE` Array** | **Modified** | Extends from `{1000, 2000, 3000}` to `{1000, 2000, 3000, 6000}`. Size expands from 24 bytes to 32 bytes. |
| **`switch (z)` Statements** | **Modified** | If switch statements are retained, must add `case Zone::Airport: base = 6000; break;`. Missing this causes compiler warning `-Wswitch`. |
| **Peak Surcharge Expressions**| **Redesigned** | The peak check can no longer depend solely on `hour`. It must incorporate `DayOfWeek`. Weekend discount ($10\%$) takes precedence, suppressing the peak $+20\%$ surcharge on Saturdays and Sundays. |
| **`SmartCard` Class** | **Redesigned / Polymorphic Subclassing** | Original `SmartCard` only supports stored-value balance deductions. It cannot model a zero-deduction unlimited monthly pass covering two specific zones. Introducing an interface `ITransitToken` with derived `StoredValueCard` and `MonthlyPass` classes cleanly resolves this without violating SRP (Single Responsibility Principle). |

---

## Part B: Evaluation of the Original Design (2 Marks)

### Can the Original Design Absorb the Changes?
1. **The Switch Statement:**
   - *Feasibility:* Can absorb the Airport zone by adding a 4th `case Zone::Airport:`.
   - *Limitation:* As stations and specialized express links expand, hardcoded switch cases violate the Open/Closed Principle (OCP). Direct array lookup `BASE_PARES[static_cast<size_t>(z)]` or an extensible tariff strategy is far cleaner.
2. **The Stored-Value `SmartCard`:**
   - *Fatal Inadequacy for Monthly Passes:* A monthly pass has **no concept of minimum stored balance (Rs 50)** or per-ride fare deduction. If forced into the existing `SmartCard` class via boolean flags (`bool isPass; Zone allowedZ1, allowedZ2;`), the class suffers from **type pollution**, violates encapsulation, and introduces conditional branching into every tap method.
3. **Worked Evaluation Example (Weekend & Peak Conflict):**
   - Suppose a commuter travels to the Airport on Sunday at 08:30 AM (morning peak):
     - If rules combine naively: $\text{Base Rs } 60 + 20\% - 10\% = \text{Rs } 66.00$.
     - **Transit Bylaw Requirement:** Weekend discount does **not** combine with peak surcharge! Peak surcharge is suppressed on weekends:
       $$\text{Correct Fare} = \text{Rs } 60.00 - 10\% = \text{Rs } 54.00\text{ (5400 paise)}$$
     - The original procedural/simple switch design completely lacked calendar/day awareness and would overcharge passengers.

---

## Part C: Adaptation or Redesign (2 Marks)

### Architecture Response: Hybrid Adaptation & Polymorphic Interface
1. **Fare Engine Enhancement (`MetroFareEngineV2`):**
   - Centralizes base fares, surcharge percentages, and weekend discount rules:
   ```cpp
   static long long computeFare(Zone z, int hour, DayOfWeek day) noexcept {
       long long fare = BASE_FARES[static_cast<size_t>(z)];
       if (isWeekend(day)) {
           fare -= (fare * WEEKEND_DISCOUNT_PCT) / 100LL;
       } else if (isPeak(hour)) {
           fare += (fare * PEAK_SURCHARGE_PCT) / 100LL;
       }
       return fare;
   }
   ```
2. **Polymorphic Token Hierarchy:**
   - Interface `ITransitToken` establishes the contract: `canTravel()`, `tap()`, `printStatus()`.
   - Derived `StoredValueCard`: Enforces `balance - fare >= Rs 50`.
   - Derived `MonthlyPass`: Holds `zone1_` and `zone2_`; checks whether destination zone matches subscription; deducts **Rs 0.00**.

---

## Part D: Evidence-Based Defence (2 Marks)

### 1. Empirical Verification & Test Results
- **Scenario 1 (Airport Weekday Peak):** Monday @ 08:00 AM $\to$ Base Rs 60.00 $+ 20\% = \mathbf{Rs\ 72.00}$ (7200 p). Result: **PASS**.
- **Scenario 2 (Airport Weekend Peak Hour):** Saturday @ 08:00 AM $\to$ Base Rs 60.00 $- 10\% = \mathbf{Rs\ 54.00}$ (5400 p). Peak surcharge suppressed. Result: **PASS**.
- **Scenario 3 (Monthly Pass Allowed Zones):** Pass covering [Inner, Airport] taps Airport Express $\to$ Allowed, Deducted Rs 0.00. Result: **PASS**.
- **Scenario 4 (Monthly Pass Out-of-Zone):** Pass covering [Inner, Airport] taps Middle Zone $\to$ Access Denied (out of pass scope). Result: **PASS**.

### 2. Architectural Trade-Off Analysis

| Metric | Monolithic Modification (Fat `SmartCard`) | Polymorphic Interface (`ITransitToken`) |
| :--- | :--- | :--- |
| **Open/Closed Principle (OCP)** | Violated (constantly modifying existing classes) | **Upheld** (new pass types created by subclassing) |
| **Memory per Card Instance** | Wasted memory (regular cards store unused pass zone IDs) | **Minimal** (each class stores only its necessary state) |
| **Gate Turnstile Coupling** | High (must inspect card internals to decide logic) | **Zero** (invokes uniform `token->tap(...)` interface) |
| **Testability & Validation** | Difficult (combinatorial state explosion) | Isolated, modular unit testing |
