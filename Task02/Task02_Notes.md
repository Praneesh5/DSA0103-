# TASK 2: Use Tokens, Keywords, Identifiers and Constants Correctly
**Bloom's Taxonomy Level:** K5 – Evaluate  
**Course Outcome:** CO1 (Tokens, Keywords, Identifiers, Constants, and Preprocessor Directives)

---

## 1. Analysis of the Five Kinds of Tokens

Every C++ program is processed by the lexical analyzer into five fundamental token categories:

| Token Category | Definition & Role | Concrete Examples in Metro Fare Logic |
| :--- | :--- | :--- |
| **Keywords** | Reserved lexical words with immutable language semantics. | `class`, `constexpr`, `enum`, `switch`, `return`, `long`, `bool`, `static_assert` |
| **Identifiers** | Programmer-defined names given to entities (types, variables, functions, namespaces). | `SmartCard`, `MetroFareConfig`, `BASE_INNER_PAISE`, `canTravel`, `Zone` |
| **Constants (Literals)** | Fixed, immutable values embedded directly into code. | `1000LL` (long long int), `20` (int), `5000LL`, `'A'` (char), `true` (bool), `"Rs "` (string literal) |
| **Operators** | Symbols instructing the compiler to execute specific computations. | `+`, `-`, `*`, `/`, `%`, `&&`, `||`, `?:`, `<<`, `::`, `static_cast` |
| **Punctuators (Delimiters)**| Structural separators organizing code units and statements. | `;` (terminator), `{}` (blocks), `()` (parameter lists), `<>` (templates), `,` (argument separator) |

---

## 2. Evaluation: `#define` vs. `const` vs. `constexpr`

| Dimension | `#define` Macro | `const` Identifier | `constexpr` Identifier (Preferred) |
| :--- | :--- | :--- | :--- |
| **Phase of Handling** | Preprocessor (textual replacement before compilation). | Compiler (symbolic processing). | Compiler (guaranteed compile-time constant expression evaluation). |
| **Type Safety** | **None.** Untyped literal replacement. Can cause silent type conversions. | **Strong.** Fully typed and validated against C++ type system. | **Strongest.** Strongly typed, checked at compile time via `static_assert`. |
| **Scope & Encapsulation** | **Global.** Ignores namespaces, classes, and blocks; pollutes global translation scope. | **Scoped.** Respects namespaces, classes, and function blocks. | **Scoped.** Encapsulated in namespaces or class definitions. |
| **Memory Allocation** | Occupies no binary storage, but text replacement may inflate code size. | May occupy data section RAM/ROM if address is taken or in debug builds. | Guaranteed inlined/folded at compile time; zero runtime memory overhead. |
| **Precedence Vulnerability**| Prone to operator precedence bugs if unparenthesized (e.g., `#define FARE 10+2`). | Immune to precedence bugs. | Immune to precedence bugs; can be used as array bounds or template arguments. |

### Technical Decision:
`constexpr` is strictly selected for all fare table values and surcharge rates. It provides absolute compile-time safety, zero runtime footprint, and allows `static_assert` verification of fare logic before binary deployment.

---

## 3. Enumeration Analysis: `enum` vs. `enum class`

### Unscoped `enum` (C++98):
```cpp
enum UnscopedZone { Inner, Middle, Outer };
```
- **Hazards:**
  1. *Namespace Pollution:* Identifiers `Inner`, `Middle`, `Outer` enter the enclosing scope. If another enum (e.g. `enum CityZone { Inner, ... }`) exists, a fatal name collision occurs.
  2. *Unchecked Implicit Conversion:* Silently converts to `int`. A programmer can write `if (Inner == 0)` or even add zones together (`Inner + Middle`), which violates domain semantics.

### Scoped `enum class` (C++11/17):
```cpp
enum class Zone : unsigned char { Inner = 0, Middle = 1, Outer = 2 };
```
- **Benefits:**
  1. *Scope Encapsulation:* Must be qualified as `Zone::Inner`.
  2. *Type Safety:* Does NOT implicitly convert to integer. Arithmetic or cross-enum comparisons yield compile-time errors.
  3. *Controlled Underlying Type:* Specifying `: unsigned char` reduces the memory footprint per zone to exactly 1 byte.

---

## 4. Identifier Naming Rules and Conventions
- **C++ Standard Compliance:** Identifiers must start with an alphabetic character or underscore, cannot contain reserved keywords, and cannot contain consecutive underscores or start with an underscore followed by an uppercase letter (reserved for compiler implementations).
- **Adopted Engineering Conventions:**
  - `PascalCase`: Classes, Structs, and Scoped Enums (`SmartCard`, `FareTable`, `Zone`).
  - `camelCase`: Member variables and functions (`balancePaise`, `canTravel()`, `calculatePeakFare()`).
  - `UPPER_SNAKE_CASE`: Compile-time `constexpr` constants (`BASE_INNER_PAISE`, `PEAK_SURCHARGE_PCT`).

---

## 5. Alternative Concept & Complexity Analysis: `#define` vs. `constexpr`

| Evaluation Metric | `#define` Macro Architecture | `constexpr` Constant Architecture | Impact Analysis |
| :--- | :--- | :--- | :--- |
| **Compile-time Check** | None (errors caught late or never) | Instantaneous compiler verification via `static_assert` | Eliminates human error during fare configuration. |
| **Time Complexity** | $O(1)$ constant substitution | $O(1)$ folded compile-time constant | Identical machine instruction execution time. |
| **Space Complexity** | $O(1)$ literal in instruction stream | $O(1)$ literal in instruction stream | Zero runtime heap or stack overhead. |
| **Debugging & Diagnostics** | Hidden from GDB/LLDB symbol tables | Visible in debugger with exact name, type, and value | Drastically reduces troubleshooting mean-time-to-repair (MTTR). |
