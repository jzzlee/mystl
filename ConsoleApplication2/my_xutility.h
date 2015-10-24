#ifndef __MY_XUTILITY_H_
#define __MY_XUTILITY_H_

#include "my_iterator.h"
#include "my_type_traits.h"

namespace my_stl
{
	using std::size_t;
	using std::ptrdiff_t;

	//----------------------------------------------------------------------------------
	//lexicographical_compare
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

	//------------------------------------------------------------------------
	//fill
	template<class ForwardIt, class T>
	ForwardIt fill(ForwardIt first, ForwardIt last, const T& value)
	{
		for (; first != last; ++first)
		{
			*first = value;
		}
		return first;
	}

	//-----------------------------------------------------------------------------
	//fill_n
	template< class ForwardIt, class Size, class T >
	ForwardIt fill_n(ForwardIt first, Size count, const T& value)
	{
		for (; count > 0; ++first, --count)
			*first = value;
		return first;
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
		return __copy_d(first, last, d_first, static_cast<ptrdiff_t*>(0));
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
		static T* copy(const T* first, const T* last, T* d_first)
		{
			typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment_type;
			return __copy_t(first, last, d_first, trivial_assignment_type());
		}
	};

	template<typename T>
	struct __copy_dispatch<const T*, T*>
	{
		static T* copy(const T* first, const T* last, T* d_first)
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

	//------------------------------------------------------------------------
	//copy_if

	template<typename RandomAccessIt, typename OutputIt, typename UnaryPredicate, typename Distance>
	inline OutputIt __copy_if_d(RandomAccessIt first, RandomAccessIt last, OutputIt d_first, UnaryPredicate pred, Distance*)
	{
		for (Distance n = last - first; n > 0; --n, ++first)
		{
			if (pred(*first))
			{
				*d_first = *first;
				++d_first;
			}
		}
		return d_first;
	}

	template<typename InputIt, typename OutputIt, typename UnaryPredicate >
	inline OutputIt __copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred, input_iterator_tag)
	{
		for (; first != last; ++first)
		{
			if (pred(*first))
			{
				*d_first = *first;
				++d_first;
			}
		}
		return d_first;
	}

