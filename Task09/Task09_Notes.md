# TASK 9: Implement and Test the Fare Logic
**Bloom's Taxonomy Level:** K5 – Evaluate  
**Course Outcome:** CO1 (Testing Methodologies, Table-Driven Verification, Boundary Analysis, Invariant Auditing)

---

## 1. Test Suite Coverage & Verification Matrix

The test framework evaluates all dimensions of the fare engine against strict acceptance criteria:

1. **Normal Scenarios:**
   - Inner, Middle, and Outer zones across standard off-peak hours (Rs 10.00, Rs 20.00, Rs 30.00).
   - Inner, Middle, and Outer zones across peak hours with $+20\%$ surcharge applied (Rs 12.00, Rs 24.00, Rs 36.00).
2. **Boundary Scenarios:**
   - 08:00 AM: Start of morning peak window $\implies$ $+20\%$ surcharge applied.
   - 10:00 AM: End of morning peak window $\implies$ strictly OFF-PEAK (base fare charged).
   - 17:00 (5:00 PM): Start of evening peak window $\implies$ $+20\%$ surcharge applied.
   - 20:00 (8:00 PM): End of evening peak window $\implies$ strictly OFF-PEAK (base fare charged).
   - Exact Rs 50.00 remaining: Balance 6000 paise $-$ Fare 1000 paise $= 5000$ paise $\implies$ **ALLOWED**.
   - 1 Paisa below threshold: Balance 5999 paise $-$ Fare 1000 paise $= 4999$ paise $\implies$ **DENIED**.
3. **Invalid Scenarios:**
   - Unknown/Corrupted Zone ID (e.g. 99): Rejected with negative fare code and zero mutation.
   - Out-of-bounds timestamps: Safely sanitized.
   - Negative top-up / insufficient balance: Transaction refused with zero balance mutation.

---

## 2. Five Hand-Calculated Sequential Journeys on a Single Card

- **Initial Stored Value:** Rs 150.00 ($15,000$ paise)
- **Minimum Retained Balance Threshold:** Rs 50.00 ($5,000$ paise)

| Step | Transit Parameters | Expected Base Fare | Surcharge | Expected Total Fare | Expected Solvency Check | Final Balance |
| :---: | :--- | :---: | :---: | :---: | :---: | :---: |
| **1** | Inner Zone @ 08:00 AM (Peak) | Rs 10.00 (1000 p) | $+20\%$ (200 p) | Rs 12.00 (1200 p) | $15000 - 1200 = 13800 \ge 5000 \implies$ **PASS** | Rs 138.00 (13,800 p) |
| **2** | Middle Zone @ 11:00 AM (Off-Peak) | Rs 20.00 (2000 p) | $0\%$ (0 p) | Rs 20.00 (2000 p) | $13800 - 2000 = 11800 \ge 5000 \implies$ **PASS** | Rs 118.00 (11,800 p) |
| **3** | Outer Zone @ 06:00 PM (18:00 Peak) | Rs 30.00 (3000 p) | $+20\%$ (600 p) | Rs 36.00 (3600 p) | $11800 - 3600 = 8200 \ge 5000 \implies$ **PASS** | Rs 82.00 (8,200 p) |
| **4** | Inner Zone @ 09:00 PM (21:00 Off-Peak) | Rs 10.00 (1000 p) | $0\%$ (0 p) | Rs 10.00 (1000 p) | $8200 - 1000 = 7200 \ge 5000 \implies$ **PASS** | Rs 72.00 (7,200 p) |
| **5** | Outer Zone @ 07:00 PM (19:00 Peak) | Rs 30.00 (3000 p) | $+20\%$ (600 p) | Rs 36.00 (3600 p) | $7200 - 3600 = 3600 < 5000 \implies$ **DENIED** | Rs 72.00 (7,200 p) |

- **Verification:** The automated C++ test driver executes these identical 5 journeys and achieves bit-for-bit, paisa-for-paisa agreement with hand calculations.

---

## 3. Alternative Concept & Complexity Analysis: Table-Driven Testing vs. Ad-Hoc Scripts

| Metric | Ad-Hoc Hand-Written Test Checks | Table-Driven Testing (`std::vector<TestCase>`) |
| :--- | :--- | :--- |
| **Execution Time for $T$ Tests** | $O(T)$ | $O(T)$ |
| **Memory Footprint** | Code bloat proportional to test count | $O(T)$ compact contiguous vector in memory |
| **Maintainability** | Adding a test requires writing new assertions and code blocks | Adding a test requires appending a single row to the data table |
| **Continuous Integration (CI)** | Difficult to export results to external dashboards | Trivial to serialize to CSV, JSON, or JUnit XML |
| **Coverage Rigor** | Prone to missed boundary conditions | Guarantees systematic parameter sweep across all boundaries |
