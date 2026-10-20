# TASK 6: Pseudocode for Fare Computation
**Standard Applied:** SIMATS Engineering – Standard for Algorithmic Pseudocode  
**Bloom's Taxonomy Level:** K5 – Evaluate

---

## Algorithm: ComputeMetroFareAndValidateJourney

```text
ALGORITHM: ComputeMetroFareAndValidateJourney
INPUT:
    zone              : ENUM (Zone::Inner, Zone::Middle, Zone::Outer)
    hour              : INTEGER [0 .. 23]
    currentBalance    : INTEGER_64 (in paise)

OUTPUT:
    journeyAllowed    : BOOLEAN
    deductedFare      : INTEGER_64 (in paise)
    updatedBalance    : INTEGER_64 (in paise)

CONSTANTS:
    BASE_INNER        <-- 1000       // Rs 10.00 in paise
    BASE_MIDDLE       <-- 2000       // Rs 20.00 in paise
    BASE_OUTER        <-- 3000       // Rs 30.00 in paise
    SURCHARGE_RATE    <-- 20         // 20 percent
    MIN_BALANCE_LIMIT <-- 5000       // Rs 50.00 in paise

BEGIN
    // Step 1: Base Fare Resolution via Scoped Switch
    SWITCH zone DO
        CASE Zone::Inner:
            baseFare <-- BASE_INNER
        CASE Zone::Middle:
            baseFare <-- BASE_MIDDLE
        CASE Zone::Outer:
            baseFare <-- BASE_OUTER
        DEFAULT:
            baseFare <-- 0
            RETURN (FALSE, 0, currentBalance) // Reject invalid zone
    END SWITCH

    // Step 2: Peak Hour Evaluation
    // Strict boundaries: [08:00, 10:00) and [17:00, 20:00)
    IF (hour >= 8 AND hour < 10) OR (hour >= 17 AND hour < 20) THEN
        isPeak <-- TRUE
    ELSE
        isPeak <-- FALSE
    END IF

    // Step 3: Compute Surcharged Fare Using Pure Integer Arithmetic
    totalFare <-- baseFare
    IF isPeak == TRUE THEN
        surchargeAmount <-- (baseFare * SURCHARGE_RATE) / 100
        totalFare <-- baseFare + surchargeAmount
    END IF

    // Step 4: Solvency and Gate Invariant Verification
    IF (currentBalance - totalFare) >= MIN_BALANCE_LIMIT THEN
        updatedBalance <-- currentBalance - totalFare
        journeyAllowed <-- TRUE
        deductedFare   <-- totalFare
    ELSE
        updatedBalance <-- currentBalance
        journeyAllowed <-- FALSE
        deductedFare   <-- 0
    END IF

    RETURN (journeyAllowed, deductedFare, updatedBalance)
END
```
