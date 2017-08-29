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
}