	template<typename InputIt, typename OutputIt, typename UnaryPredicate >
	inline OutputIt __copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred, random_access_iterator_tag)
	{
		return __copy_if_d(first, last, d_first, pred, distance_type(first));
	}

	//has_trivial_assignment_operator == __true_type.
	template<typename T, typename UnaryPredicate >
	inline T* __copy_if_t(const T *first, const T *last, T *d_first, UnaryPredicate pred, __true_type)
	{
		ptrdiff_t count = last - first;
		for (ptrdiff_t n = 0; n != count; ++n, ++first)
		{
			if (pred(*first))
			{
				std::memmove(d_first, first, 1); //一次移动一个元素
				++d_first;
			}
		}
		return d_first;
	}

	//has_trivial_assignment_operator == __false_type.
	template<typename T, typename UnaryPredicate >
	inline T* __copy_if_t(const T *first, const T *last, T *d_first, UnaryPredicate pred, __false_type)
	{
		return __copy_if_d(first, last, d_first, pred, static_cast<ptrdiff_t *>(0));
	}
	//泛化版本
	template<typename InputIt, typename OutputIt, typename UnaryPredicate >
	struct __copy_if_dispatch
	{
		static OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred)
		{
			return __copy_if(first, last, d_first, pred, iterator_category(first));
		}
	};

	//偏特化版本
	template<typename T, typename UnaryPredicate >
	struct __copy_if_dispatch<T*, T*, UnaryPredicate>
	{
		static T* copy_if(const T* first, const T* last, T* d_first, UnaryPredicate pred)
		{
			typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment_type;
			return __copy_if_t(first, last, d_first, pred, trivial_assignment_type());
		}
	};

	template<typename T, typename UnaryPredicate>
	struct __copy_if_dispatch<const T*, T*, UnaryPredicate>
	{
		static T* copy_if(const T* first, const T* last, T* d_first, UnaryPredicate pred)
		{
			typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment_type;
			return __copy_if_t(first, last, d_first, pred, trivial_assignment_type());
		}
	};

	//copy_if的泛化版本
	template< class InputIt, class OutputIt, class UnaryPredicate >
	OutputIt copy_if(InputIt first, InputIt last,
		OutputIt d_first,
		UnaryPredicate pred)
	{
		return __copy_if_dispatch<InputIt, OutputIt, UnaryPredicate>::copy_if(first, last, d_first, pred);
	}

	//特化版本
	template<typename UnaryPredicate>
	inline char* copy_if(const char* first, const char *last, char *d_first, UnaryPredicate pred)
	{
		return __copy_if_t(first, last, d_first, pred, __true_type); 
	}

	template<typename UnaryPredicate>
	inline wchar_t* copy_if(const wchar_t* first, const wchar_t *last, wchar_t *d_first, UnaryPredicate pred)
	{
		return __copy_if_t(first, last, d_first, pred, __true_type);
	}

	//------------------------------------------------------------------------------------------------------------
	//copy_n
	//从first开始复制count个元素到result开始的位置


	//复制count个元素
	template< class InputIt, class Size, class OutputIt, typename Distance >
	inline OutputIt __copy_n(InputIt first, Size count, OutputIt result, Distance*)
	{
		for (Distance n = 0; n != count; ++n, ++first, ++result)
			*result = *first;
		return result;
	}

	//has_trivial_assignment_operator == ___false_type
	template<typename T, typename Size>
	inline T* __copy_n_t(const T *first, Size count, T *result, __false_type)
	{
		return __copy_n(first, count, result);
	}

	//has_trivial_assignment_operator == ___true_type
	template<typename T, typename Size>
	inline T* __copy_n_t(const T *first, Size count, T *result, __true_type)
	{
		std::memmove(result, first, sizeof(T) * count);
		return result + count;
	}

	//copy_n_dispatch泛化版本
	template< class InputIt, class Size, class OutputIt >
	struct __copy_n_dispatch
	{
		typedef typename iterator_traits<InputIt>::difference_type Distance;
		static OutputIt copy_n(InputIt first, Size count, OutputIt result)
		{
			return __copy_n(first, count, result, static_cast<Distance*>(0));
		}
	};

	//原生Pointer的特化版本
	template<typename T, typename Size>
	struct __copy_n_dispatch < T*, Size, T* >
	{
		typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment_type;
		static T* copy_n(const T *first, Size count, T *result)
		{
			return __copy_n_t(first, count, result, trivial_assignment_type());
		}
	};

	//原生const Pointer的特化版本
	template<typename T, typename Size>
	struct __copy_n_dispatch <const T*, Size, T* >
	{
		typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment_type;
		static T* copy_n(const T *first, Size count, T *result)
		{
			return __copy_n_t(first, count, result, trivial_assignment_type());
		}
	};

	//泛化版本
	template< class InputIt, class Size, class OutputIt >
	OutputIt copy_n(InputIt first, Size count, OutputIt result)
	{
		return __copy_n_dispatch<InputIt, Size, OutputIt>::copy_n(first, count, result);
	}
	//char*的偏特化版本
	inline char* copy_n(const char *first, const size_t &count, char *result)
	{
		std::memmove(result, first, count);
		return result + count;
	}

	//wchar_t*的偏特化版本
	inline wchar_t* copy_n(const wchar_t *first, const size_t &count, wchar_t *result)
	{
		std::memmove(result, first, count * sizeof(wchar_t));
		return result + count;
	}

	//--------------------------------------------------------------------------------------------------------------
	//copy_backward
	//与copy相同，但是复制时从后向前复制

	template< class BidirIt1, class BidirIt2, typename Distance >
	inline BidirIt2 __copy_backward_d(BidirIt1 first, BidirIt1 last, BidirIt2 d_last, Distance*)
	{
		for (Distance n = last - first; n != 0; --n)
			*(--d_last) = *(--last);
		return d_last;
	}

	template< class BidirIt1, class BidirIt2 >
	inline BidirIt2 __copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last, bidirectional_iterator_tag)
	{
		while (last != first)
			*(--d_last) = *(--last);
		return d_last;
	}

	template< class BidirIt1, class BidirIt2 >
	inline BidirIt2 __copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last, random_access_iterator_tag)
	{
		typedef typename iterator_traits<BidirIt1>::difference_type Distance;
		return __copy_backward_d(first, last, d_last, static_cast<Distance *>(0));
	}

	template<typename T>
	inline T* __copy_backward_t(const T *first, const T *last, T *d_last, __true_type)
	{
		std::memmove(d_last - (last - first), first, sizeof(T) * (last - first));
		return d_last - (last - first);
	}

	template<typename T>
	inline T* __copy_backward_t(const T *first, const T *last, T *d_last, __false_type)
	{
		return __copy_backward_d(first, last, d_last, static_cast<ptrdiff_t*>(0));
	}

	//泛化copy_backward_dispatch
	template< class BidirIt1, class BidirIt2 >
	struct __copy_backward_dispatch
	{
		static BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
		{
			return __copy_backward(first, last, d_last, iterator_category(first));
		}
	};

	//原生pointer的特化
	template<typename T>
	struct __copy_backward_dispatch < T*, T* >
	{
		typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment_type;
		static T* copy_backward(const T *first, const T *last, T *d_last)
		{
			return __copy_backward_t(first, last, d_last, trivial_assignment_type());
		}
	};

	//原生const pointer的特化
	template<typename T>
	struct __copy_backward_dispatch <const T*, T* >
	{
		typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment_type;
		static T* copy_backward(const T *first, const T *last, T *d_last)
		{
			return __copy_backward_t(first, last, d_last, trivial_assignment_type());
		}
	};

	//copy_backward泛化版本
	template< class BidirIt1, class BidirIt2 >
	BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
	{
		return __copy_backward_dispatch<BidirIt1, BidirIt2>::copy_backward(first, last, d_last);
	}
	//char*的偏特化版本
	inline char* copy_backward(const char *first, const char *last, char *d_last)
	{
		char *d_first = d_last - (last - first);
		std::memmove(d_first, first, last - first);
		return d_first;
	}

	//wchar_t的偏特化
	inline wchar_t* copy_backward(const wchar_t *first, const wchar_t *last, wchar_t *d_last)
	{
		wchar_t *d_first = d_last - (last - first);
		std::memmove(d_first, first, sizeof(wchar_t) * (last - first));
		return d_first;
	}

	//---------------------------------------------------------
	//move

	template<typename RandomAccessIt, typename OutputIt, typename Distance>
	inline OutputIt __move_d(RandomAccessIt first, RandomAccessIt last, OutputIt d_first, Distance*)
	{
		for (Distance n = last - first; n > 0; --n, ++d_first, ++first)
			*d_first = std::move(*first);
		return d_first;
	}

	template<typename InputIt, typename OutputIt>
	inline OutputIt __move(InputIt first, InputIt last, OutputIt d_first, input_iterator_tag)
	{
		for (; first != last; ++first, ++d_first)
			*d_first = std::move(*first);
		return d_first;
	}

	template<typename RandomAccessIt, typename OutputIt>
	inline OutputIt __move(RandomAccessIt first, RandomAccessIt last, OutputIt d_first, random_access_iterator_tag)
	{
		return __move_d(first, last, d_first, distance_type(first));
	}

	template<typename T>
	inline T* __move_t(const T *first, const T *last, T *d_first, __true_type)
	{
		std::memmove(d_first, first, sizeof(T) * (last - first));
		return d_first + (last - first);
	}

	template<typename T>
	inline T* __move_t(const T *first, const T *last, T *d_first, __false_type)
	{
		return __move_d(first, last, d_first, static_cast<ptrdiff_t*>(0));
	}

	template<typename InputIt, typename OutputIt>
	struct __move_dispatch
	{
		static OutputIt move(InputIt first, InputIt last, OutputIt d_first)
		{
			return __move(first, last, d_first, iterator_category(first));
		}
	};

	//偏特化版本
	template<typename T>
	struct __move_dispatch<T*, T*>
	{
		static T* move(const T* first, const T* last, T* d_first)
		{
			typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment_type;
			return __move_t(first, last, d_first, trivial_assignment_type());
		}
	};

	template<typename T>
	struct __move_dispatch<const T*, T*>
	{
		static T* move(const T* first, const T* last, T* d_first)
		{
			typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment_type;
			return __move_t(first, last, d_first, trivial_assignment_type());
		}
	};

	//真正的move
	template< class InputIt, class OutputIt >
	inline OutputIt move(InputIt first, InputIt last, OutputIt d_first)
	{
		return __move_dispatch<InputIt, OutputIt>::move(first, last, d_first);
	}

	//特化版本
	inline char* move(const char* first, const char *last, char *d_first)
	{
		std::memmove(d_first, first, last - first);
		return d_first + (last - first);
	}

	inline wchar_t* move(const wchar_t* first, const wchar_t *last, wchar_t *d_first)
	{
		std::memmove(d_first, first, sizeof(wchar_t) * (last - first));
		return d_first + (last - first);
	}

	//--------------------------------------------------------------------------------
	//move_backward
	//与move相同，但是复制时从后向前复制

	template< class BidirIt1, class BidirIt2, typename Distance >
	inline BidirIt2 __move_backward_d(BidirIt1 first, BidirIt1 last, BidirIt2 d_last, Distance*)
	{
		for (Distance n = last - first; n != 0; --n)
			*(--d_last) = std::move(*(--last));
		return d_last;
	}

	template< class BidirIt1, class BidirIt2 >
	inline BidirIt2 __move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last, bidirectional_iterator_tag)
	{
		while (last != first)
			*(--d_last) = std::move(*(--last));
		return d_last;
	}

	template< class BidirIt1, class BidirIt2 >
	inline BidirIt2 __move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last, random_access_iterator_tag)
	{
		typedef typename iterator_traits<BidirIt1>::difference_type Distance;
		return __move_backward_d(first, last, d_last, static_cast<Distance *>(0));
	}

	template<typename T>
	inline T* __move_backward_t(const T *first, const T *last, T *d_last, __true_type)
	{
		std::memmove(d_last - (last - first), first, sizeof(T) * (last - first));
		return d_last - (last - first);
	}

	template<typename T>
	inline T* __move_backward_t(const T *first, const T *last, T *d_last, __false_type)
	{
		return __move_backward_d(first, last, d_last, static_cast<ptrdiff_t*>(0));
	}

	//泛化move_backward_dispatch
	template< class BidirIt1, class BidirIt2 >
	struct __move_backward_dispatch
	{
		static BidirIt2 move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
		{
			return __move_backward(first, last, d_last, iterator_category(first));
		}
	};

	//原生pointer的特化
	template<typename T>
	struct __move_backward_dispatch < T*, T* >
	{
		typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment_type;
		static T* move_backward(const T *first, const T *last, T *d_last)
		{
			return __move_backward_t(first, last, d_last, trivial_assignment_type());
		}
	};

	//原生const pointer的特化
	template<typename T>
	struct __move_backward_dispatch <const T*, T* >
	{
		typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment_type;
		static T* move_backward(const T *first, const T *last, T *d_last)
		{
			return __move_backward_t(first, last, d_last, trivial_assignment_type());
		}
	};

	//move_backward泛化版本
	template< class BidirIt1, class BidirIt2 >
	BidirIt2 move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
	{
		return __move_backward_dispatch<BidirIt1, BidirIt2>::move_backward(first, last, d_last);
	}
	//char*的偏特化版本
	inline char* move_backward(const char *first, const char *last, char *d_last)
	{
		char *d_first = d_last - (last - first);
		std::memmove(d_first, first, last - first);
		return d_first;
	}

	//wchar_t的偏特化
	inline wchar_t* move_backward(const wchar_t *first, const wchar_t *last, wchar_t *d_last)
	{
		wchar_t *d_first = d_last - (last - first);
		std::memmove(d_first, first, sizeof(wchar_t) * (last - first));
		return d_first;
	}
	//--------------------------------------------------------------------------------------------------------------
	//equal

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

	//----------------------------------------------------
	//swap
	template< class T >
	void swap(T& a, T& b);
	template< class T2, size_t N >
	void swap(T2(&a)[N], T2(&b)[N]);

	//swaps the elements pointed to by two iterators
	template<class ForwardIt1, class ForwardIt2>
	void iter_swap(ForwardIt1 a, ForwardIt2 b)
	{
		using std::swap;
		if (a != b)
			swap(*a, *b);
	}

	//swaps two ranges of elements 
	template<class ForwardIt1, class ForwardIt2>
	ForwardIt2 swap_ranges(ForwardIt1 first1,
		ForwardIt1 last1,
		ForwardIt2 first2)
	{
		while (first1 != last1) {
			iter_swap(first1++, first2++);
		}
		return first2;
	}


}
#endif
