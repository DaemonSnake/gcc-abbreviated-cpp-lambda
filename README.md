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

Download the sources of gcc-7.2.0 [Archive](https://gcc.gnu.org/mirrors.html), apply the patch and build it for your platform. 

```sh
./apply_patches.sh folder_to_gcc;
#or
git init; git add -A; git commit -am "Init"; git am path_to_patches/*.patch;
```

## Todo
* forward capture for lambdas
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
