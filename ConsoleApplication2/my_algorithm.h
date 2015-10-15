#ifndef __MY_ALGORITHM_H_
#define __MY_ALGORITHM_H_

#include "my_heap.h"
#include "my_xutility.h"
#include <iostream>

namespace my_stl
{
	using std::pair;

	///////////////////////////////////////////////
	//Non-modifying sequence operations 

	//�Ƿ�[first,last)�е�����Ԫ�ض�ʹpΪ��
	template< class InputIt, class UnaryPredicate >
	bool all_of(InputIt first, InputIt last, UnaryPredicate p)
	{
		return my_stl::find_if_not(first, last, p) == last;
	}
	
	//�Ƿ�[first,last)��������1��Ԫ�ض�ʹpΪ��
	template< class InputIt, class UnaryPredicate >
	bool any_of(InputIt first, InputIt last, UnaryPredicate p)
	{
		return my_stl::find_if(first, last, p) != last;
	}
	
	//�Ƿ�[first,last)�е�û��Ԫ��ʹpΪ��
	template< class InputIt, class UnaryPredicate >
	bool none_of(InputIt first, InputIt last, UnaryPredicate p)
	{
		return my_stl::find_if(first, last, p) == last;
	}

	//��[first, last)�е�ÿһ��Ԫ�أ�����f����
	template< class InputIt, class UnaryFunction >
	UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f)
	{
		for (; first != last; ++first)
			*first = f(*first);
		return f;
	}

	//ͳ��[first, last)��Χ��value���ֵĴ���
	template< class InputIt, class T >
	typename iterator_traits<InputIt>::difference_type
		count(InputIt first, InputIt last, const T &value)
	{
		iterator_traits<InputIt>::difference_type n = 0;
		for (; first != last; ++first)
		{
			if (*first == value)
				++n;
		}
		return n;
	}

	
	//ͳ��[first, last)��Χ������p��value���ֵĴ���
	template< class InputIt, class UnaryPredicate >
	typename iterator_traits<InputIt>::difference_type
		count_if(InputIt first, InputIt last, UnaryPredicate p)
	{
		iterator_traits<InputIt>::difference_type n = 0;
		for (; first != last; ++first)
		{
			if (p(*first))
				++n;
		}
		return n;
	}

	//����[first1, last1)��[first2, ...)�ĵ�һ�鲻��ȵ�λ��
	template< class InputIt1, class InputIt2 >
	pair<InputIt1, InputIt2>
		mismatch(InputIt1 first1, InputIt1 last1,
		InputIt2 first2)
	{
		while (first1 != last1 && *first1 == *first2)
		{
			++first1;
			++first2;
		}
		return pair<InputIt1, InputIt2>(first1, first2);
	}
	
	//����[first1, last1)��[first2, ...)�ĵ�һ�鲻��ʹp(value1) == p(value2)Ϊ���λ��
	template< class InputIt1, class InputIt2, class BinaryPredicate >
	pair<InputIt1, InputIt2>
		mismatch(InputIt1 first1, InputIt1 last1,
		InputIt2 first2,
		BinaryPredicate p)
	{
		while (first1 != last1 && p(*first1, *first2))
		{
			++first1;
			++first2;
		}
		return pair<InputIt1, InputIt2>(first1, first2);
	}
	
	//����[first1, last1)��[first2, last2)�ĵ�һ�鲻��ȵ�λ��
	template< class InputIt1, class InputIt2 >
	pair<InputIt1, InputIt2>
		mismatch(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2)
	{
		while (first1 != last1 && first2 != last2 && *first1 == *first2)
		{
			++first1;
			++first2;
		}
		return pair<InputIt1, InputIt2>(first1, first2);
	}
	
	//����[first1, last1)��[first2, last2)�ĵ�һ�鲻��ʹp(value1) == p(value2)Ϊ���λ��
	template< class InputIt1, class InputIt2, class BinaryPredicate >
	pair<InputIt1, InputIt2>
		mismatch(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		BinaryPredicate p)
	{
		while (first1 != last1 && first2 != last2 && p(*first1, *first2))
		{
			++first1;
			++first2;
		}
		return pair<InputIt1, InputIt2>(first1, first2);
	}

	//����[s_first, s_last)��[first, last)�г��ֵ���λ�ã�ʹ��==�Ƚ�Ԫ�أ����δ���֣��򷵻�last. 
	template< class ForwardIt1, class ForwardIt2 >
	ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,
		ForwardIt2 s_first, ForwardIt2 s_last)
	{
		for (; ; ++first)
		{
			ForwardIt1 it = first;
			for (ForwardIt2 s_it = s_first; ; ++it, ++s_it)
			{
				if (s_it == s_last) //ƥ��ɹ�
					return first;
				if (it == last) //ƥ��ʧ��
					return last;
				if (!(*it == *s_it))//��ǰƥ��ʧ��
					break;
			}
		}
	}
	
	//����[s_first, s_last)��[first, last)�г��ֵ���λ��,ʹ��p()�Ƚ�Ԫ�أ����δ���֣��򷵻�last. 
	template< class ForwardIt1, class ForwardIt2, class BinaryPredicate >
	ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,
		ForwardIt2 s_first, ForwardIt2 s_last, BinaryPredicate p)
	{
		for (; ; ++first)
		{
			ForwardIt1 it = first;
			for (ForwardIt2 s_it = s_first; ; ++it, ++s_it)
			{
				if (s_it == s_last) //ƥ��ɹ�
					return first;
				if (it == last) //ƥ��ʧ��
					return last;
				if (!p(*it, *s_it))//��ǰƥ��ʧ��
					break;
			}
		}
	}

	template< class ForwardIt, class Size, class T >
	ForwardIt search_n(ForwardIt first, ForwardIt last, Size count, const T& value)
	{
		for (;; ++first)
		{
			ForwardIt it = first;
			for (Size n = 0; n != count; ++n, ++it)
			{
				if (it == last)
					return last;
				if (!(*it == value))
					break;
				if (n == count - 1)
					return first;
			}
		}
	}

	template< class ForwardIt, class Size, class T, class BinaryPredicate >
	ForwardIt search_n(ForwardIt first, ForwardIt last, Size count, const T& value,
		BinaryPredicate p)
	{
		for (;; ++first)
		{
			ForwardIt it = first;
			for (Size n = 0; n != count; ++n, ++it)
			{
				if (it == last)
					return last;
				if (p(*it value))
					break;
				if (n == count - 1)
					return first;
			}
		}
	}

	//��[first, last)��Ѱ�����һ��ƥ��[s_first, s_last)�����У�ʹ��operator==����Ƿ�ƥ��
	//���ҵ�������������λ�ã����򣬷���last.
	template< class ForwardIt1, class ForwardIt2 >
	ForwardIt1 find_end(ForwardIt1 first, ForwardIt1 last,
		ForwardIt2 s_first, ForwardIt2 s_last)
	{
		if (s_first == s_last)
			return last;
		ForwardIt1 result = last;
		while (1) 
		{
			ForwardIt1 new_result = search(first, last, s_first, s_last);
			if (new_result == last) {
				return result;
			}
			else 
			{
				result = new_result;
				first = result;
				++first;
			}
		}
		return result;
	}
	
	//��[first, last)��Ѱ�����һ��ƥ��[s_first, s_last)�����У�ʹ��p(v1, v2)����Ƿ�ƥ��
	//���ҵ�������������λ�ã����򣬷���last.
	template< class ForwardIt1, class ForwardIt2, class BinaryPredicate >
	ForwardIt1 find_end(ForwardIt1 first, ForwardIt1 last,
		ForwardIt2 s_first, ForwardIt2 s_last, BinaryPredicate p)
	{
		if (s_first == s_last)
			return last;
		ForwardIt1 result = last;
		while (1)
		{
			ForwardIt1 new_result = search(first, last, s_first, s_last, p);
			if (new_result == last) {
				return result;
			}
			else
			{
				result = new_result;
				first = result;
				++first;
			}
		}
		return result;
	}

	//��[first, last)��Ѱ��[s_first, s_last)�е�����Ԫ�أ����س��ֵ�λ��
	template< class InputIt, class ForwardIt >
	InputIt find_first_of(InputIt first, InputIt last,
		ForwardIt s_first, ForwardIt s_last)
	{
		for (; first != last; ++first)
		{
			for (ForwardIt it = s_first; it != s_last; ++it)
			{
				if (*first == *it)
					return first;
			}
		}
		return last;
	}

	//ʹ��p�����Ԫ���Ƿ�ƥ��.
	template< class InputIt, class ForwardIt, class BinaryPredicate >
	InputIt find_first_of(InputIt first, InputIt last,
		ForwardIt s_first, ForwardIt s_last, BinaryPredicate p)
	{
		for (; first != last; ++first)
		{
			for (ForwardIt it = s_first; it != s_last; ++it)
			{
				if (p(*first, *it))
					return first;
			}
		}
		return last;
	}

	//��[first, last)�в�������������ͬԪ��
	template< class ForwardIt >
	ForwardIt adjacent_find(ForwardIt first, ForwardIt last)
	{
		if (first == last)
			return last;
		for (ForwardIt before = first++; first != last; ++first, ++before)
		{
			if (*before == *first)
				return before;
		}
		return last;
	}

	//ʹ��p�����Ԫ���Ƿ�ƥ��.
	template< class ForwardIt, class BinaryPredicate>
	ForwardIt adjacent_find(ForwardIt first, ForwardIt last, BinaryPredicate p)
	{
		if (first == last)
			return last;
		for (ForwardIt before = first++; first != last; ++first, ++before)
		{
			if (p(*before, *first))
				return before;
		}
		return last;
	}



}

#endif