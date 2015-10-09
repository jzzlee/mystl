#ifndef _MY_HASH_TABLE_H_
#define _MY_HASH_TABLE_H_

#include <functional>
#include "my_memory.h"
#include "my_iterator.h"
#include "my_xutility.h"
#include "my_vector.h"
#include <initializer_list>
#include <cstdio>
#include <iostream>

namespace my_stl
{
	using std::size_t;
	using std::ptrdiff_t;
	using std::pair;

	template<typename Value>
	struct __hash_node
	{
		typedef Value value_type;
		value_type value;
		__hash_node *next;
	};

	template<typename Key, typename Value, typename HashFcn, typename ExtractKey, typename EqualKey, typename Allocator>
	class hash_table;

	template<typename Key, typename Value, typename HashFcn, typename ExtractKey, typename EqualKey, typename Allocator>
	struct __hash_iterator
	{
		typedef hash_table<Key, Value, HashFcn, ExtractKey, EqualKey, Allocator> hashtable;
		typedef __hash_node<Value> hashnode;
		typedef __hash_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Allocator> iterator;
		typedef __hash_iterator<const Key, const Value, HashFcn, ExtractKey, EqualKey, Allocator> const_iterator;
		typedef forward_iterator_tag iterator_category;
		typedef Value value_type;
		typedef Key key_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef value_type* pointer;
		typedef typename vector<hashnode*>::iterator vector_iterator;

		__hash_iterator() {};
		__hash_iterator(hashnode *nd, hashtable * h) : node(nd), ht(h) {}
		__hash_iterator(hashnode *nd, vector_iterator bl, hashtable * h) : node(nd), block(bl), ht(h) {}
		reference operator*() { return node->value; }
		pointer operator->() { return &(operator*());  }
		iterator& operator++()
		{
			node = node->next;
			if (!node) //node为空
			{
				++block;
				while (!(*block) && block != ht->__end_element())
					++block;
				node = *block;
			}
			return *this;
		}

		iterator operator++(int)
		{
			iterator tmp = *this;
			++*this;
			return tmp;
		}

		bool operator==(const iterator &it) { return node == it.node; }
		bool operator!=(const iterator &it) { return node != it.node; }

		hashnode *node; //指向节点
		vector_iterator block; //指向node所在的桶
		hashtable *ht;
	};

	template<typename Key, typename Value, typename HashFcn, typename ExtractKey, typename EqualKey, typename Allocator = allocator<Value> >
	class hash_table
	{
	public:
		typedef hash_table<Key, Value, HashFcn, ExtractKey, EqualKey, Allocator> Myt;
		typedef Key key_type;
		typedef Value value_type;
		typedef HashFcn hasher;
		typedef EqualKey key_equal;
		typedef size_t size_type;
		typedef __hash_node<value_type> hashnode;
		typedef Allocator allocator_type;
		typedef __hash_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Allocator> iterator;
		typedef __hash_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Allocator> const_iterator;
		typedef typename vector<hashnode*>::iterator vector_iterator;

		template<typename Key, typename Value, typename HashFcn, typename ExtractKey, typename EqualKey, typename Allocator>
		friend struct __hash_iterator;
		
//	private:
		vector<hashnode*> buckets;
		allocator<value_type> alloc;
		allocator<hashnode> alloc_node;
		hasher hash;
		key_equal equals;
		ExtractKey get_key;
		size_type num_elements;
		float load_factor = (float)0.7;
		size_type bucket_size;

	public:
		explicit hash_table(size_type bucket_count = 11, const HashFcn& hash = HashFcn(), const EqualKey& equal = EqualKey(), const Allocator& alloc = Allocator())
			: num_elements(), bucket_size(bucket_count), buckets(bucket_count), hash(hash), equals(equal), alloc(alloc) {}

		explicit hash_table(const Allocator& alloc)
			: num_elements(), bucket_size(11), buckets(11), hash(), equals(), alloc(alloc) {}

		template< class InputIt >
		hash_table(InputIt first, InputIt last, size_type bucket_count = 11, const HashFcn& hash = HashFcn(), const EqualKey& equal = EqualKey(),
			const Allocator& alloc = Allocator())
			: num_elements(), bucket_size(bucket_count), buckets(bucket_count), hash(hash), equals(equal), alloc(alloc)
		{insert(first, last); }

		hash_table(const hash_table &other)
			: load_factor(other.load_factor), num_elements(other.num_elements), bucket_size(other.bucket_size), buckets(other.buckets),
			hash(other.hash), equals(other.equals), alloc(other.alloc) {}

		hash_table(const hash_table &other, const Allocator &alloc)
			: load_factor(other.load_factor), num_elements(other.num_elements), bucket_size(other.bucket_size), buckets(other.buckets),
			hash(other.hash), equals(other.equals), alloc(alloc) {}

		hash_table(hash_table &&other)
			: load_factor(other.load_factor), num_elements(other.num_elements), bucket_size(other.bucket_size), buckets(std::move(other.buckets)),
			hash(other.hash), equals(other.equals), alloc(other.alloc) {}

