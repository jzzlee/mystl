#ifndef __MY_ITERATOR_H_
#define __MY_ITERATOR_H_
#include <cstddef>


namespace my_stl
{
	//定义5种迭代器类型
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};
	//定义5种反向迭代器类型
	struct input_reverse_iterator_tag {};
	struct output_reverse_iterator_tag {};
	struct forward_reverse_iterator_tag : public input_reverse_iterator_tag {};
	struct bidirectional_reverse_iterator_tag : public forward_reverse_iterator_tag {};
	struct random_access_reverse_iterator_tag : public bidirectional_reverse_iterator_tag {};

	//定义iterator template
	template<typename Category, typename T, typename Distance = ptrdiff_t, typename Pointer = T*, typename Reference = T&>
	struct iterator
	{
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	//默认版本
	template<typename I>
	struct iterator_traits
	{
		typedef typename I::iterator_category iterator_category;
		typedef typename I::value_type value_type;
		typedef typename I::difference_type difference_type;
		typedef typename I::pointer pointer;
		typedef typename I::reference reference;

	};

	//针对原生pointer的偏特化版本
	template<typename T>
	struct iterator_traits < T* >
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;

	};

	//针对原生pointer-to-const的偏特化版本
	template<typename T>
	struct iterator_traits < const T* >
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef const T* pointer;
		typedef const T& reference;

	};

	//定义reverse_iterator template
	template< class Iterator >
	struct reverse_iterator : public my_stl::iterator < typename iterator_traits<Iterator>::iterator_category,
		typename iterator_traits<Iterator>::value_type,
		typename iterator_traits<Iterator>::difference_type,
		typename iterator_traits<Iterator>::pointer,
		typename iterator_traits<Iterator>::reference >
	{
		typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;
		typedef typename iterator_traits<Iterator>::value_type					value_type;
		typedef typename iterator_traits<Iterator>::difference_type				difference_type;
		typedef typename iterator_traits<Iterator>::pointer						pointer;
		typedef typename iterator_traits<Iterator>::reference					reference;
	};


	//获取某个迭代器的类型(category)
	template<typename Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator &)
	{
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	//获取某个迭代器的distance_type
	template<typename Iterator>
	inline typename iterator_traits<Iterator>::difference_type *
		distance_type(const Iterator &)
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
	}

	//获取某个迭代器的value_type
	template<typename Iterator>
	inline typename iterator_traits<Iterator>::ivalue_type *
		value_type(const Iterator &)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type *>(0);
	}

	//advance
	template<typename InputIterator, typename Distance>
	inline void __advance(InputIterator &i, Distance n, input_iterator_tag)
	{
		//单向，逐一向前
		while (n--) ++i;
	}

	template<typename ForwardIterator, typename Distance>
	inline void __advance(ForwardIterator &i, Distance n, forward_iterator_tag)
	{
		__advance(i, n, input_iterator_tag);
	}

	template<typename BidirectionalIterator, typename Distance>
	inline void __advance(BidirectionalIterator &i, Distance n, bidirectional_iterator_tag)
	{
		//双向，逐一前进
		if (n >= 0)
			while (n--) ++i;
		else
			while (n++) --i;
	}

	template<typename RandomAccessIterator, typename Distance>
	inline void __advance(RandomAccessIterator &i, Distance n, random_access_iterator_tag)
	{
		//双向，跳跃前进
		i += n;
	}

	//advance
	template<typename InputIterator, typename Distance>
	inline void __advance(InputIterator &i, Distance n, input_reverse_iterator_tag)
	{
		//单向，逐一向前
		while (n--) --i;
	}

	template<typename ForwardIterator, typename Distance>
	inline void __advance(ForwardIterator &i, Distance n, forward_reverse_iterator_tag)
	{
		__advance(i, n, input_reverse_iterator_tag);
	}

	template<typename BidirectionalIterator, typename Distance>
	inline void __advance(BidirectionalIterator &i, Distance n, bidirectional_reverse_iterator_tag)
	{
		//双向，逐一前进
		if (n >= 0)
			while (n--) --i;
		else
			while (n++) ++i;
	}

	template<typename RandomAccessIterator, typename Distance>
	inline void __advance(RandomAccessIterator &i, Distance n, random_access_reverse_iterator_tag)
	{
		//双向，跳跃前进
		i -= n;
	}

	template<typename InputIterator, typename Distance>
	inline void advance(InputIterator &i, Distance n)
	{
		//调用__advance
		typedef typename iterator_traits<InputIterator>::iterator_category category;
		__advance(i, n, category());
	}

	//distance
	template<typename InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		__distance(InputIterator first, InputIterator last, input_iterator_tag)
	{
		typedef typename iterator_traits<InputIterator>::difference_type difference_type;
		difference_type n = 0;
		//逐一累计距离
		while (first != last)
		{
			++first;
			++n;
		}
		return n;
	}

	template<typename RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
		__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
	{
		//直接计算距离
		return last - first;
	}

	//distance
	template<typename InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		__distance(InputIterator first, InputIterator last, input_reverse_iterator_tag)
	{
		typedef typename reverse_iterator_traits<InputIterator>::difference_type difference_type;
		difference_type n = 0;
		//逐一累计距离
		while (first != last)
		{
			--first;
			++n;
		}
		return n;
	}

	template<typename RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
		__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_reverse_iterator_tag)
	{
		//直接计算距离
		return first - last;
	}

	template<typename InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last)
	{
		//调用__distance
		typedef typename iterator_traits<InputIterator>::iterator_category category;
		return __distance(first, last, category());
	}




}


#endif