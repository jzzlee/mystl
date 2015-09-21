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

		static std::size_t buffer_size(){ return __deque_buf_size(sizeof(T)); }

		T *cur;	//迭代器所指缓冲区内指向的元素
		T *first;//迭代器所指缓冲区的头
		T *last;//迭代器所指缓冲区的尾后元素
		map_pointer node;//指向中控器

		static inline size_type __deque_buf_size(size_type sz)
		{
			return sz < 512 ? size_type(512 / sz) : size_type(1);
		}

		void set_node(map_pointer new_node)
		{
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
		}

		//constructor
		__deque_iterator(T *cur, T *first, T *last, map_pointer nd) : cur(cur), first(first), last(last), node(nd){}
		__deque_iterator() : cur(nullptr), first(nullptr), last(nullptr), node(0) {}
		__deque_iterator(const iterator &x) : cur(x.cur), first(x.first), last(x.last), node(x.node) {}
		__deque_iterator(const const_iterator &x) : cur(x.cur), first(x.first), last(x.last), node(x.node) {}

		bool operator==(const self &x) const { return cur == x.cur; }
		bool operator!=(const self &x) const { return !(*this == x); }
		bool operator<(const self &x) const { return (node == x.node) ? (cur < x.cur) : (node < x.node); }
		reference operator*() const { return *cur; }
		pointer operator->() const { return &(operator*()); }

		self &operator=(const self &x)
		{
			cur = x.cur; first = x.first; last = x.last; node = x.node;
			return *this;
		}

		self operator++()
		{
			++cur;
			if (cur == last) //cur指向当前缓冲区末尾元素，需要调整到下一个缓冲区
			{
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			++(*this);
			return tmp;
		}
		self operator--()
		{
			if (cur == first) //cur指向当前缓冲区起始位置，需要调整到上一个缓冲区
			{
				set_node(node - 1);
				cur = last;
			}
			--cur;
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			--(*this);
			return tmp;
		}

		difference_type operator-(const self &other) const
		{
			return (node - other.node - 1) * buffer_size() + (cur - first) + (other.last - other.cur);
		}

		self &operator+=(difference_type n)
		{
			difference_type offset = n + (cur - first);
			if (offset >= 0 && offset < difference_type(buffer_size()))
				cur += n;
			else
			{
				difference_type node_offset =
					offset > 0 ? offset / buffer_size() : -((-offset - 1) / difference_type(buffer_size())) - 1;
				set_node(node + node_offset);
				cur = first + (offset - node_offset * buffer_size());
			}
			return *this;
		}
		self &operator-=(difference_type n)
		{
			*this += -n;
			return *this;
		}
		self operator+(difference_type n) const
		{
			self tmp = *this;
			tmp += n;
			return tmp;
		}
		self operator-(difference_type n) const
		{
			self tmp = *this;
			tmp -= n;
			return tmp;
		}

		reference operator[](difference_type n) const { return *(*this + n); }

	};

	//deque的反向迭代器
	template<typename T, typename Ref, typename Ptr>
	struct __deque_reverse_iterator
	{

		typedef	__deque_reverse_iterator<T, T&, T*>						reverse_iterator;
		typedef	__deque_reverse_iterator<T, const T&, const T*>			const_reverse_iterator;
		typedef __deque_iterator<T, Ref, Ptr>							iterator;
		typedef	__deque_iterator<T, const T&, const T*>					const_iterator;
		typedef	__deque_reverse_iterator<T, Ref, Ptr>					self;
		typedef	random_access_iterator_tag								reverse_iterator_category;
		typedef	T								value_type;
		typedef Ref								reference;
		typedef Ptr								pointer;
		typedef	std::size_t						size_type;
		typedef ptrdiff_t						difference_type;
		typedef T**								map_pointer;

		static std::size_t buffer_size(){ return __deque_buf_size(sizeof(T)); }

		T *cur;	//迭代器所指缓冲区内指向的元素
		T *rfirst;//迭代器所指缓冲区的尾
		T *rlast;//迭代器所指缓冲区的首前元素
		map_pointer node;//指向中控器

		static inline size_type __deque_buf_size(size_type sz)
		{
			return sz < 512 ? size_type(512 / sz) : size_type(1);
		}

		void set_node(map_pointer new_node)
		{
			node = new_node;
			rlast = *new_node - 1;
			rfirst = rlast + difference_type(buffer_size());
		}

		//constructor
		__deque_reverse_iterator(T *cur, T *rfirst, T *rlast, map_pointer nd) : cur(cur), rfirst(rfirst), rlast(rlast), node(nd){}
		__deque_reverse_iterator() : cur(nullptr), rfirst(nullptr), rlast(nullptr), node(0) {}
		__deque_reverse_iterator(const reverse_iterator &x) : cur(x.cur), rfirst(x.rfirst), rlast(x.rlast), node(x.node) {}
		__deque_reverse_iterator(const const_reverse_iterator &x) : cur(x.cur), rfirst(x.rfirst), rlast(x.rlast), node(x.node) {}
		//		__deque_reverse_iterator(const iterator &x) : cur(x.cur), rfirst(x.last - 1), rlast(x.first - 1), node(x.node) {}
		__deque_reverse_iterator(const const_iterator &x) : cur(x.cur), rfirst(x.last - 1), rlast(x.first - 1), node(x.node) {}

		bool operator==(const self &x) const { return cur == x.cur; }
		bool operator!=(const self &x) const { return !(*this == x); }
		reference operator*() const { return *cur; }
		pointer operator->() const { return &(operator*()); }

		self &operator=(const self &x)
		{
			cur = x.cur; rfirst = x.rfirst; rlast = x.rlast; node = x.node;
			return *this;
		}

		self operator++()
		{
			--cur;
			if (cur == rlast) //cur指向当前缓冲区首前位置，需要调整到上一个缓冲区
			{
				set_node(node - 1);
				cur = rfirst;
			}
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			++(*this);
			return tmp;
		}
		self operator--()
		{
			if (cur == rfirst) //cur指向当前缓冲区起始位置，需要调整到上一个缓冲区
			{
				set_node(node + 1);
				cur = rlast;
			}
			++cur;
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			--(*this);
			return tmp;
		}

		difference_type operator-(const self &other) const
		{
			return (other.node - node - 1) * buffer_size() + (rfirst - cur) + (other.cur - other.rlast);
		}

		self &operator+=(difference_type n)
		{
			difference_type offset = n + (first - cur);
			if (offset >= 0 && offset < difference_type(buffer_size()))
				cur -= n;
			else
			{
				difference_type node_offset =
					offset > 0 ? offset / buffer_size() : -((-offset - 1) / buffer_size()) - 1;
				set_node(node + node_offset);
				cur = rlast + ((node_offset + 1) * buffer_size() - offset);
			}
			return *this;
		}
		self &operator-=(difference_type n)
		{
			*this += -n;
			return *this;
		}

		self operator+(difference_type n) const
		{
			self tmp = *this;
			tmp += n;
			return tmp;
		}
		self operator-(difference_type n) const
		{
			self tmp = *this;
			tmp -= n;
			return tmp;
		}

		reference operator[](difference_type n) const { return *(*this + n); }
	};

	template<class T, class Allocator = allocator<T> >
	class deque
	{
	public:
		typedef deque<T, Allocator>		Myt;
		typedef T		value_type;
		typedef Allocator		allocator_type;
		typedef ptrdiff_t		difference_type;
		typedef value_type&		reference;
		typedef const value_type&		const_reference;
		typedef typename Allocator::pointer			pointer;
		typedef typename Allocator::const_pointer	const_pointer;
		typedef __deque_iterator<value_type, reference, pointer>	iterator;
		typedef __deque_iterator<value_type, const_reference, const_pointer>	const_iterator;
		typedef __deque_reverse_iterator<value_type, reference, pointer>		reverse_iterator;
		typedef __deque_reverse_iterator<value_type, const_reference, const_pointer>	const_reverse_iterator;
		typedef std::size_t		size_type;
		typedef pointer*		map_pointer;
		typedef typename __type_traits<T>::is_POD_type		is_POD;

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//Member functions
		///////////////////////////////////////////////////////////////////////////////////////////////////////

		//Constructor
		explicit deque(const Allocator& alloc = Allocator()) : alloc(alloc), map(0), end_map(0), map_size(0), start(), finish() {}

		deque(size_type count, const T& value, const Allocator& alloc = Allocator())
			:alloc(alloc)
		{
			__map_initialize(count);
			__fill_initialize(count, value);
		}
		deque(int count, const T& value, const Allocator& alloc = Allocator())
			:alloc(alloc)
		{
			__map_initialize(count);
			__fill_initialize(count, value);
		}
		deque(long count, const T& value, const Allocator& alloc = Allocator())
			:alloc(alloc)
		{
			__map_initialize(count);
			__fill_initialize(count, value);
		}

		explicit deque(size_type count)
		{
			__map_initialize(count);
			__fill_initialize(count, value_type());
		}

		template< class InputIt >
		deque(InputIt first, InputIt last, const Allocator& alloc = Allocator())
			:alloc(alloc)
		{
			__map_initialize(my_stl::distance(first, last));
			__copy_initialize(first, last);
		}

		//Copy constructor.
		deque(const deque& other)
			:alloc(other.alloc)
		{
			if (other.map_size != 0)
			{
				__map_initialize(my_stl::distance(other.begin(), other.end()));
				__copy_initialize(other.begin(), other.end());
			}
			else
			{
				map = map_pointer(); map_size = 0;
				start = finish = iterator();
			}
		}
		deque(const deque& other, const Allocator& alloc)
			:alloc(alloc)
		{
			if (other.map_size != 0)
			{
				__map_initialize(my_stl::distance(other.begin(), other.end()));
				__copy_initialize(other.begin(), other.end());
			}
			else
			{
				map = map_pointer(); map_size = 0;
				start = finish = iterator();
			}
		}

		//Move constructor
		deque(deque&& other)
		{
			if (alloc == other.get_allocator())
			{
				start = other.start; finish = other.finish;
				map = other.map; end_map = other.end_map;
				map_size = other.map_size;
			}
			else
			{
				//allocator类型不一致，需要逐个元素复制
				if (other.map_size != 0)
				{
					__map_initialize(my_stl::distance(other.begin(), other.end()));
					__copy_initialize(other.begin(), other.end());
				}
				else
				{
					map = map_pointer();
					end_map = map_pointer();
					map_size = 0;
					start = finish = iterator();
				}
				other.__destroy_and_destruct();
			}
			other.start = other.finish = iterator();
			other.map = other.end_map = map_pointer(0);
			other.map_size = 0;
		}

		deque(deque&& other, const Allocator& allc)
			:alloc(allc)
		{
			if (alloc == other.get_allocator())
			{
				start = other.start; finish = other.finish;
				map = other.map; map_size = other.map_size;
			}
			else
			{

				//allocator类型不一致，需要逐个元素复制
				if (other.map_size != 0)
				{
					__map_initialize(my_stl::distance(other.begin(), other.end()));
					__copy_initialize(other.begin(), other.end());
				}
				else
				{
					map = map_pointer(); map_size = 0;
					start = finish = iterator();
				}
				other.__destroy_and_destruct();
			}
			other.start = other.finish = nullptr;
			other.map = other.end_map = map_pointer(0);
			other.map_size = 0;
		}

		deque(std::initializer_list<T> init,
			const Allocator& alloc = Allocator())
			:alloc(alloc)
		{
			//allocator类型不一致，需要逐个元素复制
			if (init.size() != 0)
			{
				__map_initialize(my_stl::distance(init.begin(), init.end()));
				__copy_initialize(init.begin(), init.end());
			}
			else
			{
				map = end_map = map_pointer();
				map_size = 0;
				start = finish = iterator();
			}
		}

		//Destructor
		~deque()
		{
			__destroy_and_destruct();
		}

		deque& operator=(const deque& other)
		{
			if (this != &other)
			{
				size_type other_size = other.size();
				size_type space_size = iterator::buffer_size(); //一个缓冲区的容量
				size_type capacity = (end_map - start.node) * space_size; // 当前deque的容量
				size_type down_capacity = (map + map_size - end_map) * space_size + capacity; //map向下扩展的总容量
				size_type up_capacity = (start.node - map) * space_size + capacity; //map向上扩展的总容量
				size_type all_capacity = map_size * space_size; //不申请新map的最大容量
				if (size() >= other_size)
				{
					my_stl::copy(other.begin(), other.end(), start);
					iterator new_finish = start + other_size;
					//删除多余元素
					while (finish != new_finish)
						alloc.destroy((--finish).cur);
					finish = new_finish;
				}
				else if (capacity > other_size)
				{
					copy(other.begin(), other.begin() + size(), start);
					my_stl::uninitialized_copy(other.begin() + size(), other.end(), finish);
					finish = start + other_size;
				}
				else if (down_capacity > other_size) //如果向下拓展空间足够
				{
					copy(other.begin(), other.begin() + size(), start);
					__alloc_new_space(other_size - capacity);  //申请新缓冲区
					finish = my_stl::uninitialized_copy(other.begin() + size(), other.end(), finish);
				}
				else if (up_capacity > other_size) //如果向上拓展空间足够
				{
					map_pointer map_start = __alloc_new_space_forward(other_size - capacity);  //申请新缓冲区
					start.set_node(map_start);
					start.cur = start.first;
					finish = my_stl::copy(other.begin(), other.end(), start);
				}
				else if (all_capacity > other_size) //全部拓展空间足够
				{
					map_pointer map_start = __alloc_new_space_forward(other_size - capacity);  //向上申请新缓冲区
					start.set_node(map_start);
					start.cur = start.first;
					copy(other.begin(), other.begin() + size(), start);
					__alloc_new_space(other_size - up_capacity);  //申请新缓冲区
					finish = my_stl::uninitialized_copy(other.begin() + size(), other.end(), finish);
				}
				else
				{
					//如果不足以存放other的所有元素，则需要重新申请map
					//申请新空间
					__alloc_new_map(other_size);
					finish = my_stl::uninitialized_copy(other.begin(), other.end(), start);
				}
			}
			return *this;
		}

		deque& operator=(deque&& other)
		{
			if (this != &other)
			{
				if (alloc == other.get_allocator())//allocator类型一致，直接复制控制信息
				{
					start = other.start; finish = other.finish;
					map = other.map; end_map = other.end_map;
					map_size = other.map_size;
				}
				else
				{
					//allocator类型不一致，需要逐个元素复制
					*this = other;
					other.__destroy_and_destruct();
				}
				other.start = other.finish = iterator();
				other.map = other.end_map = map_pointer(0);
				other.map_size = 0;
			}
			return *this;
		}

		deque& operator=(std::initializer_list<T> ilist)
		{
			size_type ilist_size = ilist.size();
			size_type space_size = iterator::buffer_size(); //一个缓冲区的容量
			size_type capacity = (end_map - start.node) * space_size; // 当前deque的容量
			size_type down_capacity = (map + map_size - end_map) * space_size + capacity; //map向下扩展的总容量
			size_type up_capacity = (start.node - map) * space_size + capacity; //map向上扩展的总容量
			size_type all_capacity = map_size * space_size; //不申请新map的最大容量
			if (size() >= ilist_size)
			{
				my_stl::copy(ilist.begin(), ilist.end(), start);
				iterator new_finish = start + ilist_size;
				//删除多余元素
				while (finish != new_finish)
					alloc.destroy((--finish).cur);
				finish = new_finish;
			}
			else if (capacity > ilist_size)
			{
				copy(ilist.begin(), ilist.begin() + size(), start);
				my_stl::uninitialized_copy(ilist.begin() + size(), ilist.end(), finish);
				finish = start + ilist_size;
			}
			else if (down_capacity > ilist_size) //如果向下拓展空间足够
			{
				copy(ilist.begin(), ilist.begin() + size(), start);
				__alloc_new_space(ilist_size - capacity);  //申请新缓冲区
				finish = my_stl::uninitialized_copy(ilist.begin() + size(), ilist.end(), finish);
			}
			else if (up_capacity > ilist_size) //如果向上拓展空间足够
			{
				map_pointer map_start = __alloc_new_space_forward(ilist_size - capacity);  //申请新缓冲区
				start.set_node(map_start);
				start.cur = start.first;
				finish = my_stl::copy(ilist.begin(), ilist.end(), start);
			}
			else if (all_capacity > ilist_size) //全部拓展空间足够
			{
				map_pointer map_start = __alloc_new_space_forward(ilist_size - capacity);  //向上申请新缓冲区
				start.set_node(map_start);
				start.cur = start.first;
				copy(ilist.begin(), ilist.begin() + size(), start);
				__alloc_new_space(ilist_size - up_capacity);  //申请新缓冲区
				finish = my_stl::uninitialized_copy(ilist.begin() + size(), ilist.end(), finish);
			}
			else
			{
				//如果不足以存放other的所有元素，则需要重新申请map
				//申请新空间
				__alloc_new_map(ilist_size);
				finish = my_stl::uninitialized_copy(ilist.begin(), ilist.end(), start);
			}
			return *this;
		}

		void assign(size_type count, const T& value)
		{
			size_type space_size = iterator::buffer_size(); //一个缓冲区的容量
			size_type capacity = (end_map - start.node) * space_size; // 当前deque的容量
			size_type down_capacity = (map + map_size - end_map) * space_size + capacity; //map向下扩展的总容量
			size_type up_capacity = (start.node - map) * space_size + capacity; //map向上扩展的总容量
			size_type all_capacity = map_size * space_size; //不申请新map的最大容量
			if (size() >= count)
			{
				iterator new_finish = my_stl::fill_n(start, count, value);
				//删除多余元素
				while (finish != new_finish)
					alloc.destroy((--finish).cur);
				finish = new_finish;
			}
			else if (capacity > count)
			{
				my_stl::fill_n(start, size(), value);
				finish = my_stl::uninitialized_fill_n(finish, count - size(), value);
			}
			else if (down_capacity > count) //如果向下拓展空间足够
			{
				my_stl::fill_n(start, size(), value);
				__alloc_new_space(count - capacity);  //申请新缓冲区
				finish = my_stl::uninitialized_fill_n(finish, count - size(), value);
			}
			else if (up_capacity > count) //如果向上拓展空间足够
			{
				map_pointer map_start = __alloc_new_space_forward(count - capacity);  //申请新缓冲区
				my_stl::fill_n(start, size(), value);
				start -= count - size();
				my_stl::uninitialized_fill_n(start, count - size(), value);
			}
			else if (all_capacity > count) //全部拓展空间足够
			{
				my_stl::fill_n(start, size(), value);
				difference_type offset = (count - capacity) / 2;
				__alloc_new_space_forward(count - capacity - offset);  //向上申请新缓冲区
				__alloc_new_space(count - capacity - offset);  //向下申请新缓冲区
				start -= offset;
				my_stl::uninitialized_fill_n(start, size_type(offset), value);
				finish = my_stl::uninitialized_fill_n(finish, size_type(count - size()), value);
			}
			else
			{
				//如果不足以存放other的所有元素，则需要重新申请map
				//申请新空间
				__alloc_new_map(count);
				finish = my_stl::uninitialized_fill_n(start, count, value);
			}
		}

		void assign(int count, const T& value) { assign(size_type(count), value); }
		void assign(long count, const T& value) { assign(size_type(count), value); }

		template< class InputIt >
		void assign(InputIt first, InputIt last)
		{
			size_type count = my_stl::distance(first, last);
			size_type space_size = iterator::buffer_size(); //一个缓冲区的容量
			size_type capacity = (end_map - start.node) * space_size; // 当前deque的容量
			size_type down_capacity = (map + map_size - end_map) * space_size + capacity; //map向下扩展的总容量
			size_type up_capacity = (start.node - map) * space_size + capacity; //map向上扩展的总容量
			size_type all_capacity = map_size * space_size; //不申请新map的最大容量
			if (size() >= count)
			{
				iterator new_finish = my_stl::copy(first, last, start);
				//删除多余元素
				while (finish != new_finish)
					alloc.destroy((--finish).cur);
				finish = new_finish;
			}
			else if (capacity > count)
			{
				my_stl::copy(first, first + size(), start);
				finish = my_stl::uninitialized_copy(first + size(), last, finish);
			}
			else if (down_capacity > count) //如果向下拓展空间足够
			{
				my_stl::copy(first, first + size(), start);
				__alloc_new_space(count - capacity);  //申请新缓冲区
				finish = my_stl::uninitialized_copy(first + size(), last, finish);
			}
			else if (up_capacity > count) //如果向上拓展空间足够
			{
				__alloc_new_space_forward(count - capacity);  //申请新缓冲区
				size_type old_size = size();
				my_stl::copy(last - old_size, last, start);
				start -= count - size();
				my_stl::uninitialized_copy(first, last - old_size, start);
			}
			else if (all_capacity > count) //全部拓展空间足够
			{
				difference_type offset = (count - capacity) / 2;
				my_stl::copy(first + offset, first + offset + size(), start);
				__alloc_new_space_forward(count - capacity - offset);  //向上申请新缓冲区
				__alloc_new_space(count - capacity - offset);  //向下申请新缓冲区
				start -= offset;
				my_stl::uninitialized_copy(first, first + offset, start);
				finish = my_stl::uninitialized_copy(first + size(), last, finish);
			}
			else
			{
				//如果不足以存放count个元素，则需要重新申请map
				//申请新空间
				__alloc_new_map(count);
				finish = my_stl::uninitialized_copy(first, last, start);
			}
		}

		void assign(std::initializer_list<T> ilist)
		{
			*this = ilist;
		}

		allocator_type get_allocator() { return alloc; }

		reference at(size_type pos)
		{
			if (pos < 0 || pos >= size())
				throw std::out_of_range("out of the range of the deque\n");
			return start[difference_type(pos)];
		}
		const_reference at(size_type pos) const
		{
			if (pos < 0 || pos >= size())
				throw std::out_of_range("out of the range of the deque\n");
			return start[difference_type(n)];
		}
		reference operator[](size_type n){ return start[difference_type(n)]; }
		const_reference operator[](size_type n) const { return start[difference_type(n)]; }
		reference front(){ return *start; }
		const_reference front() const { return *start; }
		reference back()
		{
			iterator tmp = finish;
			--tmp;
			return *tmp;
		}
		const_reference back() const
		{
			const_iterator tmp = finish;
			--tmp;
			return *tmp;
		}
		iterator begin() { return start; }
		iterator end() { return finish; }
		const_iterator begin() const { return start; }
		const_iterator end() const { return finish; }
		const_iterator cbegin() const { return ((const Myt *)this)->begin(); }
		const_iterator cend() const { return ((const Myt*)this)->end(); }

		bool empty() { return start == finish; }
		size_type size() const { return (start == finish ? 0 : finish - start); }
		size_type max_size() const { return std::numeric_limits<size_type>::max(); }

		void shrink_to_fit()
		{
			if (map)
			{
				size_type space_size = iterator::buffer_size();
				//释放那些在finish.node之后的中控节点指向的空缓冲区
				for (map_pointer p = finish.node + 1; p != end_map; ++p)
				{
					alloc.deallocate(*p, space_size);
					//					*p = pointer();
				}
				end_map = finish.node + 1;
			}
		}

		void clear()
		{
			difference_type space_size = iterator::buffer_size();
			//清理元素
			while (finish != start)
				alloc.destroy((--finish).cur);
		}

		//iterator insert(const_iterator pos, const T& value)
		//{
		//	iterator tmp_pos(pos);
		//	size_type count = size() + 1; //全部元素数量
		//	size_type space_size = iterator::buffer_size(); //一个缓冲区的容量
		//	size_type capacity = (end_map - start.node) * space_size; // 当前deque的容量
		//	size_type down_capacity = (map + map_size - end_map) * space_size + capacity; //map向下扩展的总容量
		//	size_type up_capacity = (start.node - map) * space_size + capacity; //map向上扩展的总容量
		//	size_type all_capacity = map_size * space_size; //不申请新map的最大容量
		//	
		//	difference_type offset = tmp_pos - start; //距离开始的距离
		//	if (capacity > count)  //不需要重新申请缓冲区
		//	{
		//		if (offset > size() / 2) //插入点靠前
		//		{
		//			if (start.cur != start.first)//向前有空
		//			{
		//				my_stl::copy(start, tmp_pos, start - 1);
		//				--start;
		//				*pos = value;
		//			}
		//			else  //if (finish.node != end_map - 1 || finish.cur != finish.last - 1)//向后有空
		//			{
		//				alloc.construt(finish.cur, *(finish - 1);
		//				for (iterator p = finish - 1; p != tmp_pos; --p)
		//					*p = *(p - 1);
		//				++finish;
		//				*pos = value;
		//			}
		//		}
		//		else   //插入点靠后
		//		{
		//			if (finish.node != end_map - 1 || finish.cur != finish.last - 1)//向后有空
		//			{
		//				alloc.construt(finish.cur, *(finish - 1);
		//				for (iterator p = finish - 1; p != tmp_pos; --p)
		//					*p = *(p - 1);
		//				++finish;
		//				*pos = value;
		//			}
		//			else   //向前有空
		//			{
		//				my_stl::copy(start, tmp_pos, start - 1);
		//				--start;
		//				*pos = value;
		//			}
		//		}
		//	}
		//	else if (down_capacity > count) //如果向下拓展空间足够
		//	{
		//		__alloc_new_space(count - capacity);  //申请新缓冲区
		//		alloc.construt(finish.cur, *(finish - 1);
		//		for (iterator p = finish - 1; p != tmp_pos; --p)
		//			*p = *(p - 1);
		//		++finish;
		//		*pos = value;
		//	}
		//	else if (up_capacity > count) //如果向上拓展空间足够
		//	{
		//		__alloc_new_space_forward(count - capacity);  //申请新缓冲区
		//		my_stl::copy(start, tmp_pos, start - 1);
		//		--start;
		//		*pos = value;
		//	}
		//	else
		//	{
		//		//如果不足以存放count个元素，则需要重新申请map
		//		//申请新空间
		//		__alloc_new_map(count);
		//		if (offset > size() / 2) //插入点靠前
		//		{
		//			if (start.cur != start.first)//向前有空
		//			{
		//				my_stl::copy(start, tmp_pos, start - 1);
		//				--start;
		//				*pos = value;
		//			}
		//			else  //if (finish.node != end_map - 1 || finish.cur != finish.last - 1)//向后有空
		//			{
		//				alloc.construt(finish.cur, *(finish - 1);
		//				for (iterator p = finish - 1; p != tmp_pos; --p)
		//					*p = *(p - 1);
		//				++finish;
		//				*pos = value;
		//			}
		//		}
		//		else   //插入点靠后
		//		{
		//			if (finish.node != end_map - 1 || finish.cur != finish.last - 1)//向后有空
		//			{
		//				alloc.construt(finish.cur, *(finish - 1);
		//				for (iterator p = finish - 1; p != tmp_pos; --p)
		//					*p = *(p - 1);
		//				++finish;
		//				*pos = value;
		//			}
		//			else   //向前有空
		//			{
		//				my_stl::copy(start, tmp_pos, start - 1);
		//				--start;
		//				*pos = value;
		//			}
		//		}
		//	}
		//	return tmp_pos;
		//}

		//iterator insert(const_iterator pos, T&& value)
		//{
		//	iterator tmp_pos(pos);
		//	size_type count = size() + 1; //全部元素数量
		//	size_type space_size = iterator::buffer_size(); //一个缓冲区的容量
		//	size_type capacity = (end_map - start.node) * space_size; // 当前deque的容量
		//	size_type down_capacity = (map + map_size - end_map) * space_size + capacity; //map向下扩展的总容量
		//	size_type up_capacity = (start.node - map) * space_size + capacity; //map向上扩展的总容量
		//	size_type all_capacity = map_size * space_size; //不申请新map的最大容量

		//	difference_type offset = tmp_pos - start; //距离开始的距离
		//	if (capacity > count)  //不需要重新申请缓冲区
		//	{
		//		if (offset > size() / 2) //插入点靠前
		//		{
		//			if (start.cur != start.first)//向前有空
		//			{
		//				my_stl::copy(start, tmp_pos, start - 1);
		//				--start;
		//				__move_insert(pos, std::move(value), is_POD());
		//			}
		//			else  //if (finish.node != end_map - 1 || finish.cur != finish.last - 1)//向后有空
		//			{
		//				alloc.construt(finish.cur, *(finish - 1);
		//				for (iterator p = finish - 1; p != tmp_pos; --p)
		//					*p = *(p - 1);
		//				++finish;
		//				__move_insert(pos, std::move(value), is_POD());
		//			}
		//		}
		//		else   //插入点靠后
		//		{
		//			if (finish.node != end_map - 1 || finish.cur != finish.last - 1)//向后有空
		//			{
		//				alloc.construt(finish.cur, *(finish - 1);
		//				for (iterator p = finish - 1; p != tmp_pos; --p)
		//					*p = *(p - 1);
		//				++finish;
		//				__move_insert(pos, std::move(value), is_POD());
		//			}
		//			else   //向前有空
		//			{
		//				my_stl::copy(start, tmp_pos, start - 1);
		//				--start;
		//				__move_insert(pos, std::move(value), is_POD());
		//			}
		//		}
		//	}
		//	else if (down_capacity > count) //如果向下拓展空间足够
		//	{
		//		__alloc_new_space(count - capacity);  //申请新缓冲区
		//		alloc.construt(finish.cur, *(finish - 1);
		//		for (iterator p = finish - 1; p != tmp_pos; --p)
		//			*p = *(p - 1);
		//		++finish;
		//		__move_insert(pos, std::move(value), is_POD());
		//	}
		//	else if (up_capacity > count) //如果向上拓展空间足够
		//	{
		//		__alloc_new_space_forward(count - capacity);  //申请新缓冲区
		//		my_stl::copy(start, tmp_pos, start - 1);
		//		--start;
		//		__move_insert(pos, std::move(value), is_POD());
		//	}
		//	else
		//	{
		//		//如果不足以存放count个元素，则需要重新申请map
		//		//申请新空间
		//		__alloc_new_map(count);
		//		finish = my_stl::uninitialized_copy(first, pos, start);
		//		tmp_pos = start + offset;
		//	}
		//	return tmp_pos;
		//}

		iterator insert(const_iterator pos, size_type count, const T& value)
		{
			iterator tmp_pos(pos);
			if (count != 0)
			{
				size_type n = size() + count; //全部元素数量
				size_type space_size = iterator::buffer_size(); //一个缓冲区的容量
				size_type capacity = (end_map - map) * space_size; // 当前deque的容量
				size_type all_capacity = map_size * space_size; //不申请新map的最大容量
				if (all_capacity > n)  //不需要重新申请map
				{
					if (capacity <= n) //当前缓冲区空余位置不足以放下count新元素
						__alloc_new_space(n - capacity);  //申请新缓冲区
				}
				else  //需要重新申请map
				{
					__alloc_new_map(n);
				}
				iterator old_finish = finish;
				if (size_type(finish - tmp_pos) >= count)
				{
					finish = my_stl::uninitialized_copy(finish - count, finish, finish);  //[finish-n, finish) count个元素复制到finish开始的位置去
					//[pos, old_finish)的元素后移count个单位
					for (iterator p = old_finish; p != tmp_pos; --p)
						*(p + count) = *p;
					my_stl::fill_n(tmp_pos, count, value); //在原来pos的位置填充count个value
				}
				else
				{
					finish = my_stl::uninitialized_copy(pos, const_iterator(finish), tmp_pos + count);  //[pos, finish)后移count个单位
					my_stl::uninitialized_fill(iterator(old_finish), tmp_pos + count, value);  //[old_finish, pos + count) 填充value
					my_stl::fill(tmp_pos, iterator(old_finish), value); //[pos, old)finish) 填充value
				}
			}
			return tmp_pos;
		}
		iterator insert(const_iterator pos, int count, const T& value){ return insert(pos, size_type(count), value); }
		iterator insert(const_iterator pos, long count, const T& value) { return insert(pos, size_type(count), value); }

		template< class InputIt >
		iterator insert(const_iterator pos, InputIt first, InputIt last)
		{
			iterator tmp_pos(pos);
			if (first != last)
			{
				size_type count = my_stl::distance(first, last);
				size_type n = size() + count; //全部元素数量
				size_type space_size = iterator::buffer_size(); //一个缓冲区的容量
				size_type capacity = (end_map - map) * space_size; // 当前deque的容量
				size_type all_capacity = map_size * space_size; //不申请新map的最大容量
				if (all_capacity > n)  //不需要重新申请map
				{
					if (capacity <= n) //当前缓冲区空余位置不足以放下count新元素
						__alloc_new_space(n - capacity);  //申请新缓冲区
				}
				else  //需要重新申请map
				{
					__alloc_new_map(n);
				}
				iterator old_finish = finish;
				if (size_type(finish - tmp_pos) >= count)
				{
					finish = my_stl::uninitialized_copy(finish - count, finish, finish);  //[finish-n, finish) count个元素复制到finish开始的位置去
					//[pos, old_finish)的元素后移count个单位
					for (iterator p = old_finish; p != tmp_pos; --p)
						*(p + count) = *p;
					my_stl::copy(first, last, tmp_pos); //在原来pos的位置填充count个value
				}
				else
				{
					finish = my_stl::uninitialized_copy(tmp_pos, finish, tmp_pos + count);  //[pos, finish)后移count个单位
					my_stl::copy_n(first, size_type(old_finish - pos), tmp_pos); // 前部分复制到[pos, old_finish)
					my_stl::uninitialized_copy(first + (old_finish - pos), last, old_finish);  //后部分复制到old_finish开始的位置
				}
			}
			return tmp_pos;
		}

		iterator insert(const_iterator pos, std::initializer_list<T> ilist)
		{
			return insert(pos, ilist.begin(), ilist.end());
		}

		template< class... Args >
		iterator emplace(const_iterator pos, Args&&... args)
		{
			return insert(pos, std::forward<Args>(args)...);
		}

		iterator erase(iterator pos)
		{
			my_stl::copy(pos + 1, finish, pos);
			alloc.destroy(--finish.cur);
			return pos;
		}

		iterator erase(const_iterator pos)
		{
			return iterator(iterator(pos));
		}

		iterator erase(iterator first, iterator last)
		{
			if (first != last)
			{
				iterator tmp_finish = finish - (last - first);
				my_stl::copy(last, finish, first);
				while (finish != tmp_finish)
					alloc.destroy(--finish.cur);
			}
			return first;
		}

		iterator erase(const_iterator first, const_iterator last)
		{
			return erase(iterator(first), iterator(last));
		}


		void push_back(const T& value)
		{
			if (finish.cur != finish.last - 1)
			{
				alloc.construct(finish.cur, value);
				++finish.cur;
			}
			else
			{
				__alloc_new_space(1);
				alloc.construct(finish.cur, value);
				++finish;
			}
		}
		void push_back(T&& value)
		{
			if (finish.cur != finish.last - 1)
			{
				__move_insert(finish, std::move(value), is_POD());
				++finish.cur;
			}
			else
			{
				__alloc_new_space(1);
				__move_insert(finish, std::move(value), is_POD());
				++finish;
			}
		}
		void pop_back() { alloc.destroy(--finish.cur); }

		template< class... Args >
		void emplace_back(Args&&... args)
		{
			push_back(std::forward<Args>(args)...);
		}


		void push_front(const T& value)
		{ 
			if (start.cur != start.first)
			{
				alloc.construct(--start.cur, value);
			}
			else
			{
				__alloc_new_space_forward(1);
				alloc.construct(--start.cur, value);
			}
		}
		void push_front(T&& value)
		{
			if (start.cur != start.first)
			{
				__move_insert(--start, std::move(value), is_POD());
			}
			else
			{
				__alloc_new_space_forward(1);
				__move_insert(--start, std::move(value), is_POD());
			}
		}
		template< class... Args >
		void emplace_front(Args&&... args)
		{
			push_front(std::forward<Args>(args)...);
		}
		void pop_front() { alloc.destroy(start.cur); ++start;  }


	private:
		iterator start, finish; // 首、尾迭代器
		//		reverse_iterator rstart, rfinish; //首尾反向迭代器
		map_pointer map;	//指向中控单元
		map_pointer end_map; //指向当前可用中控单元的尾后
		size_type map_size; //map的大小
		allocator_type alloc;    //内存配置器
		allocator<T*> alloc_map;

		void __map_initialize(size_type n)
		{
			//计算需要几个缓冲区
			size_type space_size = iterator::buffer_size();
			map_size = n / space_size + 1;
			//申请中控器空间
			map = alloc_map.allocate(map_size);
			//申请缓存区空间、构造中控器各元素
			for (int i = 0; i != map_size; ++i)
				alloc_map.construct(&(map[i]), alloc.allocate(space_size));
			//设置start迭代器的值
			start.set_node(map);
			start.cur = start.first;
			difference_type offset = n % space_size;
			//设置finish迭代器的值
			finish.set_node(map + map_size - 1);
			finish.cur = finish.first + offset;
			end_map = map + map_size;
		}

		void __fill_initialize(size_type n, const value_type &value)
		{
			my_stl::uninitialized_fill_n(start, n, value);
		}

		template<typename ForwardIterator>
		void __copy_initialize(ForwardIterator first, ForwardIterator last)
		{
			my_stl::uninitialized_copy(first, last, start);
		}

		void __destroy_and_destruct()
		{
			difference_type space_size = iterator::buffer_size();
			//清理那些无用的缓冲区
			shrink_to_fit();
			//清理 [start.node, finish.node)指向指向的中控区和其中控区指向的缓冲区
			for (map_pointer p = start.node; p != finish.node; ++p)
			{
				pointer fin = *p;
				for (difference_type j = 0; j != space_size; ++j)
					alloc.destroy(fin++);
				//Release the memory
				if (*p)
					alloc.deallocate(*p, space_size);
				alloc_map.destroy(p);
			}
			//清理 finish.node指向指向的中控区和其中控区指向的缓冲区
			if (finish.node)
			{
				for (pointer fin = *(finish.node); fin != finish.cur; ++fin)
					alloc.destroy(fin);
				//Release the memory
				if (*(finish.node))
					alloc.deallocate(*(finish.node), space_size);
				alloc_map.destroy(finish.node);
			}

			if (map)
				alloc_map.deallocate(map, end_map - map);
		}

		void __alloc_new_map(size_type n)
		{
			//计算需要几个缓冲区
			size_type space_size = iterator::buffer_size();
			size_type need_map_size = n / space_size + 1;
			size_type new_map_size = std::max(need_map_size + 2, 2 * map_size);
			//申请中控器空间
			map_pointer new_map = alloc_map.allocate(new_map_size);
			size_type offset = (new_map_size - need_map_size) / 2;
			//复制原中控信息
			size_type ind = 0;
			for (map_pointer p = start.node; p != end_map; ++p, ++ind)
				new_map[offset + ind] = *p;

			//申请缓存区空间、构造中控器各元素
			for (int i = ind; i != need_map_size; ++i)
				alloc_map.construct(&(new_map[i + offset]), alloc.allocate(space_size));
			map_pointer new_end_map = new_map + offset + need_map_size;
			for (map_pointer p = map; p != end_map; ++p)
				alloc_map.destroy(p);
			if (map)
				alloc_map.deallocate(map, end_map - map);
			map = new_map;
			map_size = new_map_size;
			end_map = new_end_map;
			//设置start迭代器的值
			start.set_node(map + offset);
//			start.cur = start.first;

			finish.node = end_map;
		}
		void __alloc_new_space(size_type n)
		{
			//计算需要几个缓冲区
			size_type space_size = iterator::buffer_size();
			size_type need_map_size = n / space_size + 1;
			//申请缓存区空间、从end_map位置构造中控单元
			for (int i = 0; i != need_map_size; ++i)
				alloc_map.construct(end_map++, alloc.allocate(space_size));
		}
		map_pointer __alloc_new_space_forward(size_type n)
		{
			//计算需要几个缓冲区
			size_type space_size = iterator::buffer_size();
			size_type need_map_size = n / space_size + 1;
			//申请缓存区空间、从end_map位置构造中控单元
			map_pointer tmp = start.node;
			for (int i = 0; i != need_map_size; ++i)
				alloc_map.construct(--tmp, alloc.allocate(space_size));
			return tmp;
		}

		void __move_insert(const_iterator pos, T&& value, __true_type)
		{
			memmove(pos.cur, &value, sizeof(T) / sizeof(unsigned char));
		}
		void __move_insert(const_iterator pos, T&& value, __false_type)
		{
			alloc.construct(pos.cur, value);
		}

		void __alloc_new_front(size_type n)
		{
			//计算需要几个缓冲区
			size_type space_size = iterator::buffer_size();
			size_type need_map_size = n / space_size + 1;
			size_type new_map_size = std::max(need_map_size + 2, 2 * map_size);
			//申请中控器空间
			map_pointer new_map = alloc_map.allocate(new_map_size);
			size_type offset = (new_map_size - need_map_size) / 2;
			//复制原中控信息
			size_type ind = 0;
			for (map_pointer p = start.node; p != end_map; ++p, ++ind)
				new_map[offset + ind] = *p;

			//申请缓存区空间、构造中控器各元素
			for (int i = ind; i != need_map_size; ++i)
				alloc_map.construct(&(new_map[i + offset]), alloc.allocate(space_size));
			map_pointer new_end_map = new_map + offset + need_map_size;
			for (map_pointer p = map; p != end_map; ++p)
				alloc_map.destroy(p);
			if (map)
				alloc_map.deallocate(map, end_map - map);
			map = new_map;
			map_size = new_map_size;
			end_map = new_end_map;
			start.node = map + offset;
			finish.node = map + offset + need_map_size - 1;
		}

	};
}
#endif