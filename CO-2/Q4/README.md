# Task 04 (Q4) – Specifying a Class with an Array Inside It

## Problem Context
A banking system requires that each `SavingsAccount` maintains a rolling history of its last 10 transactions for ATM mini-statements, with older transactions automatically discarded once the capacity is reached.

## Source Files
- `Q4_savings_account.cpp`: Complete class specification containing private members, member functions defined inside the class (`deposit`), and member functions defined outside using scope resolution `::` (`withdraw`, `miniStatement`, `recordTransaction`).
- `Q4_comparison_benchmark.cpp`: High-resolution empirical benchmark comparing the Shifting Array ($O(K)$), Circular Ring Buffer ($O(1)$), and `std::deque` across 100,000 transactions for capacities $K=10$ and $K=50$.
- `output_account.txt`: Program trace showing account transactions and formatted ATM mini-statement.
- `output_benchmark.txt`: Empirical timing benchmark results.

---

## 1. Class Specification & Structure

```cpp
class SavingsAccount {
private:
    static const int CAPACITY = 10;
    long long accountNumber;
    double balance;
    double transactions[CAPACITY]; // Fixed array inside the class
    int txCount;                   // Total count of transactions

    void recordTransaction(double amount); // Private helper defined outside

public:
    SavingsAccount(long long accNo, double initialBalance);

    // Defined INSIDE the class (implicitly inline)
    bool deposit(double amount);

    // Defined OUTSIDE the class via ::
    bool withdraw(double amount);
    void miniStatement() const;
    double getBalance() const;
    long long getAccountNumber() const;
};
```

---

## 2. Inquiry: Encapsulation & 11th Transaction Handling

### Why the Transaction Array and Balance Must be Private:
1. **Data Integrity & Encapsulation Invariant**: The balance and transaction array form a tightly coupled financial invariant:
   $$\text{Current Balance} = \text{Initial Balance} + \sum \text{All Valid Transactions}$$
   If `balance` or `transactions[]` were public, external code could bypass validation (e.g. deposit negative amounts, overdraw without sufficient funds, or alter audit records).
2. **Implementation Hiding**: Keeping the storage private allows the internal container (fixed array, circular buffer, deque) to change without breaking external client code.

### What Happens When the 11th Transaction Arrives?
- **Shifting Fixed Array**:
  ```cpp
  for (int i = 0; i < CAPACITY - 1; ++i) {
      transactions[i] = transactions[i + 1]; // Shift 9 elements left
  }
  transactions[CAPACITY - 1] = newAmount;    // Place 11th at index 9
  ```
  - **Mechanics**: Elements $1 \dots 9$ are shifted left by one index, effectively overwriting index $0$ (the oldest transaction). The newest transaction is inserted at the last slot.
  - **Time Cost**: Requires copying $K - 1$ elements on every single transaction once full: **$O(K)$ time per insertion**.
- **Circular Index Buffer**:
  ```cpp
  transactions[head] = newAmount;
  head = (head + 1) % CAPACITY;
  ```
  - **Mechanics**: Overwrites the oldest element directly at position `head` and advances `head` circularly using modulo arithmetic.
  - **Time Cost**: Exactly 1 array write and 1 modulo operation: **$O(1)$ time per insertion**.

---

## 3. Design Decision: Expanding to 50 Transactions

### The Problem:
The bank extends the requirement from 10 to 50 transactions.
- In the **Shifting Array design**: Every subsequent transaction now requires copying 49 elements ($O(K)$). For a high-throughput banking system with millions of transactions, this imposes severe memory-bus overhead.
- In the **Circular Buffer design**: Changing `CAPACITY` from 10 to 50 has **zero algorithmic impact** on insertion latency. It remains purely $O(1)$.

### Empirical Benchmark Evidence (100,000 Transactions):
| Buffer Architecture | Time at $K=10$ | Time at $K=50$ | Scaling Behavior |
|---------------------|----------------|----------------|------------------|
| **Shifting Array** | $531\ \mu\text{s}$ | $3,440\ \mu\text{s}$ | **$6.5\times$ slowdown** (Linear $O(K)$ degradation) |
| **Circular Ring Buffer** | $695\ \mu\text{s}$ | $750\ \mu\text{s}$ | **Virtually Constant** ($O(1)$ invariant) |
| **`std::deque`** | $727\ \mu\text{s}$ | $666\ \mu\text{s}$ | $O(1)$ amortized (Dynamic allocation overhead) |

### Defense:
The **Circular Ring Buffer** changes least and scales best:
1. It maintains a strictly fixed, compile-time memory footprint inside the class (no heap fragmentation).
2. Guarantees true worst-case $O(1)$ insertion performance regardless of whether $K=10$, $K=50$, or $K=10,000$.
3. When printing the mini-statement, elements are simply read starting from `(head - count + K) % K` up to `head`, which takes $O(K)$ read-only traversal only when an ATM printout is requested.