		hash_table(hash_table &&other, const Allocator &alloc)
			: load_factor(other.load_factor), num_elements(other.num_elements), bucket_size(other.bucket_size), buckets(std::move(other.buckets)),
			hash(other.hash), equals(other.equals), alloc(alloc) {}

		hash_table(std::initializer_list<value_type> init, size_type bucket_count = 11, const HashFcn& hash = HashFcn(), 
			const EqualKey& equal = EqualKey(), const Allocator& alloc = Allocator())
			: num_elements(), bucket_size(bucket_count), buckets(bucket_count), hash(hash), equals(equal), alloc(alloc) 
		{ insert(init.begin(), init.end()); }

		~hash_table()
		{
			clear();
		}

		hash_table& operator=(const hash_table& other)
		{
			copy_from(other);
			return *this;
		}

		hash_table& operator=(hash_table&& other)
		{
			copy_from(std::move(other));
			return *this;
		}

		hash_table& operator=(std::initializer_list<value_type> ilist)
		{
			clear();
			insert(ilist.begin(), ilist.end());
			return *this;
		}

		allocator_type get_allocator() { return alloc; }
		iterator begin() { return iterator(buckets[0], buckets.begin(), this); }
		const_iterator begin() const { return const_iterator(buckets[0], buckets.begin(), this); }
		const_iterator cbegin() const { return ((const Myt*)this)->begin(); }
		iterator end() { return iterator(buckets[buckets.size()], buckets.end(), this);  }
		const_iterator end() const { return const_iterator(buckets[buckets.size()], buckets.end(), this); }
		const_iterator cend() const { return ((const Myt*)this)->end(); }
		bool empty() const { return num_elements == 0; }
		size_type size() const { return num_elements; }
		size_type max_size() const { return std::numeric_limits<size_type>::max(); }

		//Removes all elements from the container. 
		void clear()
		{
			for (size_type i = 0; i != buckets.size(); ++i)
			{
				hashnode *nd = buckets[i];
				hashnode *next;
				while (nd)
				{
					next = nd->next;
					delete_node(nd);
					nd = next;
				}
				buckets[i] = nullptr;
			}
			num_elements = 0;
		}

		pair<iterator, bool> insert_unique(const value_type &v)
		{
			size_type index = bkt_num(v);
			hashnode *nd = buckets[index];//找到v散列的位置
			if (!nd) //nd为空
			{
				buckets[index] = get_node(v);
				return pair<iterator, bool>(iterator(buckets[index], vector_iterator(&buckets + index), this), true);
			}
			else //nd非空
			{
				if (equals(get_key(nd->value) == get_key(v))) //v的key已经存在
					return pair<iterator, bool>(iterator(nd, vector_iterator(&buckets + index), this), false);
				while (nd->next) //nd->next非空
				{
					if (equals(get_key(nd->next->value) == get_key(v))) //v的key已经存在
						return pair<iterator, bool>(iterator(nd->next, vector_iterator(&buckets + index), this), false);
					nd = nd->next;
				}
				//v的key不存在
				nd->next = get_node(v);
				return pair<iterator, bool>(iterator(nd->next, vector_iterator(&buckets + index), this), true);
			}

		}
		template< class InputIt >
		void insert(InputIt first, InputIt last);
		
		// Returns current maximum load factor. 
		float max_load_factor() const { return load_factor; }
		// Sets the maximum load factor to ml. 
		void max_load_factor(float ml) { load_factor = ml; }

		typename vector<hashnode*>::iterator __end_element()
		{
			return buckets.end();
		}

	private:
		hashnode* get_node(const value_type v)
		{
			//申请内存
			hashnode *nd = alloc_node.allocate(1);
			//构造节点值
			alloc.construct(&(nd->value), v);
			nd->next = nullptr;
			return nd;
		}
		void delete_node(hashnode *nd)
		{
			//析构节点值
			alloc.destroy(&nd->value);
			//销毁节点，释放内存
			alloc_node.deallocate(nd);
		}


		void copy_from(const hash_table &other)
		{
			clear();
			load_factor = other.load_factor;
			num_elements = other.num_elements;
			//调整buckets的大小
			buckets.reserve(other.buckets.size());
			buckets = other.buckets;
			hash = other.hash;
			equals = other.equals;
			alloc = other.alloc;
		}

		void copy_from(hash_table &&other)
		{
			clear();
			load_factor = other.load_factor;
			num_elements = other.num_elements;
			buckets = std::move(other.buckets);
			hash = other.hash;
			equals = other.equals;
			alloc = other.alloc;
		}

		//定位key所在buckets的位置
		size_type bkt_num_key(const key_type &k, size_type n) const
		{
			return hash(k) % n;
		}
		size_type bkt_num_key(const key_type& k) const
		{
			return bkt_num_key(k, buckets.size());
		}

		//定位value所在buckets的位置
		size_type bkt_num(const value_type &v) const
		{
			return bkt_num_key(get_key(v));
		}
		size_type bkt_num(const value_type& v, size_t n) const
		{
			return bkt_num_key(get_key(v), n);
		}

	};


}

#endif