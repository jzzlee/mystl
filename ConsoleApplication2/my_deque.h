#ifndef __MY_DEQUE_H_
#define __MY_DEQUE_H_

#include "my_memory.h"
#include "my_iterator.h"
#include "my_xutility.h"
#include <initializer_list>
#include <cstdio>
#include <iostream>
namespace my_stl
{
	//deque的迭代器
	template<typename T, typename Ref, typename Ptr>
	struct __deque_iterator
	{
		typedef	__deque_iterator<T, T&, T*>						iterator;
		typedef	__deque_iterator<T, const T&, const T*>			const_iterator;
		typedef	__deque_iterator<T, Ref, Ptr>					self;
		typedef	random_access_iterator_tag						iterator_category;
		typedef	T								value_type;
		typedef Ref								reference;
		typedef Ptr								pointer;
		typedef	std::size_t						size_type;
		typedef ptrdiff_t						difference_type;
		typedef T**								map_pointer;

		T *cur;
		T *first;
		T *last;
		map_pointer node;

		//constructor
		__list_iterator(link_type nd) : node(nd){}
		__list_iterator() {}
		__list_iterator(const iterator &x) : node(x.node) {}

		bool operator==(const self &x) const { return node == x.node; }
		bool operator!=(const self &x) const { return node != x.node; }
		reference operator*() const { return (*node).data; }
		pointer operator->() const { return &(operator*()); }

		self operator++()
		{
			node = (link_type)(node->next);
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			node = (link_type)(node->next);
			return tmp;
		}
		self operator--()
		{
			node = (link_type)(node->prev);
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			node = (link_type)(node->prev);
			return tmp;
		}
	};
}
#endif