#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdexcept>

class Complex {
    double real;
    double imag;

public:
    // Constructors
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    double getReal() const { return real; }
    double getImag() const { return imag; }

    double magnitude() const {
        return std::sqrt(real * real + imag * imag);
    }

    // Overloaded Arithmetic Operators
    Complex operator+(const Complex& o) const {
        return Complex(real + o.real, imag + o.imag);
    }

    Complex operator-(const Complex& o) const {
        return Complex(real - o.real, imag - o.imag);
    }

    Complex operator*(const Complex& o) const {
        return Complex(real * o.real - imag * o.imag, real * o.imag + imag * o.real);
    }

    Complex operator/(const Complex& o) const {
        double denom = o.real * o.real + o.imag * o.imag;
        if (denom == 0.0) {
            throw std::domain_error("Division by zero in AC impedance calculation!");
        }
        return Complex((real * o.real + imag * o.imag) / denom,
                       (imag * o.real - real * o.imag) / denom);
    }

    bool operator==(const Complex& o) const {
        return std::abs(real - o.real) < 1e-9 && std::abs(imag - o.imag) < 1e-9;
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << std::fixed << std::setprecision(3) << c.real;
        if (c.imag >= 0) os << " + " << c.imag << "j";
        else os << " - " << (-c.imag) << "j";
        return os;
    }
};

int main() {
    std::cout << "=====================================================
";
    std::cout << "Q7: AC CIRCUIT IMPEDANCE CALCULATOR (FIRST PRINCIPLES)
";
    std::cout << "=====================================================
";

    double R = 50.0;           // 50 Ohms
    double L = 0.1;            // 0.1 H
    double C = 100e-6;         // 100 uF
    double omega = 314.159;    // 50 Hz grid (2 * pi * 50)

    Complex Zr(R, 0.0);
    Complex Zl(0.0, omega * L);
    Complex Zc(0.0, -1.0 / (omega * C));

    std::cout << "Resistor Impedance (Zr):  " << Zr << " Ohms | Mag: " << Zr.magnitude() << " Ohms\n";
    std::cout << "Inductor Impedance (Zl):  " << Zl << " Ohms | Mag: " << Zl.magnitude() << " Ohms\n";
    std::cout << "Capacitor Impedance (Zc): " << Zc << " Ohms | Mag: " << Zc.magnitude() << " Ohms\n";

    // Series RLC: Z_series = Zr + Zl + Zc
    Complex Zseries = Zr + Zl + Zc;
    std::cout << "\n--- Series RLC Circuit ---\n";
    std::cout << "Z_series = Zr + Zl + Zc : " << Zseries << " Ohms\n";
    std::cout << "Magnitude |Z_series|    : " << Zseries.magnitude() << " Ohms\n";

    // Parallel RLC: Z_parallel = 1 / (1/Zr + 1/Zl + 1/Zc)
    // Using pairwise formula: Z12 = (Zr * Zl) / (Zr + Zl); Z_par = (Z12 * Zc) / (Z12 + Zc)
    Complex Z_rl = (Zr * Zl) / (Zr + Zl);
    Complex Zparallel = (Z_rl * Zc) / (Z_rl + Zc);
    std::cout << "\n--- Parallel RLC Circuit ---\n";
    std::cout << "Z_parallel              : " << Zparallel << " Ohms\n";
    std::cout << "Magnitude |Z_parallel|  : " << Zparallel.magnitude() << " Ohms\n";

    // Defensive Test: Safe Division by Zero Impedance
    std::cout << "\nTesting Division by Zero Handling:\n";
    try {
        Complex Zzero(0.0, 0.0);
        Complex fault = Zr / Zzero;
        std::cout << fault << "\n";
    } catch (const std::domain_error& e) {
        std::cout << "[PROTECTION ACTIVE]: " << e.what() << "\n";
    }

    return 0;
}
