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
	//��valueΪ��ֵ��ʹ��++value���������[first, last)

	template< class ForwardIterator, class T >
	void iota(ForwardIterator first, ForwardIterator last, T value)
	{
		while (first != last)
			*first++ = value++;
	}

	//------------------------------------------------------------------
	//accumulate
	//�Գ�ֵinit�ۻ�[first, last)�е�Ԫ�أ�Ĭ��ʹ��operator+�ۻ�
	//�����ۻ�ֵ

	template< class InputIt, class T >
	T accumulate(InputIt first, InputIt last, T init)
	{
		while (first != last)
			init = init + *first++;
		return init;
	}

	//�Գ�ֵinit�ۻ�[first, last)�е�Ԫ�أ�ʹ��op(sum, current_value)�ۻ�
	//�����ۻ�ֵ

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
	//��valueΪ��ֵ����[first1, last1)��[first2, ...)��ӦԪ��operator*�Ľ����operator+�ۻ���value.
	//����value.

	template< class InputIt1, class InputIt2, class T >
	T inner_product(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, T value)
	{
		while (first1 != last1)
			value = value + (*first1++) * (*first2++);
		return value;
	}

	//��valueΪ��ֵ����[first1, last1)��[first2, ...)��ӦԪ��op2����Ľ����op1�����ۻ���value.
	//����value.
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
	//����[first, last)��ÿ������Ԫ�صĲ���浽[d_first,...)��
	//d_firstԪ����firstԪ����ͬ��Ĭ��ʹ��operator-
	//����β�������

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

	//ʹ��op��������Ԫ�صĲ�
	//����β�������

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
	//ʹ��operator+�������ۻ��ͣ�����β�������

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

	//ʹ��op�����ۻ��ͣ�����β�������

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