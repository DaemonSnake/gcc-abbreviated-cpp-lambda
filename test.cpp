#include <functional>
#include <type_traits>

void check_return_type_are_correct();
void check_forward();
void check_raii();

struct Widget {};
bool test(int) => true;
bool test(Widget) => false;
void invoke(std::function<bool(int)>) {}
void invoke(std::function<bool(std::string)>) {}

template <class T>
constexpr bool is_rvalue_ref(T&&) => std::is_rvalue_reference_v<T&&>;

int&& f(int&& x) noexcept(true) => (>>x);

int main()
{
    //syntax
    constexpr auto square = [](auto&& x) => x * x;
    square(5);
    static_assert(square(5) == 25);

    check_raii();
    check_forward();
    check_return_type_are_correct();
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

template<class T> constexpr auto func_1(T&& x) => (>>x);
template<class T> constexpr auto func_2(T&& x) -> decltype((>>x)) => (>>x);
template<class T> constexpr auto func_3(T&& x) => (x);
template<class T> constexpr auto func_4(T&& x) -> decltype((x)) => (x);

void check_return_type_are_correct()
{
    int i = 0, &&v = 0;

    constexpr auto make_is_same = []<class L, class R>(L&&, R&&) {
        static_assert(std::is_same_v<L, R>);
    };
    constexpr auto lambda_1 = []<class T>(T&& x) => (>>x);
    constexpr auto lambda_2 = []<class T>(T&& x) -> decltype((>>x)) => (>>x);
    constexpr auto lambda_3 = []<class T>(T&& x) => (x);
    constexpr auto lambda_4 = []<class T>(T&& x) -> decltype((x)) => (x);

#define SAME_F(funcA, funcB, args...)           \
    make_is_same(funcA(args), funcB(args))

    SAME_F(lambda_1, lambda_2, 42);
    SAME_F(lambda_1, lambda_2, i);
    SAME_F(lambda_1, lambda_2, v);
    SAME_F(lambda_1, lambda_2, >>i);
    SAME_F(lambda_1, lambda_2, >>v);

    SAME_F(lambda_3, lambda_4, 42);
    SAME_F(lambda_3, lambda_4, i);
    SAME_F(lambda_3, lambda_4, v);
    SAME_F(lambda_3, lambda_4, >>i);
    SAME_F(lambda_3, lambda_4, >>v);

    SAME_F(func_1, func_2, 42);
    SAME_F(func_1, func_2, i);
    SAME_F(func_1, func_2, v);
    SAME_F(func_1, func_2, >>i);
    SAME_F(func_1, func_2, >>v);

    SAME_F(func_3, func_4, 42);
    SAME_F(func_3, func_4, i);
    SAME_F(func_3, func_4, v);
    SAME_F(func_3, func_4, >>i);
    SAME_F(func_3, func_4, >>v);
}
