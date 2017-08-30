#include <functional>

struct Widget {};

bool test(int) { return true; }
bool test(Widget) { return false; }

void invoke(std::function<bool(int)>) {}
void invoke(std::function<bool(std::string)>) {}

struct A
{
    constexpr int operator*(const A&) const { return 0; }
};

int main()
{
    constexpr auto square = []<class T>(T&& x) => x * x;
    square(A{});

    invoke([]<class T>(T&& x)
           => test(std::forward<T>(x)));

    ([](auto&&x) -> std::decay_t<decltype(x)> => x)(42);
    ([](auto&&x) => std::forward<decltype(x)>(x))(42);
    // ([](auto&&x) => x)(42); //error
}
