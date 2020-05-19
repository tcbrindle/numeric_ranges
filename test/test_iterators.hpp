//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef RANGES_TEST_ITERATORS_HPP
#define RANGES_TEST_ITERATORS_HPP

#include <numeric_ranges.hpp>

template <class It, bool Sized = false>
class Sentinel;

template <class It>
class OutputIterator;

template <class It>
class InputIterator;

template <class It>
class ForwardIterator;

template <class It>
class BidirectionalIterator;

template <class It>
class RandomAccessIterator;


template <class Iter, bool Sized>
constexpr Iter base(Sentinel<Iter, Sized> i) { return i.base(); }

template <class Iter>
constexpr Iter base(OutputIterator<Iter> i) { return i.base(); }

template <class Iter>
constexpr Iter base(InputIterator<Iter> i) { return i.base(); }

template <class Iter>
constexpr Iter base(ForwardIterator<Iter> i) { return i.base(); }

template <class Iter>
constexpr Iter base(BidirectionalIterator<Iter> i) { return i.base(); }

template <class Iter>
constexpr Iter base(RandomAccessIterator<Iter> i) { return i.base(); }

template <class Iter>    // everything else
constexpr Iter base(Iter i) { return i; }

template <class It, bool Sized>
class Sentinel
{
	It it_;
public:
	constexpr Sentinel() : it_() {}
	constexpr explicit Sentinel(It it) : it_(it) {}
	constexpr It base() const { return it_; }

	constexpr friend bool operator==(const Sentinel& x, const Sentinel& y)
	{
		return x.it_ == y.it_;
	}

	template <typename I>
	constexpr friend bool operator==(const I& x, const Sentinel& y)
	{
		using ::base;
		return base(x) == y.it_;
	}
	template <typename I>
	constexpr friend bool operator!=(const I& x, const Sentinel& y)
	{
		return !(x == y);
	}
	template <typename I>
	constexpr friend bool operator==(const Sentinel& x, const I& y)
	{
		using ::base;
		return x.it_ == base(y);
	}
	template <typename I>
	constexpr friend bool operator!=(const Sentinel& x, const I& y)
	{
		return !(x == y);
	}
};

// For making sized iterator ranges:
template <template <typename> class I, typename It>
constexpr
std::ptrdiff_t operator-(Sentinel<It, true> end, I<It> begin)
{
	return base(end) - base(begin);
}

template <typename It, template <typename> class I>
constexpr
std::ptrdiff_t operator-(I<It> end, Sentinel<It, true> begin)
{
	return -(begin - end);
}

template <class It>
class output_iterator
{
	It it_;

	template <class U> friend class output_iterator;
public:
	using difference_type = tcb::rng::iter_difference_t<It>;
	using pointer = It;
	using reference = tcb::rng::iter_reference_t<It>;

	constexpr It base() const {return it_;}

	constexpr output_iterator () {}
	constexpr explicit output_iterator(It it) : it_(it) {}
	template <class U>
	//  requires nanorange::ConvertibleTo<U, It>
	constexpr
	output_iterator(const output_iterator<U>& u) :it_(u.it_) {}

	constexpr reference operator*() const {return *it_;}

	constexpr output_iterator& operator++() {++it_; return *this;}
	constexpr decltype(auto) operator++(int)
	{return it_++;}
};

template <class It>
class InputIterator
{
	It it_;

	template <class U> friend class InputIterator;
public:
	typedef std::input_iterator_tag iterator_category;
	typedef tcb::rng::iter_value_t<It>      value_type;
	typedef tcb::rng::iter_difference_t<It> difference_type;
	typedef It                       pointer;
	typedef tcb::rng::iter_reference_t<It>  reference;

	constexpr It base() const {return it_;}

	constexpr InputIterator() : it_() {}
	constexpr explicit InputIterator(It it) : it_(it) {}
	template <class U>
	  //requires nanorange::ConvertibleTo<U, It>
	constexpr InputIterator(const InputIterator<U>& u) :it_(u.it_) {}

