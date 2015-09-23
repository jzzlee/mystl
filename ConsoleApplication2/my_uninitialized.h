///////////////////////////////////////////////////////////////////
//定义了4个内存基本处理工具
//uninitialized_copy, uninitialized_copy_n, uninitialized_fill and uninitialized_fill_n.
///////////////////////////////////////////////////////////////////
#ifndef __MY_UNINITIALIZED_H_
#define __MY_UNINITIALIZED_H_

#include <iostream>
#include <algorithm>
#include "my_type_traits.h"

namespace my_stl
{
	//Obtains the actual address of the object or function arg, even in presence of overloaded operator&
	template< class T >
	T* addressof(T& arg)
	{
		return reinterpret_cast<T*>(
			&const_cast<char&>(
			reinterpret_cast<const volatile char&>(arg)));
	}


	//真正的copy,如果元素是POD类型
	template< class InputIt, class ForwardIt >
	ForwardIt __uninitialized_copy_aux(InputIt first, InputIt last, ForwardIt d_first, __true_type)
	{
		return my_stl::copy(first, last, d_first);
	}

	//真正的copy,如果元素不是POD类型
	template< class InputIt, class ForwardIt >
	ForwardIt __uninitialized_copy_aux(InputIt first, InputIt last, ForwardIt d_first, __false_type)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		ForwardIt current = d_first;
		try {
			for (; first != last; ++first, ++current) {
				::new (static_cast<void*>(my_stl::addressof(*current))) Value(*first); //use placement new to initialize the object
			}
			return current;
		}
		catch (...) {
			for (; d_first != current; ++d_first) {
				d_first->~Value();
			}
			throw;
		}
	}

	//Copies elements from the range [first, last) to an uninitialized memory area beginning at d_first
	template< class InputIt, class ForwardIt >
	ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		typedef typename __type_traits<Value>::is_POD_type is_POD;
		return __uninitialized_copy_aux(first, last, d_first, is_POD());
	}

	//真正的copy n函数，如果是POD类型，直接内存复制
	template<class InputIt, class Size, class ForwardIt>
	ForwardIt __uninitialized_copy_n_aux(InputIt first, Size count, ForwardIt d_first, __true_type)
	{
		return my_stl::copy_n(first, count, d_first);
	}

	//真正的copy n函数，如果不是POD类型，必须一个一个元素构造
	template<class InputIt, class Size, class ForwardIt>
	ForwardIt __uninitialized_copy_n_aux(InputIt first, Size count, ForwardIt d_first, __false_type)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		ForwardIt current = d_first;
		try {
			for (; count > 0; ++first, ++current, --count) {
				::new (static_cast<void*>(my_stl::addressof(*current))) Value(*first); //use placement new to initialize the object
			}
		}
		catch (...) {
			for (; d_first != current; ++d_first) {
				d_first->~Value();
			}
			throw;
		}
		return current;
	}

	//Copies count elements from a range beginning at first to an uninitialized memory area beginning at d_first
	template<class InputIt, class Size, class ForwardIt>
	ForwardIt uninitialized_copy_n(InputIt first, Size count, ForwardIt d_first)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		typedef typename __type_traits<Value>::is_POD_type is_POD;
		return __uninitialized_copy_n_aux(first, count, d_first, is_POD());
	}

	//底层fill函数，如果是POD类型，直接内存填充
	template<class ForwardIt, class T>
	void __uninitialized_fill__aux(ForwardIt first, ForwardIt last, const T& value, __true_type)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		my_stl::fill(first, last, Value(value));
	}

	//底层fill函数, 如果不是POD类型，则需要一个一个元素构造填充
	template<class ForwardIt, class T>
	void __uninitialized_fill__aux(ForwardIt first, ForwardIt last, const T& value, __false_type)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		ForwardIt current = first;
		try {
			for (; current != last; ++current) {
				::new (static_cast<void*>(my_stl::addressof(*current))) Value(value); //use placement new to initialize the object
			}
		}
		catch (...) {
			for (; first != current; ++first) {
				first->~Value();
			}
			throw;
		}
	}

	//Copies the given value to an uninitialized memory area, defined by the range [first, last)
	template<class ForwardIt, class T>
	void uninitialized_fill(ForwardIt first, ForwardIt last, const T& value)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		typedef typename __type_traits<Value>::is_POD_type is_POD;
		__uninitialized_fill__aux(first, last, value, is_POD());
	}

	//底层的fill_n函数，如果是POD类型，直接内存填充
	template< class ForwardIt, class Size, class T >
	ForwardIt __uninitialized_fill_n_aux(ForwardIt first, Size count, const T& value, __true_type)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		return my_stl::fill_n(first, count, Value(value));
	}

	//底层的fill_n函数，如果不是POD类型，则必须一个一个元素的构造填充
	template< class ForwardIt, class Size, class T >
	ForwardIt __uninitialized_fill_n_aux(ForwardIt first, Size count, const T& value, __false_type)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		ForwardIt current = first;
		try {
			for (; count > 0; ++current, (void) --count) {
				::new (static_cast<void*>(my_stl::addressof(*current))) Value(value); //use placement new to initialize the object
			}
			return current;
		}
		catch (...) {
			for (; first != current; ++first) {
				first->~Value();
			}
			throw;
		}
	}

	//Copies the given value value to the first count elements in an uninitialized memory area beginning at first
	template< class ForwardIt, class Size, class T >
	ForwardIt uninitialized_fill_n(ForwardIt first, Size count, const T& value)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		typedef typename __type_traits<Value>::is_POD_type is_POD;
		return __uninitialized_fill_n_aux(first, count, value, is_POD());
		
	}
}


#endif