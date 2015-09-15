#ifndef __MY_LIST_H_
#define __MY_LIST_H_

#include "my_memory.h"
#include "my_iterator.h"
#include "my_xutility.h"
#include <initializer_list>
#include <cstdio>
#include <iostream>
namespace my_stl
{
	//list的节点结构
	template<typename T>
	struct __list_node
	{
		typedef __list_node<T>*	link_type;;
		link_type prev;
		link_type next;
		T data;
	};

	//list的迭代器
	template<typename T, typename Ref, typename Ptr>
	struct __list_iterator
	{
		typedef	__list_iterator<T, T&, T*>			iterator;
		typedef	__list_iterator<T, Ref, Ptr>		self;
		typedef	bidirectional_iterator_tag			iterator_category;
		typedef	T									value_type;
		typedef Ref									reference;
		typedef Ptr									pointer;
		typedef __list_node<T>*						link_type;
		typedef	std::size_t							size_type;
		typedef ptrdiff_t							difference_type;

		link_type node;

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

	//list的反向迭代器
	template<typename T, typename Ref, typename Ptr>
	struct __list_reverse_iterator
	{
		typedef	__list_reverse_iterator<T, T&, T*>			iterator;
		typedef	__list_reverse_iterator<T, Ref, Ptr>		self;
		typedef	bidirectional_reverse_iterator_tag			reverse_iterator_category;
		typedef	T											value_type;
		typedef Ref											reference;
		typedef Ptr											pointer;
		typedef __list_node<T>*								link_type;
		typedef	std::size_t									size_type;
		typedef ptrdiff_t									difference_type;

		link_type node;

		//constructor
		__list_reverse_iterator(link_type nd) : node(nd){}
		__list_reverse_iterator() {}
		__list_reverse_iterator(const iterator &x) : node(x.node) {}

		bool operator==(const self &x) const { return node == x.node; }
		bool operator!=(const self &x) const { return node != x.node; }
		reference operator*() const { return (*node).data; }
		pointer operator->() const { return &(operator*()); }

		self operator++()
		{
			node = (link_type)(node->prev);
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			node = (link_type)(node->prev);
			return tmp;
		}
		self operator--()
		{
			node = (link_type)(node->next);
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			node = (link_type)(node->next);
			return tmp;
		}
	};

