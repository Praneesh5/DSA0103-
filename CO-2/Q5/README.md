# Task 05 (Q5) – Static Members and Arrays of Objects

## Problem Context
A college bus reservation system manages seat allocations up to a fixed bus capacity. Every `BusTicket` object must obtain the next sequential seat number, any tickets requested beyond capacity must automatically transition to a waiting list, and the transport office must be able to inspect the total tickets issued without requiring an instance of a ticket.

## Source Files
- `Q5_predict.cpp`: Verification code for static counter initialization, object array constructors, and static member functions.
- `Q5_bus_ticket.cpp`: Completed code implementing `TODO 1` (seat allocation and waiting list logic) and `TODO 2` (static issued count query).
- `Q5_ticket_office.cpp`: Investigation of static member persistence across function calls and an alternative `BusRoute` / `TicketOffice` architectural model supporting multiple buses concurrently.
- `output_predict.txt`, `output_bus.txt`, `output_office.txt`: Complete program outputs.

---

## 1. Predict the Output Analysis (`Q5_predict.cpp`)

### Program Code:
```cpp
#include <iostream>

class Token {
    static int next;
    int id;

public:
    Token() { id = ++next; }
    int get() const { return id; }
    static int issued() { return next - 100; }
};

int Token::next = 100;

int main()
{
    Token counter[3];
    Token extra;
    std::cout << counter[0].get() << ' ' << counter[2].get() << ' ' << extra.get()
              << ' ' << Token::issued() << '\n';
}
```

### Prediction vs Actual Output:
- **Predicted**: `101 103 104 4`
- **Actual**: `101 103 104 4`
- **Match Status**: Exact match.

### Detailed Explanation:
1. **Static Data Member Initialization**:
   `int Token::next = 100;` allocates storage for `next` in the data segment (BSS/data segment) before `main()` executes. It exists once for the entire class.
2. **Array Construction (`Token counter[3]`)**:
   An array of objects invokes the default constructor sequentially for each element:
   - `counter[0]`: `++next` increments `next` from 100 to 101; `id = 101`.
   - `counter[1]`: `++next` increments `next` from 101 to 102; `id = 102`.
   - `counter[2]`: `++next` increments `next` from 102 to 103; `id = 103`.
3. **Single Object Construction (`Token extra`)**:
   - `extra`: `++next` increments `next` from 103 to 104; `id = 104`.
4. **Constructor Invocation Count**:
   The constructor ran exactly **4 times** (3 for the array elements, 1 for `extra`).
5. **Static Member Function (`Token::issued()`)**:
   `Token::issued()` evaluates `next - 100` $\rightarrow 104 - 100 = 4$.

---

## 2. Complete the Code Analysis (`Q5_bus_ticket.cpp`)

```cpp
explicit BusTicket(const std::string& n) : seat(0), name(n)
{
    // TODO 1: If fewer than CAPACITY tickets have been issued, increase issued and
    // give this ticket the next seat number; otherwise leave seat as 0 (waiting list)
    if (issued < CAPACITY) {
        seat = ++issued;
    } else {
        seat = 0; // Waiting list
    }
}

static int count()
{
    // TODO 2: Return the number of seats issued so far
    return issued;
}
```

### Observed Output:
```text
Asha: Seat 1
Bala: Seat 2
Chitra: Seat 3
Dev: Waiting
Issued: 3
```

---

## 3. Inquiry: Static Members, Storage & Invocation Inside Loops/Functions

1. **Where a Static Data Member Must be Defined**:
   - Inside the class declaration, `static int issued;` only **declares** the member.
   - It **must be defined outside the class in a single translation unit** (e.g. `int BusTicket::issued = 0;`) to allocate actual storage in the program's static data segment. Omitting this causes an unresolved external symbol linker error (`undefined reference to BusTicket::issued`).
2. **Why Static Member Functions Cannot Use `this` or Non-Static Members**:
   - In C++, non-static member functions are passed a hidden pointer: the `this` pointer pointing to the invoking instance (`*this`).
   - A static member function belongs to the class as a whole, not to an instance. It can be called even when zero objects exist (`BusTicket::count()`).
   - Consequently, **no `this` pointer is passed in the calling convention**. Because there is no instance context, attempting to access `this`, `seat`, or `name` inside a static member function produces a compile error: `error: invalid use of member '...' in static member function`.
3. **Behavior When Created Inside a Scoped Function or Loop**:
   - When objects are created inside a function called multiple times (e.g. `bookMorningShift()`):
     - The local `BusTicket` instances are constructed on the stack and destroyed when the function scope exits.
     - However, the `static int issued` variable **persists in the data segment across function boundaries**.
     - In our experiment (`Q5_ticket_office.cpp`), the second invocation remembered the previous count of 2, allocating Seat 3 to the next passenger and pushing subsequent passengers into the Waiting List.

---

## 4. Alternative Concept and Complexity Analysis: Static vs Global vs `TicketOffice` Object

### The Architectural Limitation of Class-Level Static Members:
While `static` members satisfy single-bus tracking, they introduce a fundamental design flaw: **they bind to the type, not an operational unit**. If the college runs 10 buses simultaneously:
- All 10 buses would share the single static counter `issued` and `CAPACITY = 3`.
- Reserving a seat on Bus 101 would falsely increment the counter for Bus 202.

### Comparison Table:
| Architecture | Time Complexity | Space Complexity | Encapsulation | Multi-Bus Scalability |
|--------------|-----------------|------------------|---------------|-----------------------|
| **Class Static Member** | $O(1)$ | 4 bytes (Data Segment) | Medium (Tightly coupled to class) | **Fails** (Single global bus pool only) |
| **Global Variable** | $O(1)$ | 4 bytes (Data Segment) | **Poor** (Accessible/mutable by any function) | **Fails** (Name clashes, no scoping) |
| **`BusRoute` / `TicketOffice` Object** | $O(1)$ | 8 bytes per route instance | **Excellent** (Pure OOP encapsulation) | **Ideal** (Unlimited independent buses) |

### Conclusion:
The `BusRoute` / `TicketOffice` instance architecture is the superior production design. It cleanly decouples route configuration (capacity, seat numbering, passenger manifest) from the generic ticket abstraction, allowing multiple buses to operate independently without global state conflicts.
