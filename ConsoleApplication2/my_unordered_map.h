#ifndef __MY_UNOREDERED_MAP_H_
#define __MY_UNOREDERED_MAP_H_
#include "my_hash_tables.h"

namespace my_stl
{
	//一些声明
	using std::size_t;
	using std::ptrdiff_t;
	using std::pair;

	template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
	class unordered_map;

	template< class Key, typename T, class Hash, class KeyEqual, class Allocator >
	bool operator==(const unordered_map<Key, T, Hash, KeyEqual, Allocator>& lhs,
		const unordered_map<Key, T, Hash, KeyEqual, Allocator>& rhs);

	template< class Key, typename T, class Hash, class KeyEqual, class Alloc >
	void swap(unordered_map<Key, T, Hash, KeyEqual, Alloc>& lhs,
		unordered_map<Key, T, Hash, KeyEqual, Alloc>& rhs);

	//类定义
	template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>, typename Allocator = allocator<pair<const Key, T> > >
	class unordered_map
	{
	public:
		typedef unordered_map<Key, T, Hash, KeyEqual, Allocator> Myt;
		typedef Key key_type;
		typedef T mapped_type;
		typedef pair<const Key, T> value_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef Hash hasher;
		typedef KeyEqual key_equal;
		typedef Allocator allocator_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;

		//从value选出Key
		class select1st
		{
		public:
			const Key operator()(const value_type &p) const 
			{ 
				return p.first; 
			}
		};

		typedef hash_table<key_type, value_type, hasher, select1st, key_equal, allocator_type> hashtable_type;
		typedef typename hashtable_type::iterator iterator;
		typedef typename hashtable_type::const_iterator const_iterator;
		typedef typename hashtable_type::local_iterator local_iterator;
		typedef typename hashtable_type::const_local_iterator const_local_iterator;

		template< class Key, typename T, class Hash, class KeyEqual, class Allocator >
		friend bool operator==(const unordered_map<Key, T, Hash, KeyEqual, Allocator>& lhs,
			const unordered_map<Key, T, Hash, KeyEqual, Allocator>& rhs);

		template< class Key, typename T, class Hash, class KeyEqual, class Alloc >
		friend void swap(unordered_map<Key, T, Hash, KeyEqual, Alloc>& lhs,
			unordered_map<Key, T, Hash, KeyEqual, Alloc>& rhs);

		//大批构造函数
		explicit unordered_map(size_type bucket_count = __bucket_init_count,
			const Hash& hash = Hash(),
			const KeyEqual& equal = KeyEqual(),
			const Allocator& alloc = Allocator())
			: table(bucket_count, hash, select1st(), equal, alloc) {}

		explicit unordered_map(const Allocator& alloc) : table(alloc) {}

		template< class InputIt >
		unordered_map(InputIt first, InputIt last,
			size_type bucket_count = __bucket_init_count,
			const Hash& hash = Hash(),
			const KeyEqual& equal = KeyEqual(),
			const Allocator& alloc = Allocator())
			: table(first, last, bucket_count, hash, select1st(), equal, alloc) {}

		unordered_map(const unordered_map& other) : table(other.table) {}
		unordered_map(const unordered_map& other, const Allocator& alloc) : table(other.table, alloc) {}
		unordered_map(unordered_map&& other) : table(std::move(other.table)) {}
		unordered_map(unordered_map&& other, const Allocator& alloc) : table(std::move(other.table), alloc) {}

		unordered_map(std::initializer_list<value_type> init,
			size_type bucket_count = __bucket_init_count,
			const Hash& hash = Hash(),
			const KeyEqual& equal = KeyEqual(),
			const Allocator& alloc = Allocator())
			: table(bucket_count, hash, select1st(), equal, alloc)
		{
			table.insert_unique(init.begin(), init.end());
		}

		//三个赋值操作符
		unordered_map& operator=(const unordered_map& other)
		{
			table = other.table;
			return *this;
		}

