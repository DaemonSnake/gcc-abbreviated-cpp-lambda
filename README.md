# gcc-abriviated-cpp-lambda

## Description
A patch for gcc-7.2 (7.1 in branch) to implement abdriviated lambdas to C++

The patch aims at implementing the proposals:
* lambda abriviated [P0573r1](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0573r1.html)
* Forward without forward [P0644r0](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0644r0.html)

Allowing the following:
```c++
[](auto&&x) => func(>>x);
//equivalent to
[](auto&& x) -> decltype((func(std::forward<decltype(x)&&>(x)))) noexcept(noexcept(func(std::forward<decltype(x)&&>(x)))) {
    return func(std::forward<decltype(x)&&>(x));
};
```

## Examples

```c++
constexpr auto square = [](auto&& x) => x * x;
constexpr auto t = []<class T>(T&& x)
         => func(std::foward<T>(x));
//being equivalent to the following
constexpr auto t_1 = [](auto&& x) => func(>>x);
```
## How to use

Simply run make, sit back and wait.
It will:
* download gcc-7.2
* apply the patches
* configure gcc
* compile gcc
* install gcc in $(PWD)/gcc/install/
* compile test.cpp with the new g++.

By default gcc is going to be configured with the following options:
* --disable-bootstrap //build gcc using the produced gcc
* --disable-multilib //cross-compiling
* --disable-shared //doesn't build shared standard library
Simply remove them from the gcc/build command from Makefile if you want to change the default behavior.

## Todo
* forward decay-copy capture for lambdas
```c++
int x;
[>>]() {};
//x is perfectly forwarded in the lambda
```

## Note on gcc-7.1

The reason why this patch targets gcc-7.2 more that 7.1 is because the following crashes with gcc-7.1
```c++
[](auto&& x) noexcept(noexcept(f(x))) { return f(x); };
```
The branch '7.1' therefore doesn't implement the exception specification part of the abriviated lambda proposal
