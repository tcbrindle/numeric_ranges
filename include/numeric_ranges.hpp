// numeric_ranges.hpp
//
// Copyright (c) 2020 Tristan Brindle (tcbrindle at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef TCB_NUMERIC_RANGES_HPP_INCLUDED
#define TCB_NUMERIC_RANGES_HPP_INCLUDED

#ifdef TCB_NUMERIC_RANGES_USE_NANORANGE
#include <nanorange.hpp>
#else
#include <algorithm>
#include <iterator>
#include <ranges>
#endif // USE_NANORANGE

namespace tcb {
inline namespace ranges {

#ifdef TCB_NUMERIC_RANGES_USE_NANORANGE
namespace _std = nano;
namespace rng = nano::ranges;
#else
namespace _std = std;
namespace rng = std::ranges;
#endif

template <typename I, typename O>
using adjacent_difference_result = rng::copy_result<I, O>;

template <typename I, typename O>
using partial_sum_result = rng::copy_result<I, O>;

namespace detail {

struct iota_fn {

    template <typename I, typename S, typename T>
    constexpr I operator()(I first, S last, T value) const
    {
        while (first != last) {
            *first = value;
            ++value;
            ++first;
        }

        return first;
    }

    template <typename R, typename T>
    constexpr rng::borrowed_iterator_t<R> operator()(R&& r, T value) const
    {
        return (*this)(rng::begin(r), rng::end(r), std::move(value));
    }

};

struct accumulate_fn {

    template <typename I, typename S,
        typename T = _std::iter_value_t<I>,
        typename Op = std::plus<>,
        typename Proj = _std::identity>
    constexpr auto operator()(I first, S last, T init = T{},
                              Op op = Op{}, Proj proj = Proj{}) const
    -> std::enable_if_t<_std::input_iterator<I> && _std::sentinel_for<S, I>, T>
    {
        while (first != last) {
            init = _std::invoke(op, std::move(init), _std::invoke(proj, *first));
            ++first;
        }

        return init;
    }

    template <typename R,
        typename T = rng::range_value_t<R>,
        typename Op = std::plus<>,
        typename Proj = _std::identity>
    constexpr auto operator()(R&& r, T init = T{},
                              Op op = Op{}, Proj proj = Proj{}) const
    -> std::enable_if_t<rng::input_range<R>, T>
    {
        return (*this)(rng::begin(r), rng::end(r),
                       std::move(init), std::move(op), std::move(proj));
    }
};

struct inner_product_fn {

    template <typename I1, typename S1, typename I2, typename S2,
        typename T,
        typename Op1 = std::plus<>,
        typename Op2 = std::multiplies<>,
        typename Proj1 = _std::identity,
        typename Proj2 = _std::identity>
    constexpr auto operator()(I1 first1, S1 last1,
                              I2 first2, S2 last2,
                              T init,
                              Op1 op1 = Op1{}, Op2 op2 = Op2{},
                              Proj1 proj1 = Proj1{}, Proj2 proj2 = Proj2{}) const
    -> std::enable_if_t<
        _std::input_iterator<I1> && _std::sentinel_for<S1, I1> &&
            _std::input_iterator<I2> && _std::sentinel_for<S2, I2>,
        T>
    {
        while (first1 != last1 && first2 != last2) {
            init = _std::invoke(op1, std::move(init),
                          _std::invoke(op2, _std::invoke(proj1, *first1),
                                             _std::invoke(proj2, *first2)));
            ++first1;
            ++first2;
        }

        return init;
    }

    template <typename R1, typename R2, typename T,
        typename Op1 = std::plus<>,
        typename Op2 = std::multiplies<>,
        typename Proj1 = _std::identity,
        typename Proj2 = _std::identity>
    constexpr auto operator()(R1&& r1, R2&& r2, T init,
                              Op1 op1 = Op1{}, Op2 op2 = Op2{},
                              Proj1 proj1 = Proj1{}, Proj2 proj2 = Proj2{}) const
    -> std::enable_if_t<rng::input_range<R1> && rng::input_range<R2>,
        T>
    {
        return (*this)(rng::begin(r1), rng::end(r1),
                       rng::begin(r2), rng::end(r2),
                       std::move(init),
                       std::move(op1), std::move(op2),
                       std::move(proj1), std::move(proj2));
    }

};

struct adjacent_difference_fn {
private:
    template <typename I, typename S, typename O, typename Op, typename Proj>
    static constexpr auto impl(I first, S last, O ofirst, Op& op, Proj& proj)
        -> adjacent_difference_result<I, O>
    {
        if (first == last) {
            return {std::move(first), std::move(ofirst)};
        }

        auto prev = _std::invoke(proj, *first);
        *ofirst = prev;

        while (++first != last) {
            auto cur = _std::invoke(proj, *first);
            *++ofirst = _std::invoke(op, cur, prev); // std::move(prev)???
            prev = std::move(cur);
        }

        ++ofirst;

        return {std::move(first), std::move(ofirst)};
    }

public:
    template <typename I, typename S, typename O,
        typename Op = std::minus<>, typename Proj = _std::identity,
        typename Id = std::decay_t<I>>
    constexpr auto operator()(I&& first, S last, O ofirst,
                              Op op = Op{}, Proj proj = Proj{}) const
    -> std::enable_if_t<
        !rng::input_range<I> &&
            _std::input_iterator<Id> && _std::sentinel_for<S, Id>,
        adjacent_difference_result<Id, O>>
    {
        return impl(std::forward<I>(first), std::move(last), std::move(ofirst),
                    op, proj);
    }

    template <typename R, typename O,
        typename Op = std::minus<>, typename Proj = _std::identity>
    constexpr auto operator()(R&& r, O o, Op op = Op{}, Proj proj = Proj{}) const
    -> std::enable_if_t<
        rng::input_range<R>,
        adjacent_difference_result<rng::borrowed_iterator_t<R>, O>>
    {
        return impl(rng::begin(r), rng::end(r), std::move(o), op, proj);
    }

};

struct partial_sum_fn {
private:
    template <typename I, typename S, typename O, typename Op, typename Proj>
    static constexpr auto impl(I first, S last, O ofirst, Op& op, Proj& proj)
        -> partial_sum_result<I, O>
    {
        if (first == last) {
            return {std::move(first), std::move(ofirst)};
        }

        auto sum = _std::invoke(proj, *first);
        *ofirst = sum;

        while (++first != last) {
            sum = _std::invoke(op, std::move(sum), _std::invoke(proj, *first));
            *++ofirst = sum;
        }

        ++ofirst;

        return {std::move(first), std::move(ofirst)};
    }

public:
    template <typename I, typename S, typename O,
        typename Op = std::plus<>, typename Proj = _std::identity,
        typename Id = std::decay_t<I>>
    constexpr auto operator()(I&& first, S last, O ofirst,
                              Op op = Op{}, Proj proj = Proj{}) const
    -> std::enable_if_t<
        !rng::input_range<I> &&
            _std::input_iterator<Id> && _std::sentinel_for<S, Id>,
        partial_sum_result<Id, O>>
    {
        return impl(std::forward<I>(first), std::move(last), std::move(ofirst),
                    op, proj);
    };

    template <typename R, typename O, typename Op = std::plus<>,
        typename Proj = _std::identity>
    constexpr auto operator()(R&& r, O o, Op op = Op{}, Proj proj = Proj{}) const
    -> std::enable_if_t<rng::input_range<R>,
        partial_sum_result<rng::borrowed_iterator_t<R>, O>>
    {
        return impl(rng::begin(r), rng::end(r), std::move(o), op, proj);
    };
};

} // detail


inline constexpr auto iota = detail::iota_fn{};

inline constexpr auto accumulate = detail::accumulate_fn{};

inline constexpr auto reduce = detail::accumulate_fn{};

inline constexpr auto inner_product = detail::inner_product_fn{};

inline constexpr auto transform_reduce = detail::inner_product_fn{};

inline constexpr auto adjacent_difference = detail::adjacent_difference_fn{};

inline constexpr auto partial_sum = detail::partial_sum_fn{};

}}

#endif

