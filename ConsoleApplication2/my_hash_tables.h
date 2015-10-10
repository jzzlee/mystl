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
#define _MY_DEBUG

	template<typename T>
	class compare_equal
	{
	public:
		bool operator()(const T &lhs, const T &rhs)
		{
			return lhs == rhs;
		}
	};

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
	struct __hash_const_iterator;

	template<typename Key, typename Value, typename HashFcn, typename ExtractKey, typename EqualKey, typename Allocator>
	struct __hash_iterator
	{
		typedef hash_table<Key, Value, HashFcn, ExtractKey, EqualKey, Allocator> hashtable;
		typedef __hash_node<Value> hashnode;
		typedef __hash_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Allocator> iterator;
		typedef __hash_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Allocator> const_iterator;
		typedef forward_iterator_tag iterator_category;
		typedef Value value_type;
		typedef Key key_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef value_type* pointer;
		typedef typename vector<hashnode*>::iterator vector_iterator;

		template<typename Key, typename Value, typename HashFcn, typename ExtractKey, typename EqualKey, typename Allocator>
		friend class hash_table;

		__hash_iterator() {};
		__hash_iterator(const const_iterator &it) : block(it.block)
		{
			node = const_cast<hashnode*>(it.node);
			ht = const_cast<hashtable*>(it.ht);
		}
		__hash_iterator(hashnode *nd, const vector_iterator &bl, hashtable * h) : node(nd), block(bl), ht(h) {}
		
		reference operator*() { return node->value; }
		pointer operator->() { return &(operator*());  }
		iterator& operator++()
		{
			node = node->next;
			if (!node) //node为空
			{
				++block;
				while (!(*block) && block != ht->buckets.end())
					++block;
				if (block == ht->buckets.end())
					node = nullptr;
				else
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
#ifndef _MY_DEBUG
	private:
#endif
		hashnode *node; //指向节点
		vector_iterator block; //指向node所在的桶
		hashtable *ht;
	};

	template<typename Key, typename Value, typename HashFcn, typename ExtractKey, typename EqualKey, typename Allocator>
	struct __hash_const_iterator
	{
		typedef hash_table<Key, Value, HashFcn, ExtractKey, EqualKey, Allocator> hashtable;
		typedef __hash_node<Value> hashnode;
		typedef __hash_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Allocator> iterator;
		typedef __hash_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Allocator> const_iterator;
		typedef forward_iterator_tag iterator_category;
		typedef Value value_type;
		typedef Key key_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef value_type* pointer;
		typedef typename vector<hashnode*>::iterator vector_iterator;

		template<typename Key, typename Value, typename HashFcn, typename ExtractKey, typename EqualKey, typename Allocator>
		friend class hash_table;

		__hash_const_iterator() {};
		__hash_const_iterator(const hashnode *nd, const vector_iterator &bl, const hashtable *h) : node(nd), block(bl), ht(h) {}
//		__hash_const_iterator(const hashnode * const nd, vector_iterator bl, const hashtable * const h) : node(nd), block(bl), ht(h) {}
		__hash_const_iterator(const iterator &it) : node(it.node), block(it.block), ht(it.ht) {}
		reference operator*() const { return node->value; }
		pointer operator->() const { return &(operator*()); }
		const_iterator& operator++()
		{
			node = node->next;
			if (!node) //node为空
			{
				++block;
				while (!(*block) && block != ht->buckets.end())
					++block;
				if (block == ht->buckets.end())
					node = nullptr;
				else
					node = *block;
			}
			return *this;
		}

		const_iterator operator++(int)
		{
			iterator tmp = *this;
			++*this;
			return tmp;
		}

		bool operator==(const const_iterator &it) { return node == it.node; }
		bool operator!=(const const_iterator &it) { return node != it.node; }
#ifndef _MY_DEBUG
	private:
#endif
		const hashnode *node; //指向节点
		vector_iterator block; //指向node所在的桶
		const hashtable *ht;
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
		typedef __hash_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Allocator> const_iterator;
		typedef typename vector<hashnode*>::iterator vector_iterator;

		template<typename Key, typename Value, typename HashFcn, typename ExtractKey, typename EqualKey, typename Allocator>
		friend struct __hash_iterator;
		template<typename Key, typename Value, typename HashFcn, typename ExtractKey, typename EqualKey, typename Allocator>
		friend struct __hash_const_iterator;

	private:
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
		{
			insert(first, last);
		}

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
		{
			insert(init.begin(), init.end());
		}

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
		
		//返回第一个非空节点的迭代器
		iterator begin()
		{
			for (vector_iterator it = buckets.begin(); it != buckets.end(); ++it)
			{
				if (*it)//桶不为空
					return iterator(*it, it, this);
			}
			return end(); 
		}
		const_iterator begin() const 
		{
			for (vector_iterator it = buckets.begin(); it != buckets.end(); ++it)
			{
				if (*it)//桶不为空
					return const_iterator(*it, it, this);
			}
			return end(); 
		}
		const_iterator cbegin() const { return ((const Myt*)this)->begin(); }

		iterator end() { return iterator(nullptr, buckets.end(), this); }
		const_iterator end() const { return const_iterator(nullptr, const_cast<vector_iterator>(buckets.end()), this); }
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

		//左值unique插入
		//如果key已存在，返回pair(已存在key的iterator, false)
		//否则，在key对应bucket的最后，返回pair(插入位置iterator, true).
		pair<iterator, bool> insert_unique(const value_type &v)
		{
			size_type index = bkt_num(v);
			hashnode *nd = buckets[index];//找到v散列的位置
			if (!nd) //nd为空
			{
				buckets[index] = get_node(v);
				return pair<iterator, bool>(iterator(buckets[index], vector_iterator(&(buckets[0]) + index), this), true);
			}
			else //nd非空
			{
				if (equals(get_key(nd->value), get_key(v))) //v的key已经存在
					return pair<iterator, bool>(iterator(nd, vector_iterator(&(buckets[0]) + index), this), false);
				while (nd->next) //nd->next非空
				{
					if (equals(get_key(nd->next->value), get_key(v))) //v的key已经存在
						return pair<iterator, bool>(iterator(nd->next, vector_iterator(&(buckets[0]) + index), this), false);
					nd = nd->next;
				}
				//v的key不存在
				nd->next = get_node(v);
				return pair<iterator, bool>(iterator(nd->next, vector_iterator(&(buckets[0]) + index), this), true);
			}
		}

		//左值equal插入
		//直接插到对应bucket的开始
		iterator insert_equal(const value_type &v)
		{
			size_type index = bkt_num(v);
			hashnode *nd = buckets[index];//找到v散列的位置
			if (!nd) //nd为空
			{
				buckets[index] = get_node(std::move(v));
				return iterator(buckets[index], vector_iterator(&(buckets[0]) + index), this);
			}
			else //nd非空
			{
				if (equals(get_key(nd->value), get_key(v))) //v的key已经存在,在其后位置插入
				{
					return iterator(__insert_after(nd, std::move(v)), vector_iterator(&(buckets[0]) + index), this);
				}
				while (nd->next) //nd->next非空
				{
					if (equals(get_key(nd->next->value), get_key(v))) //v的key已经存在
						return iterator(__insert_after(nd, std::move(v)), vector_iterator(&(buckets[0]) + index), this);
					nd = nd->next;
				}
				//v的key不存在,此时nd是最后一个节点
				return iterator(__insert_after(nd, std::move(v)), vector_iterator(&(buckets[0]) + index), this);
			}
		}

		//右值unique插入
		//如果key已存在，返回pair(已存在key的iterator, false)
		//否则，在key对应bucket的最后，返回pair(插入位置iterator, true).
		pair<iterator, bool> insert_unique(value_type&& v)
		{
			size_type index = bkt_num(v);
			hashnode *nd = buckets[index];//找到v散列的位置
			if (!nd) //nd为空
			{
				buckets[index] = get_node(std::move(v));
				return pair<iterator, bool>(iterator(buckets[index], vector_iterator(&(buckets[0]) + index), this), true);
			}
			else //nd非空
			{
				if (equals(get_key(nd->value), get_key(v))) //v的key已经存在
					return pair<iterator, bool>(iterator(nd, vector_iterator(&(buckets[0]) + index), this), false);
				while (nd->next) //nd->next非空
				{
					if (equals(get_key(nd->next->value), get_key(v))) //v的key已经存在
						return pair<iterator, bool>(iterator(nd->next, vector_iterator(&(buckets[0]) + index), this), false);
					nd = nd->next;
				}
				//v的key不存在
				nd->next = get_node(std::move(v));
				return pair<iterator, bool>(iterator(nd->next, vector_iterator(&(buckets[0]) + index), this), true);
			}
		}
		//右值equal插入
		//直接插到对应bucket的开始
		iterator insert_equal(value_type &&v)
		{
			size_type index = bkt_num(v);
			hashnode *nd = buckets[index];//找到v散列的位置
			if (!nd) //nd为空
			{
				buckets[index] = get_node(v);
				return iterator(buckets[index], vector_iterator(&(buckets[0]) + index), this);
			}
			else //nd非空
			{
				if (equals(get_key(nd->value), get_key(v))) //v的key已经存在,在其后位置插入
				{
					return iterator(__insert_after(nd, v), vector_iterator(&(buckets[0]) + index), this);
				}
				while (nd->next) //nd->next非空
				{
					if (equals(get_key(nd->next->value), get_key(v))) //v的key已经存在
						return iterator(__insert_after(nd, v), vector_iterator(&(buckets[0]) + index), this);
					nd = nd->next;
				}
				//v的key不存在,此时nd是最后一个节点
				return iterator(__insert_after(nd, v), vector_iterator(&(buckets[0]) + index), this);
			}
		}

		//带提示的左值unique插入
		//unique插入的提示不能减少计算量，直接调用无提示版本
		iterator insert_unique(const_iterator hint, const value_type &v)
		{
			return insert_unique(v).first;
		}

		//带提示的左值equal插入
		//提示不能减少计算量，直接调用无提示版本
		iterator insert_equal(const_iterator hint, const value_type &v)
		{
			return insert_equal(v);
		}


		//带提示的右值unique插入
		//提示不能减少计算量，直接调用无提示版本
		iterator insert_unique(const_iterator hint, value_type &&v)
		{
			return insert_unique(std::move(v)).first;
		}

		//带提示的右值equal插入
		//提示不能减少计算量，直接调用无提示版本
		iterator insert_equal(const_iterator hint, value_type &&v)
		{
			return insert_equal(std::move(v));
		}

		//范围unique插入
		template< class InputIt >
		void insert_unique(InputIt first, InputIt last)
		{
			for (; first != last; ++first)
				insert_unique(*first);
		}

		//范围equal插入
		template<typename InputIt>
		void insert_equal(InputIt first, InputIt last)
		{
			for (; first != last; ++first)
				insert_equal(*first);
		}

		//Removes the element at pos.
		//Returns iterator following the last removed element.
		iterator erase(const_iterator pos)
		{
			iterator tmp = pos;
			++tmp;
			hashnode *nd = *(pos.block);//pos所在的桶
			if (nd != pos.node) // pos.node不在起始位置
			{
				while (nd->next != pos.node)//找到pos的上一个节点
					nd = nd->next;
				nd->next = pos.node->next;//指向下一个节点
			}
			else
			{
				nd = pos.node->next;
			}
			delete_node(pos.node);
			return tmp;
		}


		// Removes the elements in the range [first; last), which must be a valid range in *this.
		iterator erase(const_iterator const_first, const_iterator const_last)
		{
			iterator first(const_first), last(const_last);
			if (first != last)
			{
				vector_iterator block = first.block; //指向要删除的第一个桶
				if (first.block == last.block) //first和last在一个桶内
				{
					__erase_range(first.node, last.node, block);
				}
				else	//first和last不在一个桶内
				{
					__erase_range(first.node, nullptr, block);//删除第1个桶从first开始的所有节点
					while (++block != last.block) // 不到最后1个要删除的桶
						if (*block) //block所指的桶非空
							__erase_range(*block, nullptr, block); //删除这个桶的所有元素
					__erase_range(*block, last.node, block);
				}
			}
			return last;
		}

		//Removes the element (if one exists) with the key equivalent to key.
		size_type erase(const key_type& k)
		{
			pair<iterator, iterator> pair_range = equal_range(k);
			size_type count = my_stl::distance(pair_range.first, pair_range.second);
			erase(pair_range.first, pair_range.second);
			return count;
		}

		//Returns a range containing all elements with key key in the container. The range is defined by two iterators, 
		//the first pointing to the first element of the wanted range and the second pointing past the last element of the range. 
		pair<iterator, iterator> equal_range(const key_type& k)
		{
			size_type index = bkt_num_key(k);
			hashnode *nd = buckets[index];//找到v散列的桶
			while (nd)
			{
				if (equals(get_key(nd->value), k)) //找到k
				{
					iterator first(nd, addressof(buckets[0]) + index, this);//第一个
					while (nd->next && equals(get_key(nd->next->value), k)) //从下一个开始比较是否相等,nd指向最后一个相等的节点
						nd = nd->next;
					iterator before_last(nd, addressof(buckets[0]) + index, this);
					return pair<iterator, iterator>(first, ++before_last);
				}
				nd = nd->next;
			}
			//没找到
			return pair<iterator, iterator>(end(), end());
		}

		pair<const_iterator, const_iterator> equal_range(const Key& k) const
		{
			size_type index = bkt_num_key(k);
			hashnode *nd = buckets[index];//找到v散列的桶
			while (nd)
			{
				value_type v = nd->value;
				if (equals(get_key(nd->value), k)) //找到k
				{
					const_iterator first(nd, addressof(buckets[0]) + index, this);//第一个
					v = nd->next->value;
					while (nd->next && equals(get_key(v), k)) //从下一个开始比较是否相等,nd指向最后一个相等的节点
					{
						nd = nd->next;
						v = nd->value;
					}
					const_iterator before_last(nd, addressof(buckets[0]) + index, this);
					return pair<const_iterator, const_iterator>(first, ++before_last);
				}
				nd = nd->next;
			}
			//没找到
			return pair<const_iterator, const_iterator>(cend(), cend());
		}

		iterator find(const key_type& k)
		{
			size_type index = bkt_num_key(k);
			hashnode *nd = buckets[index];//找到v散列的桶
			while (nd)
			{
				if (equals(get_key(nd->value), k)) //找到k
					return iterator(nd, addressof(buckets[0]) + index, this);
				nd = nd->next;
			}
			//没找到
			return end();
		}

		// Returns current maximum load factor. 
		float max_load_factor() const { return load_factor; }
		// Sets the maximum load factor to ml. 
		void max_load_factor(float ml) { load_factor = ml; }


	private:
		hashnode* get_node(const value_type &v)
		{
			//申请内存
			hashnode *nd = alloc_node.allocate(1);
			//构造节点值
			alloc.construct(&(nd->value), v);
			nd->next = nullptr;
			return nd;
		}

		hashnode* get_node(value_type &&v)
		{
			typedef typename __type_traits<value_type>::is_POD_type is_POD;
			return __get_node(std::move(v), is_POD());
		}

		hashnode *__get_node(value_type &&v, __true_type)
		{
			//申请内存
			hashnode *nd = alloc_node.allocate(1);
			//直接移动内存
			memmove(addressof(nd->value), addressof(v), sizeof(value_type) / sizeof(unsigned char));
			nd->next = nullptr;
			return nd;
		}

		hashnode *__get_node(value_type &&v, __false_type)
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

		//在nd后面插入节点,值为v
		hashnode *__insert_after(hashnode *nd, const value_type &v)
		{
			hashnode *tmp = get_node(v);
			tmp->next = nd->next;
			nd->next = tmp;
			return tmp;
		}

		//在nd后面插入节点,值为v
		hashnode *__insert_after(hashnode *nd, value_type &&v)
		{
			hashnode *tmp = get_node(std::move(v));
			tmp->next = nd->next;
			nd->next = tmp;
			return tmp;
		}

		//Removes the elements in the range[first.node, last), which must be a valid range in a bucket.
		void __erase_range(hashnode *first, hashnode *last, vector_iterator &block)
		{
			if (first != last)
			{
				hashnode *nd = *block;//first所在的桶
				if (nd != first)     // first不在起始位置
				{
					while (nd->next != first)//找到first的上一个节点
						nd = nd->next;
					nd->next = last; //指向last
				}
				else
					*block = last;
				//销毁节点
				for (; first != last; first = first->next)
					delete_node(first);
			}
		}

		//返回nd所在桶的最后一个非空节点（仅当桶非空时使用）
		hashnode *__end_of_bucket(hashnode *nd)
		{
			if (nd)
				while (nd->next)
					nd = nd->next;
			return nd;
		}
	};


}

#endif