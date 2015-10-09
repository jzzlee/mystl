#ifndef __MY_UNOREDERED_SET_H_
#define __MY_UNOREDERED_SET_H_
#include <functional>
#include "my_memory.h"
#include "my_iterator.h"
#include "my_xutility.h"
#include <initializer_list>
#include <cstdio>
#include <iostream>

namespace my_stl
{
	using std::size_t;
	using std::ptrdiff_t;

	template<typename Key, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>, typename Allocator = allocator<Key> >
	class unordered_set
	{
		typedef Key key_type;
		typedef Key value_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef Hash hasher;
		typedef KeyEqual key_equal;
		typedef Allocator allocator_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;

		template<typename Key>
		struct __iterator
		{
			typedef typename unordered_set::value_type value_type;
			typedef typename unordered_set::key_type key_type;
			typedef typename unordered_set::size_type size_type;
			typedef typename unordered_set::difference_type difference_type;
			typedef typename unordered_set::hasher hasher;
			typedef typename unordered_set::key_equal key_equal;
			typedef typename unordered_set::allocator_type allocator_type;
			typedef typename unordered_set::reference reference;
			typedef typename unordered_set::const_reference const_reference;
			typedef typename unordered_set::pointer pointer;
			typedef typename unordered_set::const_pointer const_pointer;
			typedef	__iterator<key_type> iterator;
			typedef	__iterator<const key_type> const_iterator;
			typedef	__iterator<key_type> self;
			typedef	random_access_iterator_tag iterator_category;
			typedef value_type** map_pointer;

			map_pointer node; //指向节点所在的桶
			pointer cur; //指向节点

			//constructor
			__iterator(Key *c, map_pointer *m) : cur(c), node(m) {}
			__iterator() : cur(nullptr), node(nullptr) {}
			__iterator(const iterator &)
		};
	};
}
#endif