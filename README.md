# gcc-7.1.0-abriviated-cpp-lambda

## Description
A patch for gcc-7.1.0 to implement abdriviated lambdas to C++

The patch aims at implementing the proposal [P0573r0](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0573r0.html)

## Examples

```c++
constexpr auto square = [](auto&& x) => x * x;
constexpr auto t = []<class T>(T&& x)
         => func(std::foward<T>(x));
```
## How to use

Download the sources of gcc-7.1.0 [Archive](https://gcc.gnu.org/mirrors.html), apply the patch and build it for your platform.
