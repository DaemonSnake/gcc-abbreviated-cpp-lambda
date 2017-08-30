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

int main()
{
    //syntax
    constexpr auto square = []<class T>(T&& x) => x * x;
    static_assert(square(5) == 25);

    //decltype over return expr
    invoke([]<class T>(T&& x)
           => test(std::forward<T>(x)));

    //forward operator
    constexpr auto check = [](auto&&x) => is_rvalue_ref(>>x);
    static_assert(check(42));
    int i = 0;
    static_assert(!check(i));
}
