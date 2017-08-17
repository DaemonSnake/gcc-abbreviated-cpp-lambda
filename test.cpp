#include <functional>

struct Widget {};

bool test(int) { return true; }
bool test(Widget) { return false; }

void invoke(std::function<bool(int)>) {}
void invoke(std::function<bool(std::string)>) {}

struct A
{
    int operator*(A) { return 0; }
};

int main()
{
    constexpr auto square = []<class T>(T&& x) => x * x;
    square(A{});

    invoke([]<class T>(T&& x)
           -> decltype(test(std::forward<T>(x))) //find a way to get rid of this line
           => test(std::forward<T>(x)));
}
