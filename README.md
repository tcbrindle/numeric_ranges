
# `<numeric>` algorithms for C++20 Ranges #

C++20 includes updated versions of the many algorithms in the `<algorithm>` header. This header supplements these with updated versions of
other algorithms from the `<numeric>` header.

If you're using C++20 ranges (or [NanoRange](http://github.com/tcbrindle/nanorange) -- see below)
then you can drop the [numeric_ranges.hpp]() header into your project and use it
as a modern replacement for `<numeric>` until such time as C++23 comes along.

## Examples ##

```cpp
constexpr std::array arr{1, 2, 3, 4};
std::vector<int> out;

tcb::partial_sum(arr, std::back_inserter(out));
// out contains [1, 3, 6, 10]

const int prod = tcb::inner_product(arr, out, 0);
// prod = (1 * 1) + (2 * 3) + (3 * 6) * (4 * 10) = 65
assert(prod == 65);

constexpr auto sq = [](int i) { return i * i; };
constexpr int sum = tcb::accumulate(arr, 0, {}, sq);
// sum = 1 + 4 + 9 + 16 = 30
static_assert(sum == 30);
```

## Usage ##

If your standard library provides an implementation of C++20 ranges, you can
just copy [numeric_ranges.hpp]() into your project and use it as an alternative to
the `<numeric>` header.

The rest of this respository contains testing machinery and is not required for use.

`numeric_ranges.hpp` is also compatible with [NanoRange](http://github.com/tcbrindle/nanorange). 
To use it, define the symbol `TCB_NUMERIC_RANGES_USE_NANORANGE`
before `#include`-ing this header, for example

```cpp
#define TCB_NUMERIC_RANGES_USE_NANORANGE
#include <numeric_ranges.hpp>
```

(or by using an equivalent compiler define).

## Algorithms ##

The following algorithms are provided in this header:

* [iota](https://en.cppreference.com/w/cpp/algorithm/iota)
* [accumulate](https://en.cppreference.com/w/cpp/algorithm/accumulate) / [reduce](https://en.cppreference.com/w/cpp/algorithm/reduce)
* [inner_product](https://en.cppreference.com/w/cpp/algorithm/inner_product) / [transform_reduce](https://en.cppreference.com/w/cpp/algorithm/transform_reduce)
* [adjacent_difference](https://en.cppreference.com/w/cpp/algorithm/adjacent_difference)
* [partial_sum](https://en.cppreference.com/w/cpp/algorithm/partial_sum)

Note that in this implementation, `reduce` and `transform_reduce` always perform their operations in order and so are equivalent to `accumulate` and `inner_product` respectively.

## Caveats ##

Unlike the other algorithms in `std::ranges` (and the eventual C++23) version of
`<numeric>`) the implementations in this library are **unconstrained**.

This means that it's possible to get horrible, C++98-style template error messages when things
go wrong. It's also possible that in rare cases, code you write today with this library may fail
constraint checks when they are added in future.

## Licence ##

This library is provided under the terms of the [Boost licence](https://www.boost.org/users/license.html).

## Acknowledgements ##

Thanks to Eric Niebler for writing the Range-V3 tests for all these algorithms,
which I have lovingly "borrowed".

Thanks to Phil Nash et al for the fantastic Catch2 testing framework.



