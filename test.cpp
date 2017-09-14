#include <functional>
#include <type_traits>

struct Widget {};

bool test(int) { return true; }
bool test(Widget) { return false; }

void invoke(std::function<bool(int)>) {}
void invoke(std::function<bool(std::string)>) {}

template <class T>
constexpr bool is_rvalue_ref(T&&)
{
    return std::is_rvalue_reference_v<T&&>;
}

int&& f(int&& x) noexcept(true) { return >>x; }

int main()
{
    //syntax
    constexpr auto square = [](auto&& x) => x * x;
    square(5);
    static_assert(square(5) == 25);

    invoke([]<class T>(T&& x)
           => test(std::forward<T>(x)));

    //unary operator >> : perfect forwarding
    constexpr auto check = [](auto&&x) => is_rvalue_ref(>>x);
    int i = 0;

    check(42);
    check(i);
    static_assert(check(42));
    static_assert(!check(i));

#define TEST_TYPE_EXCEPT(name, call, return, expr)              \
    call;                                                       \
    static_assert(std::is_same_v<decltype(call), return>);      \
    static_assert(noexcept(call) == noexcept(expr))

    auto c2 = [](auto&& x) => x;
    auto c3 = [](auto&& x) => f(>>x); //noexcept(noexcept(ret_expr)) todo
    auto c4 = [](auto& x) => x;
    auto c5 = [](auto x) => x;

    TEST_TYPE_EXCEPT(c2, c2(5), int, 5);
    TEST_TYPE_EXCEPT(c3, c3(5), int&&, f(5));
    TEST_TYPE_EXCEPT(c4, c4(i), int&, i);
    TEST_TYPE_EXCEPT(c5, c5(5), int, 5);
}