	constexpr reference operator*() const {return *it_;}
	constexpr pointer operator->() const {return it_;}

	constexpr InputIterator& operator++() {++it_; return *this;}
	constexpr InputIterator operator++(int)
		{InputIterator tmp(*this); ++(*this); return tmp;}

	constexpr
	friend bool operator==(const InputIterator& x, const InputIterator& y)
		{return x.it_ == y.it_;}
	constexpr
	friend bool operator!=(const InputIterator& x, const InputIterator& y)
		{return !(x == y);}
};

template <class T, class U>
constexpr
bool
operator==(const InputIterator<T>& x, const InputIterator<U>& y)
{
	return x.base() == y.base();
}

template <class T, class U>
constexpr
bool
operator!=(const InputIterator<T>& x, const InputIterator<U>& y)
{
	return !(x == y);
}

template <class It>
class ForwardIterator
{
	It it_;

	template <class U> friend class ForwardIterator;
public:
	typedef std::forward_iterator_tag iterator_category;
	typedef tcb::rng::iter_value_t<It>        value_type;
	typedef tcb::rng::iter_difference_t<It>   difference_type;
	typedef It                         pointer;
	typedef tcb::rng::iter_reference_t<It>    reference;

	constexpr It base() const {return it_;}

	constexpr ForwardIterator() : it_() {}
	constexpr explicit ForwardIterator(It it) : it_(it) {}
	template <class U>
	  //requires nanorange::ConvertibleTo<U, It>
	constexpr ForwardIterator(const ForwardIterator<U>& u) :it_(u.it_) {}

	constexpr reference operator*() const {return *it_;}
	constexpr pointer operator->() const {return it_;}

	constexpr ForwardIterator& operator++() {++it_; return *this;}
	constexpr ForwardIterator operator++(int)
	{ForwardIterator tmp(*this); ++(*this); return tmp;}

	constexpr
	friend bool operator==(const ForwardIterator& x, const ForwardIterator& y)
	{return x.it_ == y.it_;}
	constexpr
	friend bool operator!=(const ForwardIterator& x, const ForwardIterator& y)
	{return !(x == y);}
};

template <class T, class U>
constexpr
bool
operator==(const ForwardIterator<T>& x, const ForwardIterator<U>& y)
{
	return x.base() == y.base();
}

template <class T, class U>
constexpr
bool
operator!=(const ForwardIterator<T>& x, const ForwardIterator<U>& y)
{
	return !(x == y);
}

template <class It>
class BidirectionalIterator
{
	It it_;

	template <class U> friend class BidirectionalIterator;
public:
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef tcb::rng::iter_value_t<It>              value_type;
	typedef tcb::rng::iter_difference_t<It>         difference_type;
	typedef It                               pointer;
	typedef tcb::rng::iter_reference_t<It>          reference;

	constexpr It base() const {return it_;}

	constexpr BidirectionalIterator() : it_() {}
	constexpr explicit BidirectionalIterator(It it) : it_(it) {}
	template <class U>
	  //requires nanorange::ConvertibleTo<U, It>
	constexpr BidirectionalIterator(const BidirectionalIterator<U>& u) :it_(u.it_) {}

	constexpr reference operator*() const {return *it_;}
	constexpr pointer operator->() const {return it_;}

	constexpr BidirectionalIterator& operator++() {++it_; return *this;}
	constexpr BidirectionalIterator operator++(int)
	{BidirectionalIterator tmp(*this); ++(*this); return tmp;}

	constexpr BidirectionalIterator& operator--() {--it_; return *this;}
	constexpr BidirectionalIterator operator--(int)
	{BidirectionalIterator tmp(*this); --(*this); return tmp;}
};

template <class T, class U>
constexpr
bool
operator==(const BidirectionalIterator<T>& x, const BidirectionalIterator<U>& y)
{
	return x.base() == y.base();
}

