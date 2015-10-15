#ifndef __MY_MULTISET_H_
#define __MY_MULTISET_H_

#include "my_rb_tree.h"
namespace my_stl
{
	using std::pair;
	//类声明
	template<typename Key, typename Compare, typename Allocator>
	class multiset;

	template<typename Key, typename Compare, typename Allocator>
	bool operator==(const multiset<Key, Compare, Allocator> &lhs, const multiset<Key, Compare, Allocator> &rhs);

	template<typename Key, typename Compare, typename Allocator>
	bool operator<(const multiset<Key, Compare, Allocator> &lhs, const multiset<Key, Compare, Allocator> &rhs);

	template<typename Key, typename Compare = std::less<Key>, typename Allocator = allocator<Key> >
	class multiset
	{
	public:
		typedef multiset<Key, Compare, Allocator> Myt;
		typedef Key key_type;
		typedef Key value_type;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef Compare key_compare;
		typedef Compare value_compare;
		typedef Allocator allocator_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;
		typedef rb_tree<Key, Key, std::identity<Key>, Compare, Allocator> tree_type;
		typedef typename tree_type::iterator iterator;
		typedef typename tree_type::const_iterator const_iterator;

		explicit multiset(const Compare& comp = Compare(),
			const Allocator& alloc = Allocator()) : t(comp, alloc) {}
		explicit multiset(const Allocator& alloc) : t(alloc) {}
		
		template< class InputIterator >
		multiset(InputIterator first, InputIterator last,
			const Compare& comp = Compare(),
			const Allocator& alloc = Allocator()) : t(comp, alloc) { t.insert_equal(first, last); }

		multiset(const multiset& other) : t(other.t) {}
		multiset(const multiset& other, const Allocator& alloc) : t(other.t, alloc) {}

		multiset(multiset&& other) : t(std::move(other.t)) {}
		multiset(multiset&& other, const Allocator& alloc) : t(std::move(other.t), alloc) {}

		multiset(std::initializer_list<value_type> init,
			const Compare& comp = Compare(),
			const Allocator& alloc = Allocator()) : t(comp, alloc) { t.insert_equal(init.begin(), init.end()); }

		multiset& operator=(const multiset& other){ t = other.t; return *this; }
		multiset& operator=(multiset&& other) { t = std::move(other.t); return *this; }
		multiset& operator=(std::initializer_list<value_type> ilist) { t = ilist; return *this; }

		allocator_type get_allocator() const { return t.get_allocator(); }
		iterator begin() { return t.begin(); }
		const_iterator begin() const { return t.begin(); }
		const_iterator cbegin() const { return ((const Myt *)this)->begin(); }
		iterator end() { return t.end(); }
		const_iterator end() const { return t.end(); }
		const_iterator cend() const { return ((const Myt*)this)->end(); }

		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		size_type max_size() const { return t.max_size(); }

		void clear() { t.clear(); }
		iterator insert(const value_type& value) { return t.insert_equal(value); }
		iterator insert(value_type&& value) { return t.insert_equal(std::move(value)); }
		template<typename InputIt>
		iterator insert(InputIt first, InputIt last) { return t.insert_equal(first, last); }
		iterator insert(const_iterator hint, const value_type &value) { return t.insert_equal(hint, value); }
		iterator insert(const_iterator hint, value_type &&value) { return t.insert_equal(hint, std::move(value)); }
		void insert(std::initializer_list<value_type> ilist){ return t.insert_equal(ilist.begin(), ilist.end()); }
		template<typename ...Args>
		iterator emplace(Args&& ...args) { return insert(std::forward<Args>(args)...); }

		template<typename ...Args>
		iterator emplace_hint(const_iterator hint, Args&& ...args) { return insert(hint, std::forward<Args>(args)...); }
		iterator erase(const_iterator pos) { return t.erase(pos); }
		iterator erase(const_iterator first, const_iterator last) { return t.erase(first, last); }
		size_type erase(const value_type &value) { return t.erase(value); }

		void swap(multiset& other) { t.swap(other.t); }
		size_type count(const Key& key) const { return t.count(key); }
		iterator find(const Key& key) { return t.find(key); }
		const_iterator find(const Key &key) const { return t.find(key); }
		pair<iterator, iterator> equal_range(const Key &key) { return t.equal_range(key); }
		pair<const_iterator, const_iterator> equal_range(const Key &key) const { return t.equal_range(key); }
		iterator lower_bound(const Key& key) { return t.lower_bound(key); }
		const_iterator lower_bound(const Key& key) const { return t.lower_bound(key); }
		iterator upper_bound(const Key& key) { return t.upper_bound(key); }
		const_iterator upper_bound(const Key& key) const { return t.upper_bound(key); }

		key_compare key_comp() { return t.key_comp(); }
		value_compare value_comp() { return t.key_comp(); }

	private:
		tree_type t;
	};

	template< class Key, class Compare, class Alloc >
	bool operator==(const multiset<Key, Compare, Alloc>& lhs,
		const multiset<Key, Compare, Alloc>& rhs)
	{
		return lhs.t == rhs.t;
	}

	template< class Key, class Compare, class Alloc >
	bool operator!=(const multiset<Key, Compare, Alloc>& lhs,
		const multiset<Key, Compare, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template< class Key, class Compare, class Alloc >
	bool operator<(const multiset<Key, Compare, Alloc>& lhs,
		const multiset<Key, Compare, Alloc>& rhs)
	{
		return lhs.t < rhs.t;
	}
	template< class Key, class Compare, class Alloc >
	bool operator>(const multiset<Key, Compare, Alloc>& lhs,
		const multiset<Key, Compare, Alloc>& rhs)
	{
		return rhs < lhs;
	}
	template< class Key, class Compare, class Alloc >
	bool operator<=(const multiset<Key, Compare, Alloc>& lhs,
		const multiset<Key, Compare, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}
	template< class Key, class Compare, class Alloc >
	bool operator>=(const multiset<Key, Compare, Alloc>& lhs,
		const multiset<Key, Compare, Alloc>& rhs)
	{
		return !(lhs > rhs);
	}

	template< class Key, class Compare, class Alloc >
	void swap(multiset<Key, Compare, Alloc>& lhs,
		multiset<Key, Compare, Alloc>& rhs)
	{
		std::swap(lhs, rhs);
	}
}

#endif