		unordered_map& operator=(unordered_map&& other)
		{
			table = std::move(other.table);
			return *this;
		}

		unordered_map& operator=(std::initializer_list<value_type> ilist)
		{
			table = ilist;
			return *this;
		}

		allocator_type get_allocator() { return table.get_allocator(); }

		//首尾迭代器
		iterator begin() { return table.begin(); }
		const_iterator begin() const { return table.begin(); }
		const_iterator cbegin() const { return ((const Myt*)this)->begin(); }
		iterator end() { return table.end(); }
		const_iterator end() const { return table.end(); }
		const_iterator cend() const { return ((const Myt*)this)->end(); }

		bool empty() const { return table.empty(); }
		size_type size() const { return table.size(); }
		size_type max_size() const { return table.max_size(); }

		//清空
		void clear() { table.clear(); }

		//插入
		pair<iterator, bool> insert(const value_type& value)
		{
			return table.insert_unique(value);
		}

		pair<iterator, bool> insert(value_type&& value)
		{
			return table.insert_unique(std::move(value));
		}

		iterator insert(const_iterator hint, const value_type& value)
		{
			return table.insert_unique(hint, value);
		}

		iterator insert(const_iterator hint, value_type&& value)
		{
			return table.insert_unique(hint, std::move(value));
		}

		template< class InputIt >
		void insert(InputIt first, InputIt last)
		{
			table.insert_unique(first, last);
		}

		void insert(std::initializer_list<value_type> ilist)
		{
			table.insert_unique(ilist.begin(), ilist.end());
		}

		template< class... Args >
		pair<iterator, bool> emplace(Args&&... args)
		{
			return table.insert_unique(std::forward<Args>(args)...);
		}

		template<typename... Args>
		iterator emplace_hint(const_iterator hint, Args&&... args)
		{
			return table.insert_unique(hint, std::forward<Args>(args)...);
		}

		//删除
		iterator erase(const_iterator pos)
		{
			return table.erase(pos);
		}
		iterator erase(const_iterator first, const_iterator last)
		{
			return table.erase(first, last);
		}
		size_type erase(const key_type& key)
		{
			return table.erase(key);
		}

		//交换
		void swap(unordered_map& other)
		{
			table.swap(other.table);
		}

		//通过key获取T
		T& at(const Key& key)
		{
			iterator iter = table.find(key);
			if (iter == end())
				throw std::out_of_range("The key is not in the map.\n");
			return (*iter).second;
		}

		const T& at(const Key& key) const
		{
			const_iterator iter = table.find(key);
			if (iter == end())
				throw std::out_of_range("The key is not in the map.\n");
			return (*iter).second;
		}

		T& operator[](const Key& key)
		{
			iterator iter = table.find(key);
			if (iter == end()) //没找到
				iter = insert(value_type(key, T())).first;
			return (*iter).second;
		}
		T& operator[](Key&& key)
		{
			iterator iter = table.find(key);
			if (iter == end()) //没找到
				iter = insert(std::move(value_type(key, T()))).first;
			return (*iter).second;
		}

		//查找统计等
		size_type count(const Key& key) const
		{
			return table.count(key);
		}

		iterator find(const Key& key)
		{
			return table.find(key);
		}

		const_iterator find(const Key& key) const
		{
			return table.find(key);
		}

		pair<iterator, iterator> equal_range(const Key &key)
		{
			return table.equal_range(key);
		}

		pair<const_iterator, const_iterator> equal_range(const Key& key) const
		{
			return table.equal_range(key);
		}

		//桶的内部首尾迭代器
		local_iterator begin(size_type n)
		{
			return table.begin(n);
		}
		const_local_iterator begin(size_type n) const
		{
			return table.begin(n);
		}
		const_local_iterator cbegin(size_type n) const
		{
			return table.cbegin(n);
		}
		local_iterator end(size_type n)
		{
			return table.end(n);
		}
		const_local_iterator end(size_type n) const
		{
			return table.end(n);
		}
		const_local_iterator cend(size_type n) const
		{
			return table.cend(n);
		}

