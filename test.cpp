#include <functional>
#include <type_traits>

void check_forward();
void check_raii();
void check_noexcept_spec();

struct Widget {};
auto test(int) => true;
auto test(Widget) => false;
void invoke(std::function<bool(int)>) {}
void invoke(std::function<bool(std::string)>) {}

template <class T>
constexpr auto is_rvalue_ref(T&&) => std::is_rvalue_reference_v<T&&>;

auto f(int&& x) => (>>x);

int main()
{
    //syntax
    constexpr auto square = [](auto&& x) => x * x;
    square(5);
    static_assert(square(5) == 25);

    check_raii();
    check_forward();
    check_noexcept_spec();
}

void check_raii()
{
    invoke([]<class T>(T&& x) => test(std::forward<T>(x)));
    // the normal version would cause an error:
    // invoke([]<class T>(T&& x) { return test(std::forward<T>(x)); });
}

void check_forward()
{
    constexpr auto check = [](auto&&x) => is_rvalue_ref(>>x);
    int i = 0;

    check(42);
    check(i);
    static_assert(check(42));
    static_assert(!check(i));
}

void check_optional_type_for_parameter()
{
    auto sqrt = [](x) => x * x;
    static_assert(sqrt(42) == 42 * 42);
    [](x) { static_assert(std::is_rvalue_reference_v<decltype(x)>); }(42);
    [](x...) { static_assert((is_rvalue_ref(>>x) && ...)); }(42);
}

void check_noexcept_spec()
{
    auto excepts = []<class T>(T&&) noexcept(false) { throw 42; };
    auto doesnt_excepts = []<class T>(T&&) noexcept(true) {};
    constexpr auto test = []<class T>(T&& lambda, auto&&... args) => lambda(>>args...);
    static_assert(noexcept(test(excepts, 42)) == false);
    static_assert(noexcept(test(doesnt_excepts, 42)) == true);
}
