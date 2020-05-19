// Range v3 library
//
//  Copyright Eric Niebler 2014-present
//  Copyright Gonzalo Brito Gadeschi 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
// Implementation based on the code in libc++
//   http://http://libcxx.llvm.org/

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <numeric_ranges.hpp>
#include "catch.hpp"
#include "test_iterators.hpp"

namespace {

struct S {
    int i;
};

template <class InIter, class OutIter, class InSent = InIter>
void test()
{
    using tcb::partial_sum;
    using tcb::rng::subrange;
    { // iterator
        int ir[] = {1, 3, 6, 10, 15};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ia[] = {1, 2, 3, 4, 5};
        int ib[s] = {0};
        auto r = partial_sum(InIter(ia), InSent(ia + s), OutIter(ib));
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for (unsigned i = 0; i < s; ++i) { CHECK(ib[i] == ir[i]); }
    }

    { // range + output iterator
        int ir[] = {1, 3, 6, 10, 15};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ia[] = {1, 2, 3, 4, 5};
        int ib[s] = {0};
        auto rng = subrange(InIter(ia), InSent(ia + s));
        auto r = partial_sum(rng, OutIter(ib));
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for (unsigned i = 0; i < s; ++i) { CHECK(ib[i] == ir[i]); }
    }

#if 0 // range + output range not supported
    { // range + output range
        int ir[] = {1, 3, 6, 10, 15};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ia[] = {1, 2, 3, 4, 5};
        int ib[s] = {0};
        auto rng = subrange(InIter(ia), InSent(ia + s));
        auto orng = subrange(OutIter(ib), OutIter(ib + s));
        auto r = partial_sum(rng, orng);
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for (unsigned i = 0; i < s; ++i) { CHECK(ib[i] == ir[i]); }
    }

    {
        int ia[] = {1, 2, 3, 4, 5};
        int ir[] = {1, -1, -4, -8, -13};
        const unsigned s = sizeof(ia) / sizeof(ia[0]);
        int ib[s] = {0};
        auto rng = subrange(InIter(ia), InSent(ia + s));
        auto orng = subrange(OutIter(ib), OutIter(ib + s));
        auto r = partial_sum(rng, orng, std::minus<int>());
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for (unsigned i = 0; i < s; ++i) { CHECK(ib[i] == ir[i]); }
    }
#endif
}

}

TEST_CASE("partial_sum")
{
    test<InputIterator<const int *>, InputIterator<int *>>();
    test<InputIterator<const int *>, ForwardIterator<int *>>();
    test<InputIterator<const int *>, BidirectionalIterator<int *>>();
    test<InputIterator<const int *>, RandomAccessIterator<int *>>();
    test<InputIterator<const int *>, int *>();

    test<ForwardIterator<const int *>, InputIterator<int *>>();
    test<ForwardIterator<const int *>, ForwardIterator<int *>>();
    test<ForwardIterator<const int *>, BidirectionalIterator<int *>>();
    test<ForwardIterator<const int *>, RandomAccessIterator<int *>>();
    test<ForwardIterator<const int *>, int *>();

    test<BidirectionalIterator<const int *>, InputIterator<int *>>();
    test<BidirectionalIterator<const int *>, ForwardIterator<int *>>();
    test<BidirectionalIterator<const int *>, BidirectionalIterator<int *>>();
    test<BidirectionalIterator<const int *>, RandomAccessIterator<int *>>();
    test<BidirectionalIterator<const int *>, int *>();

    test<RandomAccessIterator<const int *>, InputIterator<int *>>();
    test<RandomAccessIterator<const int *>, ForwardIterator<int *>>();
    test<RandomAccessIterator<const int *>, BidirectionalIterator<int *>>();
    test<RandomAccessIterator<const int *>, RandomAccessIterator<int *>>();
    test<RandomAccessIterator<const int *>, int *>();

    test<const int *, InputIterator<int *>>();
    test<const int *, ForwardIterator<int *>>();
    test<const int *, BidirectionalIterator<int *>>();
    test<const int *, RandomAccessIterator<int *>>();
    test<const int *, int *>();

    using tcb::partial_sum;

    { // Test projections
        S ia[] = {{1}, {2}, {3}, {4}, {5}};
        int ir[] = {1, 3, 6, 10, 15};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ib[s] = {0};
        auto r = partial_sum(tcb::rng::begin(ia), tcb::rng::begin(ia) + s,
                             tcb::rng::begin(ib),
                             std::plus<int>(), &S::i);
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    { // Test BinaryOp
        int ia[] = {1, 2, 3, 4, 5};
        int ir[] = {1, 2, 6, 24, 120};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ib[s] = {0};
        auto r = partial_sum(ia, tcb::rng::begin(ib), std::multiplies<int>());
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    { // Test calling it with an array
        int ia[] = {1, 2, 3, 4, 5};
        int ir[] = {1, 2, 6, 24, 120};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ib[s] = {0};
        auto r = partial_sum(ia, ib, std::multiplies<int>());
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

#if 0 // views::zip not in C++20
    { // Test calling it with proxy iterators
        using namespace tcb::rng;
        int ia[] = {1, 2, 3, 4, 5};
        int ib[] = {99, 99, 99, 99, 99};
        int ir[] = {1, 2, 6, 24, 120};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ic[s] = {0};
        auto rng = views::zip(ia, ib);
        using CR = iter_common_reference_t<iterator_t<decltype(rng)>>;
        auto r = partial_sum(rng, ic, std::multiplies<int>(), [](CR p) {return p.first;});
        CHECK(base(r.in) == ranges::begin(rng) + s);
        CHECK(base(r.out) == ic + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ic[i] == ir[i]);
        }
    }
#endif
}
