#include <iostream>
#include <chrono>
#include <vector>
#include <memory>

// Task B3 Microbenchmark comparing Virtual Functions vs Monolithic Switch
class BillBase {
public:
    virtual ~BillBase() = default;
    virtual double total() const = 0;
};

class OPBillImpl : public BillBase {
public:
    double total() const override { return 1700.0; }
};

class IPBillImpl : public BillBase {
public:
    double total() const override { return 47500.0; }
};

struct SwitchBill {
    int type; // 0: OP, 1: IP
    double consultation, tests, days, perDay, surgery;

    double total() const {
        switch (type) {
            case 0: return consultation + tests;
            case 1: return days * perDay + surgery;
            default: return 0.0;
        }
    }
};

int main() {
    const int N = 1000000;
    std::cout << "Benchmarking 1,000,000 operations (-O2)...\n";

    // 1. Virtual Functions Benchmark
    std::vector<std::unique_ptr<BillBase>> v_bills;
    v_bills.reserve(N);
    for (int i = 0; i < N; ++i) {
        if (i % 2 == 0) v_bills.push_back(std::make_unique<OPBillImpl>());
        else v_bills.push_back(std::make_unique<IPBillImpl>());
    }

    auto start_v = std::chrono::high_resolution_clock::now();
    double sum_v = 0;
    for (const auto& b : v_bills) sum_v += b->total();
    auto end_v = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_v = end_v - start_v;

    // 2. Switch Benchmark
    std::vector<SwitchBill> s_bills;
    s_bills.reserve(N);
    for (int i = 0; i < N; ++i) {
        if (i % 2 == 0) s_bills.push_back({0, 500, 1200, 0, 0, 0});
        else s_bills.push_back({1, 0, 0, 3, 2500, 40000});
    }

    auto start_s = std::chrono::high_resolution_clock::now();
    double sum_s = 0;
    for (const auto& b : s_bills) sum_s += b.total();
    auto end_s = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_s = end_s - start_s;

    std::cout << "Virtual Dispatch Time : " << elapsed_v.count() << " ms (Sum: " << sum_v << ")\n";
    std::cout << "Switch Statement Time : " << elapsed_s.count() << " ms (Sum: " << sum_s << ")\n";

    return 0;
}
