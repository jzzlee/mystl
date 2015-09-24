#ifndef __MY_HEAP_H_
#define __MY_HEAP_H_

#include "my_iterator.h"

namespace my_stl
{
	template< class RandomIt, typename T, typename Distance, class Compare >
	void __push_heap(RandomIt first, Distance holeIndex, T value, Compare comp)
	{
		Distance parent = (holeIndex - 1) / 2;
		while (holeIndex > 0 && comp(*(first + parent), value))
		{
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}

	template< class RandomIt, class Compare >
	void push_heap(RandomIt first, RandomIt last, Compare comp)
	{
		__push_heap(first, last - first - 1, *(last - 1), comp);
	}

	template< class RandomIt >
	void push_heap(RandomIt first, RandomIt last)
	{
		__push_heap(first, last - first - 1, *(last - 1), std::less<>());
	}

	template <class RandomIt, class Distance, class T, typename Compare>
	void __adjust_heap(RandomIt first, Distance holeIndex, Distance len, T value, Compare comp)
	{
		Distance topIndex = holeIndex;
		Distance secondChild = 2 * holeIndex + 2;
		while (secondChild < len)
		{
			if (comp(*(first + secondChild), *(first + (secondChild - 1))))
				--secondChild;
			if (comp(*(first + secondChild), value))
				break;
			*(first + holeIndex) = *(first + secondChild);
			holeIndex = secondChild;
			secondChild = 2 * (secondChild + 1);
		}
		if (secondChild == len)
		{
			if (!comp(*(first + (secondChild - 1)), value))
			{
				*(first + holeIndex) = *(first + (secondChild - 1));
				holeIndex = secondChild - 1;
			}
		}
		*(first + holeIndex) = value;
	}

	template< class RandomIt, class Compare >
	void pop_heap(RandomIt first, RandomIt last, Compare comp)
	{
		typedef typename iterator_traits<RandomIt>::value_type value_type;
		typedef typename iterator_traits<RandomIt>::difference_type difference_type;
		value_type tmp = *first;
		__adjust_heap(first, difference_type(0), difference_type(last - first - 1), *(last - 1), comp);
		*(last - 1) = tmp;
	}

	template< class RandomIt >
	void pop_heap(RandomIt first, RandomIt last)
	{
		typedef typename iterator_traits<RandomIt>::value_type value_type;
		typedef typename iterator_traits<RandomIt>::difference_type difference_type;
		value_type tmp = *first;
		__adjust_heap(first, difference_type(0), difference_type(last - first - 1), *(last - 1), std::less<>());
		*(last - 1) = tmp;
	}

	template< class RandomIt, class Compare >
	void sort_heap(RandomIt first, RandomIt last, Compare comp)
	{
		while (last != first)
			pop_heap(first, last--, comp);
	}

	template< class RandomIt >
	void sort_heap(RandomIt first, RandomIt last)
	{
		while (last != first)
		{
			pop_heap(first, last--);
		}
	}

	template< class RandomIt >
	void make_heap(RandomIt first, RandomIt last)
	{
		typedef typename iterator_traits<RandomIt>::value_type value_type;
		typedef typename iterator_traits<RandomIt>::difference_type difference_type;
		if (last - first < 2)
			return;
		difference_type len = last - first;
		difference_type holeIndex = (len - 2) / 2;
		while (true)
		{
			__adjust_heap(first, holeIndex, len, *(first + holeIndex), std::less<>());
			if (holeIndex == 0)
				return;
			--holeIndex;
		}
	}

	template< class RandomIt, class Compare >
	void make_heap(RandomIt first, RandomIt last,
		Compare comp)
	{
		typedef typename iterator_traits<RandomIt>::value_type value_type;
		typedef typename iterator_traits<RandomIt>::difference_type difference_type;
		if (last - first < 2)
			return;
		difference_type len = last - first;
		difference_type holeIndex = (len - 2) / 2;
		while (true)
		{
			__adjust_heap(first, holeIndex, len, *(first + holeIndex), comp);
			if (holeIndex == 0)
				return;
			--holeIndex;
		}
	}
}

#endif