		//桶的数量
		size_type bucket_count() const
		{
			return table.bucket_count();
		}

		size_type max_bucket_count() const
		{
			return table.max_bucket_count();
		}

		//Returns the number of elements in the bucket with index n. 
		size_type bucket_size(size_type n) const
		{
			return table.bucket_size(n);
		}

		//Returns the index of the bucket for key key. 
		size_type bucket(const Key& key) const
		{
			return table.bucket(key);
		}

		//Returns the average number of elements per bucket. 
		float load_factor() const
		{
			return table.load_factor();
		}

		// Returns current maximum load factor. 
		float max_load_factor() const
		{
			return table.max_load_factor();
		}

		// Sets the maximum load factor to ml. 
		void max_load_factor(float ml)
		{
			table.max_load_factor(ml);
		}

		//Sets the number of buckets to count and rehashes the container
		void rehash(size_type count)
		{
			table.rehash(count);
		}

		//Rehashes the container so that it has space for at least count elements.
		void reserve(size_type count)
		{
			table.reserve(count);
		}

		//Returns the function that hashes the keys. 
		hasher hash_function() const
		{
			return table.hash_function();
		}

		//Return the function that compares keys for equality.
		key_equal key_eq() const
		{
			return table.key_eq();
		}

	private:
		hashtable_type table;

	};

	template< class Key, class T, class Hash, class KeyEqual, class Allocator >
	bool operator==(const unordered_map<Key, T, Hash, KeyEqual, Allocator>& lhs,
		const unordered_map<Key, T, Hash, KeyEqual, Allocator>& rhs)
	{
		return lhs.table == rhs.table;
	}

	template< class Key, class T, class Hash, class KeyEqual, class Allocator >
	bool operator!=(const unordered_map<Key, T, Hash, KeyEqual, Allocator>& lhs,
		const unordered_map<Key, T, Hash, KeyEqual, Allocator>& rhs)
	{
		return !(lhs == rhs);
	}

	template< class Key, class T, class Hash, class KeyEqual, class Alloc >
	void swap(unordered_map<Key, T, Hash, KeyEqual, Alloc>& lhs,
		unordered_map<Key, T, Hash, KeyEqual, Alloc>& rhs)
	{
		return lhs.swap(rhs);
	}


	///////////////////////////////////////////////////////////////////////
	//以下是unordered_multimap.
	///////////////////////////////////////////////////////////////////////

	template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
	class unordered_multimap;

	template< class Key, typename T, class Hash, class KeyEqual, class Allocator >
	bool operator==(const unordered_multimap<Key, T, Hash, KeyEqual, Allocator>& lhs,
		const unordered_multimap<Key, T, Hash, KeyEqual, Allocator>& rhs);

	template< class Key, typename T, class Hash, class KeyEqual, class Alloc >
	void swap(unordered_multimap<Key, T, Hash, KeyEqual, Alloc>& lhs,
		unordered_multimap<Key, T, Hash, KeyEqual, Alloc>& rhs);

	//类定义
	template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>, typename Allocator = allocator<pair<const Key, T> > >
	class unordered_multimap
	{
	public:
		typedef unordered_multimap<Key, T, Hash, KeyEqual, Allocator> Myt;
		typedef Key key_type;
		typedef T mapped_type;
		typedef pair<const Key, T> value_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef Hash hasher;
		typedef KeyEqual key_equal;
		typedef Allocator allocator_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;

		//从value选出Key
		class select1st
		{
		public:
			const Key operator()(const value_type &p) const
			{
				return p.first;
			}
		};

		typedef hash_table<key_type, value_type, hasher, select1st, key_equal, allocator_type> hashtable_type;
		typedef typename hashtable_type::iterator iterator;
		typedef typename hashtable_type::const_iterator const_iterator;
		typedef typename hashtable_type::local_iterator local_iterator;
		typedef typename hashtable_type::const_local_iterator const_local_iterator;

