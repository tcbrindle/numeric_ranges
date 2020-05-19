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
    using tcb::adjacent_difference;
    using tcb::rng::subrange;
    { // iterator
        int ia[] = {15, 10, 6, 3, 1};
        int ir[] = {15, -5, -4, -3, -2};
        const unsigned s = sizeof(ia) / sizeof(ia[0]);
        int ib[s] = {0};
        auto r = adjacent_difference(InIter(ia), InSent(ia + s), OutIter(ib));
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for (unsigned i = 0; i < s; ++i) { CHECK(ib[i] == ir[i]); }
    }

    { // range + output iterator
        int ia[] = {15, 10, 6, 3, 1};
        int ir[] = {15, -5, -4, -3, -2};
        const unsigned s = sizeof(ia) / sizeof(ia[0]);
        int ib[s] = {0};
        auto rng = subrange(InIter(ia), InSent(ia + s));
        auto r = adjacent_difference(rng, OutIter(ib));
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for (unsigned i = 0; i < s; ++i) { CHECK(ib[i] == ir[i]); }
    }

#if 0 /* We don't support range + output range */
    { // range + output range
        int ia[] = {15, 10, 6, 3, 1};
        int ir[] = {15, -5, -4, -3, -2};
        const unsigned s = sizeof(ia) / sizeof(ia[0]);
        int ib[s] = {0};
        auto rng = subrange(InIter(ia), InSent(ia + s));
        auto orng = subrange(OutIter(ib), OutIter(ib + s));
        auto r = adjacent_difference(rng, orng);
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for (unsigned i = 0; i < s; ++i) { CHECK(ib[i] == ir[i]); }
    }

    {
        int ia[] = {15, 10, 6, 3, 1};
        int ir[] = {15, 25, 16, 9, 4};
        const unsigned s = sizeof(ia) / sizeof(ia[0]);
        int ib[s] = {0};
        auto rng = subrange(InIter(ia), InSent(ia + s));
        auto orng = subrange(OutIter(ib), OutIter(ib + s));
        auto r = adjacent_difference(rng, orng, std::plus<int>());
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for (unsigned i = 0; i < s; ++i) { CHECK(ib[i] == ir[i]); }
    }
#endif

}

}

TEST_CASE("adjacent_difference")
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

    using tcb::adjacent_difference;

    { // Test projections
        S ia[] = {{15}, {10}, {6}, {3}, {1}};
        int ir[] = {15, -5, -4, -3, -2};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ib[s] = {0};
        auto r = adjacent_difference(tcb::rng::begin(ia), tcb::rng::begin(ia) + s,
                                     tcb::rng::begin(ib), std::minus<int>(), &S::i);
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    { // Test BinaryOp
        int ia[] = {15, 10, 6, 3, 1};
        int ir[] = {15, 25, 16, 9, 4};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ib[s] = {0};
        auto r = adjacent_difference(ia, tcb::rng::begin(ib), std::plus<int>());
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    { // Test calling it with an array
        int ia[] = {15, 10, 6, 3, 1};
        int ir[] = {15, 25, 16, 9, 4};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ib[s] = {0};
        auto r = adjacent_difference(ia, ib, std::plus<int>());
        CHECK(base(r.in) == ia + s);
        CHECK(base(r.out) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }
}
