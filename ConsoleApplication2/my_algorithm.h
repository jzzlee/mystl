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

	//是否[first,last)中的所有元素都使p为真
	template< class InputIt, class UnaryPredicate >
	bool all_of(InputIt first, InputIt last, UnaryPredicate p)
	{
		return my_stl::find_if_not(first, last, p) == last;
	}
	
	//是否[first,last)中至少有1个元素都使p为真
	template< class InputIt, class UnaryPredicate >
	bool any_of(InputIt first, InputIt last, UnaryPredicate p)
	{
		return my_stl::find_if(first, last, p) != last;
	}
	
	//是否[first,last)中的没有元素使p为真
	template< class InputIt, class UnaryPredicate >
	bool none_of(InputIt first, InputIt last, UnaryPredicate p)
	{
		return my_stl::find_if(first, last, p) == last;
	}

	//对[first, last)中的每一个元素，进行f操作
	template< class InputIt, class UnaryFunction >
	UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f)
	{
		for (; first != last; ++first)
			*first = f(*first);
		return f;
	}

	//统计[first, last)范围内value出现的次数
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

	
	//统计[first, last)范围内满足p的value出现的次数
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

	//查找[first1, last1)和[first2, ...)的第一组不相等的位置
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
	
	//查找[first1, last1)和[first2, ...)的第一组不能使p(value1) == p(value2)为真的位置
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
	
	//查找[first1, last1)和[first2, last2)的第一组不相等的位置
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
	
	//查找[first1, last1)和[first2, last2)的第一组不能使p(value1) == p(value2)为真的位置
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

	//返回[s_first, s_last)在[first, last)中出现的首位置，使用==比较元素，如果未出现，则返回last. 
	template< class ForwardIt1, class ForwardIt2 >
	ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,
		ForwardIt2 s_first, ForwardIt2 s_last)
	{
		for (; ; ++first)
		{
			ForwardIt1 it = first;
			for (ForwardIt2 s_it = s_first; ; ++it, ++s_it)
			{
				if (s_it == s_last) //匹配成功
					return first;
				if (it == last) //匹配失败
					return last;
				if (!(*it == *s_it))//当前匹配失败
					break;
			}
		}
	}
	
	//返回[s_first, s_last)在[first, last)中出现的首位置,使用p()比较元素，如果未出现，则返回last. 
	template< class ForwardIt1, class ForwardIt2, class BinaryPredicate >
	ForwardIt1 search(ForwardIt1 first, ForwardIt1 last,
		ForwardIt2 s_first, ForwardIt2 s_last, BinaryPredicate p)
	{
		for (; ; ++first)
		{
			ForwardIt1 it = first;
			for (ForwardIt2 s_it = s_first; ; ++it, ++s_it)
			{
				if (s_it == s_last) //匹配成功
					return first;
				if (it == last) //匹配失败
					return last;
				if (!p(*it, *s_it))//当前匹配失败
					break;
			}
		}
	}
	
	//查找连续count个value出现的位置
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

	//查找连续count个使p(v, value)为真的值出现的位置,
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
				if (p(*it, value))
					break;
				if (n == count - 1)
					return first;
			}
		}
	}
	
	//在[first, last)中寻找最后一个匹配[s_first, s_last)的序列，使用operator==检查是否匹配
	//若找到，返回序列首位置；否则，返回last.	
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
	
	//在[first, last)中寻找最后一个匹配[s_first, s_last)的序列，使用p(v1, v2)检查是否匹配
	//若找到，返回序列首位置；否则，返回last.
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
	
	//从[first, last)中寻找[s_first, s_last)中的任意元素，返回出现的位置
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

	//使用p来检查元素是否匹配.
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

	//在[first, last)中查找两个连续相同元素
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

	//使用p来检查元素是否匹配.
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
