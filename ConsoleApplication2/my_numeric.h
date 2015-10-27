#ifndef __MY_NEMERIC_H_
#define __MY_NEMERIC_H_

#include "my_iterator.h"
#include "my_xutility.h"

namespace my_stl
{
	//------------------------------------------------------------------
	//Numeric operations 
	//------------------------------------------------------------------

	//------------------------------------------------------------------
	//iota
	//以value为初值，使用++value，递增填充[first, last)

	template< class ForwardIterator, class T >
	void iota(ForwardIterator first, ForwardIterator last, T value)
	{
		while (first != last)
			*first++ = value++;
	}

	//------------------------------------------------------------------
	//accumulate
	//以初值init累积[first, last)中的元素，默认使用operator+累积
	//返回累积值

	template< class InputIt, class T >
	T accumulate(InputIt first, InputIt last, T init)
	{
		while (first != last)
			init = init + *first++;
		return init;
	}

	//以初值init累积[first, last)中的元素，使用op(sum, current_value)累积
	//返回累积值

	template< class InputIt, class T, class BinaryOperation >
	T accumulate(InputIt first, InputIt last, T init,
		BinaryOperation op)
	{
		while (first != last)
			init = op(init, *first++);
		return init;
	}

	//------------------------------------------------------------------
	//inner_product
	//以value为初值，将[first1, last1)和[first2, ...)对应元素operator*的结果用operator+累积到value.
	//返回value.

	template< class InputIt1, class InputIt2, class T >
	T inner_product(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, T value)
	{
		while (first1 != last1)
			value = value + (*first1++) * (*first2++);
		return value;
	}

	//以value为初值，将[first1, last1)和[first2, ...)对应元素op2运算的结果用op1运算累积到value.
	//返回value.
	template<class InputIt1, class InputIt2, class T,
	class BinaryOperation1, class BinaryOperation2>
		T inner_product(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, T value,
		BinaryOperation1 op1,
		BinaryOperation2 op2)
	{
		while (first1 != last1)
			value = op1(value, op2(*first1++, *first2++));
		return value;
	}

	//--------------------------------------------------------------
	//adjacent_difference
	//计算[first, last)中每组相邻元素的差，保存到[d_first,...)，
	//d_first元素与first元素相同，默认使用operator-
	//返回尾后迭代器

	template< class InputIt, class OutputIt >
	OutputIt adjacent_difference(InputIt first, InputIt last,
		OutputIt d_first)
	{
		if (first == last)
			return d_first;
		InputIt pre = first;
		typedef typename iterator_traits<InputIt>::value_type value_type;
		value_type pre_value = *pre;
		*d_first++ = pre_value;
		while (++first != last)
		{
			value_type value = *first;
			*d_first++ = value - pre_value;
			pre_value = std::move(value);
		}
		return d_first;
	}

	//使用op计算相邻元素的差
	//返回尾后迭代器

	template< class InputIt, class OutputIt, class BinaryOperation >
	OutputIt adjacent_difference(InputIt first, InputIt last,
		OutputIt d_first, BinaryOperation op)
	{
		if (first == last)
			return d_first;
		InputIt pre = first;
		typedef typename iterator_traits<InputIt>::value_type value_type;
		value_type pre_value = *pre;
		*d_first++ = pre_value;
		while (++first != last)
		{
			value_type value = *first;
			*d_first++ = op(value, pre_value);
			pre_value = std::move(value);
		}
		return d_first;
	}

	//----------------------------------------------------------
	//partial_sum
	//使用operator+，计算累积和，返回尾后迭代器

	template< class InputIt, class OutputIt >
	OutputIt partial_sum(InputIt first, InputIt last, OutputIt d_first)
	{
		if (first == last)
			return d_first;
		typename iterator_traits<InputIt>::value_type value = *first;
		*d_first++ = value;
		while (++first != last)
		{
			value = value + *first;
			*d_first++ = value;
		}
		return d_first;
	}

	//使用op计算累积和，返回尾后迭代器

	template< class InputIt, class OutputIt, class BinaryOperation >
	OutputIt partial_sum(InputIt first, InputIt last, OutputIt d_first,
		BinaryOperation op)
	{
		if (first == last)
			return d_first;
		typename iterator_traits<InputIt>::value_type value = *first;
		*d_first++ = value;
		while (++first != last)
		{
			value = op(value, *first);
			*d_first++ = value;
		}
		return d_first;
	}
}


#endif