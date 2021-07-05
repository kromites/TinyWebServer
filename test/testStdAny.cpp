#include <iostream>
#include <any>
#include <variant>
#include <type_traits>
#include <vector>

struct Op1 {};
struct Op2 {};

void test(const std::vector<Op1>& vec) {
    std::cout << "op1 vec.size() =" << vec.size() << "\n";
}

void test(const std::vector<Op2>& vec) {
    std::cout << "op2 vec.size() =" << vec.size() << "\n";
}

void test(const std::vector<int>& vec) {
    std::cout << "int vec.size() =" << vec.size() << "\n";
}

template<typename T>
void foo() {

    static_assert(std::is_same<T, Op1>::value || \
        std::is_same<T, Op2>::value || \
        std::is_same<T, int>::value,
        "wrong type");

    std::any vec;
    if constexpr (std::is_same<T, Op1>::value) {
        vec = std::make_any<std::vector<Op1>>(100);
    }
    else if (std::is_same<T, Op2>::value) {
        vec = std::make_any<std::vector<Op2>>(101);
    }
    else {
        vec = std::make_any<std::vector<int>>(102);
    }
    test(std::any_cast<std::vector<T>>(vec));
}

int main() {
    foo<Op1>();
    foo<Op2>();
    foo<int>();
    return 0;
}
