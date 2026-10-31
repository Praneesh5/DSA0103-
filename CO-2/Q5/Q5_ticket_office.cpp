#include <iostream>
#include <string>
#include <vector>

// Class demonstrating static member behavior across scoped function calls
class BusTicketStatic {
    static int issued;
    static const int CAPACITY = 3;
    int seat;
    std::string name;

public:
    explicit BusTicketStatic(const std::string& n) : seat(0), name(n) {
        if (issued < CAPACITY) seat = ++issued;
        else seat = 0;
    }
    static int count() { return issued; }
    void print() const {
        std::cout << "  " << name << ": "
                  << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting List"))
                  << '\n';
    }
};

int BusTicketStatic::issued = 0;

// Function called twice to observe static persistence across local scopes
void bookMorningShift() {
    std::cout << "  [Entering bookMorningShift()]\n";
    BusTicketStatic t1("Ravi");
    BusTicketStatic t2("Sneha");
    t1.print();
    t2.print();
    std::cout << "  Total issued so far: " << BusTicketStatic::count() << '\n';
    std::cout << "  [Exiting bookMorningShift() - local objects destroyed]\n";
}

// =========================================================================
// ALTERNATIVE ARCHITECTURAL DESIGN: TicketOffice / BusRoute Object
// Overcoming the single-bus limitation of class-level static members
// =========================================================================

class BusRoute; // Forward declaration

class IndependentTicket {
    int seatNumber;
    std::string passengerName;
    std::string routeName;

public:
    IndependentTicket(int seat, const std::string& passenger, const std::string& route)
        : seatNumber(seat), passengerName(passenger), routeName(route) {}

    void print() const {
        std::cout << "  [" << routeName << "] " << passengerName << ": "
                  << (seatNumber ? "Seat " + std::to_string(seatNumber) : std::string("Waiting List"))
                  << '\n';
    }
};

class BusRoute {
    std::string routeName;
    int capacity;
    int issuedSeats;

public:
    BusRoute(const std::string& name, int cap)
        : routeName(name), capacity(cap), issuedSeats(0) {}

    IndependentTicket issueTicket(const std::string& passengerName) {
        if (issuedSeats < capacity) {
            issuedSeats++;
            return IndependentTicket(issuedSeats, passengerName, routeName);
        } else {
            return IndependentTicket(0, passengerName, routeName);
        }
    }

    int getIssuedCount() const { return issuedSeats; }
    std::string getRouteName() const { return routeName; }
};

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Q5: STATIC PERSISTENCE & MULTI-BUS ARCHITECTURE\n";
    std::cout << "=====================================================\n\n";

    std::cout << "--- 1. Testing Static Persistence: Calling Function Twice ---\n";
    std::cout << "First Call to bookMorningShift():\n";
    bookMorningShift();

    std::cout << "\nSecond Call to bookMorningShift():\n";
    bookMorningShift();

    std::cout << "\nAnalysis of Static Behavior:\n";
    std::cout << "Even though local objects t1 and t2 went out of scope, the static variable\n"
              << "'issued' remained in static storage, causing the second invocation to hit\n"
              << "capacity and place new passengers on the Waiting List!\n\n";

    std::cout << "--- 2. Multi-Bus Simulation via TicketOffice/BusRoute Objects ---\n";
    std::cout << "Demonstrating two independent buses operating simultaneously:\n";

    BusRoute routeA("Route 101 - North Campus", 3);
    BusRoute routeB("Route 202 - South Campus", 2);

    IndependentTicket tA1 = routeA.issueTicket("Karan");
    IndependentTicket tA2 = routeA.issueTicket("Diya");
    IndependentTicket tB1 = routeB.issueTicket("Vikram");
    IndependentTicket tB2 = routeB.issueTicket("Pooja");
    IndependentTicket tB3 = routeB.issueTicket("Sanjay"); // Exceeds Route B capacity

    tA1.print();
    tA2.print();
    tB1.print();
    tB2.print();
    tB3.print();

    std::cout << "\nRoute 101 Issued: " << routeA.getIssuedCount() << " / 3\n";
    std::cout << "Route 202 Issued: " << routeB.getIssuedCount() << " / 2\n";

    return 0;
}