template <class T, class U>
constexpr
bool
operator!=(const BidirectionalIterator<T>& x, const BidirectionalIterator<U>& y)
{
	return !(x == y);
}

template <class It>
class RandomAccessIterator
{
	It it_;

	template <class U> friend class RandomAccessIterator;
public:
	typedef std::random_access_iterator_tag iterator_category;
	typedef tcb::rng::iter_value_t<It>              value_type;
	typedef tcb::rng::iter_difference_t<It>         difference_type;
	typedef It                               pointer;
	typedef tcb::rng::iter_reference_t<It>          reference;

	constexpr It base() const {return it_;}

	constexpr RandomAccessIterator() : it_() {}
	constexpr explicit RandomAccessIterator(It it) : it_(it) {}
	template <class U>
	  //requires nanorange::ConvertibleTo<U, It>
	constexpr RandomAccessIterator(const RandomAccessIterator<U>& u) :it_(u.it_) {}

	constexpr reference operator*() const {return *it_;}
	constexpr pointer operator->() const {return it_;}

	constexpr RandomAccessIterator& operator++() {++it_; return *this;}
	constexpr RandomAccessIterator operator++(int)
	{RandomAccessIterator tmp(*this); ++(*this); return tmp;}

	constexpr RandomAccessIterator& operator--() {--it_; return *this;}
	constexpr RandomAccessIterator operator--(int)
	{RandomAccessIterator tmp(*this); --(*this); return tmp;}

	constexpr
	RandomAccessIterator& operator+=(difference_type n) {it_ += n; return *this;}
	constexpr
	RandomAccessIterator operator+(difference_type n) const
	{RandomAccessIterator tmp(*this); tmp += n; return tmp;}
	constexpr
	friend RandomAccessIterator operator+(difference_type n, RandomAccessIterator x)
	{x += n; return x;}
	constexpr
	RandomAccessIterator& operator-=(difference_type n) {return *this += -n;}
	constexpr
	RandomAccessIterator operator-(difference_type n) const
	{RandomAccessIterator tmp(*this); tmp -= n; return tmp;}

	constexpr
	reference operator[](difference_type n) const {return it_[n];}
};

template <class T, class U>
constexpr
bool
operator==(const RandomAccessIterator<T>& x, const RandomAccessIterator<U>& y)
{
	return x.base() == y.base();
}

template <class T, class U>
constexpr
bool
operator!=(const RandomAccessIterator<T>& x, const RandomAccessIterator<U>& y)
{
	return !(x == y);
}

template <class T, class U>
constexpr
bool
operator<(const RandomAccessIterator<T>& x, const RandomAccessIterator<U>& y)
{
	return x.base() < y.base();
}

template <class T, class U>
constexpr
bool
operator<=(const RandomAccessIterator<T>& x, const RandomAccessIterator<U>& y)
{
	return !(y < x);
}

template <class T, class U>
constexpr
bool
operator>(const RandomAccessIterator<T>& x, const RandomAccessIterator<U>& y)
{
	return y < x;
}

template <class T, class U>
constexpr
bool
operator>=(const RandomAccessIterator<T>& x, const RandomAccessIterator<U>& y)
{
	return !(x < y);
}

template <class T, class U>
constexpr
tcb::rng::iter_difference_t<T>
operator-(const RandomAccessIterator<T>& x, const RandomAccessIterator<U>& y)
{
	return x.base() - y.base();
}

template <typename It, bool Sized = false>
struct sentinel_type
{
	using type = It;
};

template <typename T, bool Sized>
struct sentinel_type<T*, Sized>
{
	using type = Sentinel<T*, Sized>;
};

template <template <typename> class I, typename It, bool Sized>
struct sentinel_type<I<It>, Sized>
{
	using type = Sentinel<It, Sized>;
};

#endif  // ITERATORS_H
