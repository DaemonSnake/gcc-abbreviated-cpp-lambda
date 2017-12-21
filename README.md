# gcc-abbreviated-cpp-lambdal

## Description
A patch for gcc-7.2 to implement abbreviated lambdas to C++

The patch aims at implementing the proposals:
* Abbreviated Lambdas for Fun and Profit: [P0573r2](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0573r2.html)
* Forward without forward: [P0644r1](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0644r1.html)

## Try It

You can try the compiler live [here](http://www.gcc-abbreviated-lambdas-proposal.tk/).

I'm hosting a ubuntu build of the compiler on a Amazon EC2 server and running [compiler explorer](https://github.com/mattgodbolt/compiler-explorer) on it.

## Implementation status:
* adds an abbreviated syntax for lambdas and function/members [OK]
* uses *decltype\(\(ret_expr\)\)* as deduced return type when used [OK]
* uses *noexcept(noexcept(ret_expr))* as deduced exception specification [OK]
* optional type for lambda's arguments [OK]
* *\>\>x* exacltly equivalent to *static_cast<decltype(x)&&>(x)* [OK]

## Example
```c++
[](auto&&x) => func(>>x); /*or*/ [](x) => func(>>x);
//will be equivalent to
[](auto&& x)
  noexcept(noexcept(func(std::forward<decltype(x)&&>(x))))
  -> decltype((func(std::forward<decltype(x)&&>(x))))
{
    return func(std::forward<decltype(x)&&>(x));    
};

template<class T>
constexpr auto f(T&& x) => x; //allowed with functions too

[](x...) {}; //same as [](auto&&... x) {}
```

## How to use localy

Simply run make, sit back and wait.
It will:
* download gcc-7.2
* apply the patches
* configure gcc
* compile gcc
* install gcc in $(PWD)/gcc/install/
* compile test.cpp with the new g++.

By default gcc will be configured with the following options:
* --disable-bootstrap //build gcc using the produced gcc
* --disable-multilib //cross-compiling
* --disable-shared //doesn't build shared standard library
Simply remove them from the gcc/build command from Makefile if you want to change the default behavior.
