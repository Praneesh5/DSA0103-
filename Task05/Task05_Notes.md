# TASK 5: Write Correct Expressions with the Right Precedence
**Bloom's Taxonomy Level:** K5 – Evaluate  
**Course Outcome:** CO1 (Operators, Precedence, Associativity, Expressions, Control of Side Effects)

---

## 1. Predict the Output Analysis

### My Predicted Output (Written BEFORE Running):
```text
11 16
9 6 4
6 4
3 1 -3 -1
18
```

### Actual Output (Confirmed upon Compilation & Execution):
```text
11 16
9 6 4
6 4
3 1 -3 -1
18
```

---

## 2. Operator-by-Operator Detailed Breakdown

### Line 1: `std::cout << x + y * 2 << ' ' << (x + y) * 2 << '\n';`
- Initial values: `x = 5`, `y = 3`.
- **Expression 1 (`x + y * 2`):**
  - Operators: Multiplication `*` (Precedence Level 5), Addition `+` (Precedence Level 6).
  - Precedence Rule: `*` binds tighter than `+`.
  - Step 1: `y * 2` evaluates to $3 \times 2 = 6$.
  - Step 2: `x + 6` evaluates to $5 + 6 = 11$.
- **Expression 2 (`(x + y) * 2`):**
  - Operators: Grouping Parentheses `()` (Precedence Level 1), Multiplication `*` (Precedence Level 5).
  - Precedence Rule: Parentheses override natural precedence.
  - Step 1: `(x + y)` evaluates to $5 + 3 = 8$.
  - Step 2: `8 * 2` evaluates to $16$.
- **Printed Output:** `11 16`

### Line 2: `int z = x++ + ++y;` and `std::cout << z << ' ' << x << ' ' << y << '\n';`
- Initial values: `x = 5`, `y = 3`.
- **Operators Involved:** Postfix increment `++` (Level 2, left-to-right), Prefix increment `++` (Level 3, right-to-left), Addition `+` (Level 6), Assignment `=` (Level 16).
- Step 1: `x++` (postfix) evaluates to the current value of `x` ($5$). Its side effect increments `x` to $6$.
- Step 2: `++y` (prefix) increments `y` from $3$ to $4$ immediately and evaluates to the new value ($4$).
- Step 3: Addition `+` adds the sub-expressions: $5 + 4 = 9$.
- Step 4: Value $9$ is assigned to `z`.
- Post-condition: `z = 9`, `x = 6`, `y = 4`.
- **Printed Output:** `9 6 4`

> [!WARNING]
> **Why `i = i++` is a Fatal Mistake in C++:**  
> In C++03, `i = i++` invoked **Undefined Behavior (UB)** because a scalar object was modified more than once between sequence points. In C++11/17, it is classified as an unsequenced modification. Different compilers generate differing assembly (leaving `i` unmodified, incremented, or zeroed). Never mix side-effecting increments with assignment on the same variable.

### Line 3: `std::cout << (x > y ? x : y) << ' ' << x % 4 * 2 << '\n';`
- Current values: `x = 6`, `y = 4`.
- **Expression 1 (`(x > y ? x : y)`):**
  - Relational `>` (Level 9) has higher precedence than Conditional `?:` (Level 16).
  - Step 1: `x > y` $\implies 6 > 4 \implies \mathbf{true}$.
  - Step 2: The conditional operator evaluates its second operand `x`, yielding $6$.
- **Expression 2 (`x % 4 * 2`):**
  - Modulo `%` and Multiplication `*` share identical Precedence Level 5.
  - Associativity Rule: Left-to-right.
  - Step 1: `x % 4` $\implies 6 \% 4 = 2$.
  - Step 2: `2 * 2 = 4`.
- **Printed Output:** `6 4`

### Line 4: `std::cout << 10 / 3 << ' ' << 10 % 3 << ' ' << -10 / 3 << ' ' << -10 % 3 << '\n';`
- Under C++11 and later (ISO/IEC 14882:2011 §5.6/4):
  - Integer division **always truncates toward zero** (algebraic quotient truncated).
  - The remainder operator satisfies the invariant: `(a / b) * b + (a % b) == a`.
  - `10 / 3` $= 3$; `10 % 3` $= 1$ (since $3 \times 3 + 1 = 10$).
  - `-10 / 3` $= -3.333...$ truncated toward zero $\implies \mathbf{-3}$.
  - `-10 % 3` $\implies (-10) - ((-3) \times 3) = -10 - (-9) = \mathbf{-1}$.
- **Printed Output:** `3 1 -3 -1`

### Line 5: Loop accumulator `k`
```cpp
int k = 0;
for (int i = 0; i < 10; i += 3) k += i;
```
- Step 1: $i = 0 < 10 \implies k += 0 \implies k = 0$.
- Step 2: $i += 3 \implies i = 3 < 10 \implies k += 3 \implies k = 3$.
- Step 3: $i += 3 \implies i = 6 < 10 \implies k += 6 \implies k = 9$.
- Step 4: $i += 3 \implies i = 9 < 10 \implies k += 9 \implies k = 18$.
- Step 5: $i += 3 \implies i = 12 \ge 10 \implies$ loop terminates.
- **Printed Output:** `18`

---

## 3. Analysis of Peak-Hour Logical Expressions

### Parenthesized Expression:
```cpp
(hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)
```

### Unparenthesized Expression:
```cpp
hour >= 8 && hour < 10 || hour >= 17 && hour < 20
```

### Comparative Evaluation:
1. **Precedence Ranking:** Relational operators (`>=`, `<`) hold rank 9, Logical AND (`&&`) holds rank 14, and Logical OR (`||`) holds rank 15.
2. **Equivalence:** Because `&&` binds tighter than `||`, the C++ compiler parses both expressions identically.
3. **Cognitive Ergonomics & Safety:** The unparenthesized expression invites human error during code maintenance, code reviews, and audits. Explicit parentheses eliminate cognitive overhead, guarantee unambiguous execution order, and prevent accidental precedence misinterpretations.
4. **Boundary Verification:**
   - 08:00 ($h = 8$): $(8 \ge 8 \land 8 < 10) = \text{true}$ $\implies$ **PEAK**.
   - 10:00 ($h = 10$): $(10 \ge 8 \land 10 < 10) = \text{false}$, $(10 \ge 17) = \text{false}$ $\implies$ **OFF-PEAK**.
   - 17:00 ($h = 17$): $(17 \ge 17 \land 17 < 20) = \text{true}$ $\implies$ **PEAK**.
   - 20:00 ($h = 20$): $(20 \ge 17 \land 20 < 20) = \text{false}$ $\implies$ **OFF-PEAK**.

---

## 4. Alternative Concept & Complexity Analysis: Boolean Expression vs. Lookup Table

| Evaluation Dimension | Boolean Expression `(h >= 8 && h < 10) \|\| ...` | Lookup Table `bool peak[24]` |
| :--- | :--- | :--- |
| **Time Complexity** | $O(1)$ (2 to 4 comparison instructions with branch prediction) | $O(1)$ (Single memory lookup instruction `table[h]`) |
| **Space Complexity** | $O(1)$ ($0$ bytes data memory) | $O(1)$ ($24$ bytes static `constexpr` array memory) |
| **Branch Penalty** | Potential branch misprediction on erratic timestamps | **Branchless:** Uniform cycle latency regardless of hour |
| **Rule Flexibility** | Modifying peak hours requires altering boolean logic | Modifying peak hours only requires changing array initialization mask |
| **Recommendation** | Standard embedded/microcontroller use | High-throughput gate controller processing 3M journeys/day |
