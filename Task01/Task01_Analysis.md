# TASK 1: Understand the Problem and Identify the Objects
**Bloom's Taxonomy Level:** K5 – Evaluate  
**Course Outcome:** CO1 (Object-Oriented Paradigm, Principles, Types, and Control Structures)

---

## 1. Executive Summary & Problem Overview
The Namma Metro Smart-Card Fare System manages automatic fare calculation, card balance validation, passenger concession handling, and peak-hour surcharges across a multi-zone urban rapid transit network. The legacy gate software suffered from critical vulnerabilities:
1. **Precision Drift:** Fractional rupee representation using floating-point types produced off-by-a-paisa discrepancies.
2. **Boundary Inconsistencies:** Peak-hour boundary evaluation accepted 10:00 AM as peak and 8:00 PM (20:00) as off-peak inconsistently.
3. **Invariant Violation:** Insufficient encapsulation permitted cards to reach negative balances.
4. **Code Duplication:** Fare constants were duplicated across multiple translation units, hindering maintainability.
5. **Scale Challenges:** System volume scaling from a single station (20,000 journeys/day) to a single line (500,000 journeys/day) and across the entire network (3,000,000 journeys/day) exposed integer overflow risks in financial aggregation.

This document presents a rigorous Object-Oriented (OO) analysis, structural decomposition, domain rule formalization, and complexity comparison against a procedural baseline.

---

## 2. Object Identification: Domain Modeling

```
+------------------------------------------------------------------------+
|                          DOMAIN MODEL ARCHITECTURE                     |
+------------------------------------------------------------------------+
|                                                                        |
|  +--------------------+        1      1..* +---------------------+     |
|  |     Passenger      |<>----------------->|      SmartCard      |     |
|  +--------------------+                    +---------------------+     |
|  | - passengerId      |                    | - cardId            |     |
|  | - category         |                    | - balancePaise      |     |
|  | - name             |                    | - totalJourneys     |     |
|  +--------------------+                    +---------------------+     |
|            |                               | + canTravel()       |     |
|            | 1                             | + deduct()          |     |
|            |                               | + recharge()        |     |
|            v                               +---------------------+     |
|  +--------------------+                               | 1              |
|  |      Journey       |                               | undertakes     |
|  +--------------------+                               v                |
|  | - journeyId        |                    +---------------------+     |
|  | - entryStation     |                    |      FareTable      |     |
|  | - exitStation      |                    +---------------------+     |
|  | - entryTime        |                    | + getBaseFare()     |     |
|  | - farthestZone     |                    | + isPeak()          |     |
|  | - calculatedFare   |                    | + computeFare()     |     |
|  +--------------------+                    +---------------------+     |
|            |                                                           |
|            v 1..*                                                      |
|  +--------------------+                                                |
|  |       Zone         |                                                |
|  | (Inner/Middle/...) |                                                |
|  +--------------------+                                                |
+------------------------------------------------------------------------+
```

### 2.1 The Core Objects

#### A. SmartCard
- **Role:** Stored-value token held by the commuter; acts as the primary transactional entity enforcing financial integrity.
- **Data (Attributes):**
  - `cardId` (`std::string` or `unsigned long long`): Unique 64-bit identifier.
  - `balancePaise` (`long long`): Current stored value in exact integer paise (1 Rupee = 100 paise). Kept strictly `private`.
  - `cardType` (`enum class CardCategory`): Regular, Student, SeniorCitizen, or PassHolder.
  - `tripCount` (`unsigned long long`): Monotonically increasing counter of completed journeys.
- **Behavior (Methods):**
  - `canTravel(long long farePaise) const -> bool`: Validates whether $(balancePaise - farePaise) \ge MIN\_BALANCE\_PAISE$ (5000 paise / Rs 50).
  - `deduct(long long farePaise) -> bool`: Atomically debits `farePaise` if `canTravel()` holds; otherwise rejects the transaction with zero mutation.
  - `recharge(long long amountPaise) -> bool`: Credits the card with positive paise, validating against numeric overflow.
  - `getBalancePaise() const -> long long`: Read-only accessor for display purposes.

#### B. Passenger
- **Role:** The human patron interacting with the metro turnstile.
- **Data (Attributes):**
  - `passengerId` (`std::string`): Identification or KYC token.
  - `concessionCategory` (`ConcessionType`): General (0% concession), Student (e.g., 25% discount), Senior Citizen (e.g., 30% discount).
  - `associatedCardId` (`unsigned long long`): Pointer/reference to the commuter's card.
