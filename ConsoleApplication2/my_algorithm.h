#ifndef __MY_ALGORITHM_H_
#define __MY_ALGORITHM_H_

#include "my_heap.h"
#include "my_xutility.h"
#include <iostream>
#include <random>
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

	//----------------------------------------------------------
	//transform
	template< class InputIt, class OutputIt, class UnaryOperation >
	OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first,
		UnaryOperation unary_op)
	{
		while (first1 != last1) 
			*d_first++ = unary_op(*first1++);
		return d_first;
	}

	template< class InputIt1, class InputIt2, class OutputIt, class BinaryOperation >
	OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2,
		OutputIt d_first, BinaryOperation binary_op)
	{
		while (first1 != last1)

			*d_first++ = binary_op(*first1++, *first2++);
		return d_first;
	}

	//----------------------------------------------------------
	//generate
	//Assigns each element in range [first, last) a value generated by the given function object g. 
	template< class ForwardIt, class Generator >
	void generate(ForwardIt first, ForwardIt last, Generator g)
	{
		while (first != last) 
			*first++ = g();
	}
	template< class OutputIt, class Size, class Generator >
	OutputIt generate_n(OutputIt first, Size count, Generator g)
	{
		while (count--)
			*first++ = g();
		return first;
	}

	//----------------------------------------------------------
	//remove
	template< class ForwardIt, class T >
	ForwardIt remove(ForwardIt first, ForwardIt last, const T& value)
	{
		first = my_stl::find(first, last, value);
		if (first != last)
		{
			for (ForwardIt it = first; ++it != last;)
				if (!(*it == value))
					*first++ = std::move(*it);
		}
		return first;
	}
	template< class ForwardIt, class UnaryPredicate >
	ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate p)
	{
		first = my_stl::find_if(first, last, p);
		if (first != last)
		{
			for (ForwardIt it = first; ++it != last;)
				if (!p(*it))
					*first++ = std::move(*it);
		}
		return first;
	}

	//----------------------------------------------------------
	//remove_copy
	//复制到d_first位置，除去满足条件的元素
	template< class InputIt, class OutputIt, class T >
	OutputIt remove_copy(InputIt first, InputIt last, OutputIt d_first,
		const T& value)
	{
		for (; first != last; ++first)
		{
			if (!(*first == value))
				*d_first++ = *first;
		}
		return d_first;
	}
	
	template< class InputIt, class OutputIt, class UnaryPredicate >
	OutputIt remove_copy_if(InputIt first, InputIt last, OutputIt d_first,
		UnaryPredicate p)
	{
		for (; first != last; ++first)
		{
			if (!p(*first))
				*d_first++ = *first;
		}
		return d_first;
	}

	//----------------------------------------------------------
	//replace
	template< class ForwardIt, class T >
	void replace(ForwardIt first, ForwardIt last,
		const T& old_value, const T& new_value)
	{
		for (; first != last; ++first)
		{
			if (*first == old_value)
				*first = new_value;
		}
	}
	
	template< class ForwardIt, class UnaryPredicate, class T >
	void replace_if(ForwardIt first, ForwardIt last,
		UnaryPredicate p, const T& new_value)
	{
		for (; first != last; ++first)
			if (p(*first))
				*first = new_value;
	}

	//----------------------------------------------------------
	//replace_copy
	template< class InputIt, class OutputIt, class T >
	OutputIt replace_copy(InputIt first, InputIt last, OutputIt d_first,
		const T& old_value, const T& new_value)
	{
		for (; first != last; ++first)
		{
			if (*first == old_value)
				*d_first++ = new_value;
			else
				*d_first++ = *first;
		}
		return d_first;
	}
	template< class InputIt, class OutputIt, class UnaryPredicate, class T >
	OutputIt replace_copy_if(InputIt first, InputIt last, OutputIt d_first,
		UnaryPredicate p, const T& new_value)
	{
		for (; first != last; ++first)
			*d_first++ = p(*first) ? new_value : *first;
		return d_first;
	}

	//----------------------------------------------------------
	//reverse
	template< class BidirIt >
	void reverse(BidirIt first, BidirIt last)
	{
		while ((first != last) && (first != --last))
			my_stl::iter_swap(first, last);
	}

	template< class BidirIt, class OutputIt >
	OutputIt reverse_copy(BidirIt first, BidirIt last, OutputIt d_first)
	{
		while (first != last)
			*d_first++ = *(--last);
		return d_first;
	}

	//Performs a left rotation on a range of elements.
	template< class ForwardIt >
	ForwardIt rotate(ForwardIt first, ForwardIt n_first, ForwardIt last)
	{
		ForwardIt next = n_first;
		while (first != last)
		{
			my_stl::iter_swap(first++, next++);
			if (next == last)
				next = n_first;
			else if (first == n_first)
				n_first = next;
		}
		return next;
	}
	template< class ForwardIt, class OutputIt >
	OutputIt rotate_copy(ForwardIt first, ForwardIt n_first,
		ForwardIt last, OutputIt d_first)
	{
		d_first = my_stl::copy(n_first, last, d_first);
		return my_stl::copy(first, n_first, d_first);
	}

	//-------------------------------------------------------------------
	//shuffle
	template< class RandomIt >
	void random_shuffle(RandomIt first, RandomIt last)
	{
		typename iterator_traits<RandomIt>::difference_type i, n;
		n = last - first;
		for (i = n - 1; i > 0; --i) 
		{
			using std::swap;
			swap(first[i], first[std::rand() % (i + 1)]);
		}
	}
	template<class RandomIt, class RandomFunc>
	void random_shuffle(RandomIt first, RandomIt last, RandomFunc&& r)
	{
		std::random_shuffle(first, last, r);
	}
	template<class RandomIt, class UniformRandomNumberGenerator>
	void shuffle(RandomIt first, RandomIt last,
		UniformRandomNumberGenerator&& g)
	{
		std::shuffle(first, last, g);
	}

	//------------------------------------------------------------------
	//unique
	template< class ForwardIt >
	ForwardIt unique(ForwardIt first, ForwardIt last)
	{
		if (first == last)
			return last;

		ForwardIt result = first;
		while (++first != last) 
		{
			if (!(*result == *first) && ++result != first) 
			{
				*result = std::move(*first);
			}
		}
		return ++result;
	}
	
	template< class ForwardIt, class BinaryPredicate >
	ForwardIt unique(ForwardIt first, ForwardIt last, BinaryPredicate p)
	{
		if (first == last)
			return last;

		ForwardIt result = first;
		while (++first != last)
		{
			if (!p(*result, *first) && ++result != first)
			{
				*result = std::move(*first);
			}
		}
		return ++result;
	}

	//------------------------------------------------------------------
	//unique_copy
	template< class InputIt, class OutputIt >
	OutputIt unique_copy(InputIt first, InputIt last,
		OutputIt d_first)
	{
		if (first == last)
			return d_first;
		auto value = *first;
		*d_first = value;
		while (++first != last)
		{
			if (!(value == *first))
			{
				value = *first;
				*(++d_first) = value;
			}
		}
		return ++d_first;
	}
	template< class InputIt, class OutputIt, class BinaryPredicate >
	OutputIt unique_copy(InputIt first, InputIt last,
		OutputIt d_first, BinaryPredicate p)
	{
		if (first == last)
			return d_first;
		auto value = *first;
		*d_first = value;
		while (++first != last)
		{
			if (!p(value, *first))
			{
				value = *first;
				*(++d_first) = value;
			}
		}
		return ++d_first;
	}

	//------------------------------------------------------------------
	//partition
	template< class InputIt, class UnaryPredicate >
	bool is_partitioned(InputIt first, InputIt last, UnaryPredicate p)
	{
		for (; first != last; ++first)
			if (!p(*first))
				break;
		for (; first != last; ++first)
			if (p(*first))
				return false;
		return true;
	}

	template< class ForwardIt, class UnaryPredicate >
	ForwardIt partition(ForwardIt first, ForwardIt last, UnaryPredicate p)
	{
		if (first == last)
			return last;
		ForwardIt part(first++);
		while(first != last)
		{
			if (p(*part))
				++part;
			else if (p(*first))
			{
				my_stl::iter_swap(part, first);
				++part;
			}
			++first;
		}
		return part;
	}

	template< class InputIt, class OutputIt1, class OutputIt2, class UnaryPredicate >
	pair<OutputIt1, OutputIt2>
		partition_copy(InputIt first, InputIt last, OutputIt1 d_first_true, OutputIt2 d_first_false,
		UnaryPredicate p)
	{
		for (; first != last; ++first)
		{
			if (p(*first))
				*d_first_true++ = *first;
			else
				*d_first_false++ = *first;
		}
		return std::make_pair(d_first_true, d_first_false);
	}

	template< class BidirIt, class UnaryPredicate >
	BidirIt stable_partition(BidirIt first, BidirIt last, UnaryPredicate p)
	{
		if (first == last)
			return last;
		BidirIt part(first++);
		while (first != last)
		{
			if (p(*part))
				++part;
			else if (p(*first))
			{
				BidirIt it = first;
				BidirIt next = it--;
				for (; next != part; --it)
				{
					my_stl::iter_swap(it, next);
					next = it;
				}
				++part;
			}
			++first;
		}
		return part;
	}

	template< class ForwardIt, class UnaryPredicate >
	ForwardIt partition_point(ForwardIt first, ForwardIt last, UnaryPredicate p)
	{
		for (; first != last; ++first)
			if (!p(*first))
				return first;
		return last;
	}

	//-----------------------------------------------------------------------------------
	//sort

	template< class ForwardIt >
	bool is_sorted(ForwardIt first, ForwardIt last)
	{
		if (first != last)
		{
			ForwardIt next = first;
			while (++next != last)
			{
				if (*next < *first)
					return false;
				first = next;
			}
		}
		return true;
	}

	template< class ForwardIt, class Compare >
	bool is_sorted(ForwardIt first, ForwardIt last, Compare comp)
	{
		if (first != last)
		{
			ForwardIt next = first;
			while (++next != last)
			{
				if (comp(*next, *first))
					return false;
				first = next;
			}
		}
		return true;
	}

	template< class ForwardIt >
	ForwardIt is_sorted_until(ForwardIt first, ForwardIt last)
	{
		if (first != last)
		{
			ForwardIt next = first;
			while (++next != last)
			{
				if (*next < *first)
					return next;
				first = next;
			}
		}
		return last;
	}
	template< class ForwardIt, class Compare >
	ForwardIt is_sorted_until(ForwardIt first, ForwardIt last,
		Compare comp)
	{
		if (first != last)
		{
			ForwardIt next = first;
			while (++next != last)
			{
				if (comp(*next, *first))
					return next;
				first = next;
			}
		}
		return last;
	}

	//-----------------------------------------------------------------------------------
	//sort
	//元素很少使用插入排序，元素很多使用快速排序

	const int __stl_threshold = 16;

	template< class RandomIt >
	void __insert_sort(RandomIt first, RandomIt last)
	{
		typename iterator_traits<RandomIt>::value_type key;
		if (first == last)
			return;
		for (RandomIt it = first + 1; it != last; ++it)
		{
			key = *it;
			RandomIt pre = it - 1;
			while (pre - first >= 0 && key < *pre)
			{
				*(pre + 1) = *pre;
				--pre;
			}
			*(++pre) = key;
		}
	}

	template<typename RandomIt >
	inline RandomIt __mid_select(RandomIt first, RandomIt middle, RandomIt last)
	{
		typedef typename iterator_traits<RandomIt>::value_type T;
		T first_value = *first;
		T middle_value = *middle;
		T last_value = *last;
		if (first_value <= last_value)
		{
			if (middle_value <= first_value)
				return first;
			if (middle_value <= last_value)
				return middle;
			else
				return last;
		}
		else
		{
			if (middle_value <= last_value)
				return last;
			if (middle_value <= first_value)
				return middle;
			else
				return first;
		}
	}

	template< class RandomIt, typename T >
	RandomIt __partition(RandomIt first, RandomIt last, T*)
	{
		RandomIt middle = first + (last - first) / 2;
		middle = __mid_select(first, middle, (last - 1));
		my_stl::iter_swap(middle, last - 1);
		T x = *(--last);
		RandomIt it = first - 1;
		for (RandomIt p = first; p != last; ++p)
		{
			if (*p <= x)
			{
				++it;
				my_stl::iter_swap(it, p);
			}
		}
		my_stl::iter_swap(++it, last);
		return it;
	}

	template< class RandomIt >
	void __quick_sort(RandomIt first, RandomIt last)
	{
		if (last - first < 2)
			return;
		RandomIt q = __partition(first, last, value_type(first));
		__quick_sort(first, q);
		__quick_sort(++q, last);
	}

	template< class RandomIt >
	void sort(RandomIt first, RandomIt last)
	{
		if (last - first > __stl_threshold)
			__quick_sort(first, last);
		else
			__insert_sort(first, last);
	}

	//----------------------------------------------------
	//sort 使用comp比较

	template< class RandomIt, typename Compare >
	void __insert_sort(RandomIt first, RandomIt last, Compare comp)
	{
		typename iterator_traits<RandomIt>::value_type key;
		if (first == last)
			return;
		for (RandomIt it = first + 1; it != last; ++it)
		{
			key = *it;
			RandomIt pre = it - 1;
			while (pre - first >= 0 && comp(key, *pre))
			{
				*(pre + 1) = *pre;
				--pre;
			}
			*(++pre) = key;
		}
	}

	template<typename RandomIt, typename Compare >
	inline RandomIt __mid_select(RandomIt first, RandomIt middle, RandomIt last, Compare comp)
	{
		typedef typename iterator_traits<RandomIt>::value_type T;
		T first_value = *first;
		T middle_value = *middle;
		T last_value = *last;
		if (comp(first_value, last_value))
		{
			if (comp(middle_value, first_value))
				return first;
			if (comp(middle_value, last_value))
				return middle;
			else
				return last;
		}
		else
		{
			if (comp(middle_value, last_value))
				return last;
			if (comp(middle_value, first_value))
				return middle;
			else
				return first;
		}
	}

	template< class RandomIt, typename T, typename Compare >
	RandomIt __partition(RandomIt first, RandomIt last, T*, Compare comp)
	{
		RandomIt middle = first + (last - first) / 2;
		middle = __mid_select(first, middle, (last - 1), comp);
		my_stl::iter_swap(middle, --last);
		T x = *last;
		RandomIt it = first - 1;
		for (RandomIt p = first; p != last; ++p)
		{
			if (!(comp(x, *p)))
			{
				++it;
				my_stl::iter_swap(it, p);
			}
		}
		my_stl::iter_swap(++it, last);
		return it;
	}

	template< class RandomIt, typename Compare >
	void __quick_sort(RandomIt first, RandomIt last, Compare comp)
	{
		if (last - first < 2)
			return;
		RandomIt q = __partition(first, last, value_type(first), comp);
		__quick_sort(first, q, comp);
		__quick_sort(++q, last, comp);
	}
	template< class RandomIt, class Compare >
	void sort(RandomIt first, RandomIt last, Compare comp)
	{
		if (last - first > __stl_threshold)
			__quick_sort(first, last, comp);
		else
			__insert_sort(first, last, comp);
	}
	//---------------------------------------------------------------------------
	//lower_bound
	template< class ForwardIt, class T >
	ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value)
	{
		ForwardIt it;
		typename iterator_traits<ForwardIt>::difference_type count, step;
		count = my_stl::distance(first, last);

		while (count > 0) {
			it = first;
			step = count / 2;
			my_stl::advance(it, step);
			if (*it < value) {
				first = ++it;
				count -= step + 1;
			}
			else
				count = step;
		}
		return first;
	}

	template< class ForwardIt, class T, class Compare >
	ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		ForwardIt it;
		typename iterator_traits<ForwardIt>::difference_type count, step;
		count = my_stl::distance(first, last);

		while (count > 0) 
		{
			it = first;
			step = count / 2;
			my_stl::advance(it, step);
			if (comp(*it, value))
			{
				first = ++it;
				count -= step + 1;
			}
			else
				count = step;
		}
		return first;
	}

	//---------------------------------------------------------------------------
	//lower_bound
	template< class ForwardIt, class T >
	ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value)
	{
		ForwardIt it;
		typename iterator_traits<ForwardIt>::difference_type count, step;
		count = my_stl::distance(first, last);

		while (count > 0) {
			it = first;
			step = count / 2;
			my_stl::advance(it, step);
			if (!(value < *it)) {
				first = ++it;
				count -= step + 1;
			}
			else
				count = step;
		}
		return first;
	}
	template< class ForwardIt, class T, class Compare >
	ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		ForwardIt it;
		typename iterator_traits<ForwardIt>::difference_type count, step;
		count = my_stl::distance(first, last);

		while (count > 0)
		{
			it = first;
			step = count / 2;
			my_stl::advance(it, step);
			if (!comp(value, *it))
			{
				first = ++it;
				count -= step + 1;
			}
			else
				count = step;
		}
		return first;
	}

	//---------------------------------------------------------------------------
	//binary_search
	template< class ForwardIt, class T >
	bool binary_search(ForwardIt first, ForwardIt last, const T& value)
	{
		first = lower_bound(first, last, value);
		return (!(first == last) && !(value < *first));
	}
	
	template< class ForwardIt, class T, class Compare >
	bool binary_search(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		first = lower_bound(first, last, value);
		return (!(first == last) && !comp(value, *first));
	}

	//---------------------------------------------------------------------------
	//equal_range
	template< class ForwardIt, class T >
	pair<ForwardIt, ForwardIt>
		equal_range(ForwardIt first, ForwardIt last,
		const T& value)
	{
		return pair<ForwardIt, ForwardIt>(lower_bound(first, last, value),
			upper_bound(first, last, value));
	}

	template< class ForwardIt, class T, class Compare >
	pair<ForwardIt, ForwardIt>
		equal_range(ForwardIt first, ForwardIt last,
		const T& value, Compare comp)
	{
		return pair<ForwardIt, ForwardIt>(lower_bound(first, last, value, comp),
			upper_bound(first, last, value, comp));
	}

	//---------------------------------------------------------------------------
	//merge
	template< class InputIt1, class InputIt2, class OutputIt >
	OutputIt merge(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_first)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first2 < *first1)
				*d_first++ = *first2++;
			else
				*d_first++ = *first1++;
		}
		if (first1 == last1)
			return my_stl::copy(first2, last2, d_first);
		else
			return my_stl::copy(first1, last1, d_first);
	}

	template< class InputIt1, class InputIt2, class OutputIt, class Compare>
	OutputIt merge(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_first, Compare comp)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (comp(*first2, *first1))
				*d_first++ = *first2++;
			else
				*d_first++ = *first1++;
		}
		if (first1 == last1)
			return my_stl::copy(first2, last2, d_first);
		else
			return my_stl::copy(first1, last1, d_first);
	}

	//---------------------------------------------------------------------------
	//inplace_merge
	template< class InputIt1, class InputIt2, class OutputIt >
	OutputIt __merge_backward(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_last)
	{
		--first1;
		--first2;
		--last1;
		--last2;
		while (first1 != last1 && first2 != last2)
		{
			if (*last1 < *last2)
				*(--d_last) = *(last2--);
			else
				*(--d_last) = *(last1--);
		}
		if (first1 == last1)
			return my_stl::copy_backward(++first2, ++last2, d_last);
		else
			return my_stl::copy_backward(++first1, ++last1, d_last);
	}

	template< class InputIt1, class InputIt2, class OutputIt, class Compare >
	OutputIt __merge_backward(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		OutputIt d_last, Compare comp)
	{
		--first1;
		--first2;
		--last1;
		--last2;
		while (first1 != last1 && first2 != last2)
		{
			if (comp(*last1, *last2))
				*(--d_last) = *(last2--);
			else
				*(--d_last) = *(last1--);
		}
		if (first1 == last1)
			return my_stl::copy_backward(++first2, ++last2, d_last);
		else
			return my_stl::copy_backward(++first1, ++last1, d_last);
	}

	template< class BidirIt >
	void __inplace_merge(BidirIt first, BidirIt middle, BidirIt last)
	{
		typedef typename iterator_traits<BidirIt>::value_type T;
		typedef typename iterator_traits<BidirIt>::difference_type Distance;

		Distance len1 = my_stl::distance(first, middle);
		Distance len2 = my_stl::distance(middle, last);
		if (len1 < len2)
		{
			my_stl::vector<T> tmp(first, middle);
			merge(tmp.begin(), tmp.end(), middle, last, first);
		}
		else
		{
			my_stl::vector<T> tmp(middle, last);
			__merge_backward(first, middle, tmp.begin(), tmp.end(), last);
		}
	}

	template< class BidirIt, typename Compare >
	void __inplace_merge(BidirIt first, BidirIt middle, BidirIt last, Compare comp)
	{
		typedef typename iterator_traits<BidirIt>::value_type T;
		typedef typename iterator_traits<BidirIt>::difference_type Distance;

		Distance len1 = my_stl::distance(first, middle);
		Distance len2 = my_stl::distance(middle, last);
		if (len1 < len2)
		{
			my_stl::vector<T> tmp(first, middle);
			merge(tmp.begin(), tmp.end(), middle, last, first, comp);
		}
		else
		{
			my_stl::vector<T> tmp(middle, last);
			__merge_backward(first, middle, tmp.begin(), tmp.end(), last, comp);
		}
	}

	template< class BidirIt >
	void inplace_merge(BidirIt first, BidirIt middle, BidirIt last)
	{
		if (first == middle || middle == last)
			return;
		__inplace_merge(first, middle, last);
	}

	template< class BidirIt, class Compare>
	void inplace_merge(BidirIt first, BidirIt middle, BidirIt last, Compare comp)
	{
		if (first == middle || middle == last)
			return;
		__inplace_merge(first, middle, last, comp);
	}
}

#endif