		template< class Key, typename T, class Hash, class KeyEqual, class Allocator >
		friend bool operator==(const unordered_multimap<Key, T, Hash, KeyEqual, Allocator>& lhs,
			const unordered_multimap<Key, T, Hash, KeyEqual, Allocator>& rhs);

		template< class Key, typename T, class Hash, class KeyEqual, class Alloc >
		friend void swap(unordered_multimap<Key, T, Hash, KeyEqual, Alloc>& lhs,
			unordered_multimap<Key, T, Hash, KeyEqual, Alloc>& rhs);

		//大批构造函数
		explicit unordered_multimap(size_type bucket_count = __bucket_init_count,
			const Hash& hash = Hash(),
			const KeyEqual& equal = KeyEqual(),
			const Allocator& alloc = Allocator())
			: table(bucket_count, hash, select1st(), equal, alloc) {}

		explicit unordered_multimap(const Allocator& alloc) : table(alloc) {}

		template< class InputIt >
		unordered_multimap(InputIt first, InputIt last,
			size_type bucket_count = __bucket_init_count,
			const Hash& hash = Hash(),
			const KeyEqual& equal = KeyEqual(),
			const Allocator& alloc = Allocator())
			: table(first, last, bucket_count, hash, select1st(), equal, alloc) {}

		unordered_multimap(const unordered_multimap& other) : table(other.table) {}
		unordered_multimap(const unordered_multimap& other, const Allocator& alloc) : table(other.table, alloc) {}
		unordered_multimap(unordered_multimap&& other) : table(std::move(other.table)) {}
		unordered_multimap(unordered_multimap&& other, const Allocator& alloc) : table(std::move(other.table), alloc) {}

		unordered_multimap(std::initializer_list<value_type> init,
			size_type bucket_count = __bucket_init_count,
			const Hash& hash = Hash(),
			const KeyEqual& equal = KeyEqual(),
			const Allocator& alloc = Allocator())
			: table(bucket_count, hash, select1st(), equal, alloc)
		{
			table.insert_equal(init.begin(), init.end());
		}

		//三个赋值操作符
		unordered_multimap& operator=(const unordered_multimap& other)
		{
			table = other.table;
			return *this;
		}

		unordered_multimap& operator=(unordered_multimap&& other)
		{
			table = std::move(other.table);
			return *this;
		}

		unordered_multimap& operator=(std::initializer_list<value_type> ilist)
		{
			table = ilist;
			return *this;
		}

		allocator_type get_allocator() { return table.get_allocator(); }

		//首尾迭代器
		iterator begin() { return table.begin(); }
		const_iterator begin() const { return table.begin(); }
		const_iterator cbegin() const { return ((const Myt*)this)->begin(); }
		iterator end() { return table.end(); }
		const_iterator end() const { return table.end(); }
		const_iterator cend() const { return ((const Myt*)this)->end(); }

		bool empty() const { return table.empty(); }
		size_type size() const { return table.size(); }
		size_type max_size() const { return table.max_size(); }

		//清空
		void clear() { table.clear(); }

		//插入
		iterator insert(const value_type& value)
		{
			return table.insert_equal(value);
		}

		iterator insert(value_type&& value)
		{
			return table.insert_equal(std::move(value));
		}

		iterator insert(const_iterator hint, const value_type& value)
		{
			return table.insert_equal(hint, value);
		}

		iterator insert(const_iterator hint, value_type&& value)
		{
			return table.insert_equal(hint, std::move(value));
		}

		template< class InputIt >
		void insert(InputIt first, InputIt last)
		{
			table.insert_equal(first, last);
		}

		void insert(std::initializer_list<value_type> ilist)
		{
			table.insert_equal(ilist.begin(), ilist.end());
		}

