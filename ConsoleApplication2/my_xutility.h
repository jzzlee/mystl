﻿#ifndef __MY_XUTILITY_H_
#define __MY_XUTILITY_H_

#include "my_iterator.h"

namespace my_stl
{
	template <class InputIterator1, class InputIterator2>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2)
	{
		while (first1 != last1)
		{
			if (first2 == last2 || *first2 < *first1)
				return false;
			else if (*first1 < *first2)
				return true;
			++first1;
			++first2;
		}
		return (first2 != last2);
	}


	//底层fill函数
	template<class ForwardIt, class T>
	ForwardIt fill(ForwardIt first, ForwardIt last, const T& value)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		ForwardIt current = first;
		for (; current != last; ++current) {
			::new (static_cast<void*>(my_stl::addressof(*current))) Value(value); //use placement new to initialize the object
		}
		return current;
	}

	//底层的fill_n函数
	template< class ForwardIt, class Size, class T >
	ForwardIt fill_n(ForwardIt first, Size count, const T& value)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		ForwardIt current = first;

		for (; count > 0; ++current, (void) --count) {
			::new (static_cast<void*>(my_stl::addressof(*current))) Value(value); //use placement new to initialize the object
		}
		return current;
	}

	//真正的copy
	template< class InputIt, class ForwardIt >
	ForwardIt copy(InputIt first, InputIt last, ForwardIt d_first)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		ForwardIt current = d_first;
		for (; first != last; ++first, ++current) {
			::new (static_cast<void*>(my_stl::addressof(*current))) Value(*first); //use placement new to initialize the object
		}
		return current;
	}

	//真正的copy_n函数
	template<class InputIt, class Size, class ForwardIt>
	ForwardIt copy_n(InputIt first, Size count, ForwardIt d_first)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		ForwardIt current = d_first;
		for (; count > 0; ++first, ++current, --count) {
			::new (static_cast<void*>(my_stl::addressof(*current))) Value(*first); //use placement new to initialize the object
		}
		return current;
	}
	
	//真正的copy
	template< class InputIt, class ForwardIt >
	ForwardIt copy_backward(InputIt first, InputIt last, ForwardIt d_first)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		ptrdiff_t offset = my_stl::distance(first, last);
		my_stl::advance(d_first, offset);
		ForwardIt current = d_first;
		--first;
		for (--last; last != first; --last, --current) {
			::new (static_cast<void*>(my_stl::addressof(*current))) Value(*last); //use placement new to initialize the object
		}
		return current;
	}

	//检查[first, last)区间的元素是否与d_first开始的对应元素相等
	template<typename InputIt, typename ForwardIt>
	bool equal(InputIt first, InputIt last, ForwardIt d_first)
	{
		for (; first != last; ++first, ++d_first)
		{
			if (*first != *d_first)
				return false;
		}
		return true;
	}

	//在[first, last)范围内查找value，若找到，返回所在位置；否则，返回last
	template<typename InputIt, typename T>
	InputIt find(InputIt first, InputIt last, const T &value)
	{
		for (; first != last; ++first)
		{
			if (*first == value)
				return first;
		}
		return last;
	}

}
#endif
