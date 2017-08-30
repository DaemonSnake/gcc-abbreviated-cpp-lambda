# gcc-7.1.0-abriviated-cpp-lambda

## Description
A patch for gcc-7.1.0 to implement abdriviated lambdas to C++

The patch aims at implementing the proposal [P0573r0](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0573r0.html)
Currently has:
* abriviated syntax: []() => ret_expr
* unary forward operator: >>expr

## Examples

```c++
constexpr auto square = [](auto&& x) => x * x;
constexpr auto t = []<class T>(T&& x)
         => func(std::foward<T>(x));
constexpr auto t_1 = [](auto&& x) => func(>>x);
```
## How to use

Download the sources of gcc-7.1.0 [Archive](https://gcc.gnu.org/mirrors.html), apply the patch and build it for your platform. (git add -A; git commit -am "Init"; git am path/to/patch;)

## Todo

* throw is allowed as result (good? bad?)
```c++
constexpr auto f = []() => throw 0; //works fine, return type is void
```
* by using decltype(ret_expr) if ret_exr is a rvalue reference an error occurs (issue?)
```c++
constexpr auto f = [](auto&& x) => x;
int i = 0;
f(i); //ok
f(42); //error: cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘int’
```
current solution
```c++
constexpr auto f = [](auto&& x) => >>x; //std::forward<decltype(x)>(x)
int i = 0;
f(i); //ok
f(42); //ok
```
