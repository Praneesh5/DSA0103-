# TASK 8: Read and Print Data Safely
**Bloom's Taxonomy Level:** K5 – Evaluate  
**Course Outcome:** CO1 (Stream I/O, Error States, Input Sanitization, <iomanip> Formatting)

---

## 1. Stream State Management & Error Recovery with `std::cin`

When standard input reads into numerical variables using `std::cin >> val`:
1. If the operator types non-numeric characters (e.g. `ABC` instead of `10`), the extraction operator fails.
2. `std::cin` enters a failed state (`cin.fail() == true`), setting the `failbit`.
3. While `failbit` is set, **all subsequent stream reads are silently ignored**, and `val` remains uninitialized or retains its previous value. Left unhandled, this induces an infinite loop if wrapped in an unsanitized `while (!cin.eof())` loop!

### The Two-Step Recovery Protocol:
```cpp
if (std::cin.fail()) {
    std::cin.clear(); // Step 1: Clears failbit and resets stream state to goodbit
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Step 2: Flushes all offending characters until newline
}
```

---

## 2. Formatting Currency and Reports with `<iomanip>`

Financial displays require exact alignment and fixed two-decimal-place padding. Metro fare reports utilize:
- `std::setw(N)`: Enforces exact column widths for tabular integrity.
- `std::setfill('0')`: Prepends leading zeroes for two-digit paise representations (e.g., 5 paise rendered as `05` rather than `5`).
- `std::left` / `std::right`: Governs column justification (text left-aligned, monetary numerals right-aligned).

```cpp
std::cout << "Rs " << fare / 100 << '.' 
          << std::setw(2) << std::setfill('0') << fare % 100 << std::setfill(' ');
```

---

## 3. Alternative Concept & Complexity Analysis: Direct `cin >>` vs. `getline()` + `stoll()`

```cpp
// Alternative Concept:
std::string line;
if (std::getline(std::cin, line)) {
    try {
        long long val = std::stoll(line);
    } catch (const std::exception& e) {
        // Recover gracefully
    }
}
```

| Evaluation Parameter | Direct Stream Extraction `cin >> val` | Line Extraction `getline()` + `stoll()` (Alternative) |
| :--- | :--- | :--- |
| **Time Complexity** | $O(L)$ where $L$ is the number of characters in the number | $O(L)$ where $L$ is the line length |
| **Buffer Space Needed** | $O(1)$ internal stream buffer | $O(L)$ dynamic heap allocation for `std::string` buffer |
| **Crash / Exception Safety** | No exceptions thrown; signals error through bitmask flags | Throws `std::invalid_argument` or `std::out_of_range`; requires `try-catch` |
| **Partial Line Corruption Recovery** | High risk: leaves remaining characters on line in buffer unless explicitly flushed | **Superior:** Entire line is consumed and cleared regardless of whether parsing succeeds |
| **Recommendation for Production Turnstiles** | Embedded turnstiles with bounded memory | High-level server gateways parsing text logs and REST payloads |