- **Behavior (Methods):**
  - `getConcessionPercent() const -> int`: Yields concession percentage applicable to the passenger.
  - `validateEligibility() -> bool`: Verifies government ID or student status validity.

#### C. Journey
- **Role:** Represents a single point-to-point transit transaction from tap-in to tap-out.
- **Data (Attributes):**
  - `journeyId` (`unsigned long long`): Global sequential transaction index.
  - `entryStation` / `exitStation` (`std::string`): Origin and destination transit nodes.
  - `entryHour` / `entryMinute` (`int`): Timestamp used to evaluate peak-hour surcharge rules.
  - `farthestZone` (`Zone`): Maximum geographic perimeter traversed during transit.
  - `computedFarePaise` (`long long`): Final calculated toll.
  - `status` (`JourneyStatus`): `InProgress`, `Completed`, `DeniedLowBalance`, `DeniedInvalidZone`.
- **Behavior (Methods):**
  - `isPeak() const -> bool`: Evaluates time window against metro peak hours.
  - `getZoneSpan() const -> Zone`: Calculates transit range.

#### D. Zone
- **Role:** Spatial categorization of metro rail geography.
- **Data (Attributes):**
  - `Inner` (Zone 1): Downtown / Core urban sector.
  - `Middle` (Zone 2): Inner suburbs / Radial corridor.
  - `Outer` (Zone 3): Peripheral industrial / Residential limits.
  - `Airport` (Zone 4 - Day-4 Challenge): Dedicated airport express link.
- **Behavior:**
  - Ranked ordinal hierarchy allowing distance comparisons and fare-tier lookups.

#### E. FareTable (Fare Calculation Engine)
- **Role:** Centralized repository of fare rules, surcharge rates, and discount policies.
- **Data (Attributes):**
  - `BASE_PAISE[Zone]`: Immutable array `{1000, 2000, 3000}` (in paise).
  - `PEAK_SURCHARGE_PERCENT`: Immutable rate (`20%`).
  - `MIN_BALANCE_PAISE`: Safety threshold (`5000` paise = Rs 50.00).
- **Behavior (Methods):**
  - `calculateBaseFare(Zone z) -> long long`: Resolves base rate.
  - `applySurcharge(long long baseFare, bool isPeak) -> long long`: Adds 20% peak premium using purely integer arithmetic.
  - `applyConcession(long long fare, ConcessionType c) -> long long`: Computes discounted fare.

---

## 3. Procedural vs. Object-Oriented Paradigm Comparison

| Architectural Aspect | Procedural Paradigm (C Style) | Object-Oriented Paradigm (C++ Style) | Evaluation / Impact |
| :--- | :--- | :--- | :--- |
| **State Storage** | Global variables or raw parallel arrays (`int card_ids[N]`, `long long balances[N]`). | Encapsulated within `SmartCard` instances with private member variables. | Procedural allows arbitrary external mutation; OOP guarantees class invariants. |
| **Function Coupling** | Free functions take raw integers: `deduct(int id, long long amt)`. | Member methods operate on `this`: `card.deduct(fare)`. | OOP bundles state with behavior, eliminating pointer errors and loose arguments. |
| **Data Invariant Protection** | Zero protection. Any part of the codebase can write `balance = -500`. | Strictly enforced in `deduct()`: impossible to lower balance below Rs 50 threshold. | Prevents fraud, balance corruption, and revenue leakage. |
| **Extension & Change** | Adding a new zone or pass requires altering every free function and `switch` block. | Polymorphic fare policies or subclassing `SmartCard` / `Pass` without breaking existing code. | OCP (Open/Closed Principle) satisfied in OOP. |

---

## 4. Fundamental OOP Principles Applied to the Metro Fare System

### 4.1 Encapsulation
- **Implementation:** `SmartCard::balancePaise` is declared `private`.
- **Justification:** External code (gate turnstiles, logging systems, report generators) must never directly manipulate financial balances. Mutators (`recharge`, `deduct`) act as transactional gates enforcing safety checks before altering state.

### 4.2 Abstraction
- **Implementation:** The gate turnstile invokes `gate.processTap(card, journey)`.
- **Justification:** The gate does not need to know whether the fare calculation uses a switch statement, jump table, or matrix lookup, nor does it care about IEEE 754 precision issues. It interacts solely through an abstract, high-level interface.

