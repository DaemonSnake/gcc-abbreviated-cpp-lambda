# gcc-7.1.0-abriviated-cpp-lambda

## Description
A patch for gcc-7.1.0 to implement abdriviated lambdas to C++

The patch aims at implementing the proposal [P0573r1](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0573r1.html)
Currently has:
* abriviated syntax:
```c++
[]() => ret_expr;
```
* unary forward operator:
```c++
[](auto&&x) => func(>>x);
```

## Examples

```c++
constexpr auto square = [](auto&& x) => x * x;
constexpr auto t = []<class T>(T&& x)
         => func(std::foward<T>(x));
constexpr auto t_1 = [](auto&& x) => func(>>x);
```
## How to use

Download the sources of gcc-7.1.0 [Archive](https://gcc.gnu.org/mirrors.html), apply the patch and build it for your platform. 

```sh
git init; git add -A; git commit -am "Init"; git am path/to/patch/*.patch;
```

## Todo

* throw is allowed as result (good? bad?)
```c++
constexpr auto f = []() => throw 0; //works fine, return type is void
```
* noexcept(noexcept(ret_expr)) is not yet implemented
```c++
auto f = [](auto&& x) => func(>>x);
static_assert(noexcept(f(expr)) == noexcept(func(expr))); //error
```
