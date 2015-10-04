#ifndef __MY_MAP_H_
#define __MY_MAP_H_

#include "my_rb_tree.h"

namespace my_stl
{
	//using声明
	using std::pair;
	using std::size_t;
	using std::ptrdiff_t;



	//map声明
	template<typename Key, typename T, typename Compare, typename Allocator>
	class map;

	//比较运算声明
	template< typename Key, typename T, typename Compare, typename Alloc >
	bool operator==(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs);
	template< typename Key, typename T, typename Compare, typename Alloc >
	bool operator<(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs);

	//map定义
	template<typename Key, typename T, typename Compare = std::less<Key>, typename Allocator = my_stl::allocator<std::pair<const Key, T> > >
	class map
	{
	public:
		typedef map<Key, T, Compare, Allocator> Myt;
		typedef Key key_type;
		typedef T mapped_type;
		typedef pair<const Key, T> value_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef Compare key_compare;
		typedef Allocator allocator_type;

		//友元声明
		template< typename Key, typename T, typename Compare, typename Alloc >
		friend bool operator==(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs);
		template< typename Key, typename T, typename Compare, typename Alloc >
		friend bool operator<(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs);

		//从value选出Key
		class select1st
		{
		public:
			Key operator()(const value_type &p){ return p.first; }
		};

		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;
		typedef rb_tree<key_type, value_type, select1st, key_compare, Allocator> tree_type;
		typedef typename tree_type::iterator iterator;
		typedef typename tree_type::const_iterator const_iterator;
		typedef bidirectional_iterator_tag iteratore_category;

		//value
		class value_compare
		{
		public:
			typedef bool result_type;
			typedef value_type first_argument_type;
			typedef value_type second_argument_type;
			friend class map < Key, T, Compare, Allocator > ;
			bool operator()(const value_type& lhs, const value_type& rhs) const
			{
				return comp(lhs.first, rhs.first);
			}

		protected:
			Compare comp;
			value_compare(Compare c) : comp(c) {}

		};

		explicit map(const Compare& comp = Compare(), const Allocator& alloc = Allocator())
			: t(comp, alloc) {}
		explicit map(const Allocator& alloc) : t(alloc) { }
		
		template< class InputIterator >
		map(InputIterator first, InputIterator last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
			: t(comp, alloc) { t.insert_unique(first, last); }

		map(const map& other) : t(other.t) {}
		map(const map& other, const Allocator& alloc) : t(other.t, alloc) {}

		map(map&& other) : t(std::move(other.t)) {}
		map(map&& other, const Allocator& alloc) : t(std::move(other.t), alloc) {}

		map(std::initializer_list<value_type> init, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
			: t(comp, alloc) { t.insert_unique(init.begin(), init.end()); }

		
		map& operator=(const map& other) { t = other.t; return *this; }
		map& operator=(map&& other) { t = std::move(other.t); return *this; }
		map& operator=(std::initializer_list<value_type> ilist) { t = ilist; return *this; }

		//Returns the allocator associated with the container. 
		allocator_type get_allocator() const { return t.alloc; }

		iterator begin() { return t.begin(); }
		const_iterator begin() const { return t.begin(); }
		const_iterator cbegin() const { return ((const Myt*)this)->begin(); }
		iterator end() { return t.end(); }
		const_iterator end() const { return t.end(); }
		const_iterator cend() const{ return ((const Myt*)this)->end(); }
		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		size_type max_size() const { return t.max_size(); }

		void clear() { t.clear(); }

		//Returns a reference to the mapped value of the element with key equivalent to key.
		//If no such element exists, an exception of type std::out_of_range is thrown. 
		T& at(const Key& key)
		{
			iterator iter = find(key);
			if (iter == end())
				throw std::out_of_range("The key is not in the map.\n");
			return (*iter).second;
		}

		const T& at(const Key& key) const
		{
			iterator iter = find(key);
			if (iter == end())
				throw std::out_of_range("The key is not in the map.\n");
			return (*iter).second;
		}

		//Returns a reference to the value that is mapped to a key equivalent to key,
		//performing an insertion if such key does not already exist. 
		T& operator[](const Key& key)
		{
			iterator iter = lower_bound(key);
			if (iter == end() || key_comp()(key, (*iter).first))
				iter = insert(iter, value_type(key, T()));
			return (*iter).second;
		}
		
		T& operator[](Key&& key)
		{
			iterator iter = lower_bound(key);
			if (iter == end() || key_comp()(key, (*iter).first))
				iter = insert((const_iterator)iter, value_type(std::move(key), T()));
			return (*iter).second;
		}



		//Insert
		pair<iterator, bool> insert(const value_type& value) { return t.insert_unique(value); }
		pair<iterator, bool> insert(value_type&& value) { return t.insert_unique(std::move(value)); }
		iterator insert(iterator hint, const value_type& value) { return t.insert_unique(hint, value); }
		iterator insert(const_iterator hint, const value_type& value) { return t.insert_unique(hint, value); }
		iterator insert(const_iterator hint, value_type&& value){ return t.insert_unique(hint, std::move(value)); }
		template< class InputIt >
		void insert(InputIt first, InputIt last){ t.insert_unique(first, last); }
		void insert(std::initializer_list<value_type> ilist) { t.insert_unique(ilist.begin(), ilist.end()); }

		template< class... Args >
		std::pair<iterator, bool> emplace(Args&&... args)
		{
			return insert(std::forward<Args>(args)...);
		}
		template <class... Args>
		iterator emplace_hint(const_iterator hint, Args&&... args)
		{
			return insert(hint, std::forward<Args>(args)...);
		}

		iterator erase(iterator pos) { return t.erase(pos); }
		iterator erase(const_iterator pos) { return t.erase(pos); }
		iterator erase(const_iterator first, const_iterator last) { return t.erase(first, last); }
		size_type erase(const key_type& key) { return t.erase(key); }

		size_type count(const Key& key) const { return t.count(key); }

		iterator find(const Key& key) { return t.find(key); }
		const_iterator find(const Key& key) const { return t.find(key); }

		pair<iterator, iterator> equal_range(const Key& key) { return t.equal_range(key); }
		pair<const_iterator, const_iterator> equal_range(const Key& key) const { return t.equal_range(key); }

		iterator lower_bound(const Key& key){ return t.lower_bound(key); }
		const_iterator lower_bound(const Key& key) const { return t.lower_bound(key); }

		iterator upper_bound(const Key &key) { return t.upper_bound(key); }
		const_iterator upper_bound(const Key &key) const{ return t.upper_bound(key); }

		key_compare key_comp() const { return t.key_comp(); }
		value_compare value_comp() const { return value_compare(key_comp()); }

	private:
		tree_type t;
	};

	template< typename Key, typename T, typename Compare, typename Alloc >
	bool operator==(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
	{
		return lhs.t == rhs.t;
	}

	template< typename Key, typename T, typename Compare, typename Alloc >
	bool operator!=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template< typename Key, typename T, typename Compare, typename Alloc >
	bool operator<(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
	{
		return lhs.t < rhs.t;
	}

	template< typename Key, typename T, typename Compare, typename Alloc >
	bool operator>(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
	{
		return rhs > lhs;
	}

	template< typename Key, typename T, typename Compare, typename Alloc >
	bool operator<=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}

	template< typename Key, typename T, typename Compare, typename Alloc >
	bool operator>=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}
	
	template< typename Key, typename T, typename Compare, typename Alloc >
	void swap(map<Key, T, Compare, Alloc>& lhs, map<Key, Compare, Alloc>& rhs)
	{
		std::swap(lhs, rhs);
	}

}

#endif