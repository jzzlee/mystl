#ifndef __MY_SET_H_
#define __MY_SET_H_

//#include <functional>
#include "my_rb_tree.h"

namespace my_stl
{
	template<typename Key, typename Compare, typename Allocator >
	class set;

	template< class Key, class Compare, class Alloc >
	bool operator==(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs);

	template< class Key, class Compare, class Alloc >
	bool operator<(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs);

	template<typename Key, typename Compare = std::less<Key>, typename Allocator = allocator<Key> >
	class set
	{
	public:
		typedef set<Key, Compare, Allocator> Myt;
		typedef Key key_type;
		typedef Key value_type;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef Compare key_compare;
		typedef Compare value_compare;
		typedef Allocator allocator_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef rb_tree<key_type, value_type, std::identity<value_type>, key_compare, Allocator> tree_type;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;
		typedef typename tree_type::iterator iterator;
		typedef typename tree_type::const_iterator const_iterator;
		typedef bidirectional_iterator_tag iterator_category;

		template< class Key, class Compare, class Alloc >
		friend bool operator==(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs);

		template< class Key, class Compare, class Alloc >
		friend bool operator<(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs);

		explicit set(const Compare& comp = Compare(), const Allocator& alloc = Allocator()) : t(comp, alloc) {}
		explicit set(const Allocator& alloc) : t(alloc) {}

		template< class InputIt >
		set(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
			: t(comp, alloc)
		{
			t.insert_unique(first, last);
		}

		set(const set& other) : t(other.t) {}
		set(const set& other, const Allocator& alloc) : t(other.t, alloc) {}

		set(set&& other) : t(std::move(other.t)) {}
		set(set&& other, const Allocator& alloc) : t(std::move(other.t), alloc) {}

		set(std::initializer_list<value_type> init, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
			: t(comp, alloc) {
			t.insert_unique(init.begin(), init.end());
		}

		set& operator=(const set& other) { t = other.t; return *this; }
		set& operator=(const set&& other) { t = std::move(other.t); return *this; }
		set& operator=(std::initializer_list<value_type> ilist) { t = ilist; }

		allocator_type get_allocator() const { return alloc; }

		iterator begin() { return t.begin(); }
		const_iterator begin() const { return t.begin(); }
		const_iterator cbegin() const { return ((const Myt*)this->begin()); }

		iterator end() { return t.end(); }
		const_iterator end() const { return t.end(); }
		const_iterator cend() const { return ((const Myt*)this->end()); }

		bool empty() const { return t.empty(); }
		size_type size() { return t.size(); }
		size_type max_size() { return t.max_size(); }

		void clear() { t.clear(); }

		//insert value into set
		std::pair<iterator, bool> insert(const value_type& value) { return t.insert_unique(value); }

		std::pair<iterator, bool> insert(value_type&& value)
		{
			//由于rbtree没有实现 move insert, 所以此处以copy insert代替
			return t.insert_unique(value);
		}

		//Inserts value in the position as close as possible, just prior, to hint. 
		iterator insert(const_iterator hint, const value_type& value)
		{
			return t.insert_unique(hint, value);
		}

		iterator insert(const_iterator hint, value_type&& value)
		{
			return t.insert_unique(hint, value);
		}

		//Inserts elements from range [first, last).
		template< class InputIt >
		void insert(InputIt first, InputIt last)
		{
			t.insert_unique(first, last);
		}

		//Inserts elements from initializer list ilist
		void insert(std::initializer_list<value_type> ilist)
		{
			t.insert_unique(ilist.begin(), ilist.end());
		}

		//Inserts a new element into the container by constructing it in-place with the given args if there is no element with the key in the container. 
		template< class... Args >
		std::pair<iterator, bool> emplace(Args&&... args)
		{
			return insert(std::forward<Args>(args)...);
		}

		//Inserts a new element into the container as close as possible to the position just before hint. 
		template <class... Args>
		iterator emplace_hint(const_iterator hint, Args&&... args)
		{
			return insert(hint, std::forward<Args>(args)...);
		}

		//erases elements 
		iterator erase(const_iterator pos) { return t.erase(pos); }
		iterator erase(const_iterator first, const_iterator last) { return t.erase(first, last); }
		size_type erase(const key_type& key) { return t.erase(key); }

		//swap contents
		void swap(set& other){ t.swap(other); }

		//Returns the number of elements with key key, 
		size_type count(const Key& key) const { return t.count(key); }

		iterator find(const Key& key) { return t.find(key); }
		const_iterator find(const Key &key) const { return t.find(key); }

		//Returns a range containing all elements with the given key in the container.
		std::pair<iterator, iterator> equal_range(const Key& key) { return t.equal_range(key); }
		std::pair<const_iterator, const_iterator> equal_range(const Key& key) const { return t.equal_range(key); }

		iterator lower_bound(const Key& key) { return t.lower_bound(key); }
		const_iterator lower_bound(const Key& key) const { return t.lower_bound(key); }

		iterator upper_bound(const Key& key) { return t.upper_bound(key); }
		const_iterator upper_bound(const Key& key) const { return t.upper_bound(key); }

		//returns the function that compares keys 
		key_compare key_comp() const { return t.key_comp(); }

		//returns the function that compares keys in objects of type value_type 
		value_compare value_comp() const { return t.key_comp(); }

		//	private:
		tree_type t;

	};

	//lexicographically compares the values in the set 

	template< class Key, class Compare, class Alloc >
	bool operator==(const set<Key, Compare, Alloc>& lhs,
		const set<Key, Compare, Alloc>& rhs)
	{
		return lhs.t == rhs.t;
	}

	template< class Key, class Compare, class Alloc >
	bool operator!=(const set<Key, Compare, Alloc>& lhs,
		const set<Key, Compare, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template< class Key, class Compare, class Alloc >
	bool operator<(const set<Key, Compare, Alloc>& lhs,
		const set<Key, Compare, Alloc>& rhs)
	{
		return lhs.t < rhs.t;
	}

	template< class Key, class Compare, class Alloc >
	bool operator<=(const set<Key, Compare, Alloc>& lhs,
		const set<Key, Compare, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}

	template< class Key, class Compare, class Alloc >
	bool operator>(const set<Key, Compare, Alloc>& lhs,
		const set<Key, Compare, Alloc>& rhs)
	{
		return rhs < lhs;
	}

	template< class Key, class Compare, class Alloc >
	bool operator>=(const set<Key, Compare, Alloc>& lhs,
		const set<Key, Compare, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}

	template< class Key, class Compare, class Alloc >
	void swap(set<Key, Compare, Alloc>& lhs,
		set<Key, Compare, Alloc>& rhs)
	{
		std::swap(lhs, rhs);
	}
}


#endif