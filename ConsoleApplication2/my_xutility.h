#ifndef __MY_XUTILITY_H_
#define __MY_XUTILITY_H_

#include "my_iterator.h"
#include "my_type_traits.h"

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

	//---------------------------------------------------------
	//copy

	template<typename RandomAccessIt, typename OutputIt, typename Distance>
	inline OutputIt __copy_d(RandomAccessIt first, RandomAccessIt last, OutputIt d_first, Distance*)
	{
		for (Distance n = last - first; n > 0; --n, ++d_first, ++first)
			*d_first = *first;
		return d_first;
	}

	template<typename InputIt, typename OutputIt>
	inline OutputIt __copy(InputIt first, InputIt last, OutputIt d_first, input_iterator_tag)
	{
		for (; first != last; ++first, ++d_first)
			*d_first = *first;
		return d_first;
	}

	template<typename RandomAccessIt, typename OutputIt>
	inline OutputIt __copy(RandomAccessIt first, RandomAccessIt last, OutputIt d_first, random_access_iterator_tag)
	{
		return __copy_d(first, last, d_first, distance_type(first));
	}

	template<typename T>
	inline T* __copy_t(const T *first, const T *last, T *d_first, __true_type)
	{
		std::memmove(d_first, first, sizeof(T) * (last - first));
		return d_first + (last - first);
	}

	template<typename T>
	inline T* __copy_t(const T *first, const T *last, T *d_first, __false_type)
	{
		return __copy_d(first, last, result, static_cast<ptrdiff_t*>(0));
	}

	template<typename InputIt, typename OutputIt>
	struct __copy_dispatch
	{
		static OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
		{
			return __copy(first, last, d_first, iterator_category(first));
		}
	};
	
	//偏特化版本
	template<typename T>
	struct __copy_dispatch<T*, T*>
	{
		static T* copy(T* first, T* last, T* d_first)
		{
			typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment_type;
			return __copy_t(first, last, d_first, trivial_assignment_type());
		}
	};

	template<typename T>
	struct __copy_dispatch<const T*, T*>
	{
		static T* copy(T* first, T* last, T* d_first)
		{
			typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment_type;
			return __copy_t(first, last, d_first, trivial_assignment_type());
		}
	};

	//真正的copy
	template< class InputIt, class OutputIt >
	inline OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
	{
		return __copy_dispatch<InputIt, OutputIt>::copy(first, last, d_first);
	}

	//特化版本
	inline char* copy(const char* first, const char *last, char *d_first)
	{
		std::memmove(d_first, first, last - first);
		return d_first + (last - first);
	}

	inline wchar_t* copy(const wchar_t* first, const wchar_t *last, wchar_t *d_first)
	{
		std::memmove(d_first, first, sizeof(wchar_t) * (last - first));
		return d_first + (last - first);
	}

	//真正的copy_n函数
	template<class InputIt, class Size, class ForwardIt>
	ForwardIt copy_n(InputIt first, Size count, ForwardIt d_first)
	{
		typedef typename iterator_traits<ForwardIt>::value_type Value;
		ForwardIt current = d_first;
		for (; count > 0; ++first, ++current, --count) 
		{
			*current = *first;
//			::new (static_cast<void*>(my_stl::addressof(*current))) Value(*first); //use placement new to initialize the object
		}
		return current;
	}
	
	//真正的copy_backwrad
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
	bool equal(InputIt first1, InputIt last1, ForwardIt first2)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (!(*first1 == *first2))
				return false;
		}
		return true;
	}

	//检查[first1, last1)区间的元素是否与first2开始的对应元素满足p(value1, value2)恒为真
	template< class InputIt1, class InputIt2, class BinaryPredicate >
	bool equal(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, BinaryPredicate p)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (!p(*first1, *first2))
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

	//在[first, last)范围内查找满足p的value，若找到，返回所在位置；否则，返回last
	template< class InputIt, class UnaryPredicate >
	InputIt find_if(InputIt first, InputIt last,
		UnaryPredicate p)
	{
		for (; first != last; ++first)
		{
			if (p(*first))
				return first;
		}
		return last;
	}

	//在[first, last)范围内查找不满足p的value，若找到，返回所在位置；否则，返回last
	template< class InputIt, class UnaryPredicate >
	InputIt find_if_not(InputIt first, InputIt last,
		UnaryPredicate q)
	{
		for (; first != last; ++first)
		{
			if (!q(*first))
				return first;
		}
		return last;
	}


}
#endif