### 4.3 Inheritance
- **Implementation:** Base `Passenger` class inherited by `StudentPassenger`, `SeniorPassenger`, and `RegularPassenger`.
- **Justification:** Avoids conditional branching (`if (type == STUDENT) ... else if ...`) scattered throughout business logic by modeling distinct passenger categories through clean class hierarchies.

### 4.4 Polymorphism
- **Implementation:** Virtual method `virtual long long computeDiscount(long long baseFare) const = 0` overridden in derived passenger classes.
- **Justification:** Allows the fare engine to process heterogeneous streams of passengers dynamically at runtime without modifying the fare engine code.

---

## 5. Domain Rules for Every Valid Journey

1. **Rule 1 (Post-Journey Solvency Invariant):**
   $$\text{balance}_{\text{post}} = \text{balance}_{\text{pre}} - \text{fare} \ge 5000\text{ paise (Rs 50.00)}$$
   *Enforcement:* If $\text{balance}_{\text{pre}} - \text{fare} < 5000$, turnstile denies entry immediately; $\text{balance}_{\text{post}} \equiv \text{balance}_{\text{pre}}$.
2. **Rule 2 (Farthest Zone Pricing):**
   Base fare is determined strictly by the farthest zone boundary crossed during the trip:
   - Inner: Rs 10.00 (1000 paise)
   - Middle: Rs 20.00 (2000 paise)
   - Outer: Rs 30.00 (3000 paise)
   - Airport (Challenge): Rs 60.00 (6000 paise)
3. **Rule 3 (Peak-Hour Surcharge Logic):**
   - Peak Morning Window: $[08:00, 10:00)$ $\implies$ $8 \le \text{hour} < 10$. (08:00 to 09:59 is peak; 10:00:00 is off-peak).
   - Peak Evening Window: $[17:00, 20:00)$ $\implies$ $17 \le \text{hour} < 20$. (17:00 to 19:59 is peak; 20:00:00 is off-peak).
   - Surcharge Rate: $+20\%$ added to base fare using integer arithmetic:
     $$\text{fare}_{\text{peak}} = \text{fare}_{\text{base}} + \frac{\text{fare}_{\text{base}} \times 20}{100}$$
4. **Rule 4 (Concession Application):**
   Concession discounts apply to passenger categories as authorized by transit authority bylaws.
5. **Rule 5 (Non-Negativity & Monotonicity):**
   A card balance can never decrease unless a validated transit deduction occurs, and cannot increase unless a verified recharge transaction occurs.

---

## 6. Alternative Concept & Complexity Analysis: Procedural vs. OOP

### 6.1 Procedural Baseline Design
In a purely procedural system, card data and journey records exist as global C structs or arrays:
```cpp
// Procedural representation
int g_card_count = 0;
long long g_card_balance[MAX_CARDS];
int g_card_id[MAX_CARDS];

bool procedural_deduct(int card_idx, long long fare) {
    if (g_card_balance[card_idx] - fare >= 5000) {
        g_card_balance[card_idx] -= fare;
        return true;
    }
    return false;
}
```

### 6.2 Complexity Comparison Matrix

| Metric | Procedural Architecture | Object-Oriented Architecture | Theoretical & Measured Proof |
| :--- | :--- | :--- | :--- |
| **Time per Journey Deduction** | $O(1)$ | $O(1)$ | Direct indexed access or reference dereference; single arithmetic comparison. |
| **Memory per Card Instance** | $O(1)$ (8 bytes `long long`) | $O(1)$ (8 bytes `long long` + 8 bytes ID) | Zero vtable overhead for non-virtual `SmartCard` class; identical primitive footprint. |
| **System Modification Cost** (e.g., minimum balance changes from Rs 50 to Rs 60) | **$O(M)$ lines modified** ($M$ = total lines of code across entire codebase where balance logic is copied) | **$O(1)$ line modified** (single constant `SmartCard::MIN_BALANCE_PAISE` altered) | OOP localizes maintenance to a single unit; procedural requires global refactoring. |
| **Bug Vulnerability Index** | High (accidental mutation possible anywhere) | Zero (enforced private scope + compiler checking) | Compiler rejects `card.balancePaise -= fare`. |

### 6.3 Conclusion
Both paradigms achieve equivalent runtime efficiency ($O(1)$ time and memory), but the Object-Oriented paradigm radically diminishes software lifecycle maintenance cost ($O(1)$ vs $O(M)$) and mathematically prevents unauthorized state corruption.
