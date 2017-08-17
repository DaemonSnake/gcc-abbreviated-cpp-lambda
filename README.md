# gcc-7.1.0-abriviated-cpp-lambda
A patch for gcc-7.1.0 to implement abdriviated lambdas to C++

Based on the [P0573r0](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0573r0.html) proposal

Currently allows lambdas like this

```c++
constexpr auto square = [](auto&& x) => x * x;
constexpr auto t = []<class T>(T&& x)
         -> decltype(func(std::foward<T>(x))) //expected to be the default, in progress
         => func(std::foward<T>(x));
```