	template<typename T, typename Allocator = allocator<T>> class list
	{
	public:
		typedef __list_node<T>													node_type;
		typedef node_type*														link_type;
		typedef list<T, Allocator>												Myt;
		typedef T																value_type;
		typedef Allocator														allocator_type;
		typedef ptrdiff_t														difference_type;
		typedef value_type&														reference;
		typedef const value_type&												const_reference;
		typedef typename Allocator::pointer										pointer;
		typedef typename Allocator::const_pointer								const_pointer;
		typedef __list_iterator<value_type, reference, pointer>					iterator;
		typedef const iterator													const_iterator;
		typedef __list_reverse_iterator<value_type, reference, pointer>			reverse_iterator;
		typedef const reverse_iterator											const_reverse_iterator;
		typedef std::size_t														size_type;
		typedef typename __type_traits<T>::is_POD_type							is_POD;

		//////////////////////////////////////////
		//Constructor
		/////////////////////////////////////////////

		//Default constructor
		explicit list(const Allocator& alloc = Allocator())
		{
			allocat = alloc;
			node = get_node();
			node->next = node;
			node->prev = node;
		}

		//Constructs the container with count copies of elements with value value.
		list(size_type count, const T& value, const Allocator& alloc = Allocator())
		{
			__initialize_n(count, value, alloc);
		}
		list(int count, const T& value, const Allocator& alloc = Allocator())
		{
			__initialize_n(size_type(count), value, alloc);
		}
		list(long count, const T& value, const Allocator& alloc = Allocator())
		{
			__initialize_n(size_type(count), value, alloc);
		}
		explicit list(size_type count)
		{
			__initialize_n(count, T(), Allocator());
		}

		//Constructs the container with the contents of the range [first, last). 
		template< class InputIt >
		list(InputIt first, InputIt last, const Allocator& alloc = Allocator())
		{
			__initialize_iter(first, last, alloc);
		}

		//Copy constructor. Constructs the container with the copy of the contents of other
		list(const list& other)
		{
			__initialize_iter(other.begin(), other.end(), Allocator());

		}
		list(const list& other, const Allocator& alloc)
		{
			__initialize_iter(other.begin(), other.end(), alloc);
		}

		//Move constructor. 
		list(list&& other)
		{
			allocat = other.allocat;
			alloc_node = other.alloc_node;
			node = get_node();
			node->next = other.node->next;
			node->prev = other.node->prev;
			other.node->next->prev = node;
			other.node->prev->next = node;
			//使other前后指针指向自己
			other.node->prev = other.node->next = other.node;
		}
		list(list&& other, const Allocator& alloc)
		{
			auto other_alloc = other.get_allocator();
			if (alloc != other.get_allocator())
			{
				__initialize_iter(other.begin(), other.end(), alloc);
				//销毁other list内的元素，释放空间
				for (auto iter = other.begin(); iter != other.end(); ++iter)
					other.destroy_node(iter);
				other.put_node(other.node);
			}
			else
			{
				allocat = other.allocat;
				alloc_node = other.alloc_node;
				node = get_node();
				node->next = other.node->next;
				node->prev = other.node->prev;
				other.node->next->prev = node;
				other.node->prev->next = node;
				//使other前后指针指向自己
				other.node->prev = other.node->next = other.node;
			}
		}

		//Constructs the container with the contents of the initializer list init
		list(std::initializer_list<T> lst, const Allocator& alloc = Allocator())
		{
			__initialize_iter(lst.begin(), lst.end(), alloc);
		}

		/////////////////////////////////////////////
		//Destructor
		//////////////////////////////////////////////
		~list()
		{
			//销毁list内的元素，释放空间
			for (auto iter = begin(); iter != end(); ++iter)
				destroy_node(iter);
			put_node(node);
		}

		////////////////////////////////////////////
		//operator=
		///////////////////////////////////////////

		//Copy assignment operator.
		list& operator=(const list& other)
		{
			iterator p, q;
			for (p = begin(), q = other.begin(); p != end() && q != other.end(); ++p, ++q)
			{
				*p = *q;
			}
			//size()大于other.size()
			if (p != end())
			{
				p.node->prev->next = node;
				node->prev = p.node->prev;
				//销毁list内的元素，释放空间
				for (auto iter = p; iter != end(); ++iter)
					destroy_node(iter);
			}
			else if (q != other.end())
			{
				//size()小于other.size()
				for (auto iter = q; iter != other.end(); ++iter)
					insert(end(), *iter);
			}
			return *this;
		}
		//Move assignment operator.
		list& operator=(list&& other)
		{
			//销毁list内的元素，释放空间
			for (auto iter = begin(); iter != end(); ++iter)
				destroy_node(iter);
			allocat = other.allocat;
			alloc_node = other.alloc_node;

			node->next = other.node->next;
			node->prev = other.node->prev;
			other.node->next->prev = node;
			other.node->prev->next = node;

			//使other前后指针指向自己
			other.node->prev = other.node->next = other.node;

			return *this;
		}
		//Replaces the contents with those identified by initializer list ilist.
		list& operator=(std::initializer_list<T> other)
		{
			iterator p;
			const_pointer q;
			for (p = begin(), q = other.begin(); p != end() && q != other.end(); ++p, ++q)
			{
				*p = *q;
			}
			//size()大于other.size()
			if (p != end())
			{
				p.node->prev->next = node;
				node->prev = p.node->prev;
				//销毁list内的元素，释放空间
				for (auto iter = p; iter != end(); ++iter)
					destroy_node(iter);
			}
			else if (q != other.end())
			{
				//size()小于other.size()
				for (auto iter = q; iter != other.end(); ++iter)
					insert(end(), *iter);
			}
			return *this;
		}

		////////////////////////////////////////////
		//assign
		///////////////////////////////////////////

		// Replaces the contents with count copies of value value
		void assign(size_type count, const T& value)
		{
			__assign_n(count, value);
		}
		void assign(int count, const T& value)
		{
			__assign_n(count, value);
		}
		void assign(long count, const T& value)
		{
			__assign_n(count, value);
		}
		template< class InputIt >
		void assign(InputIt first, InputIt last)
		{
			iterator p;
			InputIt q;
			for (p = begin(), q = first; p != end() && q != last; ++p, ++q)
			{
				*p = *q;
			}
			//size()大于other.size()
			if (p != end())
			{
				p.node->prev->next = node;
				node->prev = p.node->prev;
				//销毁list内的元素，释放空间
				for (auto iter = p; iter != end(); ++iter)
					destroy_node(iter);
			}
			else if (q != last)
			{
				//size()小于other.size()
				for (auto iter = q; iter != last; ++iter)
					insert(end(), *iter);
			}
		}

		void assign(std::initializer_list<T> other)
		{
			operator=(other);
		}

		//Returns the allocator associated with the container
		Allocator get_allocator()
		{
			return allocat;
		}

		/////////////////////////////////////
		//Element access 
		//////////////////////////////////
		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference back() { return *(--end()); }
		const_reference back() const { return *(const_iterator(end().node->prev)); }

		/////////////////////////////////////////
		//Iterators 
		/////////////////////////////////////////////
		iterator begin(){ return (link_type)(node->next); }
		const_iterator begin() const { return (link_type)(node->next); }
		const_iterator cbegin() const { return ((const Myt*)this)->begin(); }
		reverse_iterator rbegin() { return (link_type)(node->prev); }
		reverse_iterator rbegin() const { return (link_type)(node->prev); }
		const_reverse_iterator crbegin() const { return ((const Myt*)this)->rbegin(); }
		iterator end() { return node; }
		const_iterator end() const{ return node; }
		const_iterator cend() const{ return ((const Myt*)this)->node; }
		reverse_iterator rend() { return node; }
		const_reverse_iterator rend() const{ return node; }
		const_reverse_iterator crend() const { return ((const Myt*)this)->node; }

		/////////////////////////////////////////
		//Capacity
		/////////////////////////////////////////////
		bool empty() const { return node->next == node; }
		size_type size() const{ return distance(node->next, node); }
		size_type max_size() const { return std::numeric_limits<size_type>::max(); }

		/////////////////////////////////////////
		//Modifiers
		/////////////////////////////////////////////

		//Removes all elements from the container. 
		void clear()
		{
			//销毁list内的元素，释放空间
			for (auto iter = begin(); iter != end(); ++iter)
				destroy_node(iter);
			node->next = node;
			node->prev = node;
		}

		//Inserts elements at the specified location in the container. 
		iterator insert(const_iterator pos, const T& value)
		{
			//创建插入节点
			link_type p = create_node(value);
			//插入
			p->next = pos.node;
			p->prev = pos.node->prev;
			pos.node->prev->next = p;
			pos.node->prev = p;
			return p;
		}

		iterator insert(const_iterator pos, T&& value)
		{
			//创建插入节点
			link_type p = __create_move_node(std::move(value), is_POD());
			//插入
			p->next = pos.node;
			p->prev = pos.node->prev;
			pos.node->prev->next = p;
			pos.node->prev = p;
			return p;
		}

		iterator insert(const_iterator pos, size_type count, const T& value)
		{
			size_type n = count;
			link_type first = pos.node->prev;
			link_type p = first, q;
			while (n--)
			{
				q = create_node(value);
				p->next = q;
				q->prev = p;
				p = q;
			}
			p->next = pos.node;
			pos.node->prev = p;
			return first->next;
		}
		iterator insert(const_iterator pos, int count, const T& value)
		{
			return insert(pos, size_type(count), value);
		}
		iterator insert(const_iterator pos, long count, const T& value)
		{
			return insert(pos, size_type(count), value);
		}

		template< class InputIt >
		iterator insert(iterator pos, InputIt first, InputIt last)
		{
			link_type p_begin = pos.node->prev;
			link_type p = p_begin, q;
			for (InputIt iter = first; iter != last; ++iter)
			{
				q = create_node(*iter);
				p->next = q;
				q->prev = p;
				p = q;
			}
			p->next = pos.node;
			pos.node->prev = p;
			return p_begin->next;
		}

		iterator insert(const_iterator pos, std::initializer_list<T> ilist)
		{
			return insert(pos, ilist.begin(), ilist.end());
		}

		//Appends a new element to the end of the container. 
		template< class... Args >
		void emplace_back(Args&&... args)
		{
			//创建插入节点
			link_type p = get_node();
			allocat.construct(&(p->data), std::forward<Args>(args)...);
			//插入
			p->next = node;
			p->prev = node->prev;
			node->prev->next = p;
			node->prev = p;
		}
		//Inserts a new element into the container directly before pos.
		template< class... Args >
		iterator emplace(const_iterator pos, Args&&... args)
		{
			//创建插入节点
			link_type p = get_node();
			allocat.construct(&(p->data), std::forward<Args>(args)...);
			//插入
			p->next = pos.node;
			p->prev = pos.node->prev;
			pos.node->prev->next = p;
			pos.node->prev = p;
			return p;
		}

		//Removes the element at pos. Return the iterator following the last removed element.
		iterator erase(const_iterator pos)
		{
			link_type pos_next = pos.node->next;
			pos.node->prev->next = pos_next;
			pos_next->prev = pos.node->prev;
			destroy_node(pos);
			return pos_next;
		}

		//Removes the elements in the range [first; last).
		//Return the iterator following the last removed element.
		iterator erase(const_iterator first, const_iterator last)
		{
			if (first == last)
				return first;
			link_type p = first.node->prev;
			p->next = last.node;
			last.node->prev = p;
			iterator tmp = first.node;
			for (; tmp != last.node; ++tmp)
			{
				destroy_node(tmp);
			}
			return tmp;
		}
		
	private:
		Allocator allocat;
		allocator<node_type> alloc_node;
		link_type node;
		//配置一个节点
		link_type get_node(){ return alloc_node.allocate(); }
		//释放一个节点
		void put_node(link_type p){ alloc_node.deallocate(p); }
		void put_node(iterator p){ alloc_node.deallocate(p.node); }
		//产生一个节点并构造
		link_type create_node(const T &value)
		{
			link_type p = get_node();
			allocat.construct(&(p->data), value);
			return p;
		}
		//销毁一个节点并释放空间
		void destroy_node(link_type p)
		{
			allocat.destroy(&(p->data));
			put_node(p);
		}
		void destroy_node(iterator p)
		{
			allocat.destroy(&(p.node->data));
			put_node(p);
		}

		Allocator get_node_allocator()
		{
			return alloc_node;
		}

		//n个元素初始化
		void __initialize_n(size_type count, const T &value, const Allocator& alloc)
		{
			allocat = alloc;
			node = get_node();
			link_type p = node, q;
			for (size_type i = 0; i != count; ++i)
			{
				if (i == 0)
				{
					p = create_node(value);
					node->next = p;
					p->prev = node;
				}
				else
				{
					q = create_node(value);
					p->next = q;
					q->prev = p;
					p = q;
				}
			}
			p->next = node;
			node->prev = p;
		}

		//迭代器范围初始化
		template< class InputIt >
		void __initialize_iter(InputIt first, InputIt last, const Allocator& alloc = Allocator())
		{
			allocat = alloc;
			node = get_node();
			link_type p = node, q;
			for (InputIt iter = first; iter != last; ++iter)
			{
				if (iter == first)
				{
					p = create_node(*iter);
					node->next = p;
					p->prev = node;
				}
				else
				{
					q = create_node(*iter);
					p->next = q;
					q->prev = p;
					p = q;
				}
			}
			p->next = node;
			node->prev = p;
		}

		void __assign_n(size_type count, const T& value)
		{
			iterator p; 
			size_type q;
			for (p = begin(), q = 0; p != end() && q != count; ++p, ++q)
			{
				*p = value;
			}
			//size()大于other.size()
			if (p != end())
			{
				p.node->prev->next = node;
				node->prev = p.node->prev;
				//销毁list内的元素，释放空间
				for (auto iter = p; iter != end(); ++iter)
					destroy_node(iter);
			}
			else if (q != count)
			{
				//size()小于other.size()
				for (auto iter = q; iter != count; ++iter)
					insert(end(), value);
			}
		}

		link_type __create_move_node(T &&value, __true_type)
		{
			link_type p = get_node();
			memmove(&(p->data), &value, sizeof(T) / sizeof(unsigned char));
			return p;
		}
		link_type __create_move_node(T &&value, __false_type)
		{
			return create_node(value);
		}
		
	};
}
#endif