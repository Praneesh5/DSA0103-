# TASK 11: Recommend and Defend the Final Design
**Bloom's Taxonomy Level:** K5 – Evaluate  
**Course Outcome:** CO1 (System Architecture, Production Defense, Scalability, Fare Revision Lifecycle)

---

## 1. Defended Architectural Recommendations

Based on empirical experiments across 3,000,000 journeys and rigorous type safety evaluations:

1. **Monetary Representation:**
   - **Recommendation:** Integer paise stored in 64-bit `long long` (or `int64_t`).
   - **Defense:** Completely eliminates IEEE 754 floating-point drift, conforms to international banking precision mandates, and prevents 32-bit signed overflow when processing whole-network daily revenues ($6 \times 10^9\text{ paise} > \text{INT\_MAX}$).
2. **Enumerations & Constants:**
   - **Recommendation:** Scoped `enum class Zone : unsigned char` combined with `constexpr` rate configurations.
   - **Defense:** Scoped enums eliminate namespace pollution, prevent unchecked int conversions, and compress zone memory to 1 byte. `constexpr` guarantees compile-time constant evaluation and allows `static_assert` invariant checking.
3. **Encapsulation & Object Model:**
   - **Recommendation:** `SmartCard` class encapsulating `balancePaise_` as strictly `private`, exposing only transactional methods (`canTravel()`, `deduct()`, `recharge()`).
   - **Defense:** Mathematically prevents cards from reaching negative balances or dropping below the mandatory Rs 50.00 solvency margin.
4. **Boundary Evaluation:**
   - **Recommendation:** Explicitly parenthesized logical range checks:
     ```cpp
     (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)
     ```
   - **Defense:** Accurately classifies 10:00:00 AM and 20:00:00 (8 PM) as OFF-PEAK, eliminating gate dispute claims.

---

## 2. Operational Fare Revision Workflow

When the transit authority revises fare tariffs:
1. In the static `constexpr` model, revisions are centralized in `FarePolicy::BASE_FARES`.
2. Automated regression tests verify all invariants within milliseconds.
3. Firmware binaries are signed and distributed to turnstiles via Over-The-Air (OTA) gateway update protocols.

---

## 3. Alternative Concept & Complexity Analysis: Compiled Constants vs. Runtime Config File

| Evaluation Dimension | Static Compiled `constexpr` Architecture | Dynamic Runtime Config File (JSON / INI) |
| :--- | :--- | :--- |
| **Turnstile Boot Latency** | **$0$ ms:** Constants embedded directly in CPU instructions | **$50 - 250$ ms:** Requires disk I/O, file parsing, and schema validation |
| **Memory Footprint** | $0$ heap allocation; inlined assembly constants | Dynamic buffer allocation for JSON/INI DOM tree |
| **Tamper & Fraud Risk** | **Zero:** Embedded in signed binary checksum | **High:** Local config file on disk can be altered by rogue field technicians |
| **Deployment Agility** | Requires recompilation and firmware flashing | Trivial push of a text file without binary rebuild |
| **Recommendation** | **Adopt Static `constexpr` for turnstiles** for maximum security, speed, and determinism. |