		template< class... Args >
		iterator emplace(Args&&... args)
		{
			return table.insert_equal(std::forward<Args>(args)...);
		}

		template<typename... Args>
		iterator emplace_hint(const_iterator hint, Args&&... args)
		{
			return table.insert_equal(hint, std::forward<Args>(args)...);
		}

		//删除
		iterator erase(const_iterator pos)
		{
			return table.erase(pos);
		}
		iterator erase(const_iterator first, const_iterator last)
		{
			return table.erase(first, last);
		}
		size_type erase(const key_type& key)
		{
			return table.erase(key);
		}

		//交换
		void swap(unordered_multimap& other)
		{
			table.swap(other.table);
		}

		//查找统计等
		size_type count(const Key& key) const
		{
			return table.count(key);
		}

		iterator find(const Key& key)
		{
			return table.find(key);
		}

		const_iterator find(const Key& key) const
		{
			return table.find(key);
		}

		pair<iterator, iterator> equal_range(const Key &key)
		{
			return table.equal_range(key);
		}

		pair<const_iterator, const_iterator> equal_range(const Key& key) const
		{
			return table.equal_range(key);
		}

		//桶的内部首尾迭代器
		local_iterator begin(size_type n)
		{
			return table.begin(n);
		}
		const_local_iterator begin(size_type n) const
		{
			return table.begin(n);
		}
		const_local_iterator cbegin(size_type n) const
		{
			return table.cbegin(n);
		}
		local_iterator end(size_type n)
		{
			return table.end(n);
		}
		const_local_iterator end(size_type n) const
		{
			return table.end(n);
		}
		const_local_iterator cend(size_type n) const
		{
			return table.cend(n);
		}

		//桶的数量
		size_type bucket_count() const
		{
			return table.bucket_count();
		}

		size_type max_bucket_count() const
		{
			return table.max_bucket_count();
		}

		//Returns the number of elements in the bucket with index n. 
		size_type bucket_size(size_type n) const
		{
			return table.bucket_size(n);
		}

		//Returns the index of the bucket for key key. 
		size_type bucket(const Key& key) const
		{
			return table.bucket(key);
		}

		//Returns the average number of elements per bucket. 
		float load_factor() const
		{
			return table.load_factor();
		}

		// Returns current maximum load factor. 
		float max_load_factor() const
		{
			return table.max_load_factor();
		}

		// Sets the maximum load factor to ml. 
		void max_load_factor(float ml)
		{
			table.max_load_factor(ml);
		}

		//Sets the number of buckets to count and rehashes the container
		void rehash(size_type count)
		{
			table.rehash(count);
		}

		//Rehashes the container so that it has space for at least count elements.
		void reserve(size_type count)
		{
			table.reserve(count);
		}

		//Returns the function that hashes the keys. 
		hasher hash_function() const
		{
			return table.hash_function();
		}

		//Return the function that compares keys for equality.
		key_equal key_eq() const
		{
			return table.key_eq();
		}

	private:
		hashtable_type table;

	};

	template< class Key, class T, class Hash, class KeyEqual, class Allocator >
	bool operator==(const unordered_multimap<Key, T, Hash, KeyEqual, Allocator>& lhs,
		const unordered_multimap<Key, T, Hash, KeyEqual, Allocator>& rhs)
	{
		return lhs.table == rhs.table;
	}

	template< class Key, class T, class Hash, class KeyEqual, class Allocator >
	bool operator!=(const unordered_multimap<Key, T, Hash, KeyEqual, Allocator>& lhs,
		const unordered_multimap<Key, T, Hash, KeyEqual, Allocator>& rhs)
	{
		return !(lhs == rhs);
	}

	template< class Key, class T, class Hash, class KeyEqual, class Alloc >
	void swap(unordered_multimap<Key, T, Hash, KeyEqual, Alloc>& lhs,
		unordered_multimap<Key, T, Hash, KeyEqual, Alloc>& rhs)
	{
		return lhs.swap(rhs);
	}
}
#endif