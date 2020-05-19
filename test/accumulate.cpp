// Range v3 library
//
//  Copyright Eric Niebler 2014-present
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

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
    S add(int j) const { return S{i + j}; }
};

template <class Iter, class Sent = Iter>
void test()
{
    int ia[] = {1, 2, 3, 4, 5, 6};
    constexpr auto sc = tcb::rng::size(ia);
    CHECK(tcb::accumulate(Iter(ia), Sent(ia), 0) == 0);
    CHECK(tcb::accumulate(Iter(ia), Sent(ia), 10) == 10);
    CHECK(tcb::accumulate(Iter(ia), Sent(ia + 1), 0) == 1);
    CHECK(tcb::accumulate(Iter(ia), Sent(ia + 1), 10) == 11);
    CHECK(tcb::accumulate(Iter(ia), Sent(ia + 2), 0) == 3);
    CHECK(tcb::accumulate(Iter(ia), Sent(ia + 2), 10) == 13);
    CHECK(tcb::accumulate(Iter(ia), Sent(ia + sc), 0) == 21);
    CHECK(tcb::accumulate(Iter(ia), Sent(ia + sc), 10) == 31);

    using tcb::rng::subrange;
    CHECK(tcb::accumulate(subrange(Iter(ia), Sent(ia)), 0) == 0);
    CHECK(tcb::accumulate(subrange(Iter(ia), Sent(ia)), 10) == 10);
    CHECK(tcb::accumulate(subrange(Iter(ia), Sent(ia + 1)), 0) == 1);
    CHECK(tcb::accumulate(subrange(Iter(ia), Sent(ia + 1)), 10) == 11);
    CHECK(tcb::accumulate(subrange(Iter(ia), Sent(ia + 2)), 0) == 3);
    CHECK(tcb::accumulate(subrange(Iter(ia), Sent(ia + 2)), 10) == 13);
    CHECK(tcb::accumulate(subrange(Iter(ia), Sent(ia + sc)), 0) == 21);
    CHECK(tcb::accumulate(subrange(Iter(ia), Sent(ia + sc)), 10) == 31);
}

}

TEST_CASE("accumulate")
{
    test<InputIterator<const int*> >();
    test<ForwardIterator<const int*> >();
    test<BidirectionalIterator<const int*> >();
    test<RandomAccessIterator<const int*> >();
    test<const int*>();

    test<InputIterator<const int*>, Sentinel<const int*> >();
    test<ForwardIterator<const int*>, Sentinel<const int*> >();
    test<BidirectionalIterator<const int*>, Sentinel<const int*> >();
    test<RandomAccessIterator<const int*>, Sentinel<const int*> >();
